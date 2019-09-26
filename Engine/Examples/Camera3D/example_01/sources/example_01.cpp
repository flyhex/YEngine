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

		glTranslatef(690.0f, 90.0f, -30.0f);// �������

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
class MyQuad: public GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D>
{
	// DATA
	public: float height;// ������ ���� �� ��� Y
	public: float lineSize;// ������� ����� � ��������
	public: float floorWidth, floorHeight;// ���������� ����
	
	// METHODS
	// ����������� �� ���������
	public: MyQuad(void): floorWidth(10.0f), floorHeight(10.0f), lineSize(3.0f), height(-0.3f)
	{


		GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D>::Define def;
		def.scale = Size3D(1.0f, 1.0f, 1.0f);
		def.name = "floor";
		def.position = Vector3D(0, height, 0);
		def.rotation = Quaternion::_FromAxisAngle(AxisAngle(Vector3D::UNIT_X, Angle::_DegreesToRadians(-90.0f)));
		Create(def);


		CreateQuad3D(Box2D<Vector3D>(Vector3D(), Size2D(floorWidth, floorHeight)));// ������ ���������
				
		for(int32 i = 0; i < 5; ++i)// ������� ��������� ��������
		{ 
			TotalPolygonsDivide4(); 
		} 

		CreateColors();
		SetColor(ColorRGBA(255,255,128,255)); // ����
	}

