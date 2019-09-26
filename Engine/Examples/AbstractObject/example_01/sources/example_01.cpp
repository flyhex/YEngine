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
#include <Engine/Helpers/Sprite/TexturedSprite/TexturedSprite.h>
#include <Engine/Helpers/Sprite/AnimatedSprite/AnimatedSprite.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Math/Vector/Vector3D/Vector3D.h>



// �����, ����������� ���������������� ����
class Application: public Engine
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: Scene* scene;// ����������� �����
	private: Object::ObjectType objectObjectType, sceneObjectType;// ���� ��������
	private: Object* sprite;// ������������� ������

	

    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), sprite(NIL)
	{}


	// ����������� ����������
	public: virtual ~Application(void)
	{}


	// ���������� ���������������� ������� 
	public: virtual void DestroyUserResources(void)
	{}
	

	// ������� ���������������� ������� � ���������� true � ������ ������
	public: virtual bool CreateUserResources(void)
	{
		// �����...
		{
			/*
			// ��� �������� �������
			sceneObjectType = Object::_CreateObjectType(Object::OBJECT_CLASS | ObjectScene::OBJECT_SCENE_CLASS | LogicScene::LOGIC_SCENE_CLASS | Scene::SCENE_CLASS | LogicScene::LOGIC_OBJECT_CLASS | GraphicsObject::GRAPHICS_OBJECT_CLASS | GraphicsScene::GRAPHICS_SCENE_CLASS);
			scene = dynamic_cast<Scene*>(Object::_CreateObject(sceneObjectType));
			
			if(scene) // ���� ������� ������� ������
			{
				WinAPI::_MessageBox("","Scene is created");
				scene->Rename("scene"); // ��� �����
				AssetLibrary::_GetAssets().AddObject(scene);// �������� ����� � ���������� �������, ����� �� ����������
				scene->SaveToFile();// ��������� � ����
			}
			*/ 
		
			


	
			// ��� ��������
			scene = dynamic_cast<Scene*>(AssetLibrary::_LoadCommonAsset("scene", AssetLibrary::OBJECT_ASSET));

		
		}


		// ������������� ������...
		{

			/*
			// ��� �������� �������
			objectObjectType = Object::_CreateObjectType(IAnimatedSprite::ANIMATED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS);
			sprite = Object::_CreateObject(objectObjectType);
		    
			if(sprite)// ���� ������� ������� ������
			{
				WinAPI::_MessageBox("","Animated sprite is created");
				sprite->Rename("sprite");
				AssetLibrary::_GetAssets().AddObject(sprite);

				sprite->SaveToFile();// ��������� ������ � ����
			}
			*/ 

		

	
		
		
			// ��� �������� �������
			sprite = AssetLibrary::_LoadCommonAsset<Object>("sprite");


			AssetLibrary::_LoadCommonAsset<TexturedAnimation>("run_right_anim");
			AssetLibrary::_LoadCommonAsset<Atlas2D>("run_right_source");
		 


			// ��� ����� �������
			AnimatedSprite<Vector3D, Quaternion, Matrix4x4>* it = dynamic_cast<AnimatedSprite<Vector3D, Quaternion, Matrix4x4>*>(sprite);


			if(it)// ���� ������� ��������� ������
			{
				it->CreateQuad2D(Quad<Vector3D>(Vector3D(), Vector2D(100, 100)));// ������� ���� 
					
				it->SetAnimatedMaterialEnable(true);// �������� ��������
				
				
				TexturedAnimation* anim = dynamic_cast<TexturedAnimation*>(AssetLibrary::_GetAssets().FindObject("run_right_anim"));
				
				if(anim)
				{
					Atlas2D* atlas = dynamic_cast<Atlas2D*>(AssetLibrary::_GetAssets().FindObject("run_right_source"));
					
					if(atlas)
					{
						anim->BindAtlas(atlas);
						anim->Play();
						it->BindAnimation(anim);
					}
				}

					
				if(scene)// ���� ����� ����������
				{
					scene->AddLogicObject(it);
					scene->AddGraphicsObject(it); 
				}
			}
		}
			

		// �����...
		{
			Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

			Font2D::Define def;
			def.fontName = "Haettenschweiler";// ��� ������
			def.fontSize = 20;// ������ ������
			def.fontColor = RGBA(RGBA::WHITE_COLOR);// ���� ������

			font2d = new Font2D();
			font2d->Rename("font");
			font2d->Create(def);
			scene->AddObject(font2d);// �������� � �����
			font2d->Pick();// ������� �����
		}

	 
		// ������...
		{
			Camera3D::Define def;
			def.name = "camera";
			def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// ����� �������
			if(sprite) { def.viewingPoint = (dynamic_cast<Position<Vector3D>*>(sprite))->GetPosition(); }
			def.rotationEnabled = false;

			camera = new Camera3D();
			camera->Create(def);
			scene->AddObject(camera);// �������� � �����
			camera->Pick(Title()); // ������� ������

			// ������ ����������� � ������������ ��� ������
			float maxSize = Common::_Max(100, 100);
					
			camera->SetZoomMax(maxSize + Angle::_RadiansToDegrees(camera->Fov()));
			camera->SetZoomMin(maxSize * 0.1);
			camera->SetZoomSpeed(maxSize/10);// ������ �������� ������� 1�10
			camera->SetZFar(maxSize + Angle::_RadiansToDegrees(camera->Fov()) + 1);// ������ ��������� ���������� ������
			camera->SetDistanceToViewingPoint(camera->ZoomMax());// ������ ���������� �� ������������� �������
		}
 

		// ���� ����...
		SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));
			

		MoveToCenter();// ����������� ���� � ����� �����
		Mouse::_MoveCursorToCenter(this);// ����������� ������ � ����� ���������� ������� ����	
		return true;// ������� �����
	}



	// ������� �����
	public: virtual void Update(float _delta)
	{
		Trigger::_ExecuteActions(); 
		scene->Update(_delta);
	}


	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����

		camera->BeginPerspective3D();// ����� ���������� �������

		scene->Draw();// ���������� ������� �����
	
	
	
		// ����� ������...
		camera->BeginScreen2D();// ����� ���������� �������

		int32 i = 0;
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "scene class type: %s", sceneObjectType.ToString().c_str()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "object class type: %s", objectObjectType.ToString().c_str()); 

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "object class type: %s", sprite->GetStringObjectType().c_str()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "object class type: %s", Object::_CreateObjectType(sprite->GetObjectType()).ToString().c_str()); 
		
		

		camera->BeginPerspective3D();// ����� ���������� �������
	}



	// ������ ������� ����
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Mouse().ClearEventList();// �������� ����� �������
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
		} 

		Keyboard().ClearEventList();// �������� ����� �������
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
	def.rendererDef.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_RESIZING);// ����� ���� 
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	def.rendererDef.realTimeWorkerDef.windowDef.tittle = "Object.example_01";

	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
