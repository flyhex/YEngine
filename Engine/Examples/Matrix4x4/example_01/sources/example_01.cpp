

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
  

	// METHODS
	// ����������� �� ���������
    public: Gizmo(void)
	{
		// ������ ����������� ���� X,Y,Z
		vectorX.Set(1.0f, 0.0f, 0.0f);
		vectorY.Set(0.0f, 1.0f, 0.0f);
		vectorZ.Set(0.0f, 0.0f, 1.0f);

		cyan = ColorRGBA::CYAN_COLOR; 
		red = ColorRGBA::RED_COLOR;
		green = ColorRGBA::GREEN_COLOR;
	}


	// ������������ Gizmo
	public: void Draw(void)
	{		
		// �������������...
		glMatrixMode(GL_MODELVIEW);// �������� � �������� ����
		glPushMatrix();// ��������� ������� ������� � �����  
		glLoadIdentity();// ��������� ��������� �������

		glTranslatef(690.0f, 90.0f, -300.0f);// �������

		glScalef(50.0f, 50.0f, 50.0f);// ���������������
		glRotatef(10.0f, 1.0f, 0.0f, 0.0f);// ������� �� ��� X
		glRotatef(45.0f, 0.0f, 1.0f, 0.0f);// ������� �� ��� Y

		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(3.0f);// ������� ����� ��� ���� 

		// ������������ ���� ���������� ������� ���������
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, 0.0f, 0.0f), vectorX, red);
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, 0.0f, 0.0f), vectorY, cyan);
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, 0.0f, 0.0f), vectorZ, green);

		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(1.0f); // ������������ ������� ����� 
		
		// �������������...
		glPopMatrix();// ������������ ���������� ������� �� ������  

		// ����������� �������� ���� ������� ������� ���������
		Font2D::_Picked()->SetTextColor(red);
		Font2D::_Picked()->DrawScreenText(Vector3D(610 + 10+50, 50, -1), "X"); 

		Font2D::_Picked()->SetTextColor(cyan);
		Font2D::_Picked()->DrawScreenText(Vector3D(610 + 10+25+50, 50, -1), "Y"); 

		Font2D::_Picked()->SetTextColor(green);
		Font2D::_Picked()->DrawScreenText(Vector3D(610 + 10+50+50, 50, -1), "Z"); 
		Font2D::_Picked()->SetTextColor(ColorRGBA::WHITE_COLOR);

	}
};

// �����, ����������� ���������� �������
class MyBox: public GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>
{
	// METHODS
	// ����������� �� ���������
	public: MyBox(void)
	{}

	// ������������ ��������������
    public: virtual void Draw(void)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::Draw(GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::FRONT_FACE);
				
		glColor3f(0.0f, 1.0f, 0.0f);
		GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::Draw(GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::BACK_FACE);
				
		glColor3f(0.0f, 0.0f, 1.0f);
		GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::Draw(GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::LEFT_FACE);
					
		glColor3f(1.0f, 0.0f, 1.0f);
		GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::Draw(GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::RIGHT_FACE);
						
		glColor3f(1.0f, 1.0f, 0.0f);
		GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::Draw(GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::TOP_FACE);
					
