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


// �����, ������� ��������� ����������� ����
class MyGraphicsQuad: public GraphicsObject,// ����������� ������
	                  public MessageHandler<int32>::Receiver// ���������� ���������
{
	// DATA
	private: float distToWindowBorder;// ���������� ������ ����� �� ������ ����
	private: RGBA color;// ���� �����
	private: Quad rect;// �������������, ����������� ���������� � ������ �����


	// METHODS
	// ����������� ��� ����������
	public: MyGraphicsQuad(void): distToWindowBorder(30.0f)
	{
		color = RGBA(RGBA::GREEN_COLOR);
	}


	// ����������
	public: virtual ~MyGraphicsQuad(void)
	{
		Destroying();
	}


	// ������...
	// ���������� �������������, ����������� ���������� � ������ �����
	public: Quad GetRect(void)const
	{
		return rect;
	}


	// ���������...
	// �������� ������ �����
	// ����������: _size - ��� ������ �������� ����
	public: void SetSize(Vector2 _size)
	{
		_size.x -= distToWindowBorder * 2;
		_size.y -= distToWindowBorder * 2;
		rect.SetSize(_size);
		
		// �������� ������������� � ����� ���������� ������� ����
		Vector2 clientWindowCenter = Renderer::_GetPicked()->ScreenToClient(Renderer::_GetPicked()->ClientAreaCenter());// ��������� ����� ���� � ���������� ������� ���������
		rect.SetCenter(Vector3(clientWindowCenter.x, clientWindowCenter.y, 0.0f));// ������ ����� ����� ��������������
	}


	// ������������...
	// ��������������� �������������
	public: virtual void Draw(void)
	{
		// ������ �������������� �����
		GraphicsLine::_Draw(rect.GetVertex4(), rect.GetVertex3(), color);// ������ ����� �������

		// ������� �������������� �����
		GraphicsLine::_Draw(rect.GetVertex1(), rect.GetVertex2(), color);// ������ ����� �������

		// ����� ������������ �����
		GraphicsLine::_Draw(rect.GetVertex1(), rect.GetVertex4(), color);// ������ ������ ����

		// ������ ������������ �����
		GraphicsLine::_Draw(rect.GetVertex2(), rect.GetVertex3(), color);// ������ ������ ����
	}

		
	// ���������� � ��������...
	// ��������� ������ �� ����� �� ���� _path, � ���������� true � ������ ������
	// ����������: ���� _auto ����� true, �� �������� ������� ������������ �� �����, �� ����������� ����������
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true)
	{
		return false;
	}
};

