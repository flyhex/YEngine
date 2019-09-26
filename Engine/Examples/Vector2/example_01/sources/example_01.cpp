

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






// �����, ����������� ���������������� ����
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: GraphicsScene<Vector3D, Quaternion, Size3D>* scene;// ����������� �����


    private: Vector2D xAxis;// ��� X
	private: Vector2D yAxis;// ��� Y
	private: Vector2D vector;// ����������� ������
	private: float scale;// ����������� ��������������� �������

    private: int32 circlePoints;// ���������� ��������� (������� ������������� ����������)
	private: float radius;// ������ �����
	private: float zoom;// ��� ������

    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), circlePoints(100), radius(1.0f), vector(0.0f, -1.0f), xAxis(1.0f, 0.0f), yAxis(0.0f, 1.0f), scale(1.1f), zoom(10.0f)
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
				GraphicsScene<Vector3D, Quaternion, Size3D>::Define def;
				def.logicSceneDef.scale = Size3D(1.0f, 1.0f, 1.0f);
				scene = new GraphicsScene<Vector3D, Quaternion, Size3D>();
				scene->Create(def);
			}

			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.textColor = ColorRGBA::WHITE_COLOR;
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d, false, false);
				font2d->Pick();
			}


			// ������...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.controllerEnabled = false;
				def.position = Vector3D(0, 0, 10);

				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, true, false);
				camera->ConnectTo(ResizeEvent::TYPE, this);
			}

						
			GraphicsPoint<Vector2D, ColorRGBA>::_SetPointSize(6.0);// ������ �����
			GraphicsLine<Vector2D, ColorRGBA>::_SetLineWidth(2.0);// ������� �����

			Flush();//��������� ��� ����� ���������� �������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}
	




	// ������� �����
	public: virtual void Update(float _delta)
	{}

	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����


		
	
		// ����� ������������ ����...
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(0.0, 0.0), vector, ColorRGBA(ColorRGBA::YELLOW_COLOR));// ����������� ������
		GraphicsPoint<Vector2D, ColorRGBA>::_Draw(vector, ColorRGBA(ColorRGBA::YELLOW_COLOR));

		// ������������ ��� X,Y
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(0.0, 0.0), xAxis, ColorRGBA(ColorRGBA::RED_COLOR));
		GraphicsPoint<Vector2D, ColorRGBA>::_Draw(xAxis, ColorRGBA::RED_COLOR);
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(0.0, 0.0), yAxis, ColorRGBA(ColorRGBA::CYAN_COLOR));
		GraphicsPoint<Vector2D, ColorRGBA>::_Draw(yAxis, ColorRGBA(ColorRGBA::CYAN_COLOR));
		


		// ����� ���������������� ��������
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(0.0, 0.0), Vector2D::_Cross(vector)*2, ColorRGBA(ColorRGBA::WHITE_COLOR));
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(0.0, 0.0), -Vector2D::_Cross(vector)*2, ColorRGBA(ColorRGBA::WHITE_COLOR));






		// ����� �����...
		glColor3f(1.0f, 1.0f, 1.0f);// ����� ���� ����
		glBegin(GL_LINE_LOOP);// ����������� �����: ����� ����� ����� - ��� ������ ���������; ����� ��������� ����� ���������� � ������� ��������� �����

		// ��������� �������� ����������
		for(int i = 0; i < circlePoints; ++i)
		{
			/* ����������: �������� ��������� �����������, ��� ���������� ��������� � �������� ����� - �� �����������,
			   � ������ �����, � ���������� �������������� ����������, ��� ���������� ����� �����������, ����� �����
		 	   ��������� ������� �������, � ���� �� ����� �������. ��� ���������������� ����������� ������, 
			   ��� i = circlePoints-1, ����� � ������� ������ ���� ����� 0; ��� �� ������ ��������� �����������
			   ����� (���� �������) */  
			float angle = Angle::DOUBLE_PI * i / circlePoints;// ��������� ����� ����������
			glVertex2f(cos(angle) * radius, sin(angle) * radius);// ����������� ���� � ��������� ����������
		}
		glEnd();// ��������� ��������� �����
		

		
	

		// ����� ������...
		camera->Begin2D();// ������ ����� ���������� �������

				
		int i = 0; 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "Dot(vector, xAxis): %f", vector.Dot(xAxis));  


		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "Dot(vector, yAxis): %f", vector.Dot(yAxis));  


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.length: %f", vector.Length());


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector(x:%f, y:%f)", vector.x, vector.y);


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.Angle(xAxis): %f radians)", vector.Angle(xAxis));
		

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.Angle(yAxis): %f radians)", vector.Angle(yAxis));


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.angleZ: %f", Vector2D::_AngleZ(vector));

	
	   

		// ��������������� �����...
		camera->Begin3D();// ������ ����� ���������� �������
	}


	// ������ ������� ����
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		// ��� ������...
		if(_event.type == Mouse::MOVE)
		{}
		else if(_event.type == Mouse::KEY_DOWN)
		{
			if(_event.key == Mouse::LEFT_BUTTON)
			{
			}
			else if(_event.key == Mouse::RIGHT_BUTTON)
			{
			}
		}
		else if(_event.type = Mouse::WHEEL)
		{
			if(Keyboard::_IsKeyPressed(Keyboard::S))
			{
				if(_event.wheel > 0)
				{
					vector*=scale;
				}
				else if(_event.wheel < 0)
				{
					vector/=scale;
				}	
			}
			else // ��� ������
			{
				zoom += _event.wheel; 
				camera->SetPosition(Vector3D(camera->GetPosition().x, camera->GetPosition().y, zoom));
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
			else if(_event.key == Keyboard::I)
			{
				vector.Inverse();
			}
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				vector.Rotate(Angle::DOUBLE_PI/circlePoints);
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				vector.Rotate(-Angle::DOUBLE_PI/circlePoints);
			}
			else if(_event.key == Keyboard::C)
			{
				vector.Cross();
			}
			else if(_event.key == Keyboard::N)
			{
				vector.Normalize();
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
	def.realTimeWorkerDef.windowDef.tittle = "Vector2D.example_01";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::POINT;
	//def.faceDrawMode[Renderer::BACK] = Renderer::POINT;
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
