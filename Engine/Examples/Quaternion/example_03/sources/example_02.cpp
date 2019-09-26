

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
#include <Engine/Math/Matrix/Matrix3x3/Matrix3x3.h>





// �����, ����������� ������������� ������
class Gizmo
{
	// DATA
	public: Vector3D vectorX;// ����������� ��� X
	public: Vector3D vectorY;// ����������� ��� Y
	public: Vector3D vectorZ;// ����������� ��� Z
 
	public: ColorRGBA cyan;// ���� ��� ��� Y
    public: ColorRGBA red; // ���� ��� ��� X
	public: ColorRGBA green;// ���� ��� ��� Z
  

    public: Vector3D vector;// ����������� ������
    public: ColorRGBA yellow;// ���� ��� ������������ �������
	public: bool drawRotate;// ���� true, �� ���������� ������ rotate
	public: Vector3D rotate;// �������� ������� vector


	// METHODS
	// ����������� �� ���������
	public: Gizmo(void): drawRotate(false)
	{
		// ������ ����������� ���� X,Y,Z
		vectorX.Set(1.0f, 0.0f, 0.0f);
		vectorY.Set(0.0f, 1.0f, 0.0f);
		vectorZ.Set(0.0f, 0.0f, 1.0f);

		cyan = ColorRGBA::CYAN_COLOR; 
		red = ColorRGBA::RED_COLOR;
		green = ColorRGBA::GREEN_COLOR;

		vector.Set(-1.0f, 0.0f, 0.0f);
		yellow = ColorRGBA::YELLOW_COLOR;
	}


	// ������������ Gizmo
	public: void Draw(void)
	{		
		// �������������...
		//glMatrixMode(GL_MODELVIEW);// �������� � �������� ����
		//glPushMatrix();// ��������� ������� ������� � �����  
		//glLoadIdentity();// ��������� ��������� �������

		//glTranslatef(390.0f, 290.0f, -300.0f);// �������

		//glScalef(150.0f, 150.0f, 150.0f);// ���������������
		//glRotatef(10.0f, 1.0f, 0.0f, 0.0f);// ������� �� ��� X
		//glRotatef(45.0f, 0.0f, 1.0f, 0.0f);// ������� �� ��� Y

		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(3.0f);// ������� ����� ��� ���� 

		// ������������ ���� ���������� ������� ���������
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, 0.0f, 0.0f), vectorX, red);
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, 0.0f, 0.0f), vectorY, cyan);
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, 0.0f, 0.0f), vectorZ, green);

		// ���������� ����������� ������... 

		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0,0,0), vector, yellow);// ����������� ������
		
		if(drawRotate)
		{
			GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0,0,0), rotate, yellow);// ����������� ������
		}


		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(1.0f); // ������������ ������� ����� 
		
		// �������������...
		//glPopMatrix();// ������������ ���������� ������� �� ������  
	}
};