	// ������� ������������
    public: virtual void Draw(void)
	{
		Renderer::FaceDrawMode saveMode = Renderer::_Picked()->GetFaceDrawMode(Renderer::FRONT);// ��������� ����� ������������
		Renderer::_Picked()->SetFaceDrawMode(Renderer::FRONT, Renderer::LINE);// ���������� ������
		float saveLineWidth = GraphicsLine<Vector3D, ColorRGBA>::_LineWidth();// ��������� ������� �����
		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(lineSize);// ������� ����� ��� ���� 

		GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D>::Draw();

		
		Renderer::_Picked()->SetFaceDrawMode(Renderer::FRONT, saveMode);// ������������ ����� ������������ ��� ����������� ������
		

		// ����� ������ ���������...
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, height, 0.0f), Vector3D(0.0f, 1.0f-height, 0.0f), ColorRGBA::BLUE_COLOR);
		
		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(saveLineWidth);// ������� ����� ��� ���� 
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
	private: Quaternion q;// ���������� �������� � ����������������� ���������� ���� �� �������� � ������ ������� t
	private: QuaternionSlerper slerper; // ���� ����������� �� ��������� ��������� � quaternion �� ���������� ����, ��������� SLERP-������������
	private: bool slerperEnable;// ���� true, �� ��������� ������������ slerp-������������
	private: float t;// ���������� ����� ���� ������������ slerp
	

	private: MyQuad* floor;// ���
    private: SimpleList<GraphicsPoint<Vector3D, ColorRGBA>> pointList;// ������ ������� �����
	private: int32 amountOfPoints; // ���������� ������� ������
    private: float pointSize;// ������ ������� ����� � ���������� ��������
    private: int32 pointId;// ����� �����, �� ������� ����� �������� ������ ��� �����
	private: int32 currentPoint;// ����� ������� �����, �� ������� �������� ������


    private: Vector3D worldZOld, worldXOld, worldYOld;// ������� ��������� ������ ����� �������������
	private: Matrix4x4 defaultRotationMatrix;// ������� �������� ������ �� ���������
	private: bool moveToDefaultState;// ���� true, ������ �������� ������������ �������� � ���������� �� ��������� 


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), floor(NIL), slerperEnable(false), t(0.0f), pointSize(8.0f), pointId(0), currentPoint(-1), moveToDefaultState(false)
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
				def.movementType = Camera3D::OVERLAND;

				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, true, false);
				camera->ConnectTo(ResizeEvent::TYPE, this);

				defaultRotationMatrix = camera->RotationMatrix();// ���������� ������ �� ���������
			}

		
			// ������� ���...
			{
				floor = new MyQuad();
				scene->AddObject(floor, false, true);
			}
			 

			// ������������� ������� �������...
			{
				float radius = 9;// ������ ������ (���������� �� ������ ���������)
				amountOfPoints = 10; // ���������� ������� ������
		
				for(int i = 0; i < amountOfPoints; ++i)
				{
					GraphicsPoint<Vector3D, ColorRGBA> p;
					p.SetColor(ColorRGBA::GREEN_COLOR);// ����
			
					// ��������� ������� � ������� ���������
					p.SetPosition(Vector3D(Spherical(radius, Random::_RandomFloat(0, Angle::PI/15), Random::_RandomFloat(-Angle::PI, Angle::PI))));// ����������

					pointList.AddToTail(p);// �������� �������
				}
			}

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
		if(slerperEnable)// ���� ����� ��������� ������������ �������� � ������� SLERP-������������
		{
			if(t > 1.0f) // ���� ������� �����������
			{ 
				slerperEnable = false;// ��������� ������������ SLERP
			    t = 0.0f; // �������� ����������� (������ ��������)
				moveToDefaultState = false;
			} 
			else// ����� ��������� ������������ ��������
			{
				q = slerper.Interpolate(t);// t ���������� �� 0 (identity) �� 1 (quaternion)
			
				t += _delta / 2.0f;// ������� � ���������� ���� ������������ SLERP
			}
		}
	}

	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����

		// ��������� ��������� ������������...
		if(slerperEnable && !moveToDefaultState) // ���� �������� �������������
		{
			// ���� ���������������� ������� ���� ��������...
			camera->SetRotation(Matrix4x4::_FromQuaternion(q));
		} 
		else if(moveToDefaultState)
		{

			// ���� ������ ���������� ������ ����� ������������� �� ����...
			// ������� ��� ������ �� �������� ��������� � ������� �� ��������� ���������� (worldXOld, worldYOld, worldZOld) � ������� ��������� �����������
			camera->SetRotation(Matrix4x4(q * worldXOld, q * worldYOld, q * worldZOld, Vector3D()));
		}


		// ����� �����...
		camera->Begin2D();// ������ ����� ���������� �������

	
		// ���������� ������...
		gizmo.vectorX = camera->RotationMatrix().GetX();
		gizmo.vectorY = camera->RotationMatrix().GetY();
		gizmo.vectorZ = camera->Look();
		gizmo.Draw();
			


		// ��������������� �����...
		camera->End2D();// ������ ����� ���������� �������
		scene->Draw();



		// ����� ������� ������...
		float savePointSize = GraphicsPoint<Vector3D, ColorRGBA>::_PointSize();// ��������� ������ �����
		GraphicsPoint<Vector3D, ColorRGBA>::_SetPointSize(pointSize);
		
		// ������ ��� ������� �������
		for(int i = 0; i < amountOfPoints; ++i)
		{
			SimpleList<GraphicsPoint<Vector3D, ColorRGBA>>::Node * n = pointList.Get(i);

			if(i == currentPoint)// ���� ������� �����, �� ������� �������� ������
			{
				n->data.SetColor(ColorRGBA::RED_COLOR);// �������� ������� �����
			}
			else { n->data.SetColor(ColorRGBA::GREEN_COLOR); } // ������� ���� ��� ������ �����

			n->data.Draw();// ����� �������
		}
		GraphicsPoint<Vector3D, ColorRGBA>::_SetPointSize(savePointSize);// ������������ ������ �����

		

		// ����� ������...
		camera->Begin2D();// ������ ����� ���������� �������

		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 20, -1), "moving: %d", slerperEnable);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 40, -1), "target points: %d", amountOfPoints);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 60, -1), "current point: %d", pointId);

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 80, -1), "camera->position: x: %.2f, y: %.2f, z: %.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
	    
		
		StringANSI string;  
		if(camera->GetMovementType() == Camera3D::OVERLAND) { string = "OVERLAND"; }
		else if(camera->GetMovementType() == Camera3D::FLIGHT) { string = "FLIGHT"; } 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 100, -1), "camera->MovementType: %s", string.c_str());
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 120, -1), "view matrix:");
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 140, -1), "|%.2f|%.2f|%.2f|", camera->ViewMatrix().m11, camera->ViewMatrix().m12, camera->ViewMatrix().m13);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 160, -1), "|%.2f|%.2f|%.2f|", camera->ViewMatrix().m21, camera->ViewMatrix().m22, camera->ViewMatrix().m23);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 180, -1), "|%.2f|%.2f|%.2f|", camera->ViewMatrix().m31, camera->ViewMatrix().m32, camera->ViewMatrix().m33);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 200, -1), "|%.2f|%.2f|%.2f|", camera->ViewMatrix().m41, camera->ViewMatrix().m42, camera->ViewMatrix().m43);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 220, -1), "camera->look: %.2f/%.2f/%.2f", camera->Look().x, camera->Look().y, camera->Look().z);
	
		// ��������������� �����...
		camera->End2D();// ������ ����� ���������� �������
	}


	// ������ ������� ����
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		if(_event.type == Mouse::KEY_DOWN && !slerperEnable)// ���� ������ ������� ����, � ���� ��� �������� ������������
		{
			if(_event.key == Mouse::LEFT_BUTTON)// ����� ����
			{
				Matrix4x4 s1 = camera->RotationMatrix();// ����� ���������� ������� �������

				Matrix4x4 s2;// ����� ������� ��������
				// ������� ������� ������� �� ��������� ����� pointId � ���������� ������, � �� ��������, �.�. ������ ������ ������������� ����� ������������� ��� WorldRotationMatrix
				s2.LookAt(pointList.Get(pointId)->data.GetPosition(), camera->GetPosition(), Vector3D(0.0f, 1.0f, 0.0f));
				
				slerperEnable = true;// ��������� ������������ ������������	
				slerper.Set(Quaternion(s1), Quaternion(s2));// ������ ������������ �������� �� ������� ���������� ������ � �������				

				currentPoint = pointId;
		

				// ������� � ��������� ������� �����; ����� �� �����
				if(pointId + 1 < amountOfPoints) { pointId++; } 
				else { pointId = 0; }
			}
			else if(_event.key == Mouse::RIGHT_BUTTON)// ������ ����
			{ 
				camera->LookAt(camera->GetPosition(), pointList.Get(pointId)->data.GetPosition(), Vector3D::UNIT_Y);// ����������� ������ � ������� ����� pointId
				
				currentPoint = pointId;

				// ������� � ��������� ������� �����; ����� �� �����
				if(pointId + 1 < amountOfPoints) { pointId++; } 
				else { pointId = 0; } 
			}
			else if(_event.key == Mouse::MIDDLE_BUTTON)// ������� ����
			{ 
				Matrix4x4 s1 = camera->RotationMatrix();// ����� ���������� ������� �������
				
				Matrix4x4 s2;

				// ������� ������� ������� �� ��������� ����� 0,0,0 (������ ���������) � ���������� ������, � �� ��������, �.�. ������ ������ ������������� ����� ������������� ��� WorldRotationMatrix
				s2.LookAt(Vector3D(), camera->GetPosition(), Vector3D(0.0f, 1.0f, 0.0f));
				slerperEnable = true;// ��������� ������������ ������������	
				
				slerper.Set(Quaternion(s1), Quaternion(s2));

		
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
			else if(_event.key == Keyboard::A)
			{
				camera->Strafe(-0.1f);
			}
			else if(_event.key == Keyboard::D)
			{
				camera->Strafe(0.1f);
			}
			else if(_event.key == Keyboard::W)
			{
				camera->Walk(0.1f);
			}
			else if(_event.key == Keyboard::S)
			{
				camera->Walk(-0.1f);
			}
			else if(_event.key == Keyboard::Q)
			{
				camera->Fly(0.1f);
			}
			else if(_event.key == Keyboard::E)
			{
				camera->Fly(-0.1f);
			}
			else if(_event.key == Keyboard::UP_ARROW)
			{
				camera->Pitch(0.01f);
			}
			else if(_event.key == Keyboard::DOWN_ARROW)
			{
				camera->Pitch(-0.01f);
			}
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				camera->Yaw(0.01f);
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				camera->Yaw(-0.01f);
			}
			else if(_event.key == Keyboard::ENTER)
			{
				camera->Roll(0.01f);
			}
			else if(_event.key == Keyboard::SHIFT)
			{
				camera->Roll(-0.01f);
			}
			else if(_event.key == Keyboard::L && !slerperEnable)
			{
				moveToDefaultState = true;// �������� ������� � ���������� ������ �� ���������

			    slerperEnable = true;// ��������� ������������ ������������

				// ���� ������ ���������� ������ ����� ������������� �� ����...
				Quaternion q1(camera->RotationMatrix());// ������������ ������� ���������� ������ � ����������
				Quaternion q2(defaultRotationMatrix);// ������������ ����������� ���������� ������ � ����������
				q1.Inverse();// ������ ����������� ����������� �� ���������������; ���������� q1 ��������� ������� �� ���������� ��������� (DefaultWorldMatrix) � ������� (CameraRotationMatrix); �������� ������ ���������� �� �������� ��������� � ���������

				// ������ ������������ �������� �� ��������� ���������� � �����������, ��������������� q1
				slerper.Set(q2, q1); 
				
				//��������� ���������� ���� ������ ����� �������������
				worldZOld = camera->RotationMatrix().GetZ();
				worldXOld = camera->RotationMatrix().GetX();
				worldYOld = camera->RotationMatrix().GetY(); 
			}
			else if(_event.key == Keyboard::M)
			{
				if(camera->GetMovementType() == Camera3D::FLIGHT)
				{
					camera->SetMovementType(Camera3D::OVERLAND);
				}
				else if(camera->GetMovementType() == Camera3D::OVERLAND)
				{
					camera->SetMovementType(Camera3D::FLIGHT);
				}
			}
			else if(_event.key == Keyboard::K)
			{
				camera->Move(camera->Look(), 4.0f);
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
	def.realTimeWorkerDef.windowDef.tittle = "Camera3D.example_01";
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
