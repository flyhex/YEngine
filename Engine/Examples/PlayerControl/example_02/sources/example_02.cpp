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
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Core/Interpolation/QuaternionLerp/QuaternionLerp.h>




// ���������������� ����������
class Application: public Engine,// ������
               	   public MessageHandler<int32>::Receiver// ���������� ���������
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D *font;// �����
	private: Scene* scene;// �����
	

    private: PlayerControl *act1, *act2, *act3;// �����������
    private: Mouse::Event *evt5, *evt6;// ������� ����

	private: bool act1Activated, act2Activated, act3Activated;// ���� true, �� ��������������� ���������� ��� �����������
	
	// �������� �������, � ������� �������� ������������ ��������� �� ��������� ����������� act1
    private: float act1Accum;// ���������� ����� ������������
	private: bool act1AccumEnabled;// ���� true, �� ����������� ��������
	private: float act1AccumFinishTimeInSec; // ����� ������ ������������ � ��������

	// �������� �������, � ������� �������� ������������ ��������� �� ��������� ����������� act2
    private: float act2Accum;// ���������� ����� ������������
	private: bool act2AccumEnabled;// ���� true, �� ����������� ��������
	private: float act2AccumFinishTimeInSec; // ����� ������ ������������ � ��������

	// �������� �������, � ������� �������� ������������ ��������� �� ��������� ����������� act3
    private: float act3Accum;// ���������� ����� ������������
	private: bool act3AccumEnabled;// ���� true, �� ����������� ��������
	private: float act3AccumFinishTimeInSec; // ����� ������ ������������ � ��������


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font(NIL), scene(NIL), act1(NIL), act2(NIL), act3(NIL), act1Activated(false), act2Activated(false), act3Activated(false), act1Accum(0.0f), act1AccumEnabled(false), act1AccumFinishTimeInSec(0.8f), act2Accum(0.0f), act2AccumEnabled(false), act2AccumFinishTimeInSec(0.8f), act3Accum(0.0f), act3AccumEnabled(false), act3AccumFinishTimeInSec(0.8f), evt5(NIL), evt6(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete camera;

		delete font;

	    delete evt5;
		delete evt6;

		delete act1;
		delete act2;
		delete act3;


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
				//scene->Rename("scene");// ��� �����
				//AssetLibrary::_GetAssets().AddObject(scene);// �������� ����� � ���������� �������, ����� �� ����������
				scene->Pick();
			}


			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.fontName = "Arial";// ��� ������
				def.fontSize = 16;// ������ ������
				def.color = RGBA(RGBA::WHITE_COLOR);// ���� ������

				font = new Font2D();
				font->Create(def);
				font->Pick();// ������� �����
			}


			// ������...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;
				def.pitchClampEnable = false;// ��������� ���������� �������� ������ ��� X
				
				camera = new Camera3D();
				camera->Set(def);
				
				camera->Pick(); // ������� ������
				scene->BindCamera(camera);
			}
		
			{ 
				// �������� sequence: A + S + Left click
				act1 = new PlayerControl();

				//act1->LoadFromFile("sequence");
				act1->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &Application::Act1Event));
				
			
				Keyboard::Event* evt1 = new Keyboard::Event();
				evt1->key = Keyboard::A;
				evt1->type = Keyboard::KEY_DOWN;
				act1->AddEvent(evt1);

							
				Keyboard::Event* evt2 = new Keyboard::Event();
				evt2->key = Keyboard::A;
				evt2->type = Keyboard::KEY_UP;
				act1->AddEvent(evt2);


				Keyboard::Event* evt3 = new Keyboard::Event();
				evt3->key = Keyboard::S;
				evt3->type = Keyboard::KEY_DOWN;
				act1->AddEvent(evt3);

						
				Keyboard::Event* evt4 = new Keyboard::Event();
				evt4->key = Keyboard::S;
				evt4->type = Keyboard::KEY_UP;
				act1->AddEvent(evt4);


				evt5 = new Mouse::Event();
				evt5->key = Mouse::LEFT_BUTTON;
				evt5->type = Mouse::KEY_DOWN;
				act1->AddEvent(evt5);

								
				evt6 = new Mouse::Event();
				evt6->key = Mouse::LEFT_BUTTON;
				evt6->type = Mouse::KEY_UP;
				act1->AddEvent(evt6);

				act1->SetTimingInSec(1.0f);// ������� 1 �������
				act1->Rename("sequence");
				act1->SetActivationMode(PlayerControl::SEQUENCE);
				scene->AddObject(act1);
				
				act1->SaveToFile();
			
				
			}

			{
				// �������� combo: Q + W
				act2 = new PlayerControl();

				//act2->LoadFromFile("combo");
				act2->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &Application::Act2Event));

			
				Keyboard::Event* evt1 = new Keyboard::Event();
				evt1->key = Keyboard::Q;
				evt1->type = Keyboard::KEY_DOWN;
				act2->AddEvent(evt1);

				Keyboard::Event* evt2 = new Keyboard::Event();
				evt2->key = Keyboard::W;
				evt2->type = Keyboard::KEY_DOWN;
				act2->AddEvent(evt2);


				//act2->SetTiming(1.0f);// ������� 1 �������
				act2->Rename("combo");
				act2->SetActivationMode(PlayerControl::COMBO);
				scene->AddObject(act2);
				 
				act2->SaveToFile();
			}
				
			{
				// �������� single: Space ��� Enter (�������������� �������)
				act3 = new PlayerControl();

				//act3->LoadFromFile("single");
				act3->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &Application::Act3Event));

		
				
				Keyboard::Event* evt1 = new Keyboard::Event();
				evt1->key = Keyboard::SPACE;
				evt1->type = Keyboard::KEY_UP;
				act3->AddEvent(evt1);

				Keyboard::Event* evt2 = new Keyboard::Event();
				evt2->key = Keyboard::ENTER;
				evt2->type = Keyboard::KEY_PRESSED;
				act3->AddEvent(evt2);

				act3->Rename("single");
				act3->SetActivationMode(PlayerControl::SINGLE);
				scene->AddObject(act3);

				act3->SaveToFile();
			}


			// ������ ����������
			//Keyboard().SetKeyPressedPeriodInSec(Keyboard().GetKeyPressedPeriodInSec() / 6.0f);
			
			// �����...
			GraphicsLine::_SetLineWidth(3.0f);// ������� �����


			// ���� ����...
			SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));

			MoveToCenter();// ����������� ���� � ����� ������
			//Mouse::_SetShowCursor(false);// ������ ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}



	// ������� �� ��������� ����������� act1
	public: void Act1Event(PlayerControl* _handler)
	{
		act1Activated = true;
	}
	

	// ������� �� ��������� ����������� act2
	public: void Act2Event(PlayerControl* _handler)
	{
		act2Activated = true;
	}


	// ������� �� ��������� ����������� act3
	public: void Act3Event(PlayerControl* _handler)
	{
		act3Activated = true;
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
		camera->BeginTransform(Renderer::PROJECTION);
		camera->BeginTransform(Renderer::MODELVIEW);


		// ����� ������...
		int32 i = 0;
	

		if(act1Activated)
		{
			act1Activated = false;
						
			// ��������� ������
			act1Accum = act1AccumFinishTimeInSec;// ����� ������ ������������ � ��������
			act1AccumEnabled = true; // �������� �����������
		}
			
		if(act2Activated)
		{
			act2Activated = false;
						
			// ��������� ������
			act2Accum = act2AccumFinishTimeInSec;// ����� ������ ������������ � ��������
			act2AccumEnabled = true; // �������� �����������
		}

		if(act3Activated)
		{
			act3Activated = false;
			
			// ��������� ������
			act3Accum = act3AccumFinishTimeInSec;// ����� ������ ������������ � ��������
			act3AccumEnabled = true; // �������� �����������
		}


		if(act1AccumEnabled) { act1Accum -= _delta; } // ��������� �����������
		if(act1Accum > 0.0f)// ���� ����������� �� ��������
		{
			font->DrawScreenText(Vector3(10, 70, -1), "%s", act1->GetName().c_str());
		}
		else if(act1AccumEnabled)// ���� ����������� ��������
		{
			act1AccumEnabled = false;// ��������� �����������
		}


		if(act2AccumEnabled) { act2Accum -= _delta; } // ��������� �����������
		if(act2Accum > 0.0f)// ���� ����������� �� ��������
		{
			font->DrawScreenText(Vector3(10, 90, -1), "%s", act2->GetName().c_str());
		}
		else if(act2AccumEnabled)// ���� ����������� ��������
		{
			act2AccumEnabled = false;// ��������� �����������
		}


		if(act3AccumEnabled) { act3Accum -= _delta; } // ��������� �����������
		if(act3Accum > 0.0f)// ���� ����������� �� ��������
		{
			font->DrawScreenText(Vector3(10, 110, -1), "%s", act3->GetName().c_str());
		}
		else if(act3AccumEnabled)// ���� ����������� ��������
		{
			act3AccumEnabled = false;// ��������� �����������
		}

		font->DrawScreenText(Vector3(10, i+=20, -1), "%s timing: %f", act1->GetName().c_str(), act1->GetRemainedTimeInSec()); 
		font->DrawScreenText(Vector3(10, i+=20, -1), "%s timing: %f", act2->GetName().c_str(), act2->GetRemainedTimeInSec());

		//font->DrawScreenText(Vector3(10, i+=20, -1), "quaternion: %d/%d/%d/%d", point->uparrow, point->downarrow, point->leftarrow, point->rightarrow);

		/*
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.buildIn: %d", qLerp->IsFloatLerpBuildInEnabled());

		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.mode: %d", qLerp->GetFloatLerp()->GetLerpMode());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.playing duration: %f", qLerp->GetFloatLerp()->GetPlayingDurationInSec());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.playing speed: %f", qLerp->GetFloatLerp()->GetPlayingSpeed());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.playing: %d", qLerp->GetFloatLerp()->IsPlaying());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.target: %f", qLerp->GetFloatLerp()->GetTarget());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.current: %f", qLerp->GetFloatLerp()->GetCurrent());


		*/ 
		
		// ������������ ���������� �������������...
		camera->EndTransform(Renderer::PROJECTION);
		camera->EndTransform(Renderer::MODELVIEW);
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
			else if(_event->key == Keyboard::DEL)
			{
				//act1->Destroy();
				delete evt5;
				act1->GetEvents().Remove(evt6, MagicContainer::REMOVE_NODE);
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "PlayerControl.example_02";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
