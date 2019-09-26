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
#include <Engine/Core/Time/Timer/SyncTimer/SyncTimer.h>
#include <Engine/Helpers/Sprite/AnimatedSprite/AnimatedSprite.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/MovingPoint/MovingPoint.h>


// �����, ����������� ���������� ������ (������� �������������)
class Sprite3D: public AnimatedSprite<Vector3D, Quaternion, Matrix4x4>,// ������
				public TimingHandler//���������� ��������

{
	// METHODS
	// ����������� �� ���������
	public: Sprite3D(void)
	{
		GraphicsMesh::Define def;
		Create(def);
	}


	// ����������� ����������
	public: virtual ~Sprite3D(void)
	{
		WinAPI::_MessageBox("","~Sprite3D");
	}
	
				
	// ��������� ��������� ��������
	// ����������: � �������� ��������� ����������� ������ - �����, ��������� � ���������� ������ ���� �������
	public: virtual void Update(float _delta = 0.0f)
	{
		AnimatedSprite::Update(_delta);
		TimingHandler::Update(_delta);
	}
};



// �����, ����������� ���������� ������ (������� �������������)
class Sprite2D: public AnimatedSprite<Vector2D, float, Matrix4x4>,// ������
				public TimingHandler//���������� ��������

{
	// METHODS
	// ����������� �� ���������
	public: Sprite2D(void)
	{
		GraphicsMesh::Define def;
		Create(def);
	}


	// ����������� ����������
	public: virtual ~Sprite2D(void)
	{
		WinAPI::_MessageBox("","~Sprite2D");
	}




				
	// ��������� ��������� ��������
	// ����������: � �������� ��������� ����������� ������ - �����, ��������� � ���������� ������ ���� �������
	public: virtual void Update(float _delta = 0.0f)
	{
		AnimatedSprite::Update(_delta);
		TimingHandler::Update(_delta);
	}
};


