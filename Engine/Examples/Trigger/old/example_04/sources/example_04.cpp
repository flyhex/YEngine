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



// �����, ����������� ���� � ���������� ������������
class GraphicsQuad: public GraphicsObject,// ����������� ������
	                public Quad// ���� � ���������� ������������
{
	// METHODS
	// ����������� ��� ����������
    public: GraphicsQuad(void)
	{}

	// ���������������� �����������
	public: GraphicsQuad(Vector3 _leftBottom, Vector3 _rightTop): Quad(_leftBottom, _rightTop)
	{}


	// ����������
    public: virtual ~GraphicsQuad(void)
	{
		Destroying();
	}


	// ������������ �����
	public: virtual void Draw(void)
	{
		GraphicsLine::_Draw(GetVertex1(), GetVertex2(), GetColor());
		GraphicsLine::_Draw(GetVertex2(), GetVertex3(), GetColor());
		GraphicsLine::_Draw(GetVertex3(), GetVertex4(), GetColor());
		GraphicsLine::_Draw(GetVertex4(), GetVertex1(), GetColor());
	}


	// ���������� � ��������...
	// ��������� ������ �� ����� �� ���� _path, � ���������� true � ������ ������
	// ����������: ���� _auto ����� true, �� �������� ������� ������������ �� �����, �� ����������� ����������
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true)
	{
		return false;
	}
};



// �����, ����������� ����������� �������
class VirtualKey: public ColoredMesh// ������� ������������� �����
{
	// ������������� ������
	friend class VirtualKeyboard;


	// DATA
	public: PlayerControl* playerControl;// ���������� ����� � ����������
	public: Trigger* trigger1;// �������-������������ ������� �����
    public: Trigger* trigger2;// �������-�������, ������������ �������
	public: Trigger* trigger3;// �������-�������, �������������� ������� 
	public: Trigger* trigger4;// �������, ��������� ��������� ����������� �������
	public: Trigger* trigger5;// �������������� ������ ��������� �������
	public: Trigger* trigger6;
	private: Quad quad;// ����, ����������� ������ �������



	// METHODS
	// ����������� �� ���������
	public: VirtualKey(void)
	{
		SetColor(RGBA(RGBA::RED_COLOR));

		playerControl = new PlayerControl();
		trigger1 = new Trigger();
		trigger2 = new Trigger();
		trigger3 = new Trigger();
		trigger4 = new Trigger();
		trigger5 = new Trigger();
		trigger6 = new Trigger();
	}


	// ����������� ����������
	public: virtual ~VirtualKey(void)
	{
		Destroying();

		delete playerControl;
		delete trigger1;
		delete trigger2;
		delete trigger3;
		delete trigger4;
		delete trigger5;
		delete trigger6;
	}


	// ���������� ������������� ����������� �������
	public: Keyboard::Key GetKey(void)const
	{
		Keyboard::Event* event = dynamic_cast<Keyboard::Event*>(playerControl->GetEvents().GetHead());
		if(event) { return event->key; }
		return Keyboard::NO_KEY;
	}
		

	// ���������� true ���� ������� ����������
	public: bool IsExist(void)const
	{
		Keyboard::Event* event = dynamic_cast<Keyboard::Event*>(playerControl->GetEvents().GetHead());
		if(event) { return true; }
		return false;
	}


	// ������ ����� ����������� �������
	public: void SetQuad(Quad _quad)
	{
		quad = _quad;
	}

	// ������������ ����������� �������
	public: virtual void Draw(void)
	{
		// ��������� ��������-������� �������������...
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetVertex(0)), GetTransformation().TransformVector(GetVertex(1)), GetColor());
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetVertex(1)), GetTransformation().TransformVector(GetVertex(2)), GetColor());
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetVertex(2)), GetTransformation().TransformVector(GetVertex(3)), GetColor());
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetVertex(3)), GetTransformation().TransformVector(GetVertex(0)), GetColor());
	}
};



