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
#include <Engine/Helpers/Sprite/AnimatedSprite/AnimatedSprite.h>
#include <Engine/Helpers/Sprite/TexturedSprite/TexturedSprite.h>
#include <Engine/Helpers/Mesh/ColoredMesh/ColoredMesh.h>


// �����, ����������� ���������� ������� ������
class ColoredMesh3D: public ColoredMesh<Vector3D, Quaternion, Matrix4x4>,// ������� ������������� �����
	                 public LogicObject // ���������� ������
{
	// �������������� ������
	public: enum
	{
		BOX, // �������������
		QUAD,// ���� (���������)
		ICOSPHERE, // ��������
		PYRAMID// ��������
	};

	// DATA
	private: int32 detailLevel;// ������� ����������� ������
	private: int32 shape;// ������������� ������
	private: ShapeType type;// ��� ������


	// METHODS
	// ����������� �� ���������
	// ����������: �� ��������� ��������� ��������
	public: ColoredMesh3D(void): detailLevel(0), shape(ICOSPHERE), type(IColoredMaterial::ANGULAR_TYPE)
	{}
 

	// ����������...
	// ���������� ��������� �������...
	public: virtual void Update(float _delta)
	{
		SetRotation(GetRotation() * Quaternion(Vector3D::UNIT_Y, 0.01f));
	}


	// ��������...
	// ������� ������� �������� ��� ������� ������������� ����� ���� _type, � ���������� true � ������ ������
	// ����������: ���� ������� �������� ��� ����������, �� �������� false
	// ����������: ������������� ����� ������ ������������
	public: virtual bool CreateColoredMaterial(ShapeType _type)
	{
		type = _type;// ��������� ��� ������
		bool result = ColoredMaterial::CreateColoredMaterial(_type);
		SetColors();// ������ ��������/��������� ����� �����
		return result;
	}


	// ������ ����� ��� ������/���������
	private: void SetColors(void)
	{ 	
		if(type == IColoredMaterial::ANGULAR_TYPE)// ������������� ������
		{
			// ������ ��������� �����
			for(int32 i = 0; i < AmountOfPolygons(); ++i)
			{
				if(i % 2)// ��� ������ ��������
				{ 
					IColoredMaterial::SetColor(i, RGBA(RGBA::RED_COLOR));
				}
				else// ��� �������� 
				{
					IColoredMaterial::SetColor(i, RGBA(RGBA::BLUE_COLOR));
				}
			}
		}
		else if(type == IColoredMaterial::CURVED_TYPE)// �������� ������
		{
			for(int32 i = 0; i < GetAmountOfColors(); ++i)
			{
				if(i % 2)// ��� ������ ��������
				{ 
					IColoredMaterial::SetColor(i, RGBA(RGBA::RED_COLOR));
				}
				else// ��� �������� 
				{
					IColoredMaterial::SetColor(i, RGBA(RGBA::BLUE_COLOR));
				}
			}
		}
	}


	// ���������� ��� ������
	public: StringANSI GetShapeTypeString(void)const
	{
		if(IsAngularShape())
		{
			return "ANGULAR_MATERIAL";
		}
		else if(IsCurvedShape())
		{
			return "CURVED_MATERIAL";
		}
	
		return "";
	}

	
 
	// ������...
	// ������� ������ _shape ���� _type
	public: void CreateShape(int32 _shape, ShapeType _type)
	{
		switch(_shape)
		{
			case QUAD:
			{
				SetCullingEnable(false);
				CreateQuad2D(Quad<Vector3D>(Vector3D(0,0,0), Vector2D(1.0f, 1.0f)));
				CreateColoredMaterial(_type);
				shape = _shape;
				type = _type;
				detailLevel = 0;
				break;
			}
			case BOX:
			{
				SetCullingEnable(true);
				CreateBox3D(Box3D(Vector3D(0, 0, 0), Vector3D(1.0f, 1.0f, 1.0f)));	
				CreateColoredMaterial(_type);
				shape = _shape;
				type = _type;
				detailLevel = 0;
				break;
			}
			case PYRAMID:
			{
				SetCullingEnable(true);
				CreatePyramid3D(1);
				CreateColoredMaterial(_type);
				shape = _shape;
				type = _type;
				detailLevel = 0;
				break;
			}
			case ICOSPHERE:
			{
				SetCullingEnable(true);
				CreateIcosphere3D(1);
				CreateColoredMaterial(_type);
				shape = _shape;
				type = _type;
				detailLevel = 0;
				break;
			}
		}
	}
 

	// ���������� ������������� ������
	public: int32 GetShape(void)const
	{
		return shape;
	} 
	

	// ���������� ������������� ������ � ���� ������
	public: StringANSI GetShapeString(void)
	{
		switch(shape)
		{
			case QUAD:
			{
				return "QUAD";
			}
			case BOX:
			{
				return "BOX";
			}
			case PYRAMID:
			{
				return "PYRAMID";
			}
			case ICOSPHERE:
			{
				return "ICOSPHERE";
			}
			default: break;
		}
		return "";
	}

	// ������� ��������� ������
	public: void SetNextShape(void)
	{
		CreateShape(shape + 1, type); 
	}


	// ������� ���������� ������ 
	public: void SetBackShape(void)
	{
		CreateShape(shape - 1, type);
	}


	// ���������� ������� ������� ����������� ������
	// ����������: 0 - ����������� ������� �������������
	public: int32 GetDetailLevel(void)
	{
		return detailLevel;
	}


	// ����������� ������� ����������� ������
	public: void AddDetailLevel(void)
	{
		detailLevel++;// ��������� ������� �����������
			
		TotalPolygonsDivide4();// ��������� �������� ������ �� 4

		// �������������� ��������
		switch(shape)
		{
			case QUAD:
			{
				CreateColoredMaterial(type);
				break;
			}
			case BOX:
			{
				CreateColoredMaterial(type);
				break;
			}
			case PYRAMID:
			{
				CreateColoredMaterial(type);
				break;
			}
			case ICOSPHERE:
			{
				TotalVertexNormalize();// ������������� ��� �������
				CreateColoredMaterial(type);
				break;
			}
			default: break;
		}
	}
};
 

