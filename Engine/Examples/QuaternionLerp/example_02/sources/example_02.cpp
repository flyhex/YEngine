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
	private: Scene* scene, *scene2d;// �����
    private: Camera3D* camera, *camera2d;// ������
    private: Font2D *font;// �����
	private: GraphicsSurface* box;// ���������� �������
    private: FloatLerp* fl;// �������� ������������
	private: QuaternionLerp* ql;// ������������� ������������
	private: float time;// ������������ ������������ � ��������


    // METHODS
    // ���������������� �����������
	public: Application(void): scene(NIL), scene2d(NIL), camera(NIL), camera2d(NIL), font(NIL), box(NIL), fl(NIL), ql(NIL), time(0.0f)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete font;

		delete box;

		delete fl;
		delete ql;

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
				def.color = RGBA(RGBA::WHITE_COLOR).GetRGBAf();// ���� ������

				font = new Font2D();
				font->Create(def);
				font->Pick();// ������� �����
			}

			{// �������� ������������
				fl = new FloatLerp();
				//fl->SetPlayingMode(PlayingObject::BACKWARD_PLAYING);
				//fl->SetPlayingDurationInSec(10.0f);
				fl->SetTarget(10.0f);
				fl->SetPlayingSpeed(0.01f);
				fl->SetLerpMode(FloatLerp::PARABOLA);

				
				scene->AddToUpdateList(fl);
			}

			{// ������������� ������������
				ql = new QuaternionLerp();
				ql->BindFloatLerp(fl);
				ql->GetControlQuaternions().AddToTail(Quaternion());
				ql->GetControlQuaternions().AddToTail(Quaternion(Matrix4x4::_RotationZ(Angle::HALF_PI)));
				ql->GetControlQuaternions().AddToTail(Quaternion(Matrix4x4::_RotationY(Angle::HALF_PI)) * Quaternion(Matrix4x4::_RotationZ(Angle::HALF_PI)));
				ql->GetControlQuaternions().AddToTail(Quaternion(Quaternion(Matrix4x4::_RotationX(Angle::HALF_PI)) * Quaternion(Matrix4x4::_RotationY(Angle::HALF_PI)) * Matrix4x4::_RotationZ(Angle::HALF_PI)));
			}
			
			{// �������
				box = new GraphicsSurface();

				Mesh* mesh = new Mesh();
				mesh->CreateBox3D(Box(Vector3(), Vector3(0.5, 0.5, 0.5)));
				
				box->BindMesh(mesh);

				box->SetColor4(RGBA(RGBA::WHITE_COLOR).GetRGBAf());
				scene->AddToRenderList(box);
			}


			// ���� ����...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR).GetRGBAf());

			SetWireframeEnable(true);// �������� ��������� ����� ������������

			MoveToCenter();// ����������� ���� � ����� ������
			//Mouse::_SetShowCursor(false);// ������ ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}


	// ���������� ������� ����� ��������������� � ���� ������
	public: StringANSI GetPlayingModeString(int32 _mode)const
	{
		StringANSI str;

		if(_mode & PlayingObject::FORWARD_PLAYING)
		{
			str += "FORWARD_PLAYING | ";
		}
		if(_mode & PlayingObject::BACKWARD_PLAYING)
		{
			str += "BACKWARD_PLAYING | ";
		}
		if(_mode & PlayingObject::SINGLE_SHOT_PLAYING)
		{
			str += "SINGLE_SHOT_PLAYING | ";
		}
		if(_mode & PlayingObject::LOOP_PLAYING)
		{
			str += "LOOP_PLAYING | ";
		}
		if(_mode & PlayingObject::PING_PONG_PLAYING)
		{
			str += "PING_PONG_PLAYING";
		}
		if(_mode & PlayingObject::ONEWAY_PLAYING)
		{
			str += "ONEWAY_PLAYING";
		}
		return str;
	}

	// ���������� ��������� ������������� ������ �����������
	public: StringANSI LerpModeToString(int32 _mode)
	{
		switch(_mode)
		{
			case FloatLerp::UNITS_PER_SECOND:
			{
				return "UNITS_PER_SECOND";
			}

			case FloatLerp::PARABOLA:
			{
				return "PARABOLA";
			}

			case FloatLerp::FIXED_TIME:
			{
				return "FIXED_TIME";
			}

			case FloatLerp::DAMPING:
			{
				return "DAMPING";
			}
			
			case FloatLerp::MAGNETIC:
			{
				return "MAGNETIC";
			}
		}
		return "";
	}


	// ������� ����������
	public: virtual void Update(float _delta)
	{
		Engine::Update(_delta);

		box->SetRotation(ql->GetCurrent());
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

		font->DrawScreenText(Vector3(10, i+=20, -1), "f-lerp playing: %d", fl->IsPlaying());
		font->DrawScreenText(Vector3(10, i+=20, -1), "f-lerp current: %f", fl->GetCurrent());
		font->DrawScreenText(Vector3(10, i+=20, -1), "f-lerp delta: %f", fl->GetDelta());
		font->DrawScreenText(Vector3(10, i+=20, -1), "f-lerp t: %f", fl->GetT());

		font->DrawScreenText(Vector3(10, i+=20, -1), "f-lerp mode: %s", LerpModeToString(fl->GetLerpMode()).c_str());
		font->DrawScreenText(Vector3(10, i+=20, -1), "playing mode: %s", GetPlayingModeString(fl->GetPlayingMode()).c_str()); 

	
		//if(fl->IsPlaying()) { time += _delta; }// ���� ������������ ���������������

		font->DrawScreenText(Vector3(10, i+=20, -1), "time: %f", fl->GetElapsedTimeInSec()); 



		// ������������ ���������� �������������...
		camera2d->EndTransform(Renderer::MODELVIEW);
		camera2d->EndTransform(Renderer::PROJECTION);
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
			else if(_event->key == Keyboard::P)
			{
				if(fl->IsPlaying()) 
				{
					fl->Pause(); 
				}
				else
				{
					if(fl->IsFinish()) 
					{ 
						fl->Stop(); 
						time = 0.0f;
					} 
					fl->Play();
				}
			}
			else if(_event->key == Keyboard::B)
			{
				fl->SetPlayingMode(PlayingObject::BACKWARD_PLAYING);
			}
			else if(_event->key == Keyboard::F)
			{
				fl->SetPlayingMode(PlayingObject::FORWARD_PLAYING);
			}
			else if(_event->key == Keyboard::L)
			{
				fl->SetPlayingMode(PlayingObject::LOOP_PLAYING);
			}
			else if(_event->key == Keyboard::S)
			{
				fl->SetPlayingMode(PlayingObject::SINGLE_SHOT_PLAYING);
			}
			else if(_event->key == Keyboard::G)
			{
				fl->SetPlayingMode(PlayingObject::PING_PONG_PLAYING);
			}
			else if(_event->key == Keyboard::O)
			{
				fl->SetPlayingMode(PlayingObject::ONEWAY_PLAYING);
			}
			else if(_event->key == Keyboard::NUM_1)
			{
				fl->SetLerpMode(FloatLerp::PARABOLA);
				time = 0.0f;
				fl->SetPlayingSpeed(0.001f);
			}
			else if(_event->key == Keyboard::NUM_2)
			{
				fl->SetLerpMode(FloatLerp::FIXED_TIME);
				time = 0.0f;
				fl->SetPlayingDurationInSec(3.0f);
			}
			else if(_event->key == Keyboard::NUM_3)
			{
				fl->SetLerpMode(FloatLerp::UNITS_PER_SECOND);
				time = 0.0f;
				fl->SetPlayingDurationInSec(2.0f);
			}
			else if(_event->key == Keyboard::NUM_4)
			{
				fl->SetLerpMode(FloatLerp::DAMPING);
				time = 0.0f;
				fl->SetPlayingSpeed(2.15f);
			}
			else if(_event->key == Keyboard::NUM_5)
			{
				fl->SetLerpMode(FloatLerp::MAGNETIC);
				time = 0.0f;
				fl->SetPlayingSpeed(2.0f);
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
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);// ������ ����
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	def.rendererDef.realTimeWorkerDef.windowDef.title = "QuaternionLerp.example_02";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
