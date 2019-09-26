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
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>




// ����������� ��������
class Icosphere: public ColoredMesh// ������� ������������� �����
{
	// DATA
	public: Triangle triangle;// ������� �����������

	public: bool drawOnlyCenterOfTriangles;// ���� true, �� ���������� ������ ����� �������������
	public: RGBA normalColor;// ���� ��������
	public: bool drawNormalOfTriangles;// ���� true, �� ���������� ������� �������������
	
    private: Vector3 centerOfPolygon;// ����� ��������
	private: RGBA polygonCenterColor;// ���� ������ �������������

	private: int32 intersectedTriangleIndex;// ������ ������������� ������������
    private: Vector3 intersectionPoint;// ����� ����������� ���� � ������������� �����; ��������� ������ ��� intersected = true
    private: RGBA intersectedColor;// ���� ������������� ������������


	// METHODS
	// ����������� �� ���������
	public: Icosphere(void): drawOnlyCenterOfTriangles(false), drawNormalOfTriangles(false), intersectedTriangleIndex(-1)
	{
		CreateIcosphere3D(3);

		//SetWireframeEnable(true);// ��������� ����� ������������
	
		SetColor(RGBA(RGBA::YELLOW_COLOR));
		polygonCenterColor = RGBA(RGBA::RED_COLOR);
		normalColor = RGBA(RGBA::RED_COLOR);
		intersectedColor = RGBA(RGBA::YELLOW_COLOR);
	}


	// ���������� ����� ����������� ���� � ������������� �����
	public: Vector3 GetIntersectionPoint(void)
	{
		return intersectionPoint;
	}


	// ��������� ������������ ��������� ����� � �������� _line � ���������� true � ������ ������
	public: bool CheckIntersect(Line _line)
	{
		return ((intersectedTriangleIndex = IsIntersected(_line, intersectionPoint)) >= 0);// ��������� ����������� ����� ����� ���������� ����� � �������� _line
	}

	
	// ����������� �������� �� �������������
	public: void virtual Draw(void)
	{
		int32 polygons = GetAmountOfPolygons();// ���������� ��������� � �����

		// ��������������� ��� ��������
		for(int i = 0; i < polygons; ++i)
		{
			if(GetTriangle(i, triangle))// ����� ����������� ��� �������� i
			{
				centerOfPolygon = triangle.GetCenter();// ����� ����� ��������

				if(drawOnlyCenterOfTriangles)// ���� ����� ���������� ������ ����� �������������
				{
					// ���������� ����� ������������
					GraphicsPoint::_Draw(centerOfPolygon, polygonCenterColor);
				}			 	
				else if(drawNormalOfTriangles)// ���� ����� ���������� ������� �������������
				{
					Renderer::_GetPicked()->SetWireframeEnable(false);

					_DrawCulledTriangle(triangle, GetColor());// ���������� ����������� i

					// ���������� �������
					GraphicsLine::_Draw(centerOfPolygon, 
										centerOfPolygon + triangle.GetNormal() * 0.1,
										normalColor);
					
					Renderer::_GetPicked()->SetWireframeEnable(true);
				}
				else// ����� ������ ���������� ������������
				{
					if(intersectedTriangleIndex >= 0)// ���� ���� ����������� ���� � ������������
					{
						if(i == intersectedTriangleIndex)// ���� i - ��� ������������ �����������
						{
							Renderer::_GetPicked()->SetWireframeEnable(false);

							_DrawCulledTriangle(triangle, intersectedColor);// ���������� ����������� i

							Renderer::_GetPicked()->SetWireframeEnable(true);
						}
						else// ����� i �� ������������ � �����
						{
							_DrawCulledTriangle(triangle, GetColor());// ���������� ����������� i
						}
					}
					else// ����� ��� �����������
					{
						_DrawCulledTriangle(triangle, GetColor());// ���������� ����������� i
					}
				}
			}
		}
	} 
};



// ����� ����������� ����� � ���������� ������������
class Point3D: public Point// ���������� �������
{
	// DATA
	public: StringANSI name;// ��� �����

	// METHODS
	// ����������� �� ���������
	public: Point3D(void)
	{}

	// ���������������� ����������� 
	public: Point3D(Vector3 _point, StringANSI _name): name(_name)
	{
		SetPosition(_point);
	}
};


// ���������������� ����������
class Application: public Engine// ������
{
	// DATA
	private: Camera3D* camera, *camera2d;// ������
    private: Font2D *font;// �����
	private: Scene *scene;// �����
	private: Icosphere* sphere;// �����

