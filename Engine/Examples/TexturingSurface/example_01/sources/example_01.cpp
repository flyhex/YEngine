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



// �����, ����������� ���������������� ����������� ������������� �����
class TexturedObject: public TexturingSurface, // �������������� �����������
	                  public LogicObject// ���������� ������
{
	// DATA
	private: Line selectRay;// ������������� ��� ������
	private: Vector2 pixelPosition;// ���������� �������, �� ������� ��������� ������������� ���
	private: float curPos;// ������� ������� ������� �� ����� (0-360)
	private: bool intersection;// ���� true, �� ��������� ����������� �������������� ���� � ������������� ������
	private: RGBA pixelColor;// ���� �������, �� ������� ��������� ������������� ���
	private: bool pauseEnable;// ���� true, �� ����� ���������� ����������� � �������� �������


	// METHODS
	// ����������� ��� ����������
	public: TexturedObject(void): curPos(0.0f), intersection(false), pauseEnable(false)
	{}


	// ���������� true ���� ������ �����
	public: bool IsPauseEnabled(void)const
	{
		return pauseEnable;
	}


	// ��������/��������� �����
	public: void SetPauseEnable(bool _enable)
	{
		pauseEnable = _enable;
	}


	// ���������� ���� �������, �� ������� ��������� ������������� ���
	public: RGBA GetIntersectedPixelColor(void)const
	{
		return pixelColor;
	}

	// ���������� ������� �������, �� ������� ��������� ������������� ���
	public: Vector2 GetIntersectedPixelPosition(void)const
	{
		return pixelPosition;
	}


	// ���������� true ���� ������������� ��� ���������� ������������� �����
	public: bool IsQuadIntersected(void)const
	{
		return intersection;
	}


	// ����������...
	// ��������� ��������� �������
	// ����������: � �������� ��������� ����������� ������ - �����, ��������� � ���������� ������ ���� �������
	public: virtual void Update(float _delta)
	{
		if(!pauseEnable)
		{ 
			curPos += _delta * 10.0f;// �������� ������������ � 10 ���
			
			if(curPos > 360.0f) { curPos = 0.0f; } 

			// ���������� �� �����...
			float radius = 0.5f;// ������ �����, ����� �������� ����� ��������� ������
			float angle = Angle::DOUBLE_PI * (float)curPos / 360.0f;// ��������� ������� ����������
			
			// ����������� ���� � ��������� ����������
			SetPosition(Vector3(cosf(angle) * radius, sinf(angle) * radius, 0));

			// ������� �� ���� ����...
			SetRotation(GetRotation() * Quaternion(Euler(0, _delta * 0.2, 0))); 
		}
	}


	// ������ ������� ����
	public: Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		if(_event->type == Mouse::MOVE)// ���� ������ ������������
		{
			selectRay = Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked()));// ��������� ������������� ��� ������
			intersection = IsIntersected(selectRay, &pixelPosition, &pixelColor);
		} 

		return _event;
	}


	// ������������...
	// ���������� ������������� ����� �� ����� 
    // ����������: BeginTransform � EndTransform ����� �������� �������������
	public: virtual void Draw(void)
	{
		GraphicsPoint::_SetPointSize(2.0f);// ������ �����

		Vector3 vertex;// �������
		float radius = 0.5f;// ������ �����, ����� �������� ����� ��������� ������

		// ��������� ��������� ������� ����������...
		for(int32 i = 1; i < (360/2) + 1; ++i)
		{ 
			float angle = Angle::DOUBLE_PI * (float)i / (360/2);// ��������� ������� ����������
				
			// ����������� ���� � ��������� ���������� � ���������� � ��� ���������� ������
			vertex.x = cosf(angle) * radius;
			vertex.y = sinf(angle) * radius;

			GraphicsPoint::_Draw(vertex, RGBA(RGBA::RED_COLOR));
		}
		GraphicsPoint::_SetPointSize(6.0f);// ������ �����

		/*
		// ���������� ��������� ����� ����������� ������� � �����...
		selectRay = Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked()));// ��������� ������������� ��� ������

		Vector3 intersectedPoint;// ����� �����������

		if(IsIntersected(selectRay, intersectedPoint) != -1)// ���� ��������� ����������� ������� � ������������� �����
		{
			// ������� �����
			//GraphicsPoint::_Draw(GetPosition(), RGBA(RGBA::GREEN_COLOR));
	
			// ����� �����������
			//GraphicsPoint::_Draw(intersectedPoint, RGBA(219, 108, 47, 255));// ��������� ����

			// ��������� �������
			intersectedPoint = (Quaternion::_GetConjugate(GetRotation())).RotatePointAroundPivot(intersectedPoint, GetPosition());

			// ��������� �������
			Vector3 localPosition = intersectedPoint - GetPosition();// ����� ��������� ����� �����������, � ������ ������������� �����

			// ��������� ���������������
			localPosition.x *= 1.0f / GetScale().x;
			localPosition.y *= 1.0f / GetScale().y;
			localPosition.z *= 1.0f / GetScale().z;

			// ��������� ����� �����������
			BeginTransform(); 
			GraphicsPoint::_Draw(Vector3(localPosition.x, localPosition.y, 0.01f), RGBA(RGBA::YELLOW_COLOR));
			EndTransform(); 
		}
		*/ 

		TexturingSurface::Draw();
	}


	// ��������� ������ � ����� ����� _file, � ���������� true � ������ ������
	// ����������: ���� ���� �� ������ �� ������, �� ������������ false
	public: virtual bool SaveToFile(File& _file)
	{
		return false;
	}


	// ��������� ������ �� ����� _file, � ���������� true � ������ ������
	// ����������: ���� ���� �� ���������� ��� �� ������ �� ������, �� �������� false
	protected: virtual bool LoadFromFile(File& _file)
	{
		return false;
	}
};

