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
class VirtualKey: public GraphicsSurface,// ������� ������������� �����
	              public MessageHandler<int32>::Receiver// ���������� ���������
{
	// ������������� ������
	friend class VirtualKeyboard;


	// DATA
	private: Quad quad;// ����, ����������� ������ �������
	private: bool activated;// ���� true, �� ����������� ������� ������������
	public: Keyboard::Event event;// ������� ����������, ��������������� � ������ ����������� ��������
	public: bool isValid;// ���� true, �� ������� ����������
    private: Camera3D* camera;// ������


	// METHODS
	public: VirtualKey(Camera3D* _camera): camera(_camera), activated(false), isValid(false)
	{
		SetColor(RGBA(RGBA::RED_COLOR));
	}


	// ����������� �� ���������
	public: VirtualKey(void): activated(false), isValid(false)
	{
		SetColor(RGBA(RGBA::RED_COLOR));
	}


	// ����������� ����������
	public: virtual ~VirtualKey(void)
	{
		Destroying();
	}


	// ���������� true ���� ����������� ������� ������������
    public: bool IsActivated(void)const
	{
		return activated;
	}

			
	// ���������� ������� ����� �� ����
	public: void MouseEvent(Mouse::Event _event)
	{
		if(_event.type == Mouse::KEY_DOWN)
		{
			if(_event.key == Mouse::LEFT_BUTTON)
			{
				Vector3 intersectedPoint;

				// ���� ��������� ���� �� ������ ����������� �������...
				if(IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), intersectedPoint) != -1)
				{
					activated = true;
					SetColor(RGBA(RGBA::GREEN_COLOR));
				}
				else // ����� ������
				{ 
					activated = false; 
					SetColor(RGBA(RGBA::RED_COLOR));
				}
			}
		}
	}


	// ���������� ������� ����� �� ����������
	public: void KeyboardEvent(Keyboard::Event _event)
	{
		if(event.ExactEquals(_event))
		{
			activated = true;
			SetColor(RGBA(RGBA::GREEN_COLOR));
		}
		else 
		{ 
			activated = false; 
			SetColor(RGBA(RGBA::RED_COLOR));
		}
	}


	// ���������� ������������� ����������� �������
	public: Keyboard::Key GetKey(void)const
	{
		return event.key; 
	}
		

	// ���������� true ���� ������� ����������
	public: bool IsExist(void)const
	{
		return isValid;
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
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetMesh()->GetVertex(0)), GetTransformation().TransformVector(GetMesh()->GetVertex(1)), GetColor());
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetMesh()->GetVertex(1)), GetTransformation().TransformVector(GetMesh()->GetVertex(2)), GetColor());
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetMesh()->GetVertex(2)), GetTransformation().TransformVector(GetMesh()->GetVertex(3)), GetColor());
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetMesh()->GetVertex(3)), GetTransformation().TransformVector(GetMesh()->GetVertex(0)), GetColor());
	}
};



// �����, ����������� ����������� ����������
class VirtualKeyboard: public TexturingSurface// ���������������� ������������� ����� 
{
	// DATA
	private: MagicList<VirtualKey> virtualKeys;// ������ ����������� ������
	private: MagicList<Keyboard::Event> lastKeyboardEvent;// ��������� ������������ ������� ����������
    private: Camera3D* camera;// ������

	// �������� ��������
    private: Texture2D* texture;// �������� ����������


	// METHODS
	// ����������� �� ���������
	public: VirtualKeyboard(Camera3D* _camera): camera(_camera)
	{
		// ������� ��������, ����������� ����������...
		texture = new Texture2D(); 
		texture->LoadFromFile("keyboard");

		BindTexture(texture);
		Mesh* mesh = new Mesh();
		mesh->CreateQuad2D(Quad(Vector3(), Vector2(1498, 458) / 400.0f));
		BindMesh(mesh);
	}


