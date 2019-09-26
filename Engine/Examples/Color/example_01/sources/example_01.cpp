// Win32.cpp: ���������� ����� ����� ��� ����������.
//


#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Shape/Box/GraphicsBox3DFixed/GraphicsBox3DFixed.h> 
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Output/Graphics/GraphicsScene/GraphicsScene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>





// �����, ����������� ���������������� ����
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: GraphicsScene<Vector3D, Quaternion, Matrix4x4>* scene;// ����������� �����
	private: GraphicsPoint<Vector2D, RGBA> graphicsPoint;// ����������� �����
	private: float sizeOfPoint;// ������ �����
	private: bool randomize;// ���� true, �� ���� ����� ����� �������� �������������

    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), sizeOfPoint(0), randomize(false)
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
			MoveToCenter();// ����������� ���� � ����� �����
			Mouse::_MoveCursorToClientAreaCenter(this);// ����������� ������ � ����� ���������� ������� ����	

			// �����...
			{
				GraphicsScene<Vector3D, Quaternion, Matrix4x4>::Define def;
				scene = new GraphicsScene<Vector3D, Quaternion, Matrix4x4>();
				scene->Create(def);
			}


			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;
				def.textColor = RGBA::GRAY_COLOR;

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d, false, false);
				font2d->Pick();
			}

			
			// ������...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.rotationEnabled = false;


				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, false, false);
				camera->Pick("Color.example_01");
			}

			// ������� �����...
			{
				graphicsPoint.SetPosition(ScreenToClient(ClientAreaCenter()));
				graphicsPoint.SetColor(RGBA::BLACK_COLOR);
				sizeOfPoint = 100.0f;
			}

			SetBackgroundColor(RGBA::WHITE_COLOR);// ����� ���� ����
			
			GraphicsPoint<Vector2D, RGBA>::_SetPointSize(sizeOfPoint);// ������� �����
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}
	

	// ������� �����
	public: virtual void Update(float _delta)
	{
		if(randomize) { graphicsPoint.SetColor(Color<RGBA>::_Random()); } 
	}

	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����




		// ����� ������...
		camera->BeginScreen2D();// ������ ����� ���������� �������

		// ������� �����
		graphicsPoint.Draw();

		int32 i = 0;


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "graphics point.color.key: %u", graphicsPoint.GetColor().Key()); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "graphics point.color[red/green/blue]: %d/%d/%d", graphicsPoint.GetColor()[RGBA::RED], graphicsPoint.GetColor()[RGBA::GREEN], graphicsPoint.GetColor()[RGBA::BLUE]); 

	    //font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "current shape: %s", NameOfShape(curShape).c_str()); 
	



		// ��������������� �����...
		camera->End2D();// ������ ����� ���������� �������
	}


	// ������ ������� ����
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		Mouse().ClearEventList();// �������� ����� �������
	}


	// ��������� ������� ����������
	public: virtual void KeyboardEventFilter(Keyboard::Event _event)
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
			else if(_event.key == Keyboard::R)
			{
				randomize = !randomize;
			}
			else if(_event.key == Keyboard::DOWN_ARROW)
			{
				graphicsPoint.SetColor(RGBA(graphicsPoint.GetColor().Key() - 256));
			}
			else if(_event.key == Keyboard::UP_ARROW)
			{
				graphicsPoint.SetColor(RGBA(graphicsPoint.GetColor().Key() + 256));
			}
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				graphicsPoint.SetColor(RGBA(graphicsPoint.GetColor().Key() - 256 * 256));
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				graphicsPoint.SetColor(RGBA(graphicsPoint.GetColor().Key() + 256 * 256));
			}
		}
		Keyboard().ClearEventList();// �������� ����� �������
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
	def.realTimeWorkerDef.windowDef.tittle = "Color.example_01";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;
	//def.faceDrawMode[Renderer::BACK] = Renderer::LINE;
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
