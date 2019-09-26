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



// �����, ����������� ���������
class Message: public MagicNode// ���������� ����
{
	// ������������� ������
	friend class Chat;

	// DATA
	private: StringANSI text;// ���������� ���������

	// METHODS
    // ���������������� �����������
	public: Message(StringANSI _text): text(_text)
	{}


	// ���������� ����� ���������
	public: StringANSI GetText(void)const
	{
		return text;
	}
};



// �����, ����������� ����� ��� ������ � ��������� �������
class Chat: public GraphicsObject,// ����������� ������
	        public MessageHandler<int32>::Receiver// ���������� ���������
{
	// DATA
	private: MagicList<Message> history;// ������� ��������� � ��������������� �������
	private: Quad borderRect;// ������� (�����) ���� 
	private: RGBA borderColor;// ���� ����� ����
	private: int32 messageLimit;// ������������ ���������� ������� ���������, ������� ����� ���� ���������� � ����
	//private: int32 borderWidth;// ������� ����� ���� � ��������
    private: MagicList<Message>::AddressDerived* begin;// ���������, � �������� ���������� ������������ 
	private: Font2D *font;// �����, ������� ����� ��������� �����
	private: StringANSI message;// ��������� ������������
	private: AbstractObject* gregory;// ������������ Gregory

	// METHODS
	// ����������� ��� ����������
	public: Chat(void): borderColor(RGBA(RGBA::WHITE_COLOR)), messageLimit(19), /*borderWidth(5),*/ begin(NIL), font(NIL), gregory(NIL)
	{}


	// ����������
	public: virtual ~Chat(void)
	{
		delete gregory;
	}


	// ������� ��� � ������ ����� _borderColor, �������� ����� _borderSize, � ������� ������� ��������� _messageLimit
	public: void Create(Font2D* _font, RGBA _borderColor, int32 _borderWidth)
	{
		borderRect = Quad(Vector3(350, 250, -1), Vector2(500, 400));
		borderColor = _borderColor;
		//borderWidth = _borderWidth;
		GraphicsLine::_SetLineWidth(_borderWidth);
		font = _font;

		// Gregory
		gregory = new AbstractObject();
		gregory->Rename("Gregory");
		AssetLibrary::_GetAssets().AddObject(gregory);

		// ����������� �� ����� ��������� �� ������ ����
		Connect(MessageHandler<int32>::SEND_MESSAGE, this, Caller<>(this, &Chat::SendMessageToChat));
	}


	// ��������� ���� �������
	// ����������: _character ������ ��������� ���� ������
	public: void Put(StringANSI _character)
	{
		message += _character;
	}


    // ��������� ���� ���������
    public: void PutFinish(void)
	{
		if(!message.empty())// ���� ������ �� �����
		{
			AddMessage(new Message("[Gregory]:" + message));
			gregory->SendPackage(MessageHandler<int32>::SEND_MESSAGE, message);// �������� � ��� � ����������� Gregory � ���, ��� Gregory ������ ��������� message
			message.clear();
		}
	}

	// �������� ���� ���������� �������
	public: void PutUndo(void)
	{
		if(!message.empty())// ���� ������ �� �����
		{ 
			message.resize(message.size() - 1); // ������� ��������� ������
		}
	}


			
	// �������� ��������� _message � ���
	private: void SendMessageToChat(StringANSI _message)
	{
		if(!_message.empty())// ���� ������ �� �����
		{
			AddMessage(new Message(_message));// �������� ��������� � ���
		}
	}


	// ��������� ��������� _message � ����� �������
	public: void AddMessage(Message* _message)
	{
		if(begin == NIL)
		{
			begin = history.AddToTail(_message);
		}
		else 
		{
			// ���������� ������ ��������� 10 ���������...
			MagicList<Message>::AddressDerived* it = history.AddToTail(_message);// ��������� � ����� �������
			
			if((it->GetIndex() - begin->GetIndex()) == messageLimit)
			{
				begin = begin->GetRight();
			}
		}
	}


	// ������������ ����
	public: virtual void Draw(void)
	{
		// ���������� ����� ����
		GraphicsLine::_Draw(borderRect.GetVertex4(), borderRect.GetVertex3(), borderColor);// ������� �����
		GraphicsLine::_Draw(borderRect.GetVertex3(), borderRect.GetVertex2(), borderColor);// ������� �����
		GraphicsLine::_Draw(borderRect.GetVertex2(), borderRect.GetVertex1(), borderColor);// ������ �����
		GraphicsLine::_Draw(borderRect.GetVertex1(), borderRect.GetVertex4(), borderColor);// ����� �����

		int32 i = 7; 

		// ��������������� ��� ��������� ���� ������� � begin
		for(MagicList<Message>::AddressDerived* it = begin; it != NIL; it = it->GetRight())
		{
			font->DrawScreenText(Vector3(borderRect.GetVertex4().x + 10, borderRect.GetVertex1().y + (i+=20), -1), "%s", it->GetNode()->GetText().c_str()); 
		}

		font->DrawScreenText(Vector3(borderRect.GetVertex1().x + 10, borderRect.GetVertex4().y + 20, -1), "message: %s", message.c_str()); 
	}
};