// ����������� �����
class MyGraphicsPoint: public GraphicsPoint,// ����������� �����
	                   public LogicObject,//���������� ������
					   public MessageHandler<int32>::Receiver// ���������� ���������
{
	// DATA
	private: MagicList<PlayerControl> actionList;// ������ �������� ��� ������
	
	private: MyGraphicsQuad* quad;// �������������� �������������
	private: Vector3 direction;// ����������� �������� ����
    private: float moveSpeed;// ������ ������ ���� �������� (��� ������� �� ������� ��������)
	private: bool keyPressed;// ���� true, �� ��������� ������� ������

	private: int32 size;// ������ ����� (�� ������ � ������) � ��������
	private: float scale;// ����������� ��������������� �����
	private: float maxScale;// ������������ ������� �����
	private: float minScale;// ����������� ������� �����


	// METHODS
	// ����������� ��� ����������
	public: MyGraphicsPoint(void): size(3), moveSpeed(300.0f), maxScale(5.0f), minScale(1.0f), scale(1.0f), quad(NIL), keyPressed(false)
	{
		size = 10.0f;
		SetSize(size * scale);// ������ �����

		{// ������� ��������
			PlayerControl* action = new PlayerControl();
			//action->Rename(_actionName); 
			Mouse::Event* mouseEvent = new Mouse::Event();
			mouseEvent->type = Mouse::WHEEL;
			mouseEvent->key = Mouse::NO_KEY;
			action->AddEvent(mouseEvent);// �������� ������� � ������ ������� ���������
			action->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &MyGraphicsPoint::ChangeSize));
			actionList.AddToTail(action);
		}


		{// ������� ��������
			PlayerControl* action = new PlayerControl();
			//action->Rename(_actionName);
			Keyboard::Event* keyboardEvent = new Keyboard::Event();
			keyboardEvent->type = Keyboard::KEY_PRESSED; 
			keyboardEvent->key = Keyboard::Z; 
			action->AddEvent(keyboardEvent);// �������� ������� � ������ ������� ���������
			action->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &MyGraphicsPoint::ChangeColor));
			actionList.AddToTail(action); 
		}


		{// ������� ��������
			PlayerControl* action = new PlayerControl();
			//action->Rename(_actionName);
			Keyboard::Event* keyboardEvent = new Keyboard::Event();
			keyboardEvent->type = Keyboard::KEY_PRESSED;  
			keyboardEvent->key = Keyboard::UP_ARROW; 
			action->AddEvent(keyboardEvent);// �������� ������� � ������ ������� ���������
			action->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &MyGraphicsPoint::MoveUp));
			actionList.AddToTail(action);
		}

		{// ������� ��������
			PlayerControl* action = new PlayerControl();
			//action->Rename(_actionName);
			Keyboard::Event* keyboardEvent = new Keyboard::Event();
			keyboardEvent->type = Keyboard::KEY_PRESSED; 
			keyboardEvent->key = Keyboard::DOWN_ARROW; 
			action->AddEvent(keyboardEvent);// �������� ������� � ������ ������� ���������
			action->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &MyGraphicsPoint::MoveDown));
			actionList.AddToTail(action);
		}
		
		
		{// ������� ��������
			PlayerControl* action = new PlayerControl();
			//action->Rename(_actionName);
			Keyboard::Event* keyboardEvent = new Keyboard::Event();
			keyboardEvent->type = Keyboard::KEY_PRESSED; 
			keyboardEvent->key = Keyboard::LEFT_ARROW; 
			action->AddEvent(keyboardEvent);// �������� ������� � ������ ������� ���������
			action->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &MyGraphicsPoint::MoveLeft));
			actionList.AddToTail(action);
		}
		
		{// ������� ��������
			PlayerControl* action = new PlayerControl();
			//action->Rename(_actionName);
			Keyboard::Event* keyboardEvent = new Keyboard::Event();
			keyboardEvent->type = Keyboard::KEY_PRESSED; 
			keyboardEvent->key = Keyboard::RIGHT_ARROW; 
			action->AddEvent(keyboardEvent);// �������� ������� � ������ ������� ���������
			action->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &MyGraphicsPoint::MoveRight));
			actionList.AddToTail(action);
		}

		MoveToWindowCenter();// ����������� ����� � ����� ���������� ������� ����
	}


	// ����������
	public: virtual ~MyGraphicsPoint(void)
	{
		Destroying();
	}


	// ������� �������������� ������������� _quad ��� �����
	public: void SetQuad(MyGraphicsQuad* _quad)
	{
		quad = _quad;
	}


	// ������� �� �������...
	// ��������� ����� ����� �� ���������
	private: void ChangeColor(PlayerControl* _action)
	{
		SetColor(RGBA::_GetRandom(155, 255));// ������ ��������� ���� 
	}


	// ��������� ������� �����
	private: void ChangeSize(PlayerControl* _action)
	{
		Mouse::Event* mEvent = dynamic_cast<Mouse::Event*>(_action->GetActivatedEvents().GetHead());

		if(mEvent && mEvent->type == Mouse::WHEEL)
		{
			float offset = (float)mEvent->wheel / 3.0f;

			if((scale + offset) <= maxScale && (scale + offset) >= minScale)// �������� ���������
			{
				scale += offset;
				GraphicsPoint::_SetPointSize(size * scale);// ������ �����
			}
		}
	}

	// �������� �����
	private: void MoveUp(PlayerControl* _action)
	{
		direction += Vector3(0.0f, -1.0f, 0.0f);
		keyPressed = true;
	}


	// �������� ����
	private: void MoveDown(PlayerControl* _action)
	{
		direction += Vector3(0.0f, 1.0f, 0.0f);
		keyPressed = true;
	}


	// �������� �����
	private: void MoveLeft(PlayerControl* _action)
	{
		direction += Vector3(-1.0f, 0.0f, 0.0f);
		keyPressed = true;
	}


    // �������� ������
	private: void MoveRight(PlayerControl* _action)
	{
		direction += Vector3(1.0f, 0.0f, 0.0f);
		keyPressed = true;
	}


	// ������...
	// ���������� ������ ����� � ��������
	public: int32 GetSize(void)const
	{
		return size;
	}


	// ������ ����� ������ ����� (� ��������)
	public: void SetSize(int32 _size)
	{
		GraphicsPoint::_SetPointSize(_size);// ������ �����
	}



	// ��������� ���������...
	// ��������� ��������� ����� 
	public: virtual void Update(float _delta)
	{
		if(keyPressed)// ���� ���� ��������� ������� �������
		{ 
			// �������� ������� �������
			SetPosition(GetPosition() + Vector3::_Normalize(direction) * moveSpeed * _delta); 
			direction.Set(0.0f, 0.0f, 0.0f);// �������� ������ ��������
			keyPressed = false;// ��������� ��������
		}

		if(quad && !quad->GetRect().IsPointInside(GetPosition()))// ���� ����� ����� �� ������� �����
		{
			SetPosition(quad->GetRect().ClampPoint(GetPosition()));// ���������� ���������� ����� � ������ �����
		}
	}


	// ������� ����� � ����� ���������� ������� ����
	public: void MoveToWindowCenter(void)
	{
		Vector2 winCenter = Renderer::_GetPicked()->ScreenToClient(Renderer::_GetPicked()->ClientAreaCenter());// ����� ����� ���������� ������� ���� � �������� �����������, � �������������� � ���������� ��������
		winCenter.y = Renderer::_GetPicked()->GetViewportSize().y - winCenter.y;// ������������ ���������� �������� ���������� � ����������, � ������� � ����� ������ ����
		SetPosition(Vector3(winCenter.x, winCenter.y, 0));// ��������� � ����� ���������� ������� ����
	}

		
	// ���������� � ��������...
	// ��������� ������ �� ����� �� ���� _path, � ���������� true � ������ ������
	// ����������: ���� _auto ����� true, �� �������� ������� ������������ �� �����, �� ����������� ����������
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true)
	{
		return false;
	}

				
	// ��������� ������ �� ����� _file, � ���������� true � ������ ������
	public: virtual bool LoadFromFile(File& _file)
	{
		return false;
	}


	// ��������� ������ � ����� ����� _file, � ���������� true � ������ ������
	public: virtual bool SaveToFile(File& _file)
	{
		return false;
	}
};