// �����, ����������� ����������� ����������
class VirtualKeyboard: public TexturedMesh// ���������������� ������������� ����� 
{
	// DATA
	private: MagicList<VirtualKey> virtualKeys;// ������ ����������� ������
	private: MagicList<Keyboard::Event> lastKeyboardEvent;// ��������� ������������ ������� ����������
	private: MagicList<AbstractObject> events; // ������ ���� ������� (�������)
	private: MagicList<AbstractObject> actions; // ������ ���� ��������
	
	// �������� ��������
    private: TexturedAnimation* texture;// �������� ����������
	private: Variable<int32>* keyCounter;// ������� ����������� ������



	// METHODS
	// ����������� �� ���������
	public: VirtualKeyboard(void)
	{
		// ������� ��������, ����������� ����������...
		texture = new TexturedAnimation(); 
		texture->LoadFromFile("keyboard");

		BindAnimation(texture);
		CreateQuad2D(Quad(Vector3(), Vector2(1498, 458) / 400.0f));


		// ����������...
		{
			// ������� ����������� ������
			keyCounter = new Variable<int32>(0);
			keyCounter->Rename("keyCount");
			AssetLibrary::_GetAssets().AddObject(keyCounter);	
		}

		AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &VirtualKeyboard::AddObjects));// ����������� �� ��������� ��������	
	}


	// ����������
	public: virtual ~VirtualKeyboard(void)
	{
		Destroying();
 
		virtualKeys.Clear(MagicContainer::DELETE_NODE);

		delete texture;
	}

	
	// ������� �� �������� ����� �������� � �������
	private: void AddObjects(AbstractObject* _object)
	{
		if(_object->IsPropertyExist(IEvent::EVENT_CLASS))
		{
			events.AddToTail(_object);
		}
		else if(_object->IsPropertyExist(IAction::ACTION_CLASS))
		{
			actions.AddToTail(_object);
		}
	}


	// ���������� ��������� ����������� ����������� �������
	public: VirtualKey* GetLastVirtualKey(void)
	{
		return virtualKeys.GetTail();
	}


	// ���������� ������ ���� ����������� ������
	public: MagicList<VirtualKey>& GetVirtualKeys(void)
	{
		return virtualKeys;
	}


	// ���������� ��������� ������� (����������) ����������� �������
	public: VirtualKey* GetLastSelectedVirtualKey(void) 
	{ 
		MagicList<VirtualKey>::AddressDerived* it = virtualKeys.GetHeadAddress();

		// ������ ��� ����������� �������
		for(MagicList<VirtualKey>::AddressDerived* it = virtualKeys.GetHeadAddress(); it != NIL; it = it->GetRight())
		{
			if(it->GetNode()->trigger2->GetValue())// ���� ������� �����������
			{
				return it->GetNode();
			}
		}
		return NIL;
	}


	// ��������� ����� ����������� ������� _vKey
	public: void AddVirtualKey(VirtualKey* _vKey)
	{
		virtualKeys.AddToTail(_vKey);// �������� � ������ ������
	}

		
	// ���������� ������ (���������� �����) ������� �������
	public: int32 GetCurrentKeyIndex(void)const
	{
		return keyCounter->GetValue();
	}

	// ����������� ������ (���������� �����) ������� �������
	public: void UpdateCurrentKeyIndex(void)
	{
		keyCounter->SetValue(keyCounter->GetValue() + 1);
	}  


	// ���������� � ��������...	
	// ��������� ����������� ���������� �� ���� _path, � ���������� true � ������ ������
	public: virtual bool SaveToFile(StringANSI _path = "")
	{
		File file(_path, File::REWRITE);

		// ��������� ��� �������...
		file.Write(events.GetSize());// ���������� ������� (�������)

		for(int32 i = 0; i < events.GetSize(); ++i)
		{
			events[i]->SaveToFile(file);
		}
		//events.Clear(MagicContainer::DELETE_NODE);

		// ��������� ��� ��������...
		file.Write(actions.GetSize());// ���������� ��������

		for(int32 i = 0; i < actions.GetSize(); ++i)
		{
			actions[i]->SaveToFile(file);
		}
		//actions.Clear(MagicContainer::DELETE_NODE);

		file.Write(virtualKeys.GetSize());// ���������� ����������� ������ 

		// ��������� ��� ����������� �������...
		for(int32 i = 0; i < virtualKeys.GetSize(); ++i)
		{ 
			VirtualKey* vKey = virtualKeys[i];

			// ��� ����������� �������
			file.Write(vKey->GetName().size());
			file.WriteString(vKey->GetName());

			// ����, ����������� ����� �������
			file.Write(vKey->quad);

			// ���������� ����� ��� �������
			file.Write(vKey->playerControl->IsExist()); 

			if(vKey->playerControl->IsExist())
			{ vKey->playerControl->SaveToFile(file); }

			// ��������� ������� 
			vKey->trigger1->SaveToFile(file);

			vKey->trigger2->SaveToFile(file);

			vKey->trigger3->SaveToFile(file);

			vKey->trigger4->SaveToFile(file);

			vKey->trigger5->SaveToFile(file);

			vKey->trigger6->SaveToFile(file);
		}
		return true;
	}


	// ��������� ����������� ���������� �� ����� �� ���� _path, � ���������� true � ������ ������
	// ����������: ���� ���������� ��� ���������, �� �������� false
	// ����������: ���� _auto ����� true, �� �������� ����������� ������������ �� �����, �� ��������������� ����������
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true)
	{ 
		File file(_path, File::READ);

		if(file.IsOpened())// ���� ���� ������
		{
			// ��������� ��� �������...
			int32 amountOfEvents;// ���������� ������� (�������)
			file.Read(amountOfEvents);

			// ��������� ��� ������� �������...
			for(int32 i = 0; i < amountOfEvents; ++i)
			{
				IEvent* event = IEvent::_LoadFromFile(file);
				AssetLibrary::_GetAssets().AddObject(event);
			}

			// ��������� ��� ��������...
			int32 amountOfActions;// ���������� ��������
			file.Read(amountOfActions);
		
			// ��������� ��� ������� ��������... 
			for(int32 i = 0; i < amountOfActions; ++i)
			{
				IAction* action = IAction::_LoadFromFile(file);
				AssetLibrary::_GetAssets().AddObject(action);
			}

			// ���������� ����������� ������...
			int32 amountOfVirtualKeys = 0;
			file.Read(amountOfVirtualKeys);

			// ��������� ��� ������ ����������� �������...
			for(int32 i = 0; i < amountOfVirtualKeys; ++i)
			{
				VirtualKey* vKey = new VirtualKey(); 

				// ��� ����������� �������
				StringANSI name;
				int32 length;
				file.Read(length);
				file.ReadString(name, length);
				vKey->Rename(name);

				// ����, ����������� ����� �������
				file.Read(vKey->quad);
				vKey->CreateQuad2D(vKey->quad);// ������� ������
										
				/* ����������� ������� ������ ��������� ������������ �� ����, ��� ����� ��������� �� ��������,
			       �.�. ��������� �� ��� (����� ��� ������������ �������) ����� ��������� ����� �� */ 
				AssetLibrary::_GetAssets().AddObject(vKey);// �������� � ���������� �������
				
				// ���������� ����� ��� �������
				bool exist; 
				file.Read(exist);
				if(exist) { vKey->playerControl->LoadFromFile(file); }

				// ��������� �������
				vKey->trigger1->LoadFromFile(file);

				vKey->trigger2->LoadFromFile(file);

				vKey->trigger3->LoadFromFile(file);

				vKey->trigger4->LoadFromFile(file);
		
				vKey->trigger5->LoadFromFile(file);

				vKey->trigger6->LoadFromFile(file);

				virtualKeys.AddToTail(vKey);// �������� � ������ ������

				AssetLibrary::_GetAssets().AddObject(vKey->playerControl); 
				AssetLibrary::_GetAssets().AddObject(vKey->trigger1); 
				AssetLibrary::_GetAssets().AddObject(vKey->trigger2); 
				AssetLibrary::_GetAssets().AddObject(vKey->trigger3);  
				AssetLibrary::_GetAssets().AddObject(vKey->trigger4);
				AssetLibrary::_GetAssets().AddObject(vKey->trigger5);
				AssetLibrary::_GetAssets().AddObject(vKey->trigger6);
			} 
		}
		return true;
	}
};



