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
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Core/ScreenQuad/ScreenQuad.h>



// ��������� ����������� ���� � ���������� ������������
class GraphicsQuad2D: public ScreenQuad, // ����
	                  public GraphicsObject, // ����������� ������
					  public ModelView<Vector2D, float, Matrix3x3<Vector2D>>// ��������� �������������
{
	//DATA
	public: RGBA color;// ���� ������


	// METHODS
	// ����������� �� ���������
	public: GraphicsQuad2D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
	}

	
	// ���������� ������ �� �����
	public: virtual void Draw(void)
	{
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(Vertex1()), Transformation().TransformVector(Vertex2()), color);
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(Vertex2()), Transformation().TransformVector(Vertex3()), color);
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(Vertex3()), Transformation().TransformVector(Vertex4()), color);
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(Vertex4()), Transformation().TransformVector(Vertex1()), color);

		GraphicsPoint<Vector2D>::_Draw(Transformation().TransformVector(Vertex1()), RGBA::RED_COLOR);
		GraphicsPoint<Vector2D>::_Draw(Transformation().TransformVector(Vertex2()), RGBA::GREEN_COLOR);
		GraphicsPoint<Vector2D>::_Draw(Transformation().TransformVector(Vertex3()), RGBA::BLUE_COLOR);
		GraphicsPoint<Vector2D>::_Draw(Transformation().TransformVector(Vertex4()), RGBA::YELLOW_COLOR);
	}
};

