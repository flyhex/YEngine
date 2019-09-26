// Win32.cpp: ���������� ����� ����� ��� ����������.
//


#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Math/Angle/Angle.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Core/Time/Timer/SyncTimer/SyncTimer.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>




// �����, ����������� ����������� ������������� ��������������
class MyBox: public GraphicsSurface,// ����������� �����������
	         public LogicObject// ���������� ������
{
	// DATA
	private: Box box;
	private: FloatLerp* floatLerp;// �������� ������������
	private: VectorLerp* vectorLerp;// ��������� ������������


	// METHODS
	// ���������������� �����������
	public: MyBox(void)
	{
		box = Box(Vector3(), Vector3(0.5f, 0.5f, 0.5f));
		
		// �����...
		Mesh* mesh = new Mesh();
		mesh->CreateBox3D(box);
		mesh->SetBoundingVolumeType(Mesh::BOUNDING_SPHERE);
		BindMesh(mesh);
		

		// ����...
		SetColor(RGBA(RGBA::GREEN_COLOR)); 


		// ������������...
		SetCollisionCheckEnable(true);// ��������� ������� � �������������
		Connect(PolygonalSurface::COLLISION_DETECTED_MESSAGE, this, Caller<>(this, &MyBox::CollisionStateIsChanged));

			
		// ��������...
		floatLerp = new FloatLerp();
		floatLerp->Stop();// ��������� � ������ � ����������
		floatLerp->SetLerpMode(FloatLerp::UNITS_PER_SECOND);// ����� ������������
		floatLerp->SetPlayingSpeed(0.5f);// �������� �������� � ��������/�������

		vectorLerp = new VectorLerp();
		vectorLerp->BindFloatLerp(floatLerp);
	} 


	// ����������
	public: virtual ~MyBox(void)
	{
		Destroying();

		delete floatLerp;
		delete vectorLerp;
	}



	// ������� �� ������������/����� �� ������������
	public: void CollisionStateIsChanged(bool _enable)
	{
		if(_enable)
		{
			SetColor(RGBA(RGBA::RED_COLOR));
		}
		else
		{
			SetColor(RGBA(RGBA::GREEN_COLOR));
		}
	}

	// ���������� �������� �������������� ���������������
	public: Box GetBox(void)const
	{
		return box;
	}
		

	// ��������� ��������� �������
	// ����������: � �������� ��������� ����������� ������ - �����, ��������� � ���������� ������ ���� �������
	public: virtual void Update(float _delta = 0.0f)
	{
		SetRotation(GetRotation() * Quaternion(AxisAngle(Vector3::UNIT_Y, _delta)));// ������� ������ ������ ��� Y

		if(floatLerp->IsPaused() && floatLerp->IsFinish())// ���� ���������� ������� �������
		{
			SetTarget(GetRandomPosition());// ������ ����� ������� �������
		}
		else
		{
			floatLerp->Update(GetDeltaMultiplier() * _delta);// ������� � ������� ������� 

			SetPosition(vectorLerp->GetCurrent());// �������� ������� ������� �������
		}
	}


	// ������ ����� ������� ������� ��� ��������
	public: void SetTarget(Vector3 _target)
	{
		vectorLerp->Set(GetPosition(), _target);// ������ �������� � ������� ����������� �������
 
		floatLerp->SetTarget(Line::_GetLength(GetPosition(), _target) / 10.0f);// ������ ���, ����� �������� �������� �� �������� �� ��������� �� ����
		
		// ������ ��������������� �������...
		floatLerp->Stop();
		floatLerp->Play();	 
	}

	// ���������� ��������� ������� 
	public: Vector3 GetRandomPosition(void)
	{
		/* [-5, 5] - ��� ������ ������������, � ������ ������� ����� ������������ ������ */ 
		return Vector3(Random::_RandomFloat(-5.0f + GetBox().GetWidth() / 2.0f, (5.0f - GetBox().GetWidth() / 2.0f)), 
					   GetBox().GetHeight() / 2.0f, // ��� ������� ����
					   Random::_RandomFloat(-5.0f + GetBox().GetLength() / 2.0f, (5.0f - GetBox().GetLength() / 2.0f)));
	}
};



// �����, ����������� ����������� ���� 
class MyQuad: public GraphicsSurface
{
	// DATA
	private: Quad quad;// �������� �����


