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
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/InputHandler/InputHandler.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Helpers/Sprite/AnimatedSprite/AnimatedSprite.h>
#include <Engine/Helpers/Sprite/TexturedSprite/TexturedSprite.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>


// ��������� ����������� �������������
class GraphicsQuad: public GraphicsObject// ����������� ������
{
	// DATA
	private: Quad quad;// �������������, ����������� ���������� � ������ �����


	// METHODS
	// ����������� �� ���������
	public: GraphicsQuad(void)
	{}


	// ���������� ������ �� �������� �����
	public: Quad& GetQuad(void)
	{
		return quad;
	}



	// ������������...
	// ������������� ����
	public: virtual void Draw(void)
	{
		if(IsShowEnabled())
		{
			// ������ �������������� �����
			GraphicsLine::_Draw(quad.GetVertex4(), quad.GetVertex3(), GetColor());// ������ ����� �������

			// ������� �������������� �����
			GraphicsLine::_Draw(quad.GetVertex3(), quad.GetVertex2(), GetColor());// ������ ����� �������

			// ����� ������������ �����
			GraphicsLine::_Draw(quad.GetVertex2(), quad.GetVertex1(), GetColor());// ������ ������ ����

			// ������ ������������ �����
			GraphicsLine::_Draw(quad.GetVertex1(), quad.GetVertex4(), GetColor());// ������ ������ ����
		}
	}
};



// �����, ����������� ���������� ������
class MovingObjectEx: public TexturedSprite,// ���������������� ������
	                  public VectorLerp,// ��������� ������������
					  public MessageHandler<int32>::Receiver// ���������� ���������
{
	// DATA
	private: float currentSpeed;// ������� �������� 
	private: float maxSpeed;// ������������ ��������


	// METHODS
	// ����������� ��� ����������
	public: MovingObjectEx(void): currentSpeed(0.0f), maxSpeed(0.0f)
	{}


	// �������� �������...
	// ���������� ������� �������� �������
	public: float GetCurrentSpeed(void)
	{
		return currentSpeed;
	}


	// ���������� ������������ �������� �������
	public: float GetMaxSpeed(void)
	{
		return maxSpeed;
	}


	// ������� �� ���������� ����� ������� �������
	public: void UpdateTargetPosition(Vector3D _position)
	{
		currentSpeed = (_position - GetPosition()).Length() * (1.0f / Renderer::_Picked()->GetDeltaTimeInSec());

		if(currentSpeed > maxSpeed)
		{
			maxSpeed = currentSpeed;
		}
	}


	// ������ ������� ������� ��� �������
	// ����������: ������ ����������� ������ ����� ��������
	// ����������: movingEnable ����� �������
	public: void SetTargetPosition(Vector3D _target)
	{
		// ���������� �������� � ���� ����� ������������ ����� ������ �� ������� ������� � �������
		GetControlVertices().Clear();// �������� ������ ����������� ������
		GetControlVertices().AddToTail(GetPosition());
		GetControlVertices().AddToTail(_target);
		SetTarget((GetTargetPosition() - GetPosition()).Length()); // ������������ ���������� �� ���� ��� ������� ������� ��������� �������������
		Start();// ������ �������� � ����
	}

	
	// ���������� ������� ������� �������
	public: Vector3D GetTargetPosition(void)
	{
		if(GetControlVertices().IsNotEmpty())
		{
			return GetControlVertices().GetTail()->key;
		}
		return Vector3D();
	}
	

	// ���������� ��������� ������� �������
	public: Vector3D GetStartPosition(void)
	{
		if(GetControlVertices().IsNotEmpty())
		{
			return GetControlVertices().GetHead()->key;
		}
		return Vector3D();
	}

	// ���������� ���������...
	// ��������� ��������� �������
	// ����������: ������ ������ � ������� �������
	public: void Update(float _delta)
	{
		if(!IsFreezeEnabled())// ���� ������ ����������
		{
			if(MoveToTarget(_delta))// ���� ������ �������� ��������
			{
				SetPosition(CalculateNVertex(GetT())); // ��������� ������� ���� ������������ (������; �������)
			}
		}
	}
	// ���������� � ��������...
	// ��������� ������ � ����� ����� _file, � ���������� true � ������ ������
	// ����������: ���� ���� �� ������ �� ������, �� ������������ false
	protected: virtual bool SaveToFile(File& _file)
	{
		return true;
	}
			
	// ��������� ������ �� ����� _file, � ���������� true � ������ ������
	// ����������: ���� ���� �� ���������� ��� �� ������ �� ������, �� �������� false
	protected: virtual bool LoadFromFile(File& _file)
	{
		return true;
	}
};



