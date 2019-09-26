// Win32.cpp: ���������� ����� ����� ��� ����������.
//


#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Output/Graphics/GraphicsScene/GraphicsScene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Output/Graphics/Shape/Mesh/GraphicsMesh/GraphicsMesh.h>
#include <Engine/Helpers/Scene/Scene.h>



// �����, ����������� ���������������� ����
class Application: public Renderer
{
	public: enum
	{
		BOX, // �������������
		QUAD,// ���� (���������)
		CIRCLE,// ���� (���������)
		ICOSPHERE, // ��������
		PYRAMID// ��������
	};

	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: Scene* scene;// ����������� �����
	private: GraphicsMesh<Vector3D, Quaternion, Matrix4x4>* mesh;
	private: int32 level;// ������� ����������� �����
	private: bool wire;// ���� true, �� �������� ��������� ����� ������������
	private: int32 curShape;// ������� ������

    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), mesh(NIL), level(0), wire(false), curShape(BOX)
	{}

			
	// ����������� ����������
	public: virtual ~Application(void)
	{
		delete scene;
	} 

	// ������� ����������
	public: bool Create(Renderer::Define _define)
	{
		// ������� ����������� ����
		if(Renderer::Create(_define))
		{
			// �����...
			{
				scene = new Scene();
			}

			// ������������� �����...
			{
				mesh = new GraphicsMesh<Vector3D, Quaternion, Matrix4x4>();

				RebuildMesh(curShape);// ������� ��� �������� ������� ������������� ������
				mesh->Rename("mesh");
				mesh->SetScale(Size3D(0.5f, 0.5f, 0.5f));
				
				scene->AddObject(mesh, false, true);// �������� � �����			
			}
			


			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;
				def.textColor = RGBA(RGBA::GRAY_COLOR);

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d);
				font2d->Pick();
			}


			// ������...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// ����� �������
				def.viewingPoint = mesh->GetPosition();
				def.zoomMin = 5;
				def.zoomMax = 10;


				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera);
				camera->Pick(Tittle());
				
				// ������ �������������� ������ ������
				camera->UpdateTrackballObjectViewer(-Angle::HALF_PI/3.0f, -Angle::HALF_PI/3.0f, 0.0f);
			}


			SetBackgroundColor(RGBA(RGBA::WHITE_COLOR));// ����� ���� ����
			
			GraphicsLine<Vector3D>::_SetLineWidth(4.0f);// ������� �����

			MoveToCenter();// ����������� ���� � ����� �����
			Mouse::_MoveCursorToCenter(this);// ����������� ������ � ����� ���������� ������� ����	
			Mouse::_ShowCursor(false);// ������ ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}
	
	// ������� ������������� ����� �������� ������� ������������� ������ _shape
	public: void RebuildMesh(int32 _shapeId)
	{
		switch(_shapeId)
		{
			case QUAD:
			{
				//mesh->SetCullingEnable(false); 
				mesh->CreateQuad2D(Quad<Vector3D>(Vector3D(0,0,0), Size2D(1.0f, 1.0f)));
				mesh->SetColor(Color<RGBA>::_Random());
				curShape = _shapeId;
				break;
			}

			case CIRCLE:
			{
				//mesh->SetCullingEnable(false);
				mesh->CreateCircle2D(Circle<Vector3D>(Vector3D(), 1), 10);
				mesh->SetColor(Color<RGBA>::_Random());
				curShape = _shapeId;
				break;
			}

			case BOX:
			{
				mesh->SetCullingEnable(true); 
				mesh->CreateBox3D(Box3D(Vector3D(0, 0, 0), Size3D(1.0f, 1.0f, 1.0f)));
				mesh->SetColor(Color<RGBA>::_Random());
				curShape = _shapeId;
				break;
			}
			case PYRAMID:
			{
				mesh->SetCullingEnable(true); 
				mesh->CreatePyramid3D(1);
				mesh->SetColor(Color<RGBA>::_Random());
				curShape = _shapeId;
				break;
			}
			case ICOSPHERE:
			{
				mesh->SetCullingEnable(true); 
				mesh->CreateIcosphere3D(0);
				mesh->SetColor(Color<RGBA>::_Random());
				curShape = _shapeId;
				break;
			}
			default: break;
		}
	}

	
	// ���������� ��� ������ � ��������������� _shapeId
	public: StringANSI NameOfShape(int32 _shapeId)
	{
		switch(_shapeId)
		{
			case QUAD:
			{
				return "QUAD";
			}
			case CIRCLE:
			{
				return "CIRCLE";
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
		return "NON";
	}


	// ������� ��������� ������
	public: void SetNextShape(int32 _shapeId)
	{
		RebuildMesh(_shapeId + 1); 
	}

	// ������� ���������� ������ 
	public: void SetBackShape(int32 _shapeId)
	{
		RebuildMesh(_shapeId - 1); 
	}

	// ����������� ������� ����������� ������������� �����
	public: void AddMeshDetail(void)
	{
		if(mesh) // ���� ����� ����������
		{ 
			level++;// ��������� ������� �����������
			
			
			mesh->TotalPolygonsDivide4();// ��������� �������� ������ �� 4
		} 
	}

	// ������� �����
	public: virtual void Update(float _delta)
	{}

	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����


		camera->BeginPerspective3D();

		scene->Draw();


		// ����� ������...
		camera->BeginScreen2D();// ������ ����� ���������� �������

		int32 i = 0;
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "polygons: %d", mesh->AmountOfPolygons()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vertices: %d", mesh->AmountOfVertices()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "indices: %d", mesh->AmountOfIndices());  

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "detail level: %d", level); 
	
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "current shape: %s", NameOfShape(curShape).c_str()); 
	



		// ��������������� �����...
		//camera->End2D();// ������ ����� ���������� �������
	}


	// ������ ������� ����
	public: virtual bool MouseEventFilter(Mouse::Event _event)
	{
		if(_event.type == Mouse::KEY_DOWN)
		{
			if(_event.key == Mouse::LEFT_BUTTON)
			{
				if(!wire) 
				{
					mesh->SetWireframeEnable(true);
					wire = true;
				}
			}
			else if(wire)
			{
				mesh->SetWireframeEnable(false);
				wire = false;
			}
		}

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
			else if(_event.key == Keyboard::D)
			{
				AddMeshDetail();
			}
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				SetBackShape(curShape);
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				SetNextShape(curShape);
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
	Renderer::Define def;
	def.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_RESIZING);// ����� ���� 
	def.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
	def.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	def.realTimeWorkerDef.windowDef.tittle = "GraphicsMesh.example_01";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;
	//def.faceDrawMode[Renderer::BACK] = Renderer::LINE;
	//def.faceCullMode[Renderer::FRONT] = false;
	//def.faceCullMode[Renderer::BACK] = false;
	//def.frontFaceClockwise = true;
	//def.realTimeWorkerDef.windowDef.eventBuffering = false;

	
	Application* app = new Application;// ������� ����
	
	if(app->Application::Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