	// METHODS  
	// ���������������� �����������
	public: MyQuad(void)
	{
		quad = Quad(Vector3(), Vector2(10.f, 10.0f));
		
		Mesh* mesh = new Mesh();
		mesh->CreateQuad2D(quad);
		BindMesh(mesh);

		SetColor(RGBA(RGBA::WHITE_COLOR)); 
		SetCollisionCheckEnable(true);// ��������� ������� � �������������

		SetRotation(Quaternion(AxisAngle(Vector3::UNIT_X, Angle::_DegreesToRadians(-90.0f))));// ������� ������ ��� X
	}


	// ����������
	public: virtual ~MyQuad(void)
	{
		Destroying();
	}


	// ���������� �������� ����� 
	public: Quad GetQuad(void)const
	{
		return quad;
	}
};



//�����, ����������� ����������� �������� 
class MyPyramid: public GraphicsSurface,// ����������� �����������
	             public LogicObject// ���������� ������
{
	// DATA
	private: float radius;// ������ ��������
	private: FloatLerp* floatLerp;// �������� ������������
	private: VectorLerp* vectorLerp;// ��������� ������������


	// METHODS
	// ���������������� �����������
	public: MyPyramid(void)
	{
		radius = 0.5f;// ������ ��������

		// �����...
		Mesh* mesh = new Mesh();
		mesh->CreatePyramid3D(radius);
		mesh->SetBoundingVolumeType(Mesh::BOUNDING_SPHERE);
		BindMesh(mesh);
		
		
		// ����...
		SetColor(RGBA(RGBA::GREEN_COLOR)); 


		// ������������...
		SetCollisionCheckEnable(true);// ��������� ������� � �������������
		Connect(PolygonalSurface::COLLISION_DETECTED_MESSAGE, this, Caller<>(this, &MyPyramid::CollisionStateIsChanged));


		// ��������...
		floatLerp = new FloatLerp();
		floatLerp->Stop();// ��������� � ������ � ����������
		floatLerp->SetLerpMode(FloatLerp::UNITS_PER_SECOND);// ����� ������������
		floatLerp->SetPlayingSpeed(0.5f);// �������� �������� � ��������/�������

		vectorLerp = new VectorLerp();
		vectorLerp->BindFloatLerp(floatLerp);
	} 


	// ����������
	public: virtual ~MyPyramid(void)
	{
		Destroying();

		delete floatLerp;
		delete vectorLerp;
	}


	// ������� �� ������������/����� �� ������������
	public: void CollisionStateIsChanged(bool _enable)
	{
		if(_enable)
		{
			SetColor(RGBA(RGBA::RED_COLOR));
		}
		else
		{
			SetColor(RGBA(RGBA::GREEN_COLOR));
		}
	}


	// ���������� ������ ��������
	public: float GetRadius(void)const
	{
		return radius;
	}


	// ��������� ��������� �������
	// ����������: � �������� ��������� ����������� ������ - �����, ��������� � ���������� ������ ���� �������
	public: virtual void Update(float _delta = 0.0f)
	{
		SetRotation(GetRotation() * Quaternion(AxisAngle(Vector3::UNIT_Y, _delta)));// ������� ������ ������ ��� Y

		if(floatLerp->IsPaused() && floatLerp->IsFinish())// ���� ���������� ������� �������
		{
			SetTarget(GetRandomPosition());// ������ ����� ������� �������
		}
		else
		{
			floatLerp->Update(GetDeltaMultiplier() * _delta);// ������� � ������� ������� 

			SetPosition(vectorLerp->GetCurrent());// �������� ������� ������� �������
		}
	}


	// ������ ����� ������� ������� ��� ��������
	public: void SetTarget(Vector3 _target)
	{
		vectorLerp->Set(GetPosition(), _target);// ������ �������� � ������� ����������� �������
 
		floatLerp->SetTarget(Line::_GetLength(GetPosition(), _target) / 10.0f);// ������ ���, ����� �������� �������� �� �������� �� ��������� �� ����
		
		// ������ ��������������� �������...
		floatLerp->Stop();
		floatLerp->Play();	 
	}


	// ���������� ��������� ������� 
	public: Vector3 GetRandomPosition(void)
	{
		/* [-5, 5] - ��� ������ ������������, � ������ ������� ����� ������������ ������ */ 
		return Vector3(Random::_RandomFloat(-5.0f + GetRadius(), 5.0f - GetRadius()), 
					   GetRadius(), // ��� ������� ����
					   Random::_RandomFloat(-5.0f + GetRadius(), 5.0f - GetRadius()));
	}
};