// ���������������� ����������
class Application: public Engine,// ������
               	   public MessageHandler<int32>::Receiver// ���������� ���������
{
	// DATA
	private: Camera3D/* *camera,*/ *camera2d;// ������
    private: Font2D *font;// �����
	private: Scene* scene;// �����
	private: Chat* chat;// ���


    // METHODS
    // ���������������� �����������
	public: Application(void): font(NIL), /*camera(NIL), */camera2d(NIL), scene(NIL), chat(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete font;
		delete chat;
		delete camera2d;
		//delete camera;
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
				scene->Rename("scene");// ��� �����
				AssetLibrary::_GetAssets().AddObject(scene);
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
				/*
				Camera3D::Define def;
				camera = new Camera3D();
				camera->Set(def);
				
				camera->Pick(); // ������� ������
				scene->BindCamera(camera);
				*/ 
			}
		
		
			// ������ ��� ���������� �������...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;
				
				camera2d = new Camera3D();
				camera2d->Set(def);
				camera2d->Pick(); // ������� ������
				scene->BindCamera(camera2d);
			}
			
			// ���...
			{
				chat = new Chat();
				chat->Rename("Chat");
				chat->Create(font, RGBA(RGBA::WHITE_COLOR), 3);// ������� ���
				
				//scene->AddObject(chat);
				scene->AddGraphicsObject(chat); 
				AssetLibrary::_GetAssets().AddObject(chat); 
			}


			{
				// John behaviour
				// ������ ������� =========================================================
				Trigger* johnHelloReflex1 = new Trigger();
				johnHelloReflex1->Rename("john_gregory_hello_reflex1");
				johnHelloReflex1->SetEventExpr("(hello | gregoryHelloToJohn) & ~bye");
				johnHelloReflex1->SetActionExpr("johnHelloToGregory");
				

				// ������� 
				AbstractEvent* helloEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "Gregory", "Hello");
				helloEvent->Rename("hello"); 
				AssetLibrary::_GetAssets().AddObject(helloEvent);

			
				// �������
				AbstractEvent* gregoryHelloToJohnEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "Gregory", "Hello John");
				gregoryHelloToJohnEvent->Rename("gregoryHelloToJohn");
				AssetLibrary::_GetAssets().AddObject(gregoryHelloToJohnEvent);
	
			
				// �������
				AbstractEvent* byeEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "Gregory", "Bye");
				byeEvent->Rename("bye");
				AssetLibrary::_GetAssets().AddObject(byeEvent); 


				// ��������
				AbstractAction* johnHelloToGregoryAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "[John]: Hello Gregory!");  
				johnHelloToGregoryAction->Rename("johnHelloToGregory"); 
				AssetLibrary::_GetAssets().AddObject(johnHelloToGregoryAction);
				
				
				AssetLibrary::_GetAssets().AddObject(johnHelloReflex1); 


				// ������ ������� ==============================================
				Trigger* johnHelloReflex2 = new Trigger();
				johnHelloReflex2->Rename("john_gregory_hello_reflex2");
				johnHelloReflex2->SetEventExpr("(gregoryCallToJohn) & (~bye)");
				johnHelloReflex2->SetActionExpr("johnAnswerToGregory");
				

				// �������
				AbstractEvent* gregoryCallToJohnEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "Gregory", "John");
				gregoryCallToJohnEvent->Rename("gregoryCallToJohn");
				AssetLibrary::_GetAssets().AddObject(gregoryCallToJohnEvent);

			
				// ��������
				AbstractAction* johnAnswerToGregoryAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "[John]: I'm here!"); 
				johnAnswerToGregoryAction->Rename("johnAnswerToGregory");
				AssetLibrary::_GetAssets().AddObject(johnAnswerToGregoryAction); 
				
		
				AssetLibrary::_GetAssets().AddObject(johnHelloReflex2);



				// ������ ������� ==============================================
				Trigger* johnHelloReflex3 = new Trigger();
				johnHelloReflex3->Rename("john_gregory_bye_reflex");
				johnHelloReflex3->SetEventExpr("bye");
				johnHelloReflex3->SetActionExpr("johnByeToGregory");
				johnHelloReflex3->SetRepeatLimit(1);
				

				// �������� 
				AbstractAction* johnByeToGregoryAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "[John]: Bye Gregory!");
				johnByeToGregoryAction->Rename("johnByeToGregory");
				AssetLibrary::_GetAssets().AddObject(johnByeToGregoryAction);
				

				AssetLibrary::_GetAssets().AddObject(johnHelloReflex3);
			}

			{
			
				// Marie behaviour
				// ������ ������� ===============================================
				Trigger* marieHelloReflex1 = new Trigger();
				marieHelloReflex1->Rename("marie_gregory_hello_reflex");
				marieHelloReflex1->SetEventExpr("(hello ^ helloToMarie) & ~marieJealous & ~bye");
				marieHelloReflex1->SetActionExpr("marieHelloToGregory, marieSilence");// action �� ����������; ���������� ��������; ������� ������������ ����� ��������
				

  
				// �������
				AbstractEvent* helloToMarieEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "Gregory", "Hello Marie");
				helloToMarieEvent->Rename("helloToMarie"); 
				AssetLibrary::_GetAssets().AddObject(helloToMarieEvent);
							

				// ��������
				AbstractAction* marieHelloToGregoryAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "[Marie]: Hello!"); 
				marieHelloToGregoryAction->Rename("marieHelloToGregory");
				AssetLibrary::_GetAssets().AddObject(marieHelloToGregoryAction);
				

				// �������� 
				AbstractAction* marieSilenceAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "[Marie]: ..."); 
				marieSilenceAction->Rename("marieSilence"); 
				AssetLibrary::_GetAssets().AddObject(marieSilenceAction);


				AssetLibrary::_GetAssets().AddObject(marieHelloReflex1);


				// ������ ������� =============================================================
				Trigger* marieHelloReflex2 = new Trigger();
				marieHelloReflex2->Rename("marieJealous");
				marieHelloReflex2->SetEventExpr("(gregoryCallToJohn | gregoryHelloToJohn)");  
				marieHelloReflex2->SetActionExpr("marieSilence");// ������������� �������� � ���������� ��������� ������������
				

				AssetLibrary::_GetAssets().AddObject(marieHelloReflex2);
			}


			// ���� ����...
			SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));

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
		//camera2d->BeginTransform(Renderer::PROJECTION);
		//camera2d->BeginTransform(Renderer::MODELVIEW);


	
		// ����� ������...
		int32 i = 0;
	

		//font->DrawScreenText(Vector3(10, i+=20, -1), "current mode"); 

	
		//font2d->DrawScreenText(Vector3(10, ClientAreaSize().y - (i+=20), -1), "current mode: %s", toolMode == EDIT ? "EDIT" : "READ"); 

	

	
		
		// ������������ ���������� �������������...
		//camera2d->EndTransform(Renderer::PROJECTION);
		//camera2d->EndTransform(Renderer::MODELVIEW);
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
			else if(_event->key == Keyboard::BACKSPACE)
			{
				chat->PutUndo();// �������� ��������� ��������� ������
			}
			else if(_event->key == Keyboard::ENTER)
			{
				chat->PutFinish();// �������� ������
			}
			else// ����� ������ ����� ������ �������
			{ 
				chat->Put(Keyboard::_FormatKeyName(_event));// ����� ��� ��������� ������� �������
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
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);// ������ ����
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Trigger.example_02";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
