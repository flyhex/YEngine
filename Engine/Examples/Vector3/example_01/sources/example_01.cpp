

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






// ��������� �����
class Gizmo
{
	// DATA
	public: GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D>* icosphere;// ��������

	public: Vector3D vectorX;// ����������� ��� X
	public: Vector3D vectorY;// ����������� ��� Y
	public: Vector3D vectorZ;// ����������� ��� Z
	public: Vector3D vector;// ����������� ������
 	
    public: ColorRGBA yellow;// ���� ��� ������������ �������
	public: ColorRGBA cyan;// ���� ��� ��� Y
    public: ColorRGBA red; // ���� ��� ��� X
	public: ColorRGBA green;// ���� ��� ��� Z

	private: float scaleStep;// �������� ������ ���� ���������������



	// METHODS
	// ����������� �� ���������
    public: Gizmo(void): icosphere(NIL)
	{
		// ������ ����������� ���� X,Y,Z
		vectorX.Set(1.0f, 0.0f, 0.0f);
		vectorY.Set(0.0f, 1.0f, 0.0f);
		vectorZ.Set(0.0f, 0.0f, 1.0f);
		vector.Set(-1.0f, 0.0f, 0.0f);


		scaleStep = 1.2f;
		yellow = ColorRGBA(ColorRGBA::YELLOW_COLOR);
		cyan = ColorRGBA(ColorRGBA::CYAN_COLOR); 
		red = ColorRGBA(ColorRGBA::RED_COLOR);
		green = ColorRGBA(ColorRGBA::GREEN_COLOR);

		GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D>::Define def;
		def.scale = Size3D(1, 1, 1);
		def.name = "icosphere";
		icosphere = new GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D>();
		icosphere->Create(def);
		icosphere->CreateIcosphere3D(3);
		
		// ������� ����� ������
		icosphere->CreateColors();
		icosphere->SetColor(ColorRGBA(ColorRGBA::WHITE_COLOR));
	}


	// ������������ Gizmo
	public: void Draw(void)
	{
		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(3.0f);// ������� ����� ��� ���� 

		// ������������ ���� ���������� ������� ���������
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0,0,0), vectorX, red);
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0,0,0), vectorY, cyan);
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0,0,0), vectorZ, green);

		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0,0,0), vector, yellow);// ����������� ������

		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(1.0f); // ������� ����� ��� ������� �������� 1

		icosphere->Draw();// ���������� �����
	}

	// ������������ ������������ �������
	public: void Normalize(void)
	{
		vector.Normalize();
	}
				
	// ��������������� ������������ ������� � ������� ����������
	public: void ScalePlus(void)
	{
		vector *= scaleStep;
	}
				
	// ��������������� ������������ ������� � ������� ����������
	public: void ScaleMinus(void)
	{
		vector /= scaleStep;
	}


	// �������� ����������� ������������ ������� �� ���������������
	public: void Inverse(void)
	{
		vector.Inverse();
	}

  

	// ������� ������������ ������� ������������ ��� X �� ������� �������
	public: void RotateXMinus(void)
	{
		vector.RotateAxisAngle(Vector3D::UNIT_X, -0.1f);  
	}
		

	// ������� ������������ ������� ������������ ��� X ������ ������� �������
	public: void RotateXPlus(void)
	{
		vector.RotateAxisAngle(Vector3D::UNIT_X, 0.1f); 
	}

		
	// ������� ������������ ������� ������������ ��� Y ������ ������� �������
	public: void RotateYPlus(void)
	{
		vector.RotateAxisAngle(Vector3D::UNIT_Y, 0.1f); 
	}

	// ������� ������������ ������� ������������ ��� Y �� ������� �������
	public: void RotateYMinus(void)
	{
		vector.RotateAxisAngle(Vector3D::UNIT_Y, -0.1f);   
	}
		

		
	// ������� ������������ ������� ������������ ��� Z �� ������� �������
	public: void RotateZMinus(void)
	{
		vector.RotateAxisAngle(Vector3D::UNIT_Z, -0.1f);   
	}
		

	// ������� ������������ ������� ������������ ��� Z ������ ������� �������
	public: void RotateZPlus(void)
	{
		vector.RotateAxisAngle(Vector3D::UNIT_Z, 0.1f);   
	}
};