// ���������������� ����������
class Application: public Engine// ������
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D *font;// �����
	private: Scene* scene;// �����
	
	private: MyGraphicsPoint* point; // �����, ����������� �������������
	private: MyGraphicsQuad* quad;// ����������� �������������
	
	
    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font(NIL), scene(NIL), quad(NIL), point(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete camera;

		delete font;

		delete quad;
		delete point;

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
		

			// ����������� �������������...
			{
				quad = new MyGraphicsQuad();
				//quad->Rename("quad");// ���
				quad->SetSize(Renderer::_GetPicked()->GetViewportSize());// ������ ��������������
				scene->AddGraphicsObject(quad);// �������� � ����������� �����
				Connect(Window::RESIZE_WINDOW_MESSAGE, quad, Caller<>(quad, &MyGraphicsQuad::SetSize));// ����������� �� ������� ��������� �������� ����
			}


			// ����������� �����...
			{
				point = new MyGraphicsPoint();
				point->SetQuad(quad);// ������ �������������� ������������� ��� �����

				scene->AddObject(point);// �������� � �����
				scene->AddLogicObject(point);// �������� � ���������� �����
				scene->AddGraphicsObject(point);// �������� � ����������� �����
			}

			// ������ ����������
			//Keyboard().SetKeyPressedPeriodInSec(Keyboard().GetKeyPressedPeriodInSec() / 6.0f);
			
			// �����...
			GraphicsLine::_SetLineWidth(3.0f);// ������� �����


			// ���� ����...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));

			MoveToCenter();// ����������� ���� � ����� ������
			Mouse::_SetShowCursor(false);// ������ ������
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "PlayerControl.example_01";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