	private: float sightSize;// ������ ������� (����� ������ ����� � ��������)
	private: Line selectLine;// ������������� ���
	private: SimpleList<Point3D> points;// ������ � ���������
	private: bool rightPressed;// ���� true, �� ��������� �������� ������


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font(NIL), scene(NIL), camera2d(NIL), sphere(NIL), sightSize(20.0f), rightPressed(false)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete font;
		delete scene;
		delete camera2d;
		delete camera;
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
		

			// ������...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::SCROLL_OBJECT_VIEWER;
				//def.rotationEnable = true;
				def.zoomEnable = true;
				def.nearZoom = 3.0f;
				def.farZoom = 7.0f;
				def.SetMiddleDistanceToViewingPoint();

				camera = new Camera3D();
				camera->Set(def);

				camera->Pick(); // ������� ������
				scene->BindCamera(camera);
			}


			// �����...
			{
				sphere = new Icosphere();
		
				scene->AddObject(sphere);
				scene->AddGraphicsObject(sphere);
			}


			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.fontName = "Arial";// ��� ������
				def.fontSize = 16;// ������ ������
				def.fontWeight = Font2D::WEIGHT_BOLD;
				def.color = RGBA(RGBA::WHITE_COLOR);// ���� ������

				font = new Font2D();
				font->Create(def);
				font->Pick();// ������� �����
			}

			

			// ���������� ������...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;

				camera2d = new Camera3D();
				camera2d->Set(def);

				//scene2d->BindCamera(camera2d);
			}

			SetWireframeEnable(true);// ������ ��������� ����� �����������

			GraphicsPoint::_SetPointSize(6);// ������ ����� 
			Mouse::_MoveCursorToCenter(this);// ����������� ������ � ����� ���������� ������� ����
			Mouse::_SetShowCursor(false);// ������ ������
			
			// ���� ����...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));

			MoveToCenter();// ����������� ���� � ����� ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}


			
	// ���������� �� ����� ����������� �����
	public: void DrawPoints(void)
	{
		camera->BeginTransform(Renderer::PROJECTION);
		camera->BeginTransform(Renderer::MODELVIEW);

		// ������ ��� �����...
		for(int32 i = 0; i < points.GetSize(); ++i)
		{
			Vector3 point = points[i]->key.GetPosition();

			// ���������� ������� �����
			GraphicsPoint::_Draw(point, RGBA(RGBA::GREEN_COLOR));

			Vector2 screenPoint;
			
			camera->Project(point, screenPoint);// ��������� �������� ���������� ����� point
			{
				// ����� ������...
				// ������ ����� ������������� �����...
				camera2d->BeginTransform(Renderer::PROJECTION);
				camera2d->BeginTransform(Renderer::MODELVIEW);

				//camera->BeginScreen2D();// ������ ����� ���������� �������
				
				font->DrawScreenText(Vector3(screenPoint.x,  ClientAreaSize().y - screenPoint.y, -1), "%s", points[i]->key.name.c_str());
				
				//camera->BeginPerspective3D();
				// ������������ ���������� �������������...
				camera2d->EndTransform(Renderer::MODELVIEW);
				camera2d->EndTransform(Renderer::PROJECTION);
			}
		}
		camera->EndTransform(Renderer::MODELVIEW);
		camera->EndTransform(Renderer::PROJECTION);
	}


	// ���������� ������ �� ����� �������
	public: void DrawSight(void)
	{
		// ������ ����� ���������� �������
		//camera->BeginScreen2D();
		camera2d->BeginTransform(Renderer::PROJECTION);
		camera2d->BeginTransform(Renderer::MODELVIEW);
  
		Renderer::_SetCurrentStack(Renderer::MODELVIEW);
		Renderer::_PushMatrix();
		Renderer::_LoadIdentityMatrix(); 
		Renderer::_LoadMatrix(Matrix4x4::_Translation(Vector3(0.0f, 0.0f, -1.0f)));// ��� '-1' - ���������� �� ��� Z �� ����� ���������� ������ ������

		GraphicsLine::_Draw(Vector3(Mouse::_GetCursorPosition(this).x - sightSize/2.0f, Mouse::_GetCursorPosition(this).y, 0), 
			                Vector3(Mouse::_GetCursorPosition(this).x + sightSize/2.0f,  Mouse::_GetCursorPosition(this).y, 0), RGBA(RGBA::GREEN_COLOR));// ������ �������������� ����� ����� �������
		GraphicsLine::_Draw(Vector3(Mouse::_GetCursorPosition(this).x, Mouse::_GetCursorPosition(this).y - sightSize/2.0f, 0), 
			                Vector3(Mouse::_GetCursorPosition(this).x, Mouse::_GetCursorPosition(this).y + sightSize/2.0f, 0), RGBA(RGBA::GREEN_COLOR));// ������ ������������ ����� ����� �����
		Renderer::_PopMatrix();

		// ������ ����� ���������� �������
		//camera->BeginPerspective3D();
		camera2d->EndTransform(Renderer::MODELVIEW);
		camera2d->EndTransform(Renderer::PROJECTION);
	}


	// ���������� ������������� ���
    public: void DrawSelectRay(void)
	{	
		camera->BeginTransform(Renderer::PROJECTION);
		camera->BeginTransform(Renderer::MODELVIEW);

		// ���������� ������������� ���
		GraphicsLine::_SetLineWidth(3);
		GraphicsLine::_Draw(selectLine.begin, selectLine.end, RGBA(RGBA::GREEN_COLOR));
		GraphicsLine::_SetLineWidth(1); 

		camera->EndTransform(Renderer::MODELVIEW);
		camera->EndTransform(Renderer::PROJECTION);
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
		
		DrawPoints();// ���������� �����

		DrawSelectRay();// ���������� ������������� ���

				
		if(!rightPressed) { DrawSight(); }// ������� ������ ������


		// ������ ����� ������������� �����...
		camera2d->BeginTransform(Renderer::PROJECTION);
		camera2d->BeginTransform(Renderer::MODELVIEW);


		// ����� ������...
		int32 i = 0;

		//font->DrawScreenText(Vector3(10, i+=20, -0.1), "--: %d", 9);

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "DistanceToViewingPoint: %.2f", camera->DistanceToViewingPoint());
	    
		
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sphere.AmountOfPolygons: %d", sphere->AmountOfPolygons());
	   
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.position: %.2f/%.2f/%.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
	    
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "select ray.begin(%.2f/%.2f/%.2f)", selectLine.begin.x, selectLine.begin.y, selectLine.begin.z);
	   // font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "select ray.end(%.2f/%.2f/%.2f)", selectLine.end.x, selectLine.end.y, selectLine.end.z);


		// ������������ ���������� �������������...
		camera2d->EndTransform(Renderer::MODELVIEW);
		camera2d->EndTransform(Renderer::PROJECTION);
	}




	// ������ ������� ����
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		if(_event->type == Mouse::KEY_DOWN)// ���� ������ �������
		{
			if(_event->key == Mouse::LEFT_BUTTON)// ���� ����� ���� ����
			{
				//Renderer::_SetCurrentStack(Renderer::PROJECTION);

				selectLine = camera->GetSelectRay(Mouse::_GetCursorPosition(this));// ��������� ������������� ���

				if(sphere->CheckIntersect(selectLine))// ��������� ������������ ���� � ������������� ������
				{
					// ���������� ����� ���� �� ��������� ����� � ������� �������� ����� ����� �������, ����� ��������� ������� ����� ������ ����������� � ������� ����
					selectLine.SetLengthFromBeginToEnd(Line::_GetLength(selectLine.begin, sphere->GetIntersectionPoint()));
 
					points.AddToTail(Point3D(sphere->GetIntersectionPoint(), "point" + NumberToString(points.GetSize() + 1)));
				}
			}
		}
		else if(_event->type == Mouse::KEY_PRESSED)// ���� ������ ������
		{
			if(_event->key == Mouse::RIGHT_BUTTON)// ������ ����
			{
				// ������ �������� ������...
				if(!rightPressed)// ��������� ������ ���� ���
				{
					Mouse::_MoveCursorToCenter(Renderer::_GetPicked());// ���������� ���� � ����� ���������� ������� ���� 

					camera->SetRotationEnable(true);// ��������� �������� ������
					rightPressed = true;
				}
			}
		}
		else if(_event->type == Mouse::KEY_UP)// ���� ������ ��������
		{
			if(_event->key == Mouse::RIGHT_BUTTON)// ���� ������ ����
			{
				// ����� �������� ������...
				if(rightPressed)// ��������� ������ ���� ���
				{
					camera->SetRotationEnable(false); // ��������� �������� ������
					rightPressed = false;
				}
			}
		}

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
			else if(_event->key == Keyboard::N)
			{
				sphere->drawNormalOfTriangles = true;// ���������� �������
			}
			else if(_event->key == Keyboard::C)
			{
				sphere->drawOnlyCenterOfTriangles = true;// ���������� ����� �������������
			}
		} 
		if(_event->type == Keyboard::KEY_UP)
		{
			if(_event->key == Keyboard::N)
			{
				sphere->drawNormalOfTriangles = false;
			}
			else if(_event->key == Keyboard::C)
			{
				sphere->drawOnlyCenterOfTriangles = false;
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Camera3D.example_04";
	//def.rendererDef.zBufferEnabled = true;

	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