// ���������������� ����������
class Application: public Engine,// ������
         	       public MessageHandler<int32>::Receiver// ���������� ���������
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D *font2d;// �����
	private: Scene* scene;// �����
    private: ColoredMesh3D* mesh;// ������� �����


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), mesh(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{} 



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
			}


			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.fontName = "Arial";// ��� ������
				def.fontSize = 20;// ������ ������
				def.fontColor = RGBA(RGBA::WHITE_COLOR);// ���� ������

				font2d = new Font2D();
				font2d->Rename("font2d");
				font2d->Create(def);
				scene->AddObject(font2d);// �������� � �����
				font2d->Pick();// ������� �����
			}



			// ������...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// ����� �������
				def.zoomMin = 4;
				def.zoomMax = 10;

				camera = new Camera3D();
				camera->Create(def);
				
				camera->Pick(Title()); // ������� ������ 
				
				// ������ �������������� ������ ������
				camera->UpdateTrackballObjectViewer(-Angle::HALF_PI/3.0f, 0.0f, 0.0f);
				camera->SetRotationEnable(false);// ��������� �������� ������
				
				AssetLibrary::_GetAssets().AddObject(camera);// �������� � �����
			}


			// ������� �����...
			{
				mesh = new ColoredMesh3D();
				mesh->Rename("mesh");
				mesh->CreateShape(ColoredMesh3D::BOX, IColoredMaterial::CURVED_TYPE);// ������� ������
				mesh->SetColoredMaterialEnable(true);// �������� ������� ��������
				scene->AddGraphicsObject(mesh);// �������� � ����������� �����	
				scene->AddLogicObject(mesh);// �������� � ���������� �����
			}

			// ���� ����...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));
			
			MoveToCenter();// ����������� ���� � ����� �����
			Mouse::_MoveCursorToCenter(this);// ����������� ������ � ����� ���������� ������� ����	
			//Mouse::_ShowCursor(false);// ������ ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}

	

	// ������� �����
	public: virtual void Update(float _delta)
	{
		scene->Update(_delta);
	}



	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����


		
		camera->BeginScreen2D();// ����� ���������� �������


		// ����� ������...
		int32 i = 0;


	

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape detail: %d", mesh->GetDetailLevel()); 
	    font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape: %s", mesh->GetShapeString().c_str()); 


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape type: %s", mesh->GetShapeTypeString().c_str());
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "colored material enabled: %d", mesh->IsColoredMaterialEnabled());
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "camera position: %.2f/%.2f/%.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z); 


		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape attributes: %s", mesh->StringAttributes().c_str()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape vertices: %d", mesh->AmountOfVertices()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape polygons: %d", mesh->AmountOfPolygons()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape vertex colors: %d", mesh->GetAmountOfColors()); 



		camera->BeginPerspective3D();// ����� ���������� �������

		scene->Draw();// ���������� ������� �����
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
			else if(_event->key == Keyboard::D)
			{
				mesh->AddDetailLevel();
			}
			else if(_event->key == Keyboard::LEFT_ARROW)
			{
				mesh->SetBackShape();
			}
			else if(_event->key == Keyboard::RIGHT_ARROW)
			{
				mesh->SetNextShape();
			}
			else if(_event->key == Keyboard::UP_ARROW)
			{
				mesh->CreateColoredMaterial(IColoredMaterial::ANGULAR_TYPE);
			}
			else if(_event->key == Keyboard::DOWN_ARROW)
			{
				mesh->CreateColoredMaterial(IColoredMaterial::CURVED_TYPE);
			}
			else if(_event->key == Keyboard::E)
			{
				mesh->SetColoredMaterialEnable(!mesh->IsColoredMaterialEnabled());
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
	def.rendererDef.realTimeWorkerDef.windowDef.tittle = "ColoredMesh.example_01";

	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