// ���������������� ����������
class Application: public Engine// ������
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D *font;// �����
	private: Scene* scene;// �����
	private: MovingObjectEx* point1, *point2, *point3, *point4, *point5;// ���������� �������
    private: MovingObjectEx* selected;// ���������� ������, ���������� � ������� ����� ����������
	private: int32 index;// ������ �������� ����������� ����
    private: GraphicsQuad* quad;// ����������� ����, � ������ �������� ����� ��������� �������
	private: float deep;// ������� �� Z, �� ������� ��������� ���� ���������� �������
	private: Vector3D targetWorldPos;// ������� ������� ��� ���� ���������� ��������
	private: Texture2D* texture;// �������� ��� ���������� �����

    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font(NIL), scene(NIL), point1(NIL), point2(NIL), point3(NIL), point4(NIL), point5(NIL), selected(NIL), index(0), quad(NIL), deep(3.0f), texture(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete texture;
		delete camera;
		delete font;
		delete point1;
		delete point2;
		delete point3;
		delete point4;
		delete point5;
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
				AssetLibrary::_GetAssets().AddObject(scene);// �������� ����� � ���������� �������, ����� �� ����������
				scene->Pick();
			}

			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.fontName = "Arial";// ��� ������
				def.fontSize = 16;// ������ ������
				def.fontColor = RGBA(RGBA::GRAY_COLOR);// ���� ������

				font = new Font2D();
				font->Rename("font2d");
				font->Create(def);
				scene->AddObject(font);// �������� � �����
				font->Pick();// ������� �����
			}


			// ������...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::TRACKBALL_FIRST_PERSON;
				def.rotationEnabled = false;// ��������� �������� ������
				//def.distanceToViewingPoint = 1.45f;
				def.zoomEnabled = false;// ��������� ������

				camera = new Camera3D();
				camera->Create(def);
				camera->Pick(); // ������� ������
			}
		

			// ����������� ����...
			{
				quad = new GraphicsQuad();
				quad->GetQuad().SetSize(Vector2D(2.0f, 2.0f));// �������� ������ ���������
				quad->SetColor(RGBA(RGBA::GREEN_COLOR));

				quad->GetQuad().SetCenter(Vector3D(0.0f, 0.0f, -deep + 0.3f));// ������ ����� ���������

				scene->AddGraphicsObject(quad);// �������� � ����������� �����
			}


			{// ��������
				texture = new Texture2D();
				texture->Create(Vector2D(8, 8));
			}

			point1 = new MovingObjectEx();
			point1->Rename("UNITS_PER_SECOND");// ������ ��� 
				
			point1->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
			
			point1->BindTexture(texture);
			point1->SetTexturedMaterialEnable(true);

			point1->SetMoveSpeed(0.5f);// �������� �������� � ��������/�������
			point1->SetTarget(5.0f);// ���������� �� 0 �� 5 ����� �������� �� 2 �������
			point1->SetColor(RGBA(55, 111, 194, 255));
			point1->SetMovingMode(FloatLerp::UNITS_PER_SECOND);//������� � �������
			point1->SetPosition(Vector3D(0.0f, 0.0f, -deep));
			point1->Connect(VectorLerp::COMPUTE_NEW_VECTOR_MESSAGE, point1, Caller<>(point1, &MovingObjectEx::UpdateTargetPosition));
			scene->AddLogicObject(point1);
			scene->AddGraphicsObject(point1); 


			point2 = new MovingObjectEx();
			point2->Rename("PARABOLA");// ������ ��� 

			point2->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
		
			point2->BindTexture(texture);
			point2->SetTexturedMaterialEnable(true);

			point2->SetMoveSpeed(0.1f);// �������� �������� 
			point2->SetTarget(15.0f);// ���������� �� ����
			point2->SetColor(RGBA(118, 185, 0, 255));
			point2->SetMovingMode(FloatLerp::PARABOLA);//������� � �������
			point2->SetPosition(Vector3D(0.0f, 0.0f, -deep));
			point2->Connect(VectorLerp::COMPUTE_NEW_VECTOR_MESSAGE, point2, Caller<>(point2, &MovingObjectEx::UpdateTargetPosition));
			scene->AddLogicObject(point2);
			scene->AddGraphicsObject(point2);


			point3 = new MovingObjectEx();
			point3->Rename("FIXED_TIME");// ������ ��� 
				
			point3->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
			
			point3->BindTexture(texture);
			point3->SetTexturedMaterialEnable(true);

			point3->SetMoveSpeed(7);// �������� �������� � ��������
			point3->SetTarget(15.0f);// ���������� �� ����
			point3->SetColor(RGBA(163, 21, 21, 255));
			point3->SetMovingMode(FloatLerp::FIXED_TIME);//������� � �������
			point3->SetPosition(Vector3D(0.0f, 0.0f, -deep));
			point3->Connect(VectorLerp::COMPUTE_NEW_VECTOR_MESSAGE, point3, Caller<>(point3, &MovingObjectEx::UpdateTargetPosition));
			scene->AddLogicObject(point3);
			scene->AddGraphicsObject(point3);

			
			point4 = new MovingObjectEx();
			point4->Rename("DAMPING");// ������ ��� 

			point4->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
			
			point4->BindTexture(texture);
			point4->SetTexturedMaterialEnable(true);

			point4->SetMoveSpeed(3.0f);// �������� �������� 
			point4->SetTarget(5.0f);// ���������� �� ����
			point4->SetColor(RGBA(RGBA::MAGENTA_COLOR));
			point4->SetMovingMode(FloatLerp::DAMPING);
			point4->SetPosition(Vector3D(0.0f, 0.0f, -deep));
			point4->Connect(VectorLerp::COMPUTE_NEW_VECTOR_MESSAGE, point4, Caller<>(point4, &MovingObjectEx::UpdateTargetPosition));
			scene->AddLogicObject(point4);
			scene->AddGraphicsObject(point4);

	
			point5 = new MovingObjectEx();
			point5->Rename("MAGNETIC");// ������ ��� 

			point5->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
			
			point5->BindTexture(texture);
			point5->SetTexturedMaterialEnable(true);

			point5->SetMoveSpeed(0.2f);// �������� �������� 
			point5->SetTarget(10.0f);// ���������� �� ����
			point5->SetColor(RGBA(247, 170, 0, 255));
			point5->SetMovingMode(FloatLerp::MAGNETIC);
			point5->SetPosition(Vector3D(0.0f, 0.0f, -deep));
			point5->Connect(VectorLerp::COMPUTE_NEW_VECTOR_MESSAGE, point5, Caller<>(point5, &MovingObjectEx::UpdateTargetPosition));
			scene->AddLogicObject(point5);
			scene->AddGraphicsObject(point5);


	
			GraphicsLine::_SetLineWidth(3.0f);// ������� �����

			SelectFunction(FloatLerp::UNITS_PER_SECOND);

			// ���� ����...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));
			MoveToCenter();// ����������� ���� � ����� ������
			Mouse::_ShowCursor(true);// �������� ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}


	// �������� ������� � �������� _index � �������� �������
	public: void SelectFunction(int32 _index)
	{
		if(_index >= 0 && _index < 5)
		{
			index = _index;

			if(index == FloatLerp::UNITS_PER_SECOND)
			{
				selected = point1;
			}
			else if(index == FloatLerp::PARABOLA)
			{
				selected = point2;
			}
			else if(index == FloatLerp::FIXED_TIME)
			{
				selected = point3;
			}
			else if(index == FloatLerp::DAMPING)
			{
				selected = point4;
			}
			else if(index == FloatLerp::MAGNETIC)
			{
				selected = point5;
			}
		}
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

		

		// ���� ������� ������� ����������
		if(selected->IsMovingEnabled())
		{
			GraphicsLine::_Draw(Vector3D(selected->GetTargetPosition().x, selected->GetTargetPosition().y + 0.05f, targetWorldPos.z), 
				                Vector3D(selected->GetTargetPosition().x, selected->GetTargetPosition().y - 0.05f, targetWorldPos.z), selected->GetColor());
					
			GraphicsLine::_Draw(Vector3D(selected->GetTargetPosition().x - 0.05f, selected->GetTargetPosition().y, targetWorldPos.z), 
							    Vector3D(selected->GetTargetPosition().x + 0.05f, selected->GetTargetPosition().y, targetWorldPos.z), selected->GetColor());
		}

	
		/*

		Vector2D windowPoint = Mouse::_CursorPosition(this);
		float vx = windowPoint.x;
		float vy = Renderer::_Picked()->ClientAreaSize().y - windowPoint.y - 1; 
		float vz;
	
		glReadPixels(vx, vy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &vz);// ��������� ������� ������� vx/vy � ���������� ������������, � ��������� � vz; ���� ������� ������� �����������, �� ������������ 1.0, ��� ������������� ������� ���������� ���������


		*/ 

		camera->BeginScreen2D();

		
		// ����� ������...
		int32 i = 0;



		font->SetFontColor(selected->GetColor());// ���� ������ 
		font->DrawScreenText(Vector3D(10, i+=20, -1), "name: %s", selected->GetName().c_str());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "move speed: %f", selected->GetMoveSpeed());

		font->DrawScreenText(Vector3D(10, i+=20, -1), "current speed: %f", selected->GetCurrentSpeed());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "max speed: %f", selected->GetMaxSpeed());
		
	
		font->DrawScreenText(Vector3D(10, i+=20, -1), "is moving: %d", selected->IsMovingEnabled());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "time to target (sec): %f", selected->GetTimeToTarget());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "target position: %.3f/%.3f/%.3f", selected->GetTargetPosition().x, selected->GetTargetPosition().y, selected->GetTargetPosition().z);
		font->DrawScreenText(Vector3D(10, i+=20, -1), "current position: %.3f/%.3f/%.3f", selected->GetPosition().x, selected->GetPosition().y, selected->GetPosition().z);
		//font->DrawScreenText(Vector3D(10, i+=20, -1), "vz %.3f", vz);
	}


			
	// ������ ������� ����
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		if(_event->key == Mouse::LEFT_BUTTON)
		{
			// ��������� ������� ������� � ������� �����������...
			Line ray = camera->GetSelectRay();
			targetWorldPos = ray.begin + (ray.GetDirection() * (deep));// ������� ����� �� ����� line, ������� ����������� ����� ��� Z �� ������� deep 

			if(quad->GetQuad().IsPointInside(targetWorldPos))// ���� ������ ��������� � ������� quad
			{ 
				// �������� ������� ������� ��� ���� ���������� �����
				point1->SetTargetPosition(Vector3D(targetWorldPos.x, targetWorldPos.y, targetWorldPos.z));
				point2->SetTargetPosition(Vector3D(targetWorldPos.x, targetWorldPos.y, targetWorldPos.z)); 
				point3->SetTargetPosition(Vector3D(targetWorldPos.x, targetWorldPos.y, targetWorldPos.z)); 
				point4->SetTargetPosition(Vector3D(targetWorldPos.x, targetWorldPos.y, targetWorldPos.z));
				point5->SetTargetPosition(Vector3D(targetWorldPos.x, targetWorldPos.y, targetWorldPos.z));
			}
		}
		else if(_event->type == Mouse::WHEEL)
		{
			// �������� �������� �������� ��� ���� ���������� �����
			point1->SetMoveSpeed(point1->GetMoveSpeed() + (float)_event->wheel / 3.0f);
			point2->SetMoveSpeed(point2->GetMoveSpeed() + (float)_event->wheel / 3.0f);
			point3->SetMoveSpeed(point3->GetMoveSpeed() + (float)_event->wheel / 3.0f);
			point4->SetMoveSpeed(point4->GetMoveSpeed() + (float)_event->wheel / 3.0f);
			point5->SetMoveSpeed(point5->GetMoveSpeed() + (float)_event->wheel / 3.0f);
		}

		return Engine::MouseEventFilter(_event);
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
			else if(_event->key == Keyboard::LEFT_ARROW)
			{
				index--;

				if(index < 0)
				{
					SelectFunction(4);
				}
				else { SelectFunction(index); }
			}
			else if(_event->key == Keyboard::RIGHT_ARROW)
			{
				index++;  

				if(index > 4) 
				{
					SelectFunction(0);
				}
				else { SelectFunction(index); }
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "VectorLerp.example_03";
	def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