// �����, ����������� ���������������� ����
class Application: public Engine
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: Scene *scene2d, *scene3d;// ����������� �����
	private: Sprite3D* sprite3d;// ���������� ������
	private: Sprite2D* sprite2d;// ���������� ������
	private: TexturedAnimation* anim;// ���������� ��������

	
    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene2d(NIL), sprite3d(NIL), sprite2d(NIL), anim(NIL)
	{}



	// ������� ���������������� �������
	public: virtual void DestroyUserResources(void)
	{
		delete scene2d;
		delete scene3d;
	}


	// ������� ���������������� ������� � ���������� true � ������ ������
	public: virtual bool CreateUserResources(void)
	{
		// �����...
		{	
			scene3d = new Scene();
		}
	
		// �����...
		{	
			scene2d = new Scene();
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
			scene2d->AddObject(font2d);
			font2d->Pick();
		}


		// ���������� ��������...
		{
			anim = new TexturedAnimation();
			anim->Rename("anim");
			anim->LoadFromFile("run_right_anim"); // ��������� ��������
		}
			
						
		// ���������� ������...
		{
			sprite3d = new Sprite3D();  
			sprite3d->SetPosition(Vector3D(0,0,-1));
			sprite3d->Rename("sprite3d");

			
			sprite3d->BindAnimation(anim);
				
			anim->Play();// �������� ������������ ��������

			sprite3d->CreateQuad2D(Quad<Vector3D>(Vector3D(), anim->CurrentFrameSize()));// ������� ���� 

				
			sprite3d->SetAnimatedMaterialEnable(true);// �������� ��������
				
		
			scene3d->AddObject(sprite3d);// �������� � �����
			scene3d->AddGraphicsObject(sprite3d);
			scene3d->AddLogicObject(sprite3d);
		}

		// ���������� ������...
		{
			sprite2d = new Sprite2D();  
			sprite2d->SetPosition(Vector2D(350, 300));
			sprite2d->Rename("sprite2d");

			
			sprite2d->BindAnimation(anim);
				
			anim->Play();// �������� ������������ ��������

			sprite2d->CreateQuad2D(ScreenQuad(Vector2D(), anim->CurrentFrameSize()));// ������� ���� 

				
			sprite2d->SetAnimatedMaterialEnable(true);// �������� ��������
				
			
			scene2d->AddObject(sprite2d);// �������� � �����	 
			scene2d->AddGraphicsObject(sprite2d);
			scene2d->AddLogicObject(sprite2d);
		}


		// ������...
		{
			Camera3D::Define def;
			def.name = "camera";
			def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// ����� �������
			//def.pitchMax = 0.0f;// ��������� ������ ���������� ���� ���� (��������� XZ)
			//def.projectionType = Camera3D::ORTHOGRAPHIC;
			def.viewingPoint = sprite3d->GetPosition();
			//def.rotationEnabled = false;
			
			//def.zoomMin = 15.0f;
			//def.zoomMax = 25.0f;
			//def.orthoScale = 100.0*2;// ������ ������� � ��� ���� ������ ������� box


			
			camera = new Camera3D();
			camera->Create(def);
			camera->Pick(Title()); 
			camera->SetOrthoScale(100.0*2.0*camera->AspectRatio());

			// ������ ����������� � ������������ ��� ������
			float maxSize = Common::_Max(anim->CurrentFrameSize().width, anim->CurrentFrameSize().height);		
			camera->SetZoomMax(maxSize + Angle::_RadiansToDegrees(camera->Fov()));
			camera->SetZoomMin(maxSize * 0.1);
			camera->SetZoomSpeed(maxSize/10);// ������ �������� ������� 1�10
			camera->SetZFar(maxSize + Angle::_RadiansToDegrees(camera->Fov()) + 100);// ������ ��������� ���������� ������
			camera->SetDistanceToViewingPoint(camera->ZoomMax());// ������ ���������� �� ������������� �������

			scene2d->AddObject(camera);
		}

		

		//Mouse::_ShowCursor(false);// ������ ������
		return true;// ������� �����
	}


		


	// ������� �����
	public: virtual void Update(float _delta)
	{
		scene2d->Update(_delta); 
		scene3d->Update(_delta); 
		Trigger::_ExecuteActions();
	}


	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����
		
		
		/* ������� ������������ �������� ����� �������� */ 
		// ����� ���������� ����� � ������������� ��������...
		camera->BeginPerspective3D();// ������ ����� ���������� �������
		scene3d->Draw();


	
		// ����� ���������� ����� � ��������������� ��������...
		camera->BeginOrtho2D();
		//camera->BeginPerspective3D();// ������ ����� ���������� �������
		scene3d->Draw();


		// ����� ���������� ����� � �������� ��������...
		camera->BeginScreen2D();// ������ ����� ���������� �������
		scene2d->Draw();
	
		

		// ����� ������...
		//camera->BeginScreen2D();// ������ ����� ���������� �������

		int32 i = 0;
		
		
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.zoom: %.2f", camera->DistanceToViewingPoint()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.zoom min/max: %.2f/%.2f", camera->ZoomMin(), camera->ZoomMax()); 

	    //font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.zoom speed: %.2f", camera->ZoomSpeed()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sprite3d.position: %.2f/%.2f/%.2f", sprite3d->GetPosition().x, sprite3d->GetPosition().y, sprite3d->GetPosition().z); 
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
	def.rendererDef.realTimeWorkerDef.windowDef.tittle = "2D Graphics.example_02";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;
	//def.faceDrawMode[Renderer::BACK] = Renderer::LINE;
	//def.faceCullMode[Renderer::FRONT] = false;
	//def.faceCullMode[Renderer::BACK] = false;
	//def.zBufferEnabled = false;
	//def.blendEnable = true;// �������� ����������
	
	//def.rendererDef.frontFaceClockwise = true;
	
	//def.realTimeWorkerDef.windowDef.eventBuffering = true;
	//def.vSync = false;
	//def.drawCallController.SetMode(CallController::UNLIM);

	
	Application* app = new Application;// ������� ����
	
	app->Create(def);

	app->Loop();

	delete app;
	return 0;
}