// �����, ����������� ���������������� ����
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: GraphicsScene<Vector3D, Quaternion, Size3D>* scene;// ����������� �����

    private: Gizmo* gizmo;// �����
    private: float vectorTheta, vectorPhi;



    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), gizmo(NIL), vectorTheta(0.0f), vectorPhi(0.0f)
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
			gizmo = new Gizmo();
			scene->AddObject(gizmo->icosphere, false, true);

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
				def.viewingPoint = gizmo->icosphere->GetPosition();
				def.zoomMin = 5.0f;
				def.zoomMax = 6.0f;

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
	{}

	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����


		

		// ����� �����
		gizmo->Draw();

		
	

		// ����� ������...
		camera->Begin2D();// ������ ����� ���������� �������

				
		int i = 0; 

		font2d->SetTextColor(ColorRGBA(ColorRGBA::WHITE_COLOR));
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector(x: %.3f, y: %.3f, z: %.3f)", gizmo->vector.x, gizmo->vector.y, gizmo->vector.z);

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.Length: %.3f", gizmo->vector.Length());

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "Dot(vector, xAxis): %.3f", gizmo->vector.Dot(gizmo->vectorX));  


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "Dot(vector, yAxis): %.3f", gizmo->vector.Dot(gizmo->vectorY));  
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "Dot(vector, zAxis): %.3f", gizmo->vector.Dot(gizmo->vectorZ));  


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.AngleX: %.3f", gizmo->vector.AngleX()); 


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.AngleY: %.3f", gizmo->vector.AngleY());


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.AngleZ: %.3f", gizmo->vector.AngleZ()); 



		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.theta: %.3f", gizmo->vector.ToSpherical().vertical); 

		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.phi: %.3f", gizmo->vector.ToSpherical().horizontal);
	

		// ��������������� �����...
		camera->Begin3D();// ������ ����� ���������� �������
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
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				vectorTheta -= 0.1f;

				gizmo->vector.FromSpherical(Spherical(1, vectorTheta, vectorPhi));

				gizmo->vector.FromSpherical(Spherical(1, gizmo->vector.ToSpherical().vertical, gizmo->vector.ToSpherical().horizontal));
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				vectorTheta += 0.1f;

				gizmo->vector.FromSpherical(Spherical(1, vectorTheta, vectorPhi));

				//gizmo->vector.FromSpherical(Spherical(1, gizmo->vector.ToSpherical().vertical, gizmo->vector.ToSpherical().horizontal));
			}	
			else if(_event.key == Keyboard::UP_ARROW)
			{
				vectorPhi -= 0.1f;

				gizmo->vector.FromSpherical(Spherical(1, vectorTheta, vectorPhi));

				//gizmo->vector.FromSpherical(Spherical(1, gizmo->vector.ToSpherical().vertical, gizmo->vector.ToSpherical().horizontal));
			}
			else if(_event.key == Keyboard::DOWN_ARROW)
			{
				vectorPhi += 0.1f;

				gizmo->vector.FromSpherical(Spherical(1, vectorTheta, vectorPhi));

				//gizmo->vector.FromSpherical(Spherical(1, gizmo->vector.ToSpherical().vertical, gizmo->vector.ToSpherical().horizontal));
			}
			else if(_event.key == Keyboard::N)
			{
				gizmo->Normalize();// ��������������� ���
			}
			else if(_event.key == Keyboard::P)
			{
				gizmo->ScalePlus();// ��������������� � ������� ����������
			}
			else if(_event.key == Keyboard::M)
			{
				gizmo->ScaleMinus();// ��������������� � ������� ����������
			}
			else if(_event.key == Keyboard::I)
			{
				gizmo->Inverse();// ������������� �����������
			}
			else if(_event.key == Keyboard::Q)
			{
				gizmo->RotateXPlus();
			}
			else if(_event.key == Keyboard::W)
			{
				gizmo->RotateXMinus();
			}	
			else if(_event.key == Keyboard::A)
			{
				gizmo->RotateYPlus();
			}
			else if(_event.key == Keyboard::S)
			{
				gizmo->RotateYMinus();
			}
			else if(_event.key == Keyboard::Z)
			{
				gizmo->RotateZPlus();
			}
			else if(_event.key == Keyboard::X)
			{
				gizmo->RotateZMinus();
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
	def.realTimeWorkerDef.windowDef.tittle = "Vector3D.example_01";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;
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
