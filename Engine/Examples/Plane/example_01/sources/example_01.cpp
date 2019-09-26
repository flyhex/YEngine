// Win32.cpp: ���������� ����� ����� ��� ����������.
//


#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <glaux.h>// ��� �������

#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Shape/Box/GraphicsBox3DFixed/GraphicsBox3DFixed.h> 
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Output/Graphics/GraphicsScene/GraphicsScene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Input/Controls/Controls.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Math/Plane/Plane.h>


// ����������� ���������
class GraphicsPlane: public GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D, Matrix4x4>,// ����������� ���
	                 public Plane// ���������
{
	// DATA
	private: GraphicsLine<Vector3D, ColorRGBA> graphicsNormal;// ����������� �������
    private: Vector3D intersectionPoint;// ����� ����������� ���� � ������������� �����; ��������� ������ ��� intersected = true
	private: bool intersection;// ���� true, ������ ���� ����� ����������� � ����������

	// METHODS
	// ����������� �� ���������
	public: GraphicsPlane(void): intersection(false)
	{
		// ���������...
		normal = Vector3D::UNIT_X;// ����������� �������

		// ������� ���������...
		graphicsNormal.SetColor(ColorRGBA::YELLOW_COLOR); 
				

		// ����... 
		CreateQuad2D(Quad<Vector3D>(Vector3D(0,0,0), Size2D(1.0f, 1.0f)));// ������� ����

		SetPosition(Vector3D(0, 0, -2));// ������ ������� ���������
		SetRotation(Quaternion::_ShortestArc(p0, normal * p0.Length()));// ������ ���������� ��������� � ����������� normal

		ResizeColors();
		SetColor(ColorRGBA::GREEN_COLOR);// ���� ���������
	}

	// ������� ����� ��������� ��� ���������
	public: virtual void SetPosition(Vector3D _position)
	{
		p0 = _position;
		ModelView<Vector3D, Quaternion, Size3D, Matrix4x4>::SetPosition(p0);
		graphicsNormal.Set(p0, p0 + normal * 0.19);
	}

	// ������� ����� ���������� ��� ���������
	public: virtual void SetRotation(Quaternion _rotation)
	{
		ModelView<Vector3D, Quaternion, Size3D, Matrix4x4>::SetRotation(_rotation);

		normal = -_rotation.AxisZ();// �������� ������� (����������) ���������
		graphicsNormal.Set(p0, p0 + normal * 0.19);
	}



	// ������������ ���������
	public: virtual void Draw(void)
	{
		GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D, Matrix4x4>::Draw();
		graphicsNormal.Draw();// ��������������� �������
		
		// ����� ����� �����������...
		if(intersection)
		{
			GraphicsPoint<Vector3D, ColorRGBA>::_Draw(intersectionPoint, ColorRGBA::RED_COLOR);
		}
	}

	// ���������� ����� ����������� ���� � ������������� �����
	public: Vector3D GetIntersectionPoint(void)
	{
		return intersectionPoint;
	}

	// ��������� ������������ ��������� ����� � �������� _line
	public: bool CheckIntersect(Line<Vector3D> _line)
	{
		return (intersection = Plane::IsIntersected(_line, intersectionPoint));// ��������� ����������� ������� _line � ���������� this
	}
};