// ���������������� ����������
class Application: public Engine// ������
{
	// DATA
	private: Camera3D* camera, *camera2d;
    private: Font2D *font;// �����
	private: Scene *scene, *scene2d;
	private: TexturedObject* object;// �������������� ������
	private: bool rightPressed;// ���� true, �� ������ ������ ���� ������
	private: Texture2D* texture1, *texture2;
	private: TexturedAnimation* anim1, *anim2;



    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), camera2d(NIL), font(NIL), scene(NIL), scene2d(NIL), object(NIL), rightPressed(false), texture1(NIL), anim1(NIL), texture2(NIL), anim2(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete texture1;
		delete texture2;
		delete anim1;
		delete anim2;
		delete object;

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
				//def.rotationEnable = true;
				//def.farZoom = 11;
				def.zoomNear = 1.1f;
				def.zoomEnable = true;

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

			// ��������...
			{
				texture1 = new Texture2D();
				texture1->Create("restartButtonTexture1.png");
			}

			// ��������...
			{
				texture2 = new Texture2D();
				texture2->Create("restartButtonTexture2.png");
			}

			// ��������...
			{
				anim1 = new TexturedAnimation();

				Atlas2D* atlas = new Atlas2D();
				atlas->LoadImagesFromFolder("restartButtonAnim1");
				anim1->BindAtlas(atlas);

				anim1->SetFPS(1.0f);
				anim1->Play();
				anim1->SetPlayingMode(PlayingObject::LOOP_PLAYING);
				scene->AddToUpdateList(anim1);
			}

					
			// ��������...
			{
				anim2 = new TexturedAnimation();

				Atlas2D* atlas = new Atlas2D();
				atlas->LoadImagesFromFolder("restartButtonAnim2");
				anim2->BindAtlas(atlas);

				anim2->SetFPS(1.0f);
				anim2->Play();
				anim2->SetPlayingMode(PlayingObject::LOOP_PLAYING);
				scene->AddToUpdateList(anim2);
			}


			// �������������� ������ ������...
			{
				object = new TexturedObject();
				
				Mesh* mesh = new Mesh();
				mesh->CreateQuad2D(Quad(Vector3(), Vector2(0.5f, 0.5f)));
				object->BindMesh(mesh);
				
		
				object->BindFrontTexture(texture1);
				object->BindBackTexture(texture2);

				//object->BindFrontTexturedAnimation(anim1);
				//object->BindBackTexturedAnimation(anim2);



				//object->CreatePyramid3D(0.25f/2.0f);

				//object->SetPosition(Vector3(400, 300, -1));

				//object->SetColor(RGBA(RGBA::WHITE_COLOR));
				//object->SetBBColor(RGBA(RGBA::BLACK_COLOR));
				
					
				object->SetIntersectionMode(AbstractSurface::CHECK_ONLY_FRONT_FACE, false);// ��������� �������� ������ ����������� ������, ������� � ��� � ������
				
				object->SetBackFaceCullingEnable(false);// ��������� ��������� ������ ������
				object->SetBoundingVolumeShowEnable(true);//�������� ����������� ��������������� ������

				scene->AddToRenderList(object);
				scene->AddToUpdateList(object);
			}

		
			SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));// ���� ����

			//GraphicsLine::_SetLineWidth(2.0f);// ������� �����
			GraphicsPoint::_SetPointSize(6.0f);// ������ �����

			//SetWireframeEnable(true);// ��������� ����� ������������

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

		font->DrawScreenText(Vector3(10, i+=20, -1), "quad intersection: %d", object->IsQuadIntersected());
		font->DrawScreenText(Vector3(10, i+=20, -1), "intersected pixel position: %d/%d", (int32)object->GetIntersectedPixelPosition().x, (int32)object->GetIntersectedPixelPosition().y);
		font->DrawScreenText(Vector3(10, i+=20, -1), "intersected pixel color: %d/%d/%d/%d", object->GetIntersectedPixelColor().red, object->GetIntersectedPixelColor().green, object->GetIntersectedPixelColor().blue, object->GetIntersectedPixelColor().alpha);
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
		if(_event->type == Mouse::KEY_DOWN)// ���� ������ �������
		{
			if(_event->key == Mouse::RIGHT_BUTTON)// ������ ����
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
		else if(_event->type == Mouse::KEY_UP)// ���� ������ ��������
		{
			if(_event->key == Mouse::RIGHT_BUTTON)// ���� ������ ����
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

		object->MouseEventFilter(_event);

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
			else if(_event->key == Keyboard::UP_ARROW)
			{
				object->SetScale(object->GetScale() * 1.05);
			}
			else if(_event->key == Keyboard::DOWN_ARROW)
			{
				object->SetScale(object->GetScale() * 0.95);
			}
			else if(_event->key == Keyboard::SPACE)
			{
				object->SetPauseEnable(!object->IsPauseEnabled());
			}
			else if(_event->key == Keyboard::T)
			{
				object->BindFrontTexture(texture1);
				object->BindBackTexture(texture2);
			}
			else if(_event->key == Keyboard::A)
			{
				object->BindFrontTexturedAnimation(anim1);
				object->BindBackTexturedAnimation(anim2);
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "TexturingSurface.example_01";
	//def.rendererDef.zBufferEnabled = true;

	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
