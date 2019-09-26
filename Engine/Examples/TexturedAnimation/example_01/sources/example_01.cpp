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
#include <Engine/Core/InputHandler/InputHandler.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>




// ���������������� ����������
class Application: public Engine,// ������
         	       public MessageHandler<int32>::Receiver// ���������� ���������
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D *font;// �����
	private: Scene* scene;// �����
	private: TexturedMesh* sprite;// ���������� ���������������� ����
	private: Atlas2D* atlas;// ��������� �� �����
	private: TexturedAnimation* anim;// ��������� �� ��������


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font(NIL), scene(NIL), sprite(NIL), atlas(NIL), anim(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{

		delete font;
		delete sprite;
		delete scene;
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

			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.fontName = "Arial";// ��� ������
				def.fontSize = 16;// ������ ������
				def.fontWeight = 600;
				def.color = RGBA(RGBA::GRAY_COLOR);// ���� ������

				font = new Font2D();
				font->Create(def);
				font->Pick();// ������� �����
			}


			// �����...
			{
				atlas = new Atlas2D();
				atlas->Rename("atl");
				atlas->LoadAnimationFromFolder("test");// ��������� ����� ��� �������� �� ����� test
				//scene->AddObject(atlas);// �������� � �����
				AssetLibrary::_GetAssets().AddObject(atlas);
			}
			

			// ���������� ��������...
			{
				anim = new TexturedAnimation();
				anim->Rename("anm");
				anim->BindAtlas(atlas);// ��������� ����� � ��������
				anim->SetFPS(10.0f);
				anim->Play();// �������� ��������������� ��������
				//scene->AddObject(anim);// �������� � �����
				anim->SetAtlasBuildInEnable(true);
				scene->AddLogicObject(anim);
			}

						
			// ������������� �����...
			{
				sprite = new TexturedMesh();  
				
				sprite->CreateQuad2D(Quad(Vector3(), Vector2(200.0f, 200.0f)));// ������� ���� �������� 1x1
				
				sprite->ConvertTo2DObject();
				
				
				sprite->SetPosition(Vector3(400, 300, -0.1f));// ���������� ���� ������ ������ �� ��� Z
				sprite->BindAnimation(anim);// ��������� �������� � ������������� �����
				
				//sprite->SetColor(RGBA(RGBA::RED_COLOR));
				//sprite->Rename("sprite");
  
				//sprite->ResizeTexels(4);// ������� 4 ������� ��� �������� ������ �����, �������� ����� ����� (����������������)
				
				
				
				scene->AddGraphicsObject(sprite);
			}

			// ������...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;
				def.rotationEnable = false;// ��������� �������� ������
				//def.yawClampEnable = true;
				//def.pitchClampEnable = true;
				//def.zoomSpeed = 3.0f;

				//def.pitchMax = Angle::_DegreesToRadians(40);
				//def.pitchMin = Angle::_DegreesToRadians(-40);
				//def.yawMax = Angle::_DegreesToRadians(40);
				//def.yawMin = Angle::_DegreesToRadians(-40);


				camera = new Camera3D();
				camera->Create(def);

				// ������ ����������� � ������������ ��� ������
				//camera->SetNearZoom(camera->GetNearZoom() * 100);
				//camera->SetFarZoom(camera->GetFarZoom() * 100);
				//camera->SetDistanceToViewingPoint(140.0f); 

				camera->Pick(); // ������� ������
				scene->BindCamera(camera);
			}



			// ���� ����...
			SetBackgroundColor(RGBA(RGBA::WHITE_COLOR));
			
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
		camera->BeginTransform(Renderer::PROJECTION);
		camera->BeginTransform(Renderer::MODELVIEW);


		// ����� ������...
		int32 i = 0;

			
		font->DrawScreenText(Vector3(10, i+=20, -1), "amount of frame: %d", sprite->GetAnimation()->GetAmountOfFrames()); 
		font->DrawScreenText(Vector3(10, i+=20, -1), "current frame: %d", sprite->GetAnimation()->GetCurrentFrame()); 
		font->DrawScreenText(Vector3(10, i+=20, -1), "is playing: %d", sprite->GetAnimation()->IsPlaying()); 

		font->DrawScreenText(Vector3(10, i+=20, -1), "current frame size: %.2f/%.2f", sprite->GetAnimation()->GetCurrentFrameSize().x, sprite->GetAnimation()->GetCurrentFrameSize().y); 

		font->DrawScreenText(Vector3(10, i+=20, -1), "frame duration: %.2f", sprite->GetAnimation()->GetPlayingFrameDurationInSec()); 
		font->DrawScreenText(Vector3(10, i+=20, -1), "total playing duration: %.2f", sprite->GetAnimation()->GetPlayingDurationInSec()); 
		font->DrawScreenText(Vector3(10, i+=20, -1), "fps: %.2f", sprite->GetAnimation()->GetFPS()); 

		
		font->DrawScreenText(Vector3(10, i+=20, -1), "playing mode: %s", GetPlayingModeString(sprite->GetAnimation()->GetPlayingMode()).c_str()); 


		//font->DrawScreenText(Vector3(10, ClientAreaSize().y - (i+=20), -1), "scale(x,y): %.2f/%.2f", sprite->GetScale().x, quad->GetScale().y); 



	

		// ������������ ���������� �������������...
		camera->EndTransform(Renderer::PROJECTION);
		camera->EndTransform(Renderer::MODELVIEW);
	}



	// ������ ������� ����
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Engine::MouseEventFilter(_event);

		if(_event->type == Mouse::WHEEL)
		{
			sprite->GetAnimation()->SetPlayingDurationInSec(sprite->GetAnimation()->GetPlayingDurationInSec() + _event->wheel);
		}

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
				Destroy();// ���������� ����
			} 
			else if(_event->key == Keyboard::P)
			{
				if(sprite->GetAnimation()->IsPlaying())
				{
					sprite->GetAnimation()->Pause();
				}
				else 
				{
					sprite->GetAnimation()->Play();
				}
			}
			else if(_event->key == Keyboard::S)
			{
				sprite->GetAnimation()->Stop();
			}
			else if(_event->key == Keyboard::NUM_1)
			{
				sprite->GetAnimation()->SetPlayingMode(PlayingObject::FORWARD_PLAYING);
			}
			else if(_event->key == Keyboard::NUM_2)
			{
				sprite->GetAnimation()->SetPlayingMode(PlayingObject::BACKWARD_PLAYING);
			}
			else if(_event->key == Keyboard::NUM_2)
			{
				sprite->GetAnimation()->SetPlayingMode(PlayingObject::PING_PONG_PLAYING);	
			}
			else if(_event->key == Keyboard::NUM_4)
			{
				sprite->GetAnimation()->SetPlayingMode(PlayingObject::ONEWAY_PLAYING);
			}
			else if(_event->key == Keyboard::NUM_5)
			{
				sprite->GetAnimation()->SetPlayingMode(PlayingObject::LOOP_PLAYING);
			}
			else if(_event->key == Keyboard::NUM_6)
			{
				sprite->GetAnimation()->SetPlayingMode(PlayingObject::SINGLE_SHOT_PLAYING);
			}
			else if(_event->key == Keyboard::L)
			{
				anim->SaveToFile();

				//atlas->SaveToFile();
				//atlas->Destroy();
				//atlas->LoadFromFile("atl");

	
				anim->Destroy();
				anim->LoadFromFile("anm");
			}
		}

		return Engine::KeyboardEventFilter(_event);
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "TexturedAnimation.example_01";
	//def.rendererDef.zBufferEnabled = false;
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