		glColor3f(0.0f, 1.0f, 1.0f);
		GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::Draw(GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::BOTTOM_FACE);
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
	private: MyBox* box;// ���������� �������������, ����������� �������

			 	
    private: float xPos, yPos, zPos;// ������� ���� 
    private: float spinX, spinY, spinZ;// ���� �������� ���� �� ���� X,Y,Z
	private: float step;// ������ ���� ���� ��������
	private: float scaleX, scaleY, scaleZ; // ������������ ��������������� �� ���� X,Y,Z
    Matrix4x4 T, S, R;

    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), box(NIL), xPos(0.0f), yPos(0.0f), zPos(0.0f), spinX(0.0f), spinY(0.0f), spinZ(0.0f), step(0.1f), scaleX(1.0f), scaleY(1.0f), scaleZ(1.0f)
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

			// ���...
			{
				GraphicsMesh<Vector3D, uint8, ColorRGBA, Quaternion, Size3D>::Define def;
				def.name = "box3d";
				def.scale = Size3D(1,1,1);  
					
				box = new MyBox();
				box->Create(def);
				box->CreateBox3D(Box3D(Vector3D(0, 0, 0), Size3D(0.5f, 0.5f, 0.5f)));
	

				// ����� ������
				box->CreateColors(); 

				box->SetColor(0, ColorRGBA::_Random());
				box->SetColor(1, ColorRGBA::_Random());
				box->SetColor(2, ColorRGBA::_Random());
				box->SetColor(3, ColorRGBA::_Random());

				box->SetColor(4, ColorRGBA::_Random());
				box->SetColor(5, ColorRGBA::_Random());
				box->SetColor(6, ColorRGBA::_Random());
				box->SetColor(7, ColorRGBA::_Random());
				scene->AddObject(box, false, true);// �������� � �����
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
				def.position = Vector3D(0,0,10);
				def.controllerEnabled = false;

				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, true, false);
				camera->ConnectTo(ResizeEvent::TYPE, this);
			}

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


	
		// ������� �����������
		T.Translation(Vector3D(xPos, yPos, zPos));


		// ������� ���������������
		S.Scaling(Vector3D(scaleX, scaleY, scaleZ));


		// ������� ��������
		/* ���� ����� ������� � ������������ ��� ������� ���������, �������������, ������ ������� ���������� ������ ��� ���������� 
		   ���, � ���� ������������ ������� ���������, ������ ������� ���������� ������ ������� */ 
		
		//R = Matrix3x3::_RotationX(spinX) * Matrix3x3::_RotationY(spinY) * Matrix3x3::_RotationZ(spinZ); // �������������� ������ ������� ���������� �� ���� ����
		R = Matrix3x3::_FromAxisAngle(AxisAngle(Vector3D::UNIT_X, spinX)) * Matrix3x3::_FromAxisAngle(AxisAngle(Vector3D::UNIT_Y, spinY)) * Matrix3x3::_FromAxisAngle(AxisAngle(Vector3D::UNIT_Z, spinZ));
  

			
		// �������������� �������������
		box->SetPosition(Vector3D(xPos, yPos, zPos)); // �������� �������
		box->SetRotation(R.ToQuaternion());
		box->SetScale(Size3D(scaleX, scaleY, scaleZ));
		

		// ������������� ���� �����
		//_SetCurrentStack(Renderer::MODELVIEW);
		//_PushMatrix();
		//_LoadIdentityMatrix();
		//_LoadMatrix(T * R * S);// �������� �������
		box->Draw();// ���������� ����
		//_PopMatrix();
		 
		

	

		// ����� ������...
		camera->Begin2D();// ������ ����� ���������� �������

				
		// ���������� ������...
		gizmo.vectorX = (T * R * S).GetX();
		gizmo.vectorY = (T * R * S).GetY();
		gizmo.vectorZ = (T * R * S).GetZ();
		gizmo.Draw();

			
	
				
		int i = 0; 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "box.Position (X: %.2f, Y: %.2f, Z: %.2f)", xPos, yPos, zPos); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "box.Rotation (X: %.2f, Y: %.2f, Z: %.2f)", spinX, spinY, spinZ); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "box.Scaling (X: %.2f, Y: %.2f, Z: %.2f)", scaleX, scaleY, scaleZ); 

		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),  "Scaling matrix:"); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", S.m11, S.m12, S.m13, S.m14); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", S.m21, S.m22, S.m23, S.m24); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", S.m31, S.m32, S.m33, S.m34); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", S.m41, S.m42, S.m43, S.m44); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),  "Rotation matrix:"); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m11, R.m12, R.m13, R.m14); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m21, R.m22, R.m23, R.m24); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m31, R.m32, R.m33, R.m34); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m41, R.m42, R.m43, R.m44);
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),  "Translation matrix:"); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", T.m11, T.m12, T.m13, T.m14); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", T.m21, T.m22, T.m23, T.m24); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", T.m31, T.m32, T.m33, T.m34); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", T.m41, T.m42, T.m43, T.m44);


		// ��������������� �����...
		camera->Begin3D();// ������ ����� ���������� �������
	}


	// ������ ������� ����
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		if(Keyboard::_IsKeyPressed(Keyboard::E))
		{
			if(_event.wheel > 0)
			{
				scaleX += step;
			}
			else if(_event.wheel < 0)
			{
				scaleX -= step;
			}

			if(scaleX > 10.0f) { scaleX = 10.0f; }
			else if (scaleX < 0.1f) { scaleX = 0.1f; }
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::D))
		{
			if(_event.wheel > 0)
			{
				scaleY += step;
			}
			else if(_event.wheel < 0)
			{
				scaleY -= step;
			}

			if(scaleY > 10.0f) { scaleY = 10.0f; }
			else if (scaleY < 0.1f) { scaleY = 0.1f; }
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::C))
		{
			if(_event.wheel > 0)
			{
				scaleZ += step;
			}
			else if(_event.wheel < 0)
			{
				scaleZ -= step;
			}

			if(scaleZ > 10.0f) { scaleZ = 10.0f; }
			else if (scaleZ < 0.1f) { scaleZ = 0.1f; }
		}
		else if(_event.type == Mouse::WHEEL)
		{
			if(_event.wheel > 0)
			{
				zPos += 1.0f;
			}
			else if(_event.wheel < 0)
			{
				zPos -= 1.0f;
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
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				xPos--;
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				xPos++;
			}	
				else if(_event.key == Keyboard::UP_ARROW)
			{
				yPos++;
			}
			else if(_event.key == Keyboard::DOWN_ARROW)
			{
				yPos--; 
			}	
			else if(_event.key == Keyboard::Q)
			{
				spinX += step;
				// ������������ ����� �� �������� 360 ��������
				if(spinX > Angle::DOUBLE_PI) { spinX = spinX - Angle::DOUBLE_PI; }
			}
			else if(_event.key == Keyboard::W)
			{
				spinX -= step;
				// ������������ ����� �� �������� 360 ��������
				if(spinX > Angle::DOUBLE_PI) { spinX = spinX - Angle::DOUBLE_PI; }
			}
			else if(_event.key == Keyboard::A)
			{
				spinY += step;
				// ������������ ����� �� �������� 360 ��������
				if(spinY > Angle::DOUBLE_PI) { spinY = spinY - Angle::DOUBLE_PI; }
			}
			else if(_event.key == Keyboard::S)
			{
				spinY -= step;
				// ������������ ����� �� �������� 360 ��������
				if(spinY > Angle::DOUBLE_PI) { spinY = spinY - Angle::DOUBLE_PI; }
			}
			else if(_event.key == Keyboard::Z)
			{
				spinZ += step;
				// ������������ ����� �� �������� 360 ��������
				if(spinZ > Angle::DOUBLE_PI) { spinZ = spinZ - Angle::DOUBLE_PI; }
			}
			else if(_event.key == Keyboard::X)
			{
				spinZ -= step;
				// ������������ ����� �� �������� 360 ��������
				if(spinZ > Angle::DOUBLE_PI) { spinZ = spinZ - Angle::DOUBLE_PI; }
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
	def.realTimeWorkerDef.windowDef.tittle = "Matrix4x4.example_01";
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