// �����, ����������� ���������������� ����
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: GraphicsScene<Vector3D, Quaternion, Size3D>* scene;// ����������� �����
    private: Gizmo gizmo;// ������, ������������ ������� ������� ��������� ������
	
   
    private: Quaternion quaternion;// ����������� ����������
	private: QuaternionLerper lerper; // ���� ����������� �� ��������� ��������� � quaternion �� ���������� ����, ��������� LERP-������������
	private: bool lerperEnable;// ���� true, �� ��������� ������������ LERP-������������
	private: float t;// ���������� ����� ���� ������������ LERP
	private: Quaternion previousCameraRotation;// ���������� ���������� ������
	private: bool rotateAroundPivot;// ���� true, �� ������� ������ ������ ������ ������ ��������� �� ��� Y
	private: float spinZ;


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), lerperEnable(false), t(0.0f), rotateAroundPivot(false), spinZ(0.0f)
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
				
				def.controllerType = Camera3D::TRACKBALL_OBJECT_VIEWER;// ����� �������
				def.pitchMax = 0.0f;// ��������� ������ ���������� ���� ���� (��������� XZ)
				def.viewingPoint = Vector3D();
				def.zoomMin = 7.0f;
				def.zoomMax = 9.0f;

				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, true, false);
				camera->ConnectTo(ResizeEvent::TYPE, this);
				camera->ConnectTo(Mouse::Event::TYPE, this);

				// ������ �������������� ������ ������
				camera->UpdateTrackBallObjectViewer(-Angle::HALF_PI/3.0f, -Angle::HALF_PI/3.0f, 0.0f);
			}

			Mouse::_MoveCursorToClientAreaCenter(this);// ����������� ������ � ����� ���������� ������� ����
			Mouse::_ShowCursor(false);// ������ ������

			Flush();//��������� ��� ����� ���������� �������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}
	




	// ������� �����
	public: virtual void Update(float _delta)
	{
		// ������������ �������� ����� 2 ������� (t ������������ ������ _delta, � ��� t = 1, �������� 1 ������� �������; 
		// �� ����� _delta �� ���, ����� ��������� ������������ �������� � ��� ����, � ���������� ���� t ����������� �� 2 �������)
		if(lerperEnable)// ���� ����� ��������� ������������ �������� � ������� LERP-������������
		{
			if(t > 1.0f) // ���� ������� �����������
			{ 
				lerperEnable = false;// ��������� ������������ LERP
				gizmo.drawRotate = false;
			    t = 0.0f; // �������� ����������� (������ ��������)
			} 
			else// ����� ��������� ������������ ��������
			{ 
				quaternion = lerper.Interpolate(t);// t ���������� �� 0 (identity) �� 1 (quaternion)
			
				t += _delta / 2.0f;// ������� � ���������� ���� ������������ LERP
			}
		}
	}

	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����


		if(lerperEnable)// ���� ���� ������������
		{
			gizmo.rotate = quaternion.RotateVector(gizmo.vector);// ������� ������ 
		}
		

		if(rotateAroundPivot)// ���� ����� ������� ������ �����
		{
			Quaternion q1(AxisAngle(Vector3D::UNIT_Y, 0.1f));

			gizmo.vector = q1.RotatePointAroundPivot(gizmo.vector, Vector3D());// ������� ����� gizmo.vector �� ��� Y, ������ ������ ���������
		
			// �������������� ������ 
			//Matrix4x4 m = Matrix4x4::_RotationAroundPivot(Matrix4x4(q1), Vector3D());
			//gizmo.vector = m * gizmo.vector;

			// �������������� ������ 
			//gizmo.vector = Matrix4x4(q1).RotatePointAroundPivot(gizmo.vector, Vector3D());
		}
		else
		{
			// ������� ������-���������� ����� ����������� ������� ������� �� ��������� � ������� ����������� ������, � ������� � ������� ����� ����������� ����������� ������ gizmo.vector
			// ������ ������ ������ ��������� � ����� � ��� �� �����������
			gizmo.vector = Quaternion::_FromTo(Quaternion(), camera->GetRotation()).RotateVector(Vector3D(-1.0f, 0.0f, 0.0f));
		}
	
		// ���������� ������
		gizmo.Draw();

			
	
			
		// ����� ������...
		camera->Begin2D();// ������ ����� ���������� �������

	
		// ����������� �������� ���� ������� ������� ���������
		Font2D::_Picked()->SetTextColor(ColorRGBA(ColorRGBA::RED_COLOR));
		
		Font2D::_Picked()->DrawScreenText(Vector3D(390.0f - 20, 210, -1), "X"); 

		Font2D::_Picked()->SetTextColor(ColorRGBA(ColorRGBA::CYAN_COLOR));
		Font2D::_Picked()->DrawScreenText(Vector3D(390.0f + 5, 210, -1), "Y"); 

		Font2D::_Picked()->SetTextColor(ColorRGBA(ColorRGBA::GREEN_COLOR));
		Font2D::_Picked()->DrawScreenText(Vector3D(390.0f + 30, 210, -1), "Z"); 
		Font2D::_Picked()->SetTextColor(ColorRGBA::WHITE_COLOR);
				

		int i = 0; 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quaternion(x: %.2f, y: %.2f, z: %.2f, w: %.2f)", quaternion.x, quaternion.y, quaternion.z, quaternion.w); 
	


		Matrix4x4 R = Matrix4x4::_FromQuaternion(quaternion);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),  "Rotation matrix:"); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m11, R.m12, R.m13, R.m14); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m21, R.m22, R.m23, R.m24); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m31, R.m32, R.m33, R.m34); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m41, R.m42, R.m43, R.m44);
		
		



		// ��������������� �����...
		camera->Begin3D();// ������ ����� ���������� �������


		previousCameraRotation = camera->GetRotation();// ��������� ������� �������� ������
	}


	// ������ ������� ����
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{	
	
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
			else if(_event.key == Keyboard::L)
			{
				lerperEnable = true;// ��������� ������������ ������������
				gizmo.drawRotate = true;

				Quaternion q = Quaternion::_ShortestArc(gizmo.vector, gizmo.vectorZ);// ����� ���������� �� ����������� gizmo.vector � ����������� gizmo.vectorZ
				
				// ������� �� ��������� q � ��������� �� ���������
				lerper.Set(q, Quaternion());
			}
			else if(_event.key == Keyboard::R)
			{
				rotateAroundPivot = !rotateAroundPivot;
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
	def.realTimeWorkerDef.windowDef.tittle = "Quaternion.example_02";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;
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