// �����, ����������� ���������������� ����
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: GraphicsScene* scene;// ����������� �����

    private: GraphicsQuad2D* quad2D;
	private: int32 curVertex;// ������ ������� ���������� �������

	//private: GraphicsQuad2D* quad3D;



    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), quad2D(NIL), curVertex(0)
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
				
				scene = new GraphicsScene();
			}


			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;
				def.textColor = RGBA::WHITE_COLOR;

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d);
				font2d->Pick();
			}


	
			
			// ���������� �������������...
			{
				quad2D = new GraphicsQuad2D();
				//quad2D->Set(Vector3D(290, 240, 0), Vector3D(390, 240, 0), Vector3D(390, 340, 0), Vector3D(290, 340, 0));
				
				// ������ ������������� � �������� �����������
				quad2D->Set(Vector2D(400, 200), Size2D(100, 100));

				
				//quad2D->Set(Vector2D(350, 350), Vector2D(450, 350), Vector2D(450, 250), Vector2D(350, 250));
				


				// �������������
				//quad2D->SetPosition(Vector2D(400, 350)); 
				//quad2D->SetRotation(Angle::HALF_PI/2);
				//quad2D->SetScale(Vector2D(1, 1));
				
				quad2D->Rename("quad2D");
				scene->AddObject(quad2D, true, true);// �������� � �����
			}
			

			// ������...
			{
				Camera3D::Define def;
				def.name = "camera";
				//def.rotationType = Camera3D::TRACKBALL_FIRST_PERSON; 
				//def.movementType = Camera3D::OVERLAND;
				//def.rotationType = Camera3D::TRACKBALL_FIRST_PERSON; 
				//def.projectionType = Camera3D::ORTHOGRAPHIC;
				//def.viewingPoint = quad->GetPosition();
				//def.zoomMin = 2;
				//def.zoomMax = 10;
				def.rotationEnabled = false;
	
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// ����� �������
				def.viewingPoint = Vector3D(quad2D->GetPosition().x, quad2D->GetPosition().y, 0);
				def.zoomMin = 2;
				def.zoomMax = 10;

				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera);
				
				camera->Pick("ScreenQuad.example_01"); 
				camera->SetPosition(Vector3D(0,0,2));
				
				// ������ �������������� ������ ������
				//camera->UpdateTrackballObjectViewer(-Angle::HALF_PI/3.0f, Angle::HALF_PI/3.0f, 0.0f);
			}


			SetBackgroundColor(RGBA::GRAY_COLOR);// ���� ����
		
			GraphicsPoint<Vector3D>::_SetPointSize(6);// ������ ����� 
			GraphicsLine<Vector3D>::_SetLineWidth(3);// ������� �����

			MoveToCenter();// ����������� ���� � ����� �����
			Mouse::_MoveCursorToClientAreaCenter(this);// ����������� ������ � ����� ���������� ������� ����	
			//Mouse::_ShowCursor(false);// ������ ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}




		


	// ������� �����
	public: virtual void Update(float _delta)
	{



		// ������� ����� �� �������� � �������
		//quad->SetPosition(quad->GetPosition() + Vector3D(0,0,0.01));
	}

	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����


	
		// ����� ������...
		camera->BeginScreen2D();// ������ ����� ���������� �������
 
		//camera->BeginOrtho2D();
 
		scene->Draw();
	
		//camera->End2D();// ������ ����� ���������� �������

		int32 i = 0;
		
		camera->BeginScreen2D();// ������ ����� ���������� �������

		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.center: %.2f/%.2f", quad2D->Center().x, quad2D->Center().y); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.size(width/height): %.2f/%.2f", quad2D->Size().width,  quad2D->Size().height); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "cursor.client position: %.2f/%.2f", Mouse()._CursorClientAreaPosition(this).x, Mouse()._CursorClientAreaPosition(this).y); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.is point inside: %d", quad2D->IsPointInside(Vector2D(Mouse::_CursorClientAreaPosition(this).x, Mouse::_CursorClientAreaPosition(this).y))); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.leftBottom: %.2f/%.2f, rightTop: %.2f/%.2f", quad2D->leftBottom.x, quad2D->leftBottom.y, quad2D->rightTop.x, quad2D->rightTop.y); 

		
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "cursor.is over %d", Mouse::_IsCursorOver(this, true)); 


		
		//if(quad1->GetTexture())
		{
			//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad1.texture.IsExist: %d", quad1->GetTexture()->IsExist()); 

			//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad1.texture.SizeOfImage: %.2f/%.2f", quad1->GetTexture()->ImageSize().width, quad1->GetTexture()->ImageSize().height); 
			//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad1.texture.SizeOfTexture: %.2f/%.2f", quad1->GetTexture()->TextureSize().width, quad1->GetTexture()->TextureSize().height); 
		}

	

		
	
		//int32 val;
		

		//uint64 uu = 0;
		//Bitset::_SetBit(uu, 0, 1);
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.texture.sizeOfTexture: %.2f/%.2f", quad->Texture()->SizeOfTexture().width, quad->Texture()->SizeOfTexture().height); 
		
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "uu: %d", glGetTextureLevelParameteriv(GL_TEXTURE_2D, , 0, GL_TEXTURE_WIDTH)); 
		//Bitset::_SetBit(uu, 1, 1);
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "uu: %s", Bitset::_String(uu).c_str()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "uu: %d", uu); 

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vertices: %d", mesh->AmountOfVertices()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "indices: %d", mesh->AmountOfIndices()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "normals: %d", mesh->AmountOfNormals()); 

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sizeinByte: %d", mesh->SizeInByte()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sizeInKbyte: %.5f", MemoryCounter::_FromByteToKB(mesh->SizeInByte())); 
 


		/*
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "shape detail: %d", mesh->DetailLevel()); 
	    font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "shape name: %s", mesh->ShapeNameString().c_str()); 


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "colored material.type: %s", mesh->ColoredMaterialTypeString().c_str());

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera position: %.2f/%.2f/%.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z); 


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "shape attributes: %s", mesh->StringAttributes().c_str()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "shape vertices: %d", mesh->AmountOfVertices()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "shape polygons: %d", mesh->AmountOfPolygons()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "shape vertex colors: %d", mesh->AmountOfVertexColors()); 

		*/ 


		// ��������������� �����...
		camera->BeginPerspective3D();// ������ ����� ���������� �������
	}


	// ������ ������� ����
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		if(_event.type == Mouse::KEY_DOWN)
		{
			if(quad2D->IsPointInside(Vector2D(Mouse::_CursorClientAreaPosition(this).x, Mouse::_CursorClientAreaPosition(this).y)))
			{ 
				quad2D->color = Color<RGBA>::_Random(10, 200);
			}
		}

		Mouse().ClearEventList();// �������� ����� �������
	}


	// ��������� ������� ����������
	public: virtual void KeyboardEventFilter(Keyboard::Event _event)
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
			else if(_event.key == Keyboard::C)
			{
				Vector2D pos = quad2D->ClampPoint(Vector2D(Mouse::_CursorClientAreaPosition(this).x, Mouse::_CursorClientAreaPosition(this).y));

				Mouse::_SetCursorClientAreaPosition(this, pos);
			}
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				quad2D->SetCenter(quad2D->Center() + Vector2D(-3, 0));
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				quad2D->SetCenter(quad2D->Center() + Vector2D(3, 0));
			}
			else if(_event.key == Keyboard::UP_ARROW)
			{
				quad2D->SetCenter(quad2D->Center() + Vector2D(0, -3));
			}
			else if(_event.key == Keyboard::DOWN_ARROW)
			{
				quad2D->SetCenter(quad2D->Center() + Vector2D(0, 3));
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
	def.realTimeWorkerDef.windowDef.tittle = "ScreenQuad.example_01";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;
	//def.faceDrawMode[Renderer::BACK] = Renderer::LINE;
	//def.faceCullMode[Renderer::FRONT] = false;
	def.faceCullMode[Renderer::BACK] = false;
	def.zBufferEnabled = false;
	def.blendEnable = true;// �������� ����������
	
	//def.frontFaceClockwise = true;
	//def.realTimeWorkerDef.windowDef.eventBuffering = true;
	//def.vSync = false;
	//def.drawCallController.SetMode(CallController::UNLIM);

	
	Application* app = new Application;// ������� ����
	
	if(app->Application::Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
