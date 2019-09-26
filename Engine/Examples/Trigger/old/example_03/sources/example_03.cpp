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

	// METHODS
	// ����������� ��� ����������
	public: Chat(void): borderColor(RGBA(RGBA::WHITE_COLOR)), messageLimit(19), /*borderWidth(5),*/ begin(NIL), font(NIL)
	{}


	// ����������
	public: virtual ~Chat(void)
	{}


	// ������� ��� � ������ ����� _borderColor, �������� ����� _borderSize, � ������� ������� ��������� _messageLimit
	public: void Create(Font2D* _font, RGBA _borderColor, int32 _borderWidth)
	{
		borderRect = Quad(Vector3(350, 250, 0), Vector2(500, 400));
		borderColor = _borderColor;
		//borderWidth = _borderWidth;
		GraphicsLine::_SetLineWidth(_borderWidth);
		font = _font;

		// ����������� �� ����� ��������� �� ������ ����
		Connect(MessageHandler<int32>::SEND_MESSAGE, this, Caller<>(this, &Chat::SendMessageToChat));
	}


	// ��������� ���� �������
	// ����������: _character ������ ��������� ���� ������
	public: void Put(StringANSI _character)
	{
		message += _character;
	}


	// �������� ��������� _message � ���
	private: void SendMessageToChat(StringANSI _message)
	{
		if(!_message.empty())// ���� ������ �� �����
		{
			AddMessage(new Message(_message));// �������� ��������� � ���
		}
	}


    // ��������� ���� ���������
    public: void PutFinish(void)
	{
		if(!message.empty())// ���� ������ �� �����
		{
			AddMessage(new Message(message));
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

		Disconnect(MessageHandler<int32>::SEND_MESSAGE, this);// ���������� �� ���� ����� �������� ������������ ����� ��� �������� ��������� ������
		SendPackage(MessageHandler<int32>::SEND_MESSAGE, _message->GetText());// �������� ���� ��� Chat ������ ��������� message
		Connect(MessageHandler<int32>::SEND_MESSAGE, this, Caller<>(this, &Chat::SendMessageToChat)); // ����������� �� ���� ����� ��������������� ��������� �� ������ �����������
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



// �����, ����������� ��������� ��������
class GoblinGenerator: public AbstractObject,// ������� �������� �������
	                   public MessageHandler<int32>::Receiver// ���������� ���������
{
	// METHODS
	// ����������� ��� ����������
	public: GoblinGenerator(void)
	{
		// ������� ����������...
		Variable<bool>* goblinExistVariable = new Variable<bool>(false);
		goblinExistVariable->Rename("goblinExist");
		AssetLibrary::_GetAssets().AddObject(goblinExistVariable);


		Variable<bool>* falseVariable = new Variable<bool>(false);
		falseVariable->Rename("FALSE");
		AssetLibrary::_GetAssets().AddObject(falseVariable);

	
		AbstractObject* currentTimer = new AbstractObject();
		currentTimer->Rename("CurrentTimer");
		AssetLibrary::_GetAssets().AddObject(currentTimer);


		{
			// ����� �������...
			Trigger* goblinMessenger1 = new Trigger();
			goblinMessenger1->Rename("goblin_messenger1");
			goblinMessenger1->SetActionExpr("restart1, restart2, restart3");
				
	
			// ��������
			IAction* restart1Action = Trigger::_CreateAction(TimingHandler::CREATE_TIMER_MESSAGE, "startTimer");
			restart1Action->Rename("restart1");
			AssetLibrary::_GetAssets().AddObject(restart1Action); 


			// ��������
			IAction* restart2Action = Trigger::_CreateAction(PlayingObject::CHANGE_PLAYING_DURATION_IN_SEC_MESSAGE, "startTimer", 4.0f);
			restart2Action->Rename("restart2");
			AssetLibrary::_GetAssets().AddObject(restart2Action);


			// ��������
			IAction* restart3Action = Trigger::_CreateAction(PlayingObject::CHANGE_PLAYING_STATE_MESSAGE, "startTimer", PlayingObject::PLAY);
			restart3Action->Rename("restart3");
			AssetLibrary::_GetAssets().AddObject(restart3Action);



			goblinMessenger1->SetEventExpr("");
			
			AssetLibrary::_GetAssets().AddObject(goblinMessenger1);
		}

		
		{
			/* ��������� ����� ���������� ��� ������� TIMER_FINISH */ 
			// ���������� � ����������� �������...
			Trigger* tr1 = new Trigger();
			tr1->Rename("tr1");
			tr1->SetActionExpr("startTimerSend");
		
			
			// ������� 
			IEvent* startTimerIsFinishEvent = Trigger::_CreateEvent(Timer::TIMER_FINISH_MESSAGE, "startTimer");
			startTimerIsFinishEvent->Rename("startTimerIsFinish");
			AssetLibrary::_GetAssets().AddObject(startTimerIsFinishEvent);


			// ��������
			IAction* disableStartTimerAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "CurrentTimer", "startTimer"); 
			disableStartTimerAction->Rename("startTimerSend");
			AssetLibrary::_GetAssets().AddObject(disableStartTimerAction);
			

			tr1->SetEventExpr("startTimerIsFinish");   

			AssetLibrary::_GetAssets().AddObject(tr1);  
		}



		{
			// ���������� � ����������� �������...
			Trigger* goblinMessenger2 = new Trigger();
			goblinMessenger2->Rename("goblin_messenger2");
			goblinMessenger2->SetActionExpr("goblinWarning1Message, goblinPrepareToCreate1, goblinPrepareToCreate2, goblinPrepareToCreate3");
		

			// ��������
			IAction* goblinWarning1MessageAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "goblin is coming..."); 
			goblinWarning1MessageAction->Rename("goblinWarning1Message");
			AssetLibrary::_GetAssets().AddObject(goblinWarning1MessageAction);	

			
			// ��������
			IAction* goblinPrepareToCreate1Action = Trigger::_CreateAction(TimingHandler::CREATE_TIMER_MESSAGE, "createGoblinTimer");
			goblinPrepareToCreate1Action->Rename("goblinPrepareToCreate1");
			AssetLibrary::_GetAssets().AddObject(goblinPrepareToCreate1Action); 


			// �������� 
			IAction* goblinPrepareToCreate2Action = Trigger::_CreateAction(PlayingObject::CHANGE_PLAYING_DURATION_IN_SEC_MESSAGE, "createGoblinTimer", 4.0f);
			goblinPrepareToCreate2Action->Rename("goblinPrepareToCreate2");
			AssetLibrary::_GetAssets().AddObject(goblinPrepareToCreate2Action);	
	
				
			// �������� 
			IAction* goblinPrepareToCreate3Action = Trigger::_CreateAction(PlayingObject::CHANGE_PLAYING_STATE_MESSAGE, "createGoblinTimer", Timer::PLAY);
			goblinPrepareToCreate3Action->Rename("goblinPrepareToCreate3");
			AssetLibrary::_GetAssets().AddObject(goblinPrepareToCreate3Action);	
	

			// �������
			IEvent* startTimerMessageEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "CurrentTimer", "startTimer");// ���������� �������� �������������� ������ ������� ��� ��������� ������ ������� ���� ����� "������ ����, ������ ������������" � �.�. ��� ���������� ��� ����, ����� ������� goblin_messenger8 �� ������ ���� ������ �������� ����� ����� ���
			startTimerMessageEvent->SetOverlappingEnable(true);
			startTimerMessageEvent->Rename("startTimerMessage");
			AssetLibrary::_GetAssets().AddObject(startTimerMessageEvent);


			goblinMessenger2->SetEventExpr("~goblinExist & startTimerMessage");

			AssetLibrary::_GetAssets().AddObject(goblinMessenger2);  
		} 


		{
			/* ��������� ����� ���������� ��� ������� TIMER_FINISH */ 
			// ���������� � ����������� �������...
			Trigger* tr2 = new Trigger();
			tr2->Rename("tr2");
			tr2->SetActionExpr("createGoblinTimerSend");
		
			
			// ������� 
			IEvent* startTimerIsFinishEvent = Trigger::_CreateEvent(Timer::TIMER_FINISH_MESSAGE, "createGoblinTimer");
			startTimerIsFinishEvent->Rename("createGoblinTimerIsFinish");
			AssetLibrary::_GetAssets().AddObject(startTimerIsFinishEvent);


			// ��������
			IAction* disableStartTimerAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "CurrentTimer", "createGoblinTimer"); 
			disableStartTimerAction->Rename("createGoblinTimerSend");
			AssetLibrary::_GetAssets().AddObject(disableStartTimerAction);
			

			tr2->SetEventExpr("createGoblinTimerIsFinish");   

			AssetLibrary::_GetAssets().AddObject(tr2);  
		}

		{
			// �������� ������� ...
			Trigger* goblinMessenger3 = new Trigger();
			goblinMessenger3->Rename("goblin_messenger3"); 
			goblinMessenger3->SetActionExpr("createGoblin, checkGoblinExist"); 
				

			// ��������
			IAction* createGoblinAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "GoblinGenerator", "goblin");
			createGoblinAction->Rename("createGoblin");   
			AssetLibrary::_GetAssets().AddObject(createGoblinAction);


			// ��������
			IAction* checkGoblinExistAction = Trigger::_CreateAction(AssetLibrary::CHECK_OBJECT_EXIST_MESSAGE, "goblin", "goblinExist");
			checkGoblinExistAction->Rename("checkGoblinExist"); 
			AssetLibrary::_GetAssets().AddObject(checkGoblinExistAction);

						
			// �������
			IEvent* createGoblinTimerMessageEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "CurrentTimer", "createGoblinTimer");
			createGoblinTimerMessageEvent->SetOverlappingEnable(true);
			createGoblinTimerMessageEvent->Rename("createGoblinTimerMessage");
			AssetLibrary::_GetAssets().AddObject(createGoblinTimerMessageEvent);


			goblinMessenger3->SetEventExpr("createGoblinTimerMessage");  


			AssetLibrary::_GetAssets().AddObject(goblinMessenger3);
		}


		{
			// ���������� � �������� �������...
			Trigger* goblinMessenger4 = new Trigger();
			goblinMessenger4->Rename("goblin_messenger4");
			goblinMessenger4->SetActionExpr("goblinWarning2Message, goblinPrepareToGone1, goblinPrepareToGone2, goblinPrepareToGone3");
				

			// ��������
			IAction* goblinWarning2MessageAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "goblin is blocking your way...");  
			goblinWarning2MessageAction->Rename("goblinWarning2Message");
			AssetLibrary::_GetAssets().AddObject(goblinWarning2MessageAction);
			

			// ��������
			IAction* goblinPrepareToGone1Action = Trigger::_CreateAction(TimingHandler::CREATE_TIMER_MESSAGE, "leaveGoblinTimer");
			goblinPrepareToGone1Action->Rename("goblinPrepareToGone1");
			AssetLibrary::_GetAssets().AddObject(goblinPrepareToGone1Action);


			// ��������  
			IAction* goblinPrepareToGone2Action = Trigger::_CreateAction(PlayingObject::CHANGE_PLAYING_DURATION_IN_SEC_MESSAGE, "leaveGoblinTimer", 4.0f); 
			goblinPrepareToGone2Action->Rename("goblinPrepareToGone2");
			AssetLibrary::_GetAssets().AddObject(goblinPrepareToGone2Action);	


			// ��������  
			IAction* goblinPrepareToGone3Action = Trigger::_CreateAction(PlayingObject::CHANGE_PLAYING_STATE_MESSAGE, "leaveGoblinTimer", Timer::PLAY); 
			goblinPrepareToGone3Action->Rename("goblinPrepareToGone3");
			AssetLibrary::_GetAssets().AddObject(goblinPrepareToGone3Action);	

				
			// ������� 
			IEvent* goblinIsCreatedEvent = Trigger::_CreateEvent(AssetLibrary::CREATE_OBJECT_MESSAGE, "goblin"); 
			goblinIsCreatedEvent->Rename("goblinIsCreated");
			AssetLibrary::_GetAssets().AddObject(goblinIsCreatedEvent); 
			


			goblinMessenger4->SetEventExpr("goblinIsCreated");


			
			AssetLibrary::_GetAssets().AddObject(goblinMessenger4);
		} 
	


		{
			/* ��������� ����� ���������� ��� ������� TIMER_FINISH */ 
			// ���������� � ����������� �������...
			Trigger* tr3 = new Trigger();
			tr3->Rename("tr3");
			tr3->SetActionExpr("leaveGoblinTimerSend");
		
			
			// ������� 
			IEvent* startTimerIsFinishEvent = Trigger::_CreateEvent(Timer::TIMER_FINISH_MESSAGE, "leaveGoblinTimer");
			startTimerIsFinishEvent->Rename("leaveGoblinTimerIsFinish");
			AssetLibrary::_GetAssets().AddObject(startTimerIsFinishEvent);


			// ��������
			IAction* disableStartTimerAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "CurrentTimer", "leaveGoblinTimer"); 
			disableStartTimerAction->Rename("leaveGoblinTimerSend");
			AssetLibrary::_GetAssets().AddObject(disableStartTimerAction);
			

			tr3->SetEventExpr("leaveGoblinTimerIsFinish");   

			AssetLibrary::_GetAssets().AddObject(tr3);  
		}

		{
			// ���������� � ����� �������...
			Trigger* goblinMessenger5 = new Trigger();
			goblinMessenger5->Rename("goblin_messenger5");  
				

			// �������
			IEvent* leaveGoblinTimerMessageEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "CurrentTimer", "leaveGoblinTimer");
			leaveGoblinTimerMessageEvent->SetOverlappingEnable(true);
			leaveGoblinTimerMessageEvent->Rename("leaveGoblinTimerMessage");
			AssetLibrary::_GetAssets().AddObject(leaveGoblinTimerMessageEvent);


			goblinMessenger5->SetEventExpr("leaveGoblinTimerMessage & goblinExist");
	
	

			AssetLibrary::_GetAssets().AddObject(goblinMessenger5);
		}



		{
			// ���������� �� ����� �������...
			Trigger* goblinMessenger6 = new Trigger();
			goblinMessenger6->Rename("goblin_messenger6");  
			goblinMessenger6->SetActionExpr("goblinGoneMessage, destroyGoblin");    
				
			
 
			// �������� 
			IAction* goblinGoneMessageAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "goblin is gone...");
			goblinGoneMessageAction->Rename("goblinGoneMessage");
			AssetLibrary::_GetAssets().AddObject(goblinGoneMessageAction);

				
			// ��������
			IAction* destroyGoblinAction = Trigger::_CreateAction(AbstractObject::DESTROY_OBJECT_MESSAGE, "goblin");
			destroyGoblinAction->Rename("destroyGoblin");
			AssetLibrary::_GetAssets().AddObject(destroyGoblinAction);


			goblinMessenger6->SetEventExpr("goblin_messenger5"); 



			AssetLibrary::_GetAssets().AddObject(goblinMessenger6);
		}

		{
			// ���������� �� �������� �������...
			Trigger* goblinMessenger8 = new Trigger();
			goblinMessenger8->Rename("goblin_messenger8");
			goblinMessenger8->SetActionExpr("rubbush, killGoblinMessage, destroyGoblin");// �������� rubbish �� ����������, ������ ��� �� �������� ���������� ��������� �������� ��������

				
			
			// ��������
			IAction* killGoblinMessageMessage = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "goblin is dead...");
			killGoblinMessageMessage->Rename("killGoblinMessage");
			AssetLibrary::_GetAssets().AddObject(killGoblinMessageMessage);

						
			// �������
			IEvent* killGoblinEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "Chat", "kill goblin");// ���������� �������� �������������� ������ ������� ��� ��������� ������ ������� ���� ����� "������ ����, ������ ������������" � �.�. ��� ���������� ��� ����, ����� ������� goblin_messenger8 �� ������ ���� ������ �������� ����� ����� ���
			killGoblinEvent->SetOverlappingEnable(true);
			killGoblinEvent->Rename("killGoblin");
			AssetLibrary::_GetAssets().AddObject(killGoblinEvent);


			goblinMessenger8->SetEventExpr("killGoblin & goblinExist");  


			AssetLibrary::_GetAssets().AddObject(goblinMessenger8);	
		}



		{
			// ������������ �������...
			Trigger* goblinMessenger7 = new Trigger();
			goblinMessenger7->Rename("goblin_messenger7");
			goblinMessenger7->SetActionExpr("checkGoblinExist, restart1, restart2, restart3"); 
			
		
			// �������
			IEvent* goblinIsDeadEvent = Trigger::_CreateEvent(AbstractObject::DESTROY_OBJECT_MESSAGE, "goblin");
			goblinIsDeadEvent->Rename("goblinIsDead");
			AssetLibrary::_GetAssets().AddObject(goblinIsDeadEvent); 

			goblinMessenger7->SetEventExpr("goblinIsDead");

		 
			AssetLibrary::_GetAssets().AddObject(goblinMessenger7); 
		}


        // ����������� �� ������ ����
		Connect(MessageHandler<int32>::SEND_MESSAGE, this, Caller<>(this, &GoblinGenerator::CreateGoblin));
	} 
	


	// ������� ������� � ���������� ������ � �������� ��� � ���������� �������
	public: void CreateGoblin(StringANSI _name)
	{
		AbstractObject* goblin = new AbstractObject();
		goblin->Rename(_name);// ������ ������� ���
		AssetLibrary::_GetAssets().AddObject(goblin);
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
	private: GoblinGenerator* goblinGenerator;// ��������� ��������


    // METHODS
    // ���������������� �����������
	public: Application(void): font(NIL), /*camera(NIL), */camera2d(NIL), scene(NIL), chat(NIL), goblinGenerator(NIL)
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

			// ��������� ��������...
			{
				goblinGenerator = new GoblinGenerator();
				goblinGenerator->Rename("GoblinGenerator");
				AssetLibrary::_GetAssets().AddObject(goblinGenerator); 
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Trigger.example_03";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
