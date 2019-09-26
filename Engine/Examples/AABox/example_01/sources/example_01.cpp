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
class MyGraphicsObject: public GraphicsSurface, // ����������� �����������
	                    public LogicObject// ���������� ������
{
	// DATA
	public: bool boundingBoxIntersection;// ���� true, �� ������������� ��� ���������� aabb ������
    private: Line result;// �������������� �������, ������������ aabb ������
	private: Line selectRay;// ������������� ��� ������
	private: Vector3 intersectedPoint;// ����� ����������� ������ � ������������� �����
	public: bool shapeIntersection;// ���� true, �� ��������� ������������ �������������� ���� � ������� ������
	private: float curPos;// ������� ������� ������� �� ����� (0-360)


	// METHODS
	// ����������� ��� ����������
	public: MyGraphicsObject(void): boundingBoxIntersection(false), shapeIntersection(false), curPos(0.0f)
	{}


	// ����������...
	// ��������� ��������� �������
	// ����������: � �������� ��������� ����������� ������ - �����, ��������� � ���������� ������ ���� �������
	public: virtual void Update(float _delta)
	{
		if(!boundingBoxIntersection) 
		{ 
			curPos += _delta * 10.0f;// �������� ������������ � 10 ���
			
			if(curPos > 360.0f) { curPos = 0.0f; } 

			// ���������� �� �����...
			Vector3 pos = GetPosition();
			float radius = 0.5f;// ������ �����, ����� �������� ����� ��������� ������
			float angle = Angle::DOUBLE_PI * (float)curPos / 360.0f;// ��������� ������� ����������
			
			// ����������� ���� � ��������� ���������� � ���������� � ��� ���������� ������
			pos.x = cosf(angle) * radius;
			pos.y = sinf(angle) * radius;

			SetPosition(pos);


			// ������� �� ���� ����...
			SetRotation(GetRotation() * Quaternion(Euler(_delta * 0.2, _delta * 0.2, _delta * 0.2))); 
		}
	}


	// ������ ������� ����
	public: Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		if(_event->type == Mouse::KEY_DOWN)// ���� ������ �������
		{
			if(_event->key == Mouse::LEFT_BUTTON)// ����� ����
			{
				selectRay = Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked()));// ��������� ������������� ��� ������
				
				boundingBoxIntersection = GetTransformedAABB().IsIntersected(selectRay, &result);// ��������� �������, ������������ ������������� 

				if(boundingBoxIntersection) 
				{ 
					if(IsIntersected(selectRay, intersectedPoint) != -1)
					{
						shapeIntersection = true;
					}
					else 
					{ 
						shapeIntersection = false; 
					}
				}
				else { shapeIntersection = false; } 
			}
		}
		return _event;
	}


	// ������������...
	// ���������� ������������� ����� �� ����� 
	// ����������: ����� ������������� aabb ��������������, ������� ������� ������� ������� �� ��������� � ������������ ��������
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


		GraphicsSurface::Draw();
	
		if(boundingBoxIntersection)// ���� ������������� ��� ���������� �������������� ����� ��������
		{
			// ���������� ������������� ��� ������
			GraphicsLine::_Draw(selectRay.origin, result.origin, RGBA(RGBA::YELLOW_COLOR));

			// ���������� ����� �������������� ����, ����������� ����� aabb ��������
			GraphicsLine::_Draw(result.origin, result.end, RGBA(RGBA::RED_COLOR));

			// ���������� ����� �������������� ���� ����������� � ���������
			GraphicsLine::_Draw(selectRay.end, result.end, RGBA(RGBA::YELLOW_COLOR));

			if(shapeIntersection) 
			{ 
				GraphicsPoint::_Draw(intersectedPoint, RGBA(RGBA::RED_COLOR)); 
			} 
		}
		else// ����� ��� �����������
		{
			// ���������� ������������� ��� ������
			GraphicsLine::_Draw(selectRay.origin, selectRay.end, RGBA(RGBA::YELLOW_COLOR));
		}
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
	private: MyGraphicsObject* object;// ���������������� ������������� �����
	private: bool rightPressed;// ���� true, �� ������ ������ ���� ������



    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), camera2d(NIL), font(NIL), scene(NIL), scene2d(NIL), object(NIL), rightPressed(false)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
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
				scene2d->BindCamera(camera2d);

				scene->AddToRenderList(scene2d);
			}
						

			// ���������� ������...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;

				camera2d = new Camera3D();
				camera2d->Create(def);

				scene2d->BindCamera(camera2d);
			}

			
			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.fontName = "Arial";// ��� ������
				def.fontSize = 16;// ������ ������
				def.fontWeight = Font2D::WEIGHT_BOLD;
				def.color = RGBA(RGBA::WHITE_COLOR);// ���� ������

				font = new Font2D();
				font->Create(def);
				font->Pick();// ������� �����
			}


			// ����������� ������...
			{
				object = new MyGraphicsObject();
				
				Mesh* mesh = new Mesh();
				//mesh->CreateQuad2D(Quad(Vector3(), Vector2(0.5, 0.5)));
				mesh->CreatePyramid3D(0.25f/2.0f);
				object->BindMesh(mesh);

				object->SetColor(RGBA(RGBA::WHITE_COLOR));
				//object->SetBBColor(RGBA(RGBA::BLACK_COLOR));
				object->SetBackFaceCullingEnable(false);// ��������� ��������� ������ ������
				object->SetBoundingVolumeShowEnable(true);// �������� ����������� ��������������� ������

				scene->AddToRenderList(object);
				scene->AddToUpdateList(object);
			}

		
			SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));// ���� ����

			GraphicsLine::_SetLineWidth(2.0f);// ������� �����
			GraphicsPoint::_SetPointSize(6.0f);// ������ �����

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

		font->DrawScreenText(Vector3(10, i+=20, -1), "bounding volume show enable: %d", object->IsBoundingVolumeShowEnabled());
		//font->DrawScreenText(Vector3(10, i+=20, -1), "bounding volume show mode: %s", GetDrawModeString(object->GetBoundingVolumeShowMode()).c_str());

		
		font->DrawScreenText(Vector3(10, i+=20, -1), "aabb intersection: %d", object->boundingBoxIntersection); 
		font->DrawScreenText(Vector3(10, i+=20, -1), "shape intersection: %d", object->shapeIntersection); 
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

		Engine::MouseEventFilter(_event);

		object->MouseEventFilter(_event);

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
			else if(_event->key == Keyboard::E)
			{
				object->SetBoundingVolumeShowEnable(!object->IsBoundingVolumeShowEnabled());
			}
			else if(_event->key == Keyboard::UP_ARROW)
			{
				object->SetScale(object->GetScale() * 1.05);
			}
			else if(_event->key == Keyboard::DOWN_ARROW)
			{
				object->SetScale(object->GetScale() * 0.95);
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "AABox.example_01";
	//def.rendererDef.zBufferEnabled = true;

	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