// �����, ����������� ���������������� ����
class Application: public Renderer
{
	public: enum
	{
		BOX, // �������������
		QUAD,// ���� (���������)
		ICOSPHERE, // ��������
		PYRAMID,// ��������
		CUSTOM// ���������������� ������
	};

	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: GraphicsScene<Vector3D, Quaternion, Size3D, Matrix4x4>* scene;// ����������� �����
	private: GraphicsPlane* plane;//����������� ���������
	private: bool rightReleased, rightPressed;// �������, �������������� ������� "�������" � "�������" ������ ������ ����
	private: Line<Vector3D> selectLine;// ������������� ���
	private: GraphicsPoint<Vector3D, ColorRGBA> *p1, *p2, *p3;// �������� �������
	private: float sightSize;// ������ ������� (����� ������ ����� � ��������)


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), plane(NIL), p1(NIL), p2(NIL), p3(NIL), sightSize(20.0f), rightReleased(false), rightPressed(false)
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
				GraphicsScene<Vector3D, Quaternion, Size3D, Matrix4x4>::Define def;
				def.logicSceneDef.scale = Size3D(1.0f, 1.0f, 1.0f);
				scene = new GraphicsScene<Vector3D, Quaternion, Size3D, Matrix4x4>();
				scene->Create(def);
			}

	 

			// ����������� ���������...
			{
				plane = new GraphicsPlane;

				plane->Rename("plane");
				plane->SetScale(Size3D(0.5f, 0.5f, 0.5f));
				
				scene->AddObject(plane, false, true);// �������� � �����			
			}
			

			// �����...
			{
				p1 = new GraphicsPoint<Vector3D, ColorRGBA>();
				p1->Rename("p1");
				p1->SetPosition(plane->p0);
				p1->SetColor(ColorRGBA::MAGENTA_COLOR);
				scene->AddObject(p1, false, true);// �������� � �����		
				 
			
				p2 = new GraphicsPoint<Vector3D, ColorRGBA>();
				p2->Rename("p2");
				p2->SetPosition(plane->p0 + plane->normal * 0.5);
				p2->SetColor(ColorRGBA::GREEN_COLOR);
				scene->AddObject(p2, false, true);// �������� � �����		

								
				p3 = new GraphicsPoint<Vector3D, ColorRGBA>();
				p3->Rename("p3");
				p3->SetPosition(plane->p0 + -plane->normal * 0.5);
				p3->SetColor(ColorRGBA::BLUE_COLOR);
				scene->AddObject(p3, false, true);// �������� � �����		
			}

			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.textColor = ColorRGBA::WHITE_COLOR;
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;
				def.textColor = ColorRGBA::GRAY_COLOR;

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d, false, false);
				font2d->Pick();
			}


			// ������...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.controllerType = Camera3D::TRACKBALL_OBJECT_VIEWER;// ����� �������
				def.viewingPoint = plane->GetPosition();
				def.distanceToViewingPoint = 4.0f;


				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, true, false);
				camera->ConnectTo(ResizeEvent::TYPE, this);
				camera->ConnectTo(Mouse::Event::TYPE, this);
				
				// ������ �������������� ������ ������
				//camera->UpdateTrackBallObjectViewer(-Angle::HALF_PI/3.0f, -Angle::HALF_PI/3.0f, 0.0f);
			}


			SetFaceCullMode(Renderer::BACK, false);
			SetBackgroundColor(ColorRGBA::WHITE_COLOR);// ����� ���� ����
			
			GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(3.0f);// ������� �����
			GraphicsPoint<Vector3D, ColorRGBA>::_SetPointSize(8.0f);// ������ �����


			MoveToCenter();// ����������� ���� � ����� �����
			Mouse::_MoveCursorToClientAreaCenter(this);// ����������� ������ � ����� ���������� ������� ����
			
			Mouse::_ShowCursor(false);// ������ ������

			Flush();//��������� ��� ����� ���������� �������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}
	


	// ���������� ������ �� ����� �������
	public: void DrawSight(void)
	{
		// ������ ����� ���������� �������
		camera->BeginScreen2D();
  
		Renderer::_SetCurrentStack(Renderer::MODELVIEW);
		Renderer::_PushMatrix();
		Renderer::_LoadIdentityMatrix(); 
		Renderer::_Translate(Vector3D(0.0f, 0.0f, -1.0f));// ��� '-1' - ���������� �� ��� Z �� ����� ���������� ������ ������
 

		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(Mouse::_CursorClientAreaPosition(this).x - sightSize/2.0f, Mouse::_CursorClientAreaPosition(this).y), 
			                                     Vector2D(Mouse::_CursorClientAreaPosition(this).x + sightSize/2.0f,  Mouse::_CursorClientAreaPosition(this).y), ColorRGBA::RED_COLOR);// ������ �������������� ����� ����� �������
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(Mouse::_CursorClientAreaPosition(this).x, Mouse::_CursorClientAreaPosition(this).y - sightSize/2.0f), 
			                                     Vector2D(Mouse::_CursorClientAreaPosition(this).x, Mouse::_CursorClientAreaPosition(this).y + sightSize/2.0f), ColorRGBA::RED_COLOR);// ������ ������������ ����� ����� �����


		Renderer::_PopMatrix();

		// ������ ����� ���������� �������
		camera->End2D();
	}



	// ���������� ������������� ���
    public: void DrawSelectRay(void)
	{	
		// ���������� ������������� ���
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(selectLine.begin, selectLine.end, ColorRGBA::RED_COLOR);
	}


	// ������� �����
	public: virtual void Update(float _delta)
	{
		// ��������� �������� ������ ������ ��� ������� ������ �����
		if(Mouse::_IsKeyPressed(Mouse::RIGHT_BUTTON))
		{
			if(!rightPressed)// ��������� ������ ���� ���
			{
				camera->SetControllerEnable(true);
				rightPressed = true;
				rightReleased = false;
			}
		}
		else 
		{ 
			if(!rightReleased)// ��������� ������ ���� ���
			{
				camera->SetControllerEnable(false); 
				rightReleased = true;
				rightPressed = false;
			}
		}
	}


	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����

		if(rightReleased) { DrawSight(); }// ������� ������ ������


		scene->Draw();


		
		DrawSelectRay();// ���������� ������������� ���


		// ����� ������...
		camera->BeginScreen2D();// ������ ����� ���������� �������

		int32 i = 0;

	
		font2d->SetTextColor(ColorRGBA::MAGENTA_COLOR);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "distance to p1: %.2f", plane->DistanceTo(p1->GetPosition())); 
		font2d->SetTextColor(ColorRGBA::GREEN_COLOR);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "distance to p2: %.2f", plane->DistanceTo(p2->GetPosition())); 
		font2d->SetTextColor(ColorRGBA::BLUE_COLOR);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "distance to p3: %.2f", plane->DistanceTo(p3->GetPosition())); 
		


		font2d->SetTextColor(ColorRGBA::BLACK_COLOR);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "plane.normal(x/y/z): %.2f/%.2f/%.2f", plane->normal.x, plane->normal.y, plane->normal.z); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "plane.p0(x/y/z): %.2f/%.2f/%.2f", plane->p0.x, plane->p0.y, plane->p0.z); 


		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sizeinByte: %d", mesh->SizeInByte()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sizeInKbyte: %.5f", MemoryCounter::_FromByteToKB(mesh->SizeInByte())); 

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "detail level: %d", level); 
	
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "current shape: %s", NameOfShape(curShape).c_str()); 
	



		// ��������������� �����...
		camera->End2D();// ������ ����� ���������� �������
	}


	// ������ ������� ����
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		if(_event.type == Mouse::KEY_DOWN)
		{
			if(_event.key == Mouse::LEFT_BUTTON)
			{
				selectLine = camera->GetSelectRay(Mouse::_CursorClientAreaPosition(this));// ��������� ������������� ���

				plane->CheckIntersect(selectLine);// ��������� ������������ ���� � ����������

				// ���������� ����� ���� �� ��������� ����� � ������� �������� ����� ����� �������, ����� ��������� ������� ����� ������ ����������� � ������� ����
				selectLine.SetLengthFromBeginPoint(Line<Vector3D>::_GetLength(selectLine.begin, plane->GetIntersectionPoint()));
			}
			else if(_event.key == Mouse::RIGHT_BUTTON)
			{
				Mouse::_MoveCursorToClientAreaCenter(Renderer::_Picked());// ���������� ���� � ����� ���������� ������� ���� 
			}
		}
		Mouse().ClearEventList();// �������� ����� �������
	}


	// ��������� ������� ����������
	public: virtual void KeyboardEventFilter(Keyboard::Event _event)
	{
		if(_event.type == Keyboard::KEY_DOWN)
		{
			if(_event.key == Keyboard::NUM_1)
			{
				SetWindowStyle(Window::FULLSCREEN);
			}
			else if(_event.key == Keyboard::NUM_2)
			{
				SetWindowStyle(Window::WND_CLASSIC);
			}
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// ���������� ����
			} 
			else if(_event.key == Keyboard::X)
			{
				plane->SetRotation(plane->GetRotation() * Quaternion(Vector3D::UNIT_X, 0.1f));
			}
			else if(_event.key == Keyboard::Y)
			{
				plane->SetRotation(plane->GetRotation() * Quaternion(Vector3D::UNIT_Y, 0.1f));
			}
			else if(_event.key == Keyboard::Z)
			{
				plane->SetRotation(plane->GetRotation() * Quaternion(Vector3D::UNIT_Z, 0.1f));
			}
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				plane->SetPosition(Vector3D(plane->GetPosition().x - 0.1, plane->GetPosition().y, plane->GetPosition().z));
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				plane->SetPosition(Vector3D(plane->GetPosition().x + 0.1, plane->GetPosition().y, plane->GetPosition().z));
			}
			else if(_event.key == Keyboard::UP_ARROW)
			{
				plane->SetPosition(Vector3D(plane->GetPosition().x, plane->GetPosition().y + 0.1, plane->GetPosition().z));
			}
			else if(_event.key == Keyboard::DOWN_ARROW)
			{
				plane->SetPosition(Vector3D(plane->GetPosition().x, plane->GetPosition().y - 0.1, plane->GetPosition().z));
			}
		}

		Keyboard().ClearEventList();// �������� ����� �������
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
	def.realTimeWorkerDef.windowDef.tittle = "Plane.example_01";
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