// ���������������� ����������
class Application: public Engine// ������
{
	// DATA
	private: Camera3D* camera, *camera2d;
    private: Font2D *font;// �����
	private: Scene *scene, *scene2d;
	private: MyBox* box;// �������
	private: MyQuad* quad;// ����
	private: MyPyramid* pyramid;// ��������
	private: bool rightPressed;// ���� true, �� ������ ������ ���� ������
	private: SyncTimer timer;// ������, ���������� �� ������������ ������� slow motion


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), camera2d(NIL), font(NIL), scene(NIL), scene2d(NIL), box(NIL), rightPressed(false), quad(NIL), pyramid(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete pyramid;
		delete quad;
		delete box;

		delete font;

		delete camera2d;
		delete camera;

		delete scene2d;
		delete scene;
	} 



	// ������� ����������
	public: bool Create(Engine::Define _define)
	{
		// ������� ������...
		if(Engine::Create(_define))
		{
			// �����...
			{
				scene = new Scene();
				scene->Pick();
			}
		

			// ������...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::OBJECT_VIEWER_ROTATION;
				def.rotationEnable = false;
				def.zoomEnable = true;
				def.zoomNear = 10.0f;
				def.zoomFar = 100.0f;// ��������� ���������, �� ������� ������ ����� ���������� �� �����

				camera = new Camera3D();
				camera->Create(def);

				camera->Pick(); // ������� ������
				scene->BindCamera(camera);
			}


			// ���������� �����...
			{
				scene2d = new Scene();

				scene->AddToRenderList(scene2d);
			}
						

			// ���������� ������...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;
				def.distanceToViewingPoint = 1.0f;

				camera2d = new Camera3D();
				camera2d->Create(def); 
				
				scene2d->BindCamera(camera2d);
				//camera2d->Pick(); // ������� ������
			}

			
			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.fontName = "Arial";// ��� ������
				def.fontSize = 16;// ������ ������
				def.fontWeight = Font2D::WEIGHT_BOLD;
				def.color = RGBA(RGBA::WHITE_COLOR).GetRGBAf();// ���� ������

				font = new Font2D();
				font->Create(def);
				font->Pick();// ������� �����
			}


			// ������
			{
				timer.StartSec(3.0f);// ������������ ������� slow motion
				timer.SetMode(SyncTimer::SINGLE_SHOT);
				timer.Pause();
			}

			// ����������� ������...
			{
				box = new MyBox();
				box->SetPosition(box->GetRandomPosition());// ������ ������� ��������� ������� � ������ ��������� quad
				box->SetTarget(box->GetRandomPosition());// ������ ��������� ������� �������

				scene->AddToRenderList(box);
				scene->AddToUpdateList(box);
			}

			// ����������� ������...
			{
				quad = new MyQuad();
				
				scene->AddToRenderList(quad);
			}

			// ����������� ������...
			{
				pyramid = new MyPyramid();
				
				pyramid->SetPosition(pyramid->GetRandomPosition());// ������ ������� ��������� ������� � ������ ��������� quad
				pyramid->SetTarget(pyramid->GetRandomPosition());// ������ ��������� ������� �������

				scene->AddToRenderList(pyramid);
				scene->AddToUpdateList(pyramid);
			}


			SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));// ���� ����

			GraphicsLine::_SetLineWidth(3.0f);// ������� �����
			//GraphicsPoint::_SetPointSize(6.0f);// ������ �����

			SetWireframeEnable(true);// ��������� ����� ������������

			MoveToCenter();// ����������� ���� � ����� ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}



	// ������� ����������
	public: virtual void Update(float _delta)
	{
		Engine::Update(_delta);

		PolygonalSurface::_CheckCollision(box, pyramid);// ��������� ������������ ����� ����� ���������

		if(timer.Check(_delta))// ���� ������ ������ ����
		{
			// ������������ �������� ��������
			box->SetDeltaMultiplier(1.0f);
			pyramid->SetDeltaMultiplier(1.0f);
		}
	}



	// ������� ������������
	public: virtual void Draw(float _delta)
	{
		Engine::Draw(_delta);
		
		
		// ������ ����� ������������� �����...
		camera2d->BeginTransform(Renderer::PROJECTION);
		camera2d->BeginTransform(Renderer::MODELVIEW);


		// ����� ������...
		int32 i = 0;

		font->DrawScreenText(Vector3(10, i+=20, -1), "box is intersected with pyramid: %d", box->IsCollisionDetected());
		
		if(!timer.IsPaused() && timer.RemainedTimeInSec() > 0.0f)// ���� ������ �� ������ ������� �������
		{
			font->DrawScreenText(Vector3(10, i+=20, -1), "timer. slow motion: %.3f", timer.RemainedTimeInSec());
		}

		font->DrawScreenText(Vector3(10, i+=20, -1), "bounding volume type: %s", box->GetMesh()->GetBoundingVolumeType() == Mesh::BOUNDING_BOX ? "BOUNDING_BOX" : box->GetMesh()->GetBoundingVolumeType() == Mesh::BOUNDING_SPHERE ? "BOUNDING_SPHERE" : "");
		//font->DrawScreenText(Vector3(10, i+=20, -1), "texture-pos: %f/%f", object->texturePosition.x, object->texturePosition.y);


		
		//font->DrawScreenText(Vector3(10, i+=20, 0), "scene.amount of objects: %d", scene->AmountOfObjects()); 
		//font->DrawScreenText(Vector3(10, i+=20, 0), "scene.amount of logic objects: %d", scene->AmountOfLogicObjects()); 
		//font->DrawScreenText(Vector3(10, i+=20, 0), "scene.amount of graphics objects: %d", scene->AmountOfGraphicsObjects()); 
		


		// ������������ ���������� �������������...
		camera2d->EndTransform(Renderer::MODELVIEW);
		camera2d->EndTransform(Renderer::PROJECTION);
	}


	// ������ ������� ����
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		if(_event->type == Mouse::KEY_DOWN)
		{
			if(_event->key == Mouse::LEFT_BUTTON)
			{}
			else if(_event->key == Mouse::RIGHT_BUTTON)
			{
				// ������ �������� ������...
				if(!rightPressed)// ��������� ������ ���� ���
				{
					camera->SetRotationEnable(true); // ��������� �������� ������
					rightPressed = true;

					Mouse::_MoveCursorToCenter(Renderer::_GetPicked());// ���������� ���� � ����� ���������� ������� ���� 
					Mouse::_SetShowCursor(false);
				}
			}
		}
		else if(_event->type == Mouse::KEY_UP)
		{
			if(_event->key == Mouse::RIGHT_BUTTON)
			{
				// ����� �������� ������...
				if(rightPressed)// ��������� ������ ���� ���
				{
					camera->SetRotationEnable(false);// ��������� �������� ������ 
					rightPressed = false;
					Mouse::_SetShowCursor(true);
				}
			}
		}

		Engine::MouseEventFilter(_event);
		return _event;
	}


	// ��������� ������� ����������
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event)
	{
		if(_event->type == Keyboard::KEY_DOWN)
		{
			if(_event->key == Keyboard::F1)
			{
				SetWindowStyle(Window::FULLSCREEN);
			}
			else if(_event->key == Keyboard::F2)
			{
				SetWindowStyle(Window::WND_RESIZING);
			}
			else if(_event->key == Keyboard::ESC)
			{
				Destroy();
			}
			else if(_event->key == Keyboard::S)
			{
				// ��������� �������� ��������
				box->SetDeltaMultiplier(0.2f);
				pyramid->SetDeltaMultiplier(0.2f);

				timer.Restart();
			}
			else if(_event->key == Keyboard::NUM_1)
			{
				box->GetMesh()->SetBoundingVolumeType(Mesh::BOUNDING_BOX);
				pyramid->GetMesh()->SetBoundingVolumeType(Mesh::BOUNDING_BOX);
			}
			else if(_event->key == Keyboard::NUM_2)
			{
				box->GetMesh()->SetBoundingVolumeType(Mesh::BOUNDING_SPHERE);
				pyramid->GetMesh()->SetBoundingVolumeType(Mesh::BOUNDING_SPHERE);
			}
		}

		Engine::KeyboardEventFilter(_event);
		return _event;
	}
};



// ����� ����� � ���������
// ���������� � ������ ������� WINAPI, ���������, ��� ��������� � ������� WinMain ���������� ����� �������
int WINAPI WinMain(HINSTANCE hInstance,// ���������� (�������������), ������������ �������� ��� ������� ����������
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	// �������� ������...
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Collision detection";
	def.rendererDef.zBufferEnable = true;

	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