	// ����������
	public: virtual ~VirtualKeyboard(void)
	{
		Destroying();
 
		virtualKeys.Clear(MagicContainer::DELETE_NODE);

		delete texture;
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
			if(it->GetNode()->IsActivated())
			{
				return it->GetNode();
			}
		}
		return NIL;
	}


	// ��������� ����� ����������� ������� _virtualKey
	public: void AddVirtualKey(VirtualKey* _virtualKey)
	{
		virtualKeys.AddToTail(_virtualKey);// �������� � ������ ������
	}


	// ���������� � ��������...	
	// ��������� ����������� ���������� �� ���� _path, � ���������� true � ������ ������
	public: virtual bool SaveToFile(StringANSI _path = "")
	{
		File file(_path, File::REWRITE);

		file.Write(virtualKeys.GetSize());// ���������� ����������� ������ 

		// ��������� ��� ����������� �������...
		for(int32 i = 0; i < virtualKeys.GetSize(); ++i)
		{ 
			VirtualKey* virtualKey = virtualKeys[i];

			// ����, ����������� ����� �������
			file.Write(virtualKey->quad);

			virtualKey->event.SaveToFile(file);
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
			// ���������� ����������� ������...
			int32 amountOfVirtualKeys = 0;
			file.Read(amountOfVirtualKeys);

			// ��������� ��� ������ ����������� �������...
			for(int32 i = 0; i < amountOfVirtualKeys; ++i)
			{
				VirtualKey* virtualKey = new VirtualKey(camera);  
				Mesh* mesh = new Mesh();
				
				// ����, ����������� ����� �������
				file.Read(virtualKey->quad);
				mesh->CreateQuad2D(virtualKey->quad);// ������� ������
				virtualKey->BindMesh(mesh);

				virtualKey->event.LoadFromFile(file);

				virtualKey->isValid = true;
				Renderer::_GetPicked()->Connect(Window::KEYBOARD_KEY_DOWN_MESSAGE, virtualKey, Caller<>(virtualKey, &VirtualKey::KeyboardEvent));
				Renderer::_GetPicked()->Connect(Window::MOUSE_KEY_DOWN_MESSAGE, virtualKey, Caller<>(virtualKey, &VirtualKey::MouseEvent));
				
				virtualKeys.AddToTail(virtualKey);// �������� � ������ ������
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
	private: VirtualKey* lastVirtualKey;// ��������� ��������� ����������� �������
	private: Vector3 pointUnderCursor;// ����� � ������� ������������, �� ������� ��������� ������


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font(NIL), scene(NIL), virtualKeyboard(NIL), rightPressed(false), fantomQuad(NIL), fantomQuadDrawing(false), lastVirtualKey(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		virtualKeyboard->SaveToFile("keyboard.script"); 

		delete virtualKeyboard;
		delete fantomQuad;

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
				//scene->Pick();
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
				//font->Pick();// ������� �����
			}


			// ��������� ����...
			{
				fantomQuad = new GraphicsQuad();
				fantomQuad->Rename("fantomQuad");
				fantomQuad->SetColor(RGBA(RGBA::GREEN_COLOR));// ���� �����
				scene->AddObject(fantomQuad);
			}



			// ������...
			{
				Camera3D::Define def;
			
				def.rotationMode = Camera3D::OBJECT_VIEWER_ROTATION;// ����� �������
				//def.viewingPoint = virtualKeyboard->GetPosition();
				def.zoomEnable = true;
				def.rotationEnable = false;
				def.yawClampEnable = true;
				def.pitchClampEnable = true;

				def.pitchMax = Angle::_DegreesToRadians(40);
				def.pitchMin = Angle::_DegreesToRadians(-40);
				def.yawMax = Angle::_DegreesToRadians(40);
				def.yawMin = Angle::_DegreesToRadians(-40);

				camera = new Camera3D();
				camera->Create(def);
				
				//camera->Pick(); // ������� ������
				scene->BindCamera(camera);

				// ������ ����������� � ������������ ��� ������
				camera->SetZoomNear(camera->GetZoomNear() * 10);
				camera->SetZoomFar(camera->GetZoomFar() * 2);
			}
	


			// ����������� ����������...
			{
				virtualKeyboard = new VirtualKeyboard(camera); 
				virtualKeyboard->Rename("virtualKeyboard");
				
				scene->AddGraphicsObject(virtualKeyboard);
				
					
				// ��������� ����������� ���������� �� �����
				virtualKeyboard->LoadFromFile("keyboard.script");   


				// ��������� ��� ����� ����������� �������...
				for(int32 i = 0; i < virtualKeyboard->GetVirtualKeys().GetSize(); ++i)
				{
					VirtualKey* it = virtualKeyboard->GetVirtualKeys()[i];
								
					scene->AddGraphicsObject(it);// �������� ������� � ����������� �����
				}
			}


			camera->SetPosition(virtualKeyboard->GetPosition());



			// ������ ��� ���������� �������...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;

				def.pitchClampEnable = false;// ��������� ���������� �������� ������ ��� X
				
				camera2d = new Camera3D();
				camera2d->Create(def);
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
		//Engine::Update(_delta);
		scene->Update();
	}



	// ������� ������������
	public: virtual void Draw(float _delta)
	{
		ClearScene();

		//Engine::Draw(_delta);
		scene->Draw();
		
		pointUnderCursor = camera->GetPointUnderPixel(Mouse::_GetCursorPosition(this));


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

		//font->DrawScreenText(Vector3(10, ClientAreaSize().y - (i+=20), -1), "amount of virtual keys: %d", virtualKeyboard->GetVirtualKeys().GetSize()); 

	
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
		camera->MouseEventFilter(_event);

		
		if(_event->type == Mouse::MOVE)// ���� ���� ���������
		{
			//if(_event->type == Mouse::KEY_PRESSED)
			{
				if(Mouse().IsKeyPressed(Mouse::RIGHT_BUTTON))// ������ �������� ������
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
				else if(Mouse().IsKeyPressed(Mouse::LEFT_BUTTON))// ������ ��������� ����� ��������
				{
					if(!fantomQuadDrawing)// ��������� ������ ���� ���
					{
						// �������� ��������� ����... 
						scene->AddGraphicsObject(fantomQuad);// �������� � ����������� �����

						Vector3 pos = pointUnderCursor;//�������� ���������� �������
							
						fantomQuad->SetVertex4(Vector3(pos.x, pos.y, 0.003));// ������ ����� ������� �������	
						fantomQuad->SetVertex2(Vector3(pos.x, pos.y, 0.003));// ������ ������ ������ �������
						fantomQuadDrawing = true;
					} 
				} 
			}

			if(fantomQuadDrawing)// ���� �������� ����
			{
				Vector3 pos = pointUnderCursor;//�������� ���������� �������

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
						lastVirtualKey = new VirtualKey(camera);
						Mesh* mesh = new Mesh();
						mesh->CreateQuad2D(*(static_cast<Quad*>(fantomQuad)));// ������� ������
						lastVirtualKey->BindMesh(mesh); 

						lastVirtualKey->SetColor(RGBA(RGBA::RED_COLOR));// ������ ����  
						lastVirtualKey->SetQuad(*(static_cast<Quad*>(fantomQuad)));// ��������� ������


						if(mesh->IsClockwise())// ���� ������� ����������� �� ������� �������
						{
							mesh->InverseVertices();// �������� ����������� ������
						}
							 
						scene->GetDrawList().AddObject(lastVirtualKey);// �������� ����������� ������� � ������ ������������
					}
				}
			}
		}


		Mouse().ClearEventList();// �������� ����� �������
		return _event;
	}


	// ��������� ������� ����������
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event)
	{
		//Engine::KeyboardEventFilter(_event);

		if(_event->type == Keyboard::KEY_DOWN)
		{	
			if(lastVirtualKey)// ����� ��������� ����������� ������� �� �������
			{
				// ��������� ��� ����������� ������� hot-key...
				if(lastVirtualKey && !lastVirtualKey->IsExist())// ��������� ������ ���� ��� ��� ����� � ��� �� �������
				{ 
					virtualKeyboard->AddVirtualKey(lastVirtualKey);// �������� ������� � ������ ����������� ����������
					

					// ���� ��������� ������� playerControl...
					//    ������������ ������ ����������� �������, ����� �� ���� GREEN_COLOR.
                    // ���� ���� ������������ ������ ������� (�� playerControl)...
					//    �������������� ������ ����������� �������, ����� �� ���� RED_COLOR.
						
					// =�������������� ������ ���������=
					// ���� ��������� ����� ���� ����...
					//    ��������� ������������ ������ ����������� ������� � ��������...
					//    ���� ��������� ������������, �� ��������� ������� _event->key & Keyboard::KEY_DOWN � _event->key & Keyboard::KEY_UP...


					lastVirtualKey->isValid = true;

					// ��������� ������� �����...
					lastVirtualKey->event.key = _event->key;
					lastVirtualKey->event.type = Keyboard::KEY_DOWN;
 
					Connect(KEYBOARD_KEY_DOWN_MESSAGE, lastVirtualKey, Caller<>(lastVirtualKey, &VirtualKey::KeyboardEvent));
					Connect(MOUSE_KEY_DOWN_MESSAGE, lastVirtualKey, Caller<>(lastVirtualKey, &VirtualKey::MouseEvent));
				}
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
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(1300, 760);// ������ ����
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Virtual Keyboard";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
