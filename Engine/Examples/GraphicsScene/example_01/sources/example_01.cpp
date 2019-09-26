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
#include <Engine/Output/Graphics/Shape/Mesh/ColoredMesh/ColoredMesh.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Output/Graphics/Shape/Mesh/TexturedMesh/TexturedMesh.h>
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
	private: Camera3D* camera, *camera2;// ������
    private: Font2D *font;// �����
	private: Scene* scene, *scene2;// �����
	
	private: TexturedMesh* mesh;// ���������������� ������������� �����
	private: ColoredMesh* box;// ���������� �������
	private: Texture2D* texture;// �������� �����



    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font(NIL), scene(NIL), scene2(NIL), camera2(NIL), mesh(NIL), texture(NIL), box(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete texture;

		delete mesh;
		delete box;

		delete camera;
		delete camera2;

		delete font;

		delete scene2;
		delete scene;
	} 



	// ������� ����������
	public: bool Create(Engine::Define _define)
	{
		// ������� ������...
		if(Engine::Create(_define))
		{
			// ������� �����...
			{
				scene = new Scene();
				scene->Pick();
			}
		

			// �������� ��� ����������� ������...
			{
				texture = new Texture2D();
				PixelImage img(Vector2(800, 600), RGBA(RGBA::WHITE_COLOR));
				texture->Create(img);
			}

						
			// ���������������� ���� ��� ������� �����...
			{
				mesh = new TexturedMesh();
				mesh->CreateQuad2D(Quad(Vector3(), Vector2(1,1)));
				
				mesh->BindTexture(texture);

				scene->AddGraphicsObject(mesh);
			}


			// ����� ��� ����������� ������...
			{
				scene2 = new Scene();
				scene2->BindTexture(texture);
				scene->AddGraphicsObject(scene2); 
			}


			// ������ ��� ����������� ������...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::SCROLL_FIRST_PERSON;
				def.aspectRatioAutoEnable = false;
				def.aspectRatio = texture->GetImageSize().x / texture->GetImageSize().y;// ������ ����������� ������ ��������, � �������� �������� ��������� ��������
				//def.rotationEnable = true;
				//def.zoomEnable = true;

				camera2 = new Camera3D();
				camera2->Set(def);

				scene2->BindCamera(camera2);
			}


			// ���������� ������� ��� ����������� ������...
			{
				box = new ColoredMesh();
				box->CreateBox3D(Box3D(Vector3(), Vector3(0.5,0.5,0.5)));
				box->SetPosition(Vector3(0,0,-2));
				box->SetColor(RGBA(RGBA::RED_COLOR));

				scene2->AddGraphicsObject(box);
			}
  

			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.fontName = "Arial";// ��� ������
				def.fontSize = 16;// ������ ������
				def.fontWeight = Font2D::WEIGHT_BOLD;
				def.color = RGBA(RGBA::GRAY_COLOR);// ���� ������

				font = new Font2D();
				font->Create(def);
				font->Pick();// ������� �����
			}


			// ������ ��� ������� �����...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::SCROLL_OBJECT_VIEWER;
				def.rotationEnable = true;
				def.zoomEnable = true;

				camera = new Camera3D();
				camera->Set(def);

				camera->Pick(); // ������� ������
				scene->BindCamera(camera);
			}
			

			// ���� ����...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));

			MoveToCenter();// ����������� ���� � ����� ������
			Mouse::_MoveCursorToCenter(this);
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}



	// ������� ����������
	public: virtual void Update(float _delta)
	{
		Engine::Update(_delta);

		// ������� ������� ������ ��� Y
		box->SetRotation(box->GetRotation() * Quaternion(Matrix4x4::_RotationY(_delta)));
	}



	// ������� ������������
	public: virtual void Draw(float _delta)
	{
		Engine::Draw(_delta);
	}


	// ������ ������� ����
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Engine::MouseEventFilter(_event);
		return _event;
	}


	// ��������� ������� ����������
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event)
	{
		Engine::KeyboardEventFilter(_event);

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
				Destroy();// ���������� ����
			} 
			else if(_event->key == Keyboard::D)
			{
				texture->Destroy();
			}
			else if(_event->key == Keyboard::S)
			{
				PixelImage img;
				texture->GetImage(img);
				img.SaveImageToPNGFile("pbuffer.png");
			}
		} 

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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "GraphicsScene.example_01";
	//def.rendererDef.zBufferEnabled = true;

	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
