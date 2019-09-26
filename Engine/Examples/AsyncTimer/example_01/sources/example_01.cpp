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
#include <Engine/Output/Graphics/Shape/Mesh/GraphicsMesh/GraphicsMesh.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Output/Graphics/Material/AnimatedMaterial/AnimatedMaterial.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Helpers/Sprite/Sprite.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/Trigger/Trigger.h>




// �����, ����������� ���������������� ����
class Application: public Engine
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: Scene* scene;// �����
	private: AsyncTimer timer;// ���������� ������
	

    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL)
	{}

			

	// ������� ���������������� �������
	public: virtual void DestroyUserResources(void)
	{
		delete scene;
	}


	// ������� ���������������� ������� � ���������� true � ������ ������
	public: virtual bool CreateUserResources(void)
	{
		// �����...
		{	
			scene = new Scene();
		}


		// �����...
		{
			Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

			Font2D::Define def;
			def.name = "font2d";
			def.fontName = "Haettenschweiler";
			def.fontSize = 20;
			def.textColor.SetRGBAi(RGBA::WHITE_COLOR);

			font2d = new Font2D();
			font2d->Create(def);
			scene->AddObject(font2d);
			font2d->Pick();
		}


	

		// ������...
		{
			Camera3D::Define def;
			def.name = "camera";
			def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// ����� �������
			def.rotationEnabled = false;


			camera = new Camera3D();
			camera->Create(def);
			camera->Pick(Tittle()); 

			
			scene->AddObject(camera);
		}
		
		// ������
		{
			timer.StartSec(3.0f);
		}


		//Mouse::_ShowCursor(false);// ������ ������
		return true;// ������� �����
	}


		
	// ���������� ����� ������� � ���� ���������� ������
	public: StringANSI NameOfMode(AsyncTimer::Mode _mode)
	{
		switch(_mode)
		{
			case AsyncTimer::SINGLE_SHOT:
			{
				return "SINGLE_SHOT";
			}
			case AsyncTimer::EVER_SHOT:
			{
				return "EVER_SHOT";
			}
			case AsyncTimer::INTERVAL:
			{
				return "INTERVAL";
			}
		}
		return "";
	}

	// ������� �����
	public: virtual void Update(float _delta)
	{
		scene->Update(_delta);
	}


	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����

	
		// ����� ������...
		camera->BeginPerspective3D();// ������ ����� ���������� �������

		scene->Draw();
	
	
		// ����� ������...
		camera->BeginScreen2D();// ������ ����� ���������� �������

		int32 i = 0;
		
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "timer.IsPaused: %d", timer.IsPaused()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "timer.ElapsedTime: %.2f", timer.ElapsedTimeInSec()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "timer.RemainedTime: %.2f", timer.RemainedTimeInSec()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "timer.FinishTime: %.2f", timer.FinishTimeInSec()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "timer.Mode: %s", NameOfMode(timer.CurrentMode()).c_str()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "timer.Check: %d", timer.Check()); 
		
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
			if(_event.key == Keyboard::F1)
			{
				SetWindowStyle(Window::FULLSCREEN);
			}
			else if(_event.key == Keyboard::F2)
			{
				SetWindowStyle(Window::WND_CLASSIC);
			}
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// ���������� ����
			} 
			else if(_event.key == Keyboard::P)
			{
				if(timer.IsPaused())
				{
					timer.Resume();
				}
				else 
				{
					timer.Pause();
				}
			}
			else if(_event.key == Keyboard::R)
			{
				timer.Restart();
			}
			else if(_event.key == Keyboard::C)
			{
				timer.Reset();
			}
			else if(_event.key == Keyboard::S)
			{
				timer.StartMs(1000);
			}
			else if(_event.key == Keyboard::NUM_1)
			{
				timer.SetMode(AsyncTimer::SINGLE_SHOT);
			}
			else if(_event.key == Keyboard::NUM_2)
			{
				timer.SetMode(AsyncTimer::EVER_SHOT);
			}
			else if(_event.key == Keyboard::NUM_3)
			{
				timer.SetMode(AsyncTimer::INTERVAL);
			}
			else if(_event.key == Keyboard::F)
			{
				timer.SetFinishTimeInSec(1.0f);
			}
			else if(_event.key == Keyboard::L)
			{
				AsyncTimer::Define def(4, 1, AsyncTimer::INTERVAL, false);
				timer.Create(def);
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
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_RESIZING);// ����� ���� 
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	def.rendererDef.realTimeWorkerDef.windowDef.tittle = "AsyncTimer.example_01";


	Application* app = new Application();// ������� ����
	
	app->Create(def);

	app->Loop();

	delete app;
	return 0;
}