// ���������������� ����������
class Application: public Engine,// ������
               	   public MessageHandler<int32>::Receiver// ���������� ���������
{
	// DATA
    private: Camera3D* camera, *camera2d;// ������
    private: Font2D *font;// �����
	private: Scene* scene;// �����
	
	private: VirtualKeyboard* virtualKeyboard;// ����������� ����������
	private: bool rightPressed;// ���� true, �� ������ ������ ���� ������
	private: GraphicsQuad* fantomQuad;// ��������� ����; ������������ ��� ��������� �����
	private: bool fantomQuadDrawing; // ���� true, �� ��������� ���� ��������� �� �����
	private: PlayerControl* leftClickController;// ���������� ������� �����
	private: VirtualKey* lastVirtualKey;// ��������� ��������� ����������� �������
	


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font(NIL), scene(NIL), virtualKeyboard(NIL), rightPressed(false), fantomQuad(NIL), fantomQuadDrawing(false), leftClickController(NIL), lastVirtualKey(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		virtualKeyboard->SaveToFile("keyboard.script"); 

		delete virtualKeyboard;
		delete fantomQuad;
		delete leftClickController;

		delete camera;
		delete camera2d;

		delete font;

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

			// ������� �����...
			{
				leftClickController = new PlayerControl();
			
				Mouse::Event* evt1 = new Mouse::Event();
				evt1->key = Mouse::LEFT_BUTTON;
				evt1->type = Mouse::KEY_DOWN;
				leftClickController->AddEvent(evt1);
				
				leftClickController->Rename("leftClick");
				leftClickController->SetActivationMode(PlayerControl::SINGLE);
				
				AssetLibrary::_GetAssets().AddObject(leftClickController);
			}

			// ��������� ����...
			{
				fantomQuad = new GraphicsQuad();
				fantomQuad->Rename("fantomQuad");
				fantomQuad->SetColor(RGBA(RGBA::GREEN_COLOR));// ���� �����
				AssetLibrary::_GetAssets().AddObject(fantomQuad);
			}


			// ����������� ����������...
			{
				virtualKeyboard = new VirtualKeyboard(); 
				virtualKeyboard->Rename("virtualKeyboard");
				
				scene->AddGraphicsObject(virtualKeyboard);
				AssetLibrary::_GetAssets().AddObject(virtualKeyboard);
					
				// ��������� ����������� ���������� �� �����
				virtualKeyboard->LoadFromFile("keyboard.script");   


				// ��������� ��� ����� ����������� �������...
				for(int32 i = 0; i < virtualKeyboard->GetVirtualKeys().GetSize(); ++i)
				{
					VirtualKey* it = virtualKeyboard->GetVirtualKeys()[i];
								
					scene->AddGraphicsObject(it);// �������� ������� � ����������� �����
				}
			}

			// ������...
			{
				Camera3D::Define def;
			
				def.rotationMode = Camera3D::SCROLL_OBJECT_VIEWER;// ����� �������
				def.viewingPoint = virtualKeyboard->GetPosition();
				def.rotationEnable = false;
				def.yawClampEnable = true;
				def.pitchClampEnable = true;
				def.zoomEnable = true;

				def.pitchMax = Angle::_DegreesToRadians(40);
				def.pitchMin = Angle::_DegreesToRadians(-40);
				def.yawMax = Angle::_DegreesToRadians(40);
				def.yawMin = Angle::_DegreesToRadians(-40);

				camera = new Camera3D();
				camera->Set(def);
				
				camera->Pick(); // ������� ������
				scene->BindCamera(camera);

				// ������ ����������� � ������������ ��� ������
				camera->SetNearZoom(camera->GetNearZoom() * 10);
				camera->SetFarZoom(camera->GetFarZoom() * 2);
				
				AssetLibrary::_GetAssets().AddObject(camera);// �������� � �����
			}
	

			// ������ ��� ���������� �������...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;

				def.pitchClampEnable = false;// ��������� ���������� �������� ������ ��� X
				
				camera2d = new Camera3D();
				camera2d->Set(def);
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
	

		if(virtualKeyboard->GetLastSelectedVirtualKey())
		{
			font->DrawScreenText(Vector3(10, ClientAreaSize().y - (i+=20), -1), "last key: %s", Keyboard::_GetKeyName(virtualKeyboard->GetLastSelectedVirtualKey()->GetKey()).c_str()); 
		}
		else 
		{ 
			font->DrawScreenText(Vector3(10, ClientAreaSize().y - (i+=20), -1), "last key: ");
		}
		
	
		//font2d->DrawScreenText(Vector3(10, ClientAreaSize().y - (i+=20), -1), "current mode: %s", toolMode == EDIT ? "EDIT" : "READ"); 

		font->DrawScreenText(Vector3(10, ClientAreaSize().y - (i+=20), -1), "amount of virtual keys: %d", virtualKeyboard->GetVirtualKeys().GetSize()); 

	
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
		camera2d->EndTransform(Renderer::PROJECTION);
		camera2d->EndTransform(Renderer::MODELVIEW);
	}


	// ������ ������� ����
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Engine::MouseEventFilter(_event);

		if(_event->type == Mouse::KEY_PRESSED)
		{
			if(_event->key == Mouse::RIGHT_BUTTON)// ������ �������� ������
			{
				// ��������� �������� ������ ������ ��� ������� ������ �����

				if(!rightPressed)// ��������� ������ ���� ���
				{
					Mouse::_MoveCursorToCenter(Renderer::_GetPicked());// ���������� ���� � ����� ���������� ������� ���� 
					camera->SetRotationEnable(true);// ��������� �������� ������
					rightPressed = true;
					Mouse::_SetShowCursor(false);// ������ ������
				}
			}
			else if(_event->key == Mouse::LEFT_BUTTON)// ������ ��������� ����� ��������
			{
				if(!fantomQuadDrawing)// ��������� ������ ���� ���
				{
					// �������� ��������� ����...
					scene->AddGraphicsObject(fantomQuad);// �������� � ����������� �����

					Vector3 pos = GetPointUnderCursor();//�������� ���������� �������
						
					fantomQuad->SetVertex4(Vector3(pos.x, pos.y, 0.003));// ������ ����� ������� �������	
					fantomQuad->SetVertex2(Vector3(pos.x, pos.y, 0.003));// ������ ������ ������ �������
					fantomQuadDrawing = true;
				} 
			} 
		}
		else if(_event->type == Mouse::MOVE)// ���� ���� ���������
		{
			if(fantomQuadDrawing)// ���� �������� ����
			{
				Vector3 pos = GetPointUnderCursor();//�������� ���������� �������

				fantomQuad->SetVertex2(Vector3(pos.x, pos.y, 0.003));// ������ ������ ������ �������
			}
		}
		else if(_event->type == Mouse::KEY_UP)// ����� ������ ��������
		{
			if(_event->key == Mouse::RIGHT_BUTTON)// ����� �������� ������
			{
				if(rightPressed)// ��������� ������ ���� ���
				{
					camera->SetRotationEnable(false); // ��������� �������� ������
					rightPressed = false;
					Mouse::_SetShowCursor(true);// �������� ������
				}
			}
			else if(_event->key == Mouse::LEFT_BUTTON)// ����� ��������� ����� ��������
			{
				// �������� ��������� ����...
				if(fantomQuadDrawing) // ���� �������� ����
				{
					scene->GetDrawList().Remove(fantomQuad);// ������ ��������� ���� �� ����������� �����
					fantomQuadDrawing = false;
						
					if(fantomQuad->GetSize().x > 0.03f && fantomQuad->GetSize().y > 0.03f)// ������ �� �������� ������ ������ 0.03 � 0.03
					{
						// ������ �� �������� ������ ��� ���-���...
						if(lastVirtualKey)// ���� ���������� ���� �� ���� ����������� �������
						{
							if(!lastVirtualKey->IsExist())// ���� ��������� ����������� ������� �� ����������
							{
								delete lastVirtualKey;// ������� ������������� ����������� �������
								lastVirtualKey = NIL;
							}
						}

						// ����� ������� ����������...
						// ������� �������� ���� �� ���� ����������...
						lastVirtualKey = new VirtualKey();
  
						lastVirtualKey->CreateQuad2D(*(static_cast<Quad*>(fantomQuad)));// ������� ������
						lastVirtualKey->SetColor(RGBA(RGBA::RED_COLOR));// ������ ���� 
						lastVirtualKey->SetQuad(*(static_cast<Quad*>(fantomQuad)));// ��������� ������

						if(lastVirtualKey->IsClockwise())// ���� ������� ����������� �� ������� �������
						{
							lastVirtualKey->InverseVertices();// �������� ����������� ������
						}
							 
						scene->GetDrawList().AddObject(lastVirtualKey);// �������� ����������� ������� � ������ ������������
					}
				}
			}
		}
		return _event;
	}


	// ��������� ������� ����������
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event)
	{
		Engine::KeyboardEventFilter(_event);

		if(_event->type == Keyboard::KEY_DOWN)
		{	
			if(lastVirtualKey)// ����� ��������� ����������� ������� �� �������
			{
				// ��������� ��� ����������� ������� hot-key...
				if(lastVirtualKey && !lastVirtualKey->IsExist())// ��������� ������ ���� ��� ��� ����� � ��� �� �������
				{ 
					lastVirtualKey->Rename("virtualKey_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// ������ ��� ����������� �������
					virtualKeyboard->AddVirtualKey(lastVirtualKey);// �������� ������� � ������ ����������� ����������
					AssetLibrary::_GetAssets().AddObject(lastVirtualKey);


					// ������� ���������� �����...
					Keyboard::Event* evt1 = new Keyboard::Event();
					evt1->key = _event->key;
					evt1->type = Keyboard::KEY_DOWN;
 
					lastVirtualKey->playerControl->AddEvent(evt1);// �������� ������� �����
					lastVirtualKey->playerControl->Rename("playerControl_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// ������������� ����������
					lastVirtualKey->playerControl->SetActivationMode(PlayerControl::SINGLE);// ����� �����
					AssetLibrary::_GetAssets().AddObject(lastVirtualKey->playerControl);
                    
						
					
					// �������� ������ OVERLAP ��� �������, �� �������������� ������ ������...
					// ������� ������������...
					lastVirtualKey->trigger1->Rename("trigger1_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// ������ ��� ��������
					lastVirtualKey->trigger1->SetActionExpr("inputEventMessage" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// �������


					// ������� �������
					IEvent* event1 = Trigger::_CreateEvent(PlayerControl::INPUT_EVENT_MESSAGE, lastVirtualKey->playerControl->GetName());
					event1->Rename("inputEvent" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(event1);

					
					// ������� ��������
					//IAction* action1 = Trigger::_CreateTemplateAction(IVariable::CREATE_VARIABLE_MESSAGE, "#var" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()), IVariable::_GetValueType<StringANSI>(), event1->GetName());
					//action1->Rename("createVariable" + NumberToString(virtualKeyboard->GetCurrentKeyIndex())); 
					//AssetLibrary::_GetAssets().AddObject(action1);

					
					// ������� ��������
					IAction* action2 = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, virtualKeyboard->GetName(), event1->GetName()); 
					action2->Rename("inputEventMessage" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(action2);

					lastVirtualKey->trigger1->SetEventExpr("inputEvent" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// ������� ���������

					
				
					
					// ������������ �������
					lastVirtualKey->trigger2->Rename("trigger2_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					lastVirtualKey->trigger2->SetActionExpr("enableColor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// �������


					// ������� �������  
					IEvent* event2 = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, virtualKeyboard->GetName(), event1->GetName());
					event2->SetOverlappingEnable(true);// ����� ����������
					event2->Rename("inputMessage" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(event2);

 
					// ������� ��������
					IAction* action3 = Trigger::_CreateAction(Color::CHANGE_COLOR_MESSAGE, lastVirtualKey->GetName(), RGBA(RGBA::GREEN_COLOR));
					action3->Rename("enableColor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex())); 
					AssetLibrary::_GetAssets().AddObject(action3);
					
					lastVirtualKey->trigger2->SetEventExpr("inputMessage" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// ������� ���������
				
					
					
					// �������������� �������...
					lastVirtualKey->trigger3->Rename("trigger3_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					lastVirtualKey->trigger3->SetActionExpr("disableColor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// �������

					// ������� ��������
					IAction* action4 = Trigger::_CreateAction(Color::CHANGE_COLOR_MESSAGE, lastVirtualKey->GetName(), RGBA(RGBA::RED_COLOR));
					action4->Rename("disableColor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(action4);

					lastVirtualKey->trigger3->SetEventExpr("~inputMessage" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// ������� ���������
					
					


					// �������� ����������, ������������ ��������� ����������� �������...
					lastVirtualKey->trigger4->Rename("trigger4_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					lastVirtualKey->trigger4->SetActionExpr("createUnderCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// �������

					// ������� �������� 
					IAction* action5 = Trigger::_CreateAction(IVariable::CREATE_VARIABLE_MESSAGE, StringANSI("underCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex())), IVariable::_GetValueType<bool>(), false);
					action5->Rename("createUnderCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(action5);
									
					lastVirtualKey->trigger4->SetEventExpr("");// ������� ���������
  

					

					// �������������� ������ ��������� �������
					lastVirtualKey->trigger5->Rename("trigger5_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					lastVirtualKey->trigger5->SetActionExpr("checkUnderCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// �������


					// ������� �������
					IEvent* event3 = Trigger::_CreateEvent(PlayerControl::INPUT_EVENT_MESSAGE, leftClickController->GetName());// ���� ������
					event3->Rename("mouseClick" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(event3);

					
					// ������� ��������
					IAction* action6 = Trigger::_CreateAction(AbstractSurface::CHECK_SURFACE_UNDER_CURSOR_MESSAGE, lastVirtualKey->GetName(), "underCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()), "", "");
					action6->Rename("checkUnderCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(action6);

					lastVirtualKey->trigger5->SetEventExpr("mouseClick" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// ������� ���������
					 


					// �������������� ������ ������ ����������� �������...
					lastVirtualKey->trigger6->Rename("trigger6_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					lastVirtualKey->trigger6->SetActionExpr("createInputEvent1" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()) + ", createInputEvent2" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// �������

					
					// ������� ��������
					IAction* action7 = Trigger::_CreateAction(Keyboard::CREATE_KEYBOARD_EVENT_MESSAGE, _event->key, Keyboard::KEY_DOWN);// ������������� ������� ����������
					action7->Rename("createInputEvent1" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(action7);
	

					// ������� ��������
					IAction* action8 = Trigger::_CreateAction(Keyboard::CREATE_KEYBOARD_EVENT_MESSAGE, _event->key, Keyboard::KEY_UP);// ������������� ������� ����������
					action8->Rename("createInputEvent2" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(action8);
						
					lastVirtualKey->trigger6->SetEventExpr("underCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// ������� ���������
				

					virtualKeyboard->UpdateCurrentKeyIndex();// ��������� ������� ������		
				}
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
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(1200, 760);// ������ ����
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Trigger.example_04";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
