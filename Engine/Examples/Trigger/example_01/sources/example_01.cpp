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
#include <Engine/Core/Parser/List/IntegerList.h> 
#include <Engine/Core/Parser/List/FloatList.h> 
#include <Engine/Core/Parser/List/BooleanList.h> 
#include <Engine/Core/Parser/List/StringList.h> 
#include <Engine/Core/Parser/List/Vector2List.h> 
#include <Engine/Core/Parser/List/Vector3List.h> 
#include <Engine/Core/Parser/List/QuaternionList.h> 
#include <Engine/Core/Parser/List/HMSList.h> 
#include <Engine/Core/Parser/List/RGBAList.h> 


// ���������������� ����������
class Application: public Engine,// ������
               	   public MessageHandler<int32>::Receiver// ���������� ���������
{
	// DATA
	private: Camera3D* camera2d;// ������
    private: Font2D* font;// �����
	private: Scene* scene;// �����
	private: Trigger* trigger;// �������
	private: AbstractObject* object;// ������

    // METHODS
    // ���������������� �����������
	public: Application(void): camera2d(NIL), font(NIL), scene(NIL), trigger(NIL), object(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete camera2d;
		delete font;
		delete scene;

		delete object;

		delete trigger;
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
				def.color = RGBA(RGBA::WHITE_COLOR);// ���� ������

				font = new Font2D();
				font->Create(def);
				font->Pick();// ������� �����
			}

			// ������ ��� ���������� �������...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;


				camera2d = new Camera3D();
				camera2d->Create(def);
			}

			
			// �������� ������...
			{
				object = new AbstractObject();
				object->Rename("unnamed");
				AssetLibrary::_GetAssets().AddObject(object);
			}


			// �������� �������...
			{
				trigger = new Trigger();


				trigger->SetActionExpr("act");
				trigger->SetEventExpr("");
			}


			// ���������� 1...
			{
				Variable<StringANSI>* var = new Variable<StringANSI>("new_name_2");
				var->Rename("arg_var1");
				AssetLibrary::_GetAssets().AddObject(var);
			}

			// ���������� 2...
			{
				Variable<StringANSI>* var = new Variable<StringANSI>("new_name_3");
				var->Rename("arg_var2");
				AssetLibrary::_GetAssets().AddObject(var);
			}


			// ���� ����...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));


			MoveToCenter();// ����������� ���� � ����� ������
			//Mouse::_SetShowCursor(false);// ������ ������
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


		font->DrawScreenText(Vector3(10, i+=20, -1), "object.name: %s", object->GetName().c_str());
		//font->DrawScreenText(Vector3(10, i+=20, -1), "%f", camera->GetDistanceToViewingPoint());
	

		// ������������ ���������� �������������...
		camera2d->EndTransform(Renderer::PROJECTION);
		camera2d->EndTransform(Renderer::MODELVIEW);
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
			else if(_event->key == Keyboard::A)
			{
				delete AssetLibrary::_GetAssets().FindObject("act");

				AbstractAction* act = Trigger::_CreateAction(AbstractObject::CHANGE_NAME_MESSAGE);

				if(act)
				{
					act->Rename("act");
					act->SetStringKeyArg(AbstractAction::ARG1, "new_name_1");
					act->SetVariableArg(AbstractAction::TARGET, object->GetName());
					AssetLibrary::_GetAssets().AddObject(act);

					trigger->SetValue(true);
				}
			}
			else if(_event->key == Keyboard::B)
			{
				delete AssetLibrary::_GetAssets().FindObject("act");

				AbstractAction* act = Trigger::_CreateAction(AbstractObject::CHANGE_NAME_MESSAGE);

				if(act)
				{
					act->Rename("act");
					act->SetVariableArg(AbstractAction::ARG1, "arg_var1");
					act->SetVariableArg(AbstractAction::TARGET, object->GetName());
					AssetLibrary::_GetAssets().AddObject(act);

					trigger->SetValue(true);
				}
			}
			else if(_event->key == Keyboard::C)
			{
				delete AssetLibrary::_GetAssets().FindObject("act");

				AbstractAction* act = Trigger::_CreateAction(AbstractObject::CHANGE_NAME_MESSAGE);

				if(act)
				{
					act->Rename("act");
					act->SetTemplateArg(AbstractAction::ARG1, "$arg_var2");
					act->SetTemplateArg(AbstractAction::TARGET, StringANSI("$") + object->GetName());
					AssetLibrary::_GetAssets().AddObject(act);

					trigger->SetValue(true);
				}
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Trigger.example_01";
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
