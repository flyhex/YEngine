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


// ���������������� ����������
class Application: public Engine// ������
{
	// DATA
	private: Camera3D* camera, *camera2d;
    private: Font2D *font;// �����
	private: Scene *scene, *scene2d;
	private: GraphicsSurface* shape;// ������
	private: bool rightPressed;// ���� true, �� ������ ������ ���� ������
	private: GraphicsPoint* intersectedPoint;// ����� ����������� �������������� ���� � ������
	private: Mesh* mesh; // ������������� �����


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), camera2d(NIL), font(NIL), scene(NIL), scene2d(NIL), shape(NIL), intersectedPoint(NIL), rightPressed(false), mesh(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete mesh;
		delete shape;
		delete intersectedPoint;

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


			// ����������� ������...
			{
				shape = new GraphicsSurface();
				
				mesh = new Mesh();
				mesh->CreateBox3D(Box(Vector3(), Vector3(0.5, 0.5, 0.5)));
				mesh->SetBoundingVolumeType(Mesh::BOUNDING_SPHERE);

				shape->BindMesh(mesh);
		
				shape->SetColor(RGBA(RGBA::SKY_COLOR));
				shape->SetBoundingVolumeShowEnable(true);//�������� ����������� ��������������� ������
				scene->AddToRenderList(shape);
			}


			// ����� �����������...
			{
				intersectedPoint = new GraphicsPoint();
				intersectedPoint->SetColor(RGBA(RGBA::GREEN_COLOR));
				intersectedPoint->SetShowEnable(false);

				scene->AddToRenderList(intersectedPoint);
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

		font->DrawScreenText(Vector3(10, i+=20, -1), "%d", intersectedPoint->IsShowEnabled());
		//font->DrawScreenText(Vector3(10, i+=20, -1), "intersected pixel position: %d/%d", (int32)object->GetIntersectedPixelPosition().x, (int32)object->GetIntersectedPixelPosition().y);
		//font->DrawScreenText(Vector3(10, i+=20, -1), "intersected pixel color: %d/%d/%d/%d", object->GetIntersectedPixelColor().red, object->GetIntersectedPixelColor().green, object->GetIntersectedPixelColor().blue, object->GetIntersectedPixelColor().alpha);
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
			{
				Vector3 point;

				if(shape->IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(this)), &point) != -1)
				{
					intersectedPoint->SetPosition(point);// �������� ������� �����
					intersectedPoint->SetShowEnable(true);
				}
				else
				{
					intersectedPoint->SetShowEnable(false);
				}
			}
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
			else if(_event->key == Keyboard::NUM_1)
			{
				mesh->CreateBox3D(Box(Vector3(), Vector3(0.5, 0.5, 0.5)));
				mesh->SetBoundingVolumeType(Mesh::BOUNDING_SPHERE);
			}
			else if(_event->key == Keyboard::NUM_2)
			{
				mesh->CreateIcosphere3D(2);
				mesh->SetBoundingVolumeType(Mesh::BOUNDING_BOX);
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
	def.rendererDef.zBufferEnable = false;

	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
