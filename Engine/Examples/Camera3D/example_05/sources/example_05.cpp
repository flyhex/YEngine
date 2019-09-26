

#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <glaux.h>// ��� �������

#include <Engine/Math/Ratio/Ratio.h>

#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Output/Graphics/GraphicsScene/GraphicsScene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Output/Graphics/Shape/Mesh/GraphicsMesh/GraphicsMesh.h>

#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Core/Shape/Sphere/Sphere3D/Sphere3D.h>
#include <Engine/Helpers/Scene/Scene.h>




// �����, ����������� ���������������� ����
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: Scene* scene;// ����������� �����
	private: GraphicsMesh<Vector3D, Quaternion, Matrix4x4>* box;// �������
	private: StringANSI currentProjectionType;// ��� �������� ���� ��������
	private: float screenAxisXOffset;// �������� �� �������� ��� X; ��� ����������� ����������� ������, ����� ��� X ������ ������ ������

    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), box(NIL), screenAxisXOffset(0.0f)
	{}

			
	// ����������� ����������
	public: virtual ~Application(void)
	{
		delete scene;
	} 

	// ������� ����������
	public: bool Create(Renderer::Define _define)
	{
		// ������� ����������� ����
		if(Renderer::Create(_define))
		{
			// �����...
			{
				scene = new Scene();
			}


			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.textColor = RGBA(RGBA::WHITE_COLOR);
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d, false, false);
				font2d->Pick();
			}

			// ���...
			{
				GraphicsMesh<Vector3D, Quaternion, Matrix4x4>::Define def;
				def.meshDef.name = "box3d";
				
				
				
				box = new GraphicsMesh<Vector3D, Quaternion, Matrix4x4>();
				box->Create(def);
				box->CreateBox3D(Box3D(Vector3D(0, 0, 0), Size3D(2.5f, 2.5f, 7.0f)));
				
				scene->AddObject(box, false, true);// �������� � �����
			} 

			// ������...
			{
				Camera3D::Define def;
				def.name = "camera";
		
				def.movementType = Camera3D::OVERLAND;
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// ����� �������
				def.pitchMax = 0.0f;// ��������� ������ ���������� ���� ���� (��������� XZ)
			    def.viewingPoint = box->GetPosition();
			    def.zoomMin = 15.0f;
				def.zoomMax = 25.0f;
				def.orthoScale = 5.0;// ������ ������� � ��� ���� ������ ������� box
				


				camera = new Camera3D();
				camera->Create(def);
				camera->Pick("Camera3D.example_05"); 
				scene->AddObject(camera);


				// ������ �������������� ������ ������
				camera->UpdateTrackballObjectViewer(-0.06, 0.1, 0.0f);

				currentProjectionType = "PERSPECTIVE";
			}


			GraphicsPoint<Vector3D>::_SetPointSize(6);// ������ ����� 
			
			Mouse::_ShowCursor(false);// ������ ������
			MoveToCenter();// ����������� ���� � ����� ������
			Mouse::_MoveCursorToCenter(this);// ����������� ������ � ����� ���������� ������� ����
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}
	





	// ������� �����
	public: virtual void Update(float _delta)
	{}


	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����


		if(currentProjectionType.compare("ORTHOGRAPHIC")==0)
		{
			camera->BeginOrtho2D();
		}
		else if(currentProjectionType.compare("PERSPECTIVE")==0)
		{
			camera->BeginPerspective3D();// ������ ����� ���������� �������	
		}
		
		scene->Draw();// ����������� �����
		
		//if(camera->GetProjectionType() == Camera3D::ORTHOGRAPHIC)
		//{
		//	camera->End2D();// ������ ����� ���������� �������	
		//}

		
		
 
		// ����� ������...
		camera->BeginScreen2D();
		

		int i = 0;
		font2d->DrawScreenText(Vector3D(screenAxisXOffset + 10, i+=20, -1), "camera.position: %.2f/%.2f/%.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
		font2d->DrawScreenText(Vector3D(screenAxisXOffset + 10, i+=20, -1), "camera.projection type: %s", currentProjectionType.c_str());
	}

			


	// ������ ������� ����
	public: virtual bool MouseEventFilter(Mouse::Event _event)
	{
		Mouse().ClearEventList();// �������� ����� �������
		return true;
	}


	// ��������� ������� ����������
	public: virtual bool KeyboardEventFilter(Keyboard::Event _event)
	{
		if(_event.type == Keyboard::KEY_DOWN)
		{
			if(_event.key == Keyboard::NUM_1)
			{
				SetWindowStyle(Window::FULLSCREEN);
			}
			else if(_event.key == Keyboard::NUM_2)
			{
				SetWindowStyle(Window::WND_CLASSIC);
			}
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// ���������� ����
			} 
			else if(_event.key == Keyboard::O)
			{
				currentProjectionType = "ORTHOGRAPHIC";
			}
			else if(_event.key == Keyboard::P)
			{
				currentProjectionType = "PERSPECTIVE";
			}
			else if(_event.key == Keyboard::Q)
			{
				camera->SetScreenProjectionOrigin(Camera3D::LEFT_TOP_CORNER);
				screenAxisXOffset = 0;
			}
			else if(_event.key == Keyboard::W)
			{
				camera->SetScreenProjectionOrigin(Camera3D::RIGHT_TOP_CORNER);
				screenAxisXOffset = 240;
			}
			else if(_event.key == Keyboard::A)
			{
				camera->SetScreenProjectionOrigin(Camera3D::LEFT_BOTTOM_CORNER);
				screenAxisXOffset = 0;
			}
			else if(_event.key == Keyboard::S)
			{
				camera->SetScreenProjectionOrigin(Camera3D::RIGHT_BOTTOM_CORNER);
				screenAxisXOffset = 240;
			}
		}

		Keyboard().ClearEventList();// �������� ����� �������
		return true;
	}
};



// ������� ����� ����� � ���������
// ���������� � ������ ������� WINAPI, ���������, ��� ��������� � ������� WinMain ���������� ����� �������
int WINAPI WinMain(HINSTANCE hInstance,// ���������� (�������������), ������������ �������� ��� ������� ����������
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	// �������� ������������ ����
	Renderer::Define def;
	def.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_RESIZING);// ����� ���� 
	def.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
	def.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	def.realTimeWorkerDef.windowDef.tittle = "Camera3D.example_05";
	def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	
	//def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;


	//def.faceDrawMode[Renderer::BACK] = Renderer::POINT;
	//def.faceCullMode[Renderer::FRONT] = false;
	//def.faceCullMode[Renderer::BACK] = false;
	//def.frontFaceClockwise = true;
	//def.realTimeWorkerDef.windowDef.eventBuffering = true;


	Application* app = new Application;// ������� ����
	
	if(app->Application::Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
