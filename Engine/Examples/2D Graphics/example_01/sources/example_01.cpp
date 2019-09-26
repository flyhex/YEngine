

#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <glaux.h>// ��� �������

#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Output/Graphics/Shape/Mesh/GraphicsMesh/GraphicsMesh.h>

#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Core/Shape/Sphere3D/Sphere3D.h>
#include <Engine/Core/Shape/Circle/Circle.h>
#include <Engine/Core/Shape/Triangle/Triangle.h>
#include <Engine/Core/Shape/Quad/Quad.h>
#include <Engine/Core/Transformation/ModelView/ModelView.h>
#include <Engine/Core/Bitset/Bitset.h>


// ��������� ����������� ����������� � ���������� ������������
class GraphicsTriangle2D: public Triangle<Vector2D>, // �����������
	                      public GraphicsObject, // ����������� ������
						  public ModelView<Vector2D, float, Matrix3x3<Vector2D>>// ��������� �������������
{
	//DATA
	public: RGBA color;// ���� ������



	// METHODS
	// ����������� �� ���������
	public: GraphicsTriangle2D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
	}

	
	// ���������� ������ �� �����
	public: virtual void Draw(void)
	{
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(v1), Transformation().TransformVector(v2), color);
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(v2), Transformation().TransformVector(v3), color);
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(v3), Transformation().TransformVector(v1), color);
	}
};


// ��������� ����������� ����������� � ���������� ������������
class GraphicsTriangle3D: public Triangle<Vector3D>, // �����������
	                      public GraphicsObject, // ����������� ������
						  public ModelView<Vector3D, Quaternion, Matrix4x4>// ��������� �������������
{
	//DATA
	public: RGBA color;// ���� ������



	// METHODS
	// ����������� �� ���������
	public: GraphicsTriangle3D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
	}

	
	// ���������� ������ �� �����
	public: virtual void Draw(void)
	{
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(v1), Transformation().TransformVector(v2), color);
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(v2), Transformation().TransformVector(v3), color);
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(v3), Transformation().TransformVector(v1), color);
	}
};



// ��������� ����������� ���� � ���������� ������������
class GraphicsQuad2D: public Quad<Vector2D>, // ����
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
	}
};


// ��������� ����������� ���� � ���������� ������������
class GraphicsQuad3D: public Quad<Vector3D>, // ����
	                  public GraphicsObject, // ����������� ������
					  public ModelView<Vector3D, Quaternion, Matrix4x4>// ��������� �������������
{
	//DATA
	public: RGBA color;// ���� ������


	// METHODS
	// ����������� �� ���������
	public: GraphicsQuad3D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
	}

	
	// ��������� ������� �� �����
	public: virtual void Draw(void)
	{
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(Vertex1()), Transformation().TransformVector(Vertex2()), color);
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(Vertex2()), Transformation().TransformVector(Vertex3()), color);
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(Vertex3()), Transformation().TransformVector(Vertex4()), color);
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(Vertex4()), Transformation().TransformVector(Vertex1()), color);
	}
};



// ��������� ����������� ����� � ���������� ������������
class GraphicsLine2D: public GraphicsLine<Vector2D> // ����������� �����
{
	//DATA
	public: RGBA color;// ���� ������


	// METHODS
	// ����������� �� ���������
	public: GraphicsLine2D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
		SetColor(color);


		// ������ ������ �����
		int32 stipple = 0;

		for(int32 i = 0; i < sizeof(stipple) * 8; ++i)// ������� ��� ���� stipple
		{
			if(i % 2 == 0)// ������ ������ ��� ������ ����������
			{
				Bitset::_SetBit(stipple, i, true);
			}
		}

		GraphicsLine<Vector2D>::_SetStipple(stipple, 5);// ������� ������ �����
	}
			

	// ���������� ������ �� �����
	public: virtual void Draw(void)
	{
		GraphicsLine<Vector2D>::_SetStippleEnable(true);
		GraphicsLine<Vector2D>::Draw();
		GraphicsLine<Vector2D>::_SetStippleEnable(false);
	}
};

// ��������� ����������� ����� � ���������� ������������
class GraphicsLine3D: public GraphicsLine<Vector3D> // ����������� �����
{
	//DATA
	public: RGBA color;// ���� ������


	// METHODS
	// ����������� �� ���������
	public: GraphicsLine3D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
		SetColor(color);
	}
};



// ��������� ����������� ���� � ���������� ������������
class GraphicsCircle2D: public Circle<Vector2D>, // ����������
	                    public GraphicsObject, // ����������� ������
					    public ModelView<Vector2D, float, Matrix3x3<Vector2D>>// ��������� �������������
{
	//DATA
	public: RGBA color;// ���� ������
	private: vector<Vector2D> vertices;// ������ ������


	// METHODS
	// ����������� �� ���������
	public: GraphicsCircle2D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
	}

	
	// ������ ������������� ����������, ��������� �� _segments ������
	// ����������: ��� ������ �������� _segments, ��� ����� ������� (������������ � ����� ����������) ����� ������������� ����������
	// ����������: ���� _segments ������ 3, �� ������� ����� ��������
	public: void BuildCircle(int32 _segments)
	{
		Circle<Vector2D>::BuildCircle(_segments, vertices);
	}

	// ���������� ������ �� �����
	public: virtual void Draw(void)
	{
		for(int i = 0; i < vertices.size() - 1; ++i)
		{
			GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(vertices[i]), Transformation().TransformVector(vertices[i + 1]), color);
		}

		// ������ ���������� �����
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(vertices[vertices.size() - 1]), Transformation().TransformVector(vertices[0]), color);
	}
};



// ��������� ����������� ���� � ���������� ������������
class GraphicsCircle3D: public Circle<Vector3D>, // ����������
	                    public GraphicsObject, // ����������� ������
					    public ModelView<Vector3D, Quaternion, Matrix4x4>// ��������� �������������
{
	//DATA
	public: RGBA color;// ���� ������
	private: vector<Vector3D> vertices;// ������ ������


	// METHODS
	// ����������� �� ���������
	public: GraphicsCircle3D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
	}

	// ������ ������������� ����������, ��������� �� _segments ������
	// ����������: ��� ������ �������� _segments, ��� ����� ������� (������������ � ����� ����������) ����� ������������� ����������
	// ����������: ���� _segments ������ 3, �� ������� ����� ��������
	public: void BuildCircle(int32 _segments)
	{
		Circle<Vector3D>::BuildCircle(_segments, vertices);
	}

	// ���������� ������ �� �����
	public: virtual void Draw(void)
	{
		for(int i = 0; i < vertices.size() - 1; ++i)
		{
			GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(vertices[i]), Transformation().TransformVector(vertices[i + 1]), color);
		}

		// ������ ���������� �����
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(vertices[vertices.size() - 1]), Transformation().TransformVector(vertices[0]), color);
	}
};


// ��������� ����������� ������������� ����� � ���������� ������������
class GraphicsMesh2D: public GraphicsMesh<Vector2D, float, Matrix4x4>// ����������� ���; �� ���������� Matrix4x4 � �� Matrix3x3<Vector2D>
{
	//DATA
	public: RGBA color;// ���� ������


	// METHODS
	// ����������� �� ���������
	public: GraphicsMesh2D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
		SetWireframeEnable(true);
	}
		
	// ���������� ������������� ����� �� ����� 
	public: virtual void Draw(void)
	{
		BeginTransform();
		GraphicsMesh::Draw();
		EndTransform();
	}
};



// ��������� ����������� ������������� ����� � ���������� ������������
class GraphicsMesh3D: public GraphicsMesh<Vector3D, Quaternion, Matrix4x4>// ����������� ���
{
	//DATA
	public: RGBA color;// ���� ������


	// METHODS
	// ����������� �� ���������
	public: GraphicsMesh3D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
		SetWireframeEnable(true);
	}
				
			
	// ���������� ������������� ����� �� ����� 
	public: virtual void Draw(void)
	{
		BeginTransform();
		GraphicsMesh::Draw();
		EndTransform();
	}
};



// ��������� ����������� ����� � ���������� ������������
class GraphicsPoint2D: public GraphicsPoint<Vector2D>// ����������� ����� 
{
	//DATA
	public: RGBA color;// ���� ������


	// METHODS
	// ����������� �� ���������
	public: GraphicsPoint2D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
		SetColor(color);
	}
};


// ��������� ����������� ����� � ���������� ������������
class GraphicsPoint3D: public GraphicsPoint<Vector3D>// ����������� 
{
	//DATA
	public: RGBA color;// ���� ������


	// METHODS
	// ����������� �� ���������
	public: GraphicsPoint3D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
		SetColor(color);
	}
};

// �����, ����������� ���������������� ����
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: Scene* scene;// ����������� �����
	private: StringANSI currentProjectionType;// ��� �������� ���� ��������



    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL)
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

				
			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.textColor = RGBA(RGBA::WHITE_COLOR);
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d);
				font2d->Pick();
			}
			
			// ���������� �����������...
			{
				GraphicsTriangle2D* triangle2D = new GraphicsTriangle2D();

				// ��������� ������� ������ ������� �������, � ���������� ���������� ������� ���������
				triangle2D->v1 = Vector2D(0, 20);
				triangle2D->v2 = Vector2D(20, -20);
				triangle2D->v3 = Vector2D(-20, -20);
				
				// �������������
				triangle2D->SetPosition(Vector2D(250, 250)); 
				triangle2D->SetScale(Vector2D(2, 2));
				triangle2D->SetRotation(Angle::HALF_PI);


				// �������������� ������ �������������
				//triangle2D->Transform(Matrix3x3<Vector2D>(Vector2D(250, 250), Vector2D(2, 2), Angle::HALF_PI));


				triangle2D->Rename("triangle2D");
				scene->AddObject(triangle2D);// �������� � �����
				scene->AddGraphicsObject(triangle2D);
			}

			// ���������� �����������...
			{
				GraphicsTriangle3D* triangle3D = new GraphicsTriangle3D();

				// ��������� ������� ������ ������� �������, � ���������� ���������� ������� ���������
				triangle3D->v1 = Vector3D(0, 20, 0);
				triangle3D->v2 = Vector3D(20, -20, 0);
				triangle3D->v3 = Vector3D(-20, -20, 0);

				// �������������
				triangle3D->SetPosition(Vector3D(100, 100, -10)); 
				triangle3D->SetScale(Vector3D(3, 3, 0));
				triangle3D->SetRotation(Quaternion(Vector3D::UNIT_Z, Angle::HALF_PI));
				
				// �������������� ������ �������������
				//triangle3D->Transform(Matrix4x4(Vector3D(100, 100, -10), Vector3D(3, 3, 0), Quaternion(Vector3D::UNIT_Z, Angle::HALF_PI)));


				triangle3D->Rename("triangle3D");
				scene->AddObject(triangle3D);// �������� � �����
				scene->AddGraphicsObject(triangle3D);
			}

		

			// ���������� �������������...
			{ 
				GraphicsQuad2D* quad2D = new GraphicsQuad2D();

				// ��������� ������� ������ ������� �������, � ���������� ���������� ������� ���������
				quad2D->Set(Vector2D(-20, 20), Vector2D(20, 20), Vector2D(20, -20), Vector2D(-20, -20));
				
				// �������������
				quad2D->SetPosition(Vector2D(70, 350)); 
				quad2D->SetRotation(Angle::HALF_PI/2);
				quad2D->SetScale(Vector2D(1, 1));
				
				quad2D->Rename("quad2D");
				scene->AddObject(quad2D);// �������� � �����
				scene->AddGraphicsObject(quad2D);
			}
				 
			// ���������� �������������...
			{
				GraphicsQuad3D* quad3D = new GraphicsQuad3D();

				// ��������� ������� ������ ������� �������, � ���������� ���������� ������� ���������
				quad3D->Set(Vector3D(-20, 20, 0), Vector3D(20, 20, 0), Vector3D(20, -20, 0), Vector3D(-20, -20, 0));
				
				// �������������
				quad3D->SetPosition(Vector3D(70, 550, -10)); 
				quad3D->SetScale(Vector3D(1.5, 1.5, 0));
				//quad3D->SetRotation(Quaternion(Vector3D::UNIT_Z, Angle::HALF_PI/2));
				
				quad3D->Rename("quad3D");	
				scene->AddObject(quad3D);// �������� � �����
				scene->AddGraphicsObject(quad3D);
			} 


			// ���������� �����...
			{
				GraphicsLine2D* line2D = new GraphicsLine2D();
				line2D->Set(Vector2D(-40+370, 20+450), Vector2D(40+370+100, 20+450+40+50));
				

				line2D->Rename("line2D");
				scene->AddObject(line2D);// �������� � �����
				scene->AddGraphicsObject(line2D);
			}

			// ���������� �����...
			{
				GraphicsLine3D* line3D = new GraphicsLine3D();
				line3D->Set(Vector3D(-20+470, 20+150, -10), Vector3D(20+470, 20+150, -10));
				

				line3D->Rename("line3D");
				scene->AddObject(line3D);// �������� � �����
				scene->AddGraphicsObject(line3D);
			}

			// ���������� ����...
			{
				GraphicsCircle2D* circle2D = new GraphicsCircle2D();
				circle2D->Set(Vector2D(0, 0), 40.0f);
				circle2D->BuildCircle(32); 
				
				// �������������
				circle2D->SetPosition(Vector2D(170, 350)); 
				circle2D->SetScale(Vector2D(1, 2));// ������ ������ 
				
				// �������������� ������ �������������
				//circle2D->Transform(Matrix4x4(Vector2D(170, 350), Vector2D(1, 2), Quaternion()));


				circle2D->Rename("circle2D");
				scene->AddObject(circle2D);// �������� � �����
				scene->AddGraphicsObject(circle2D);
			} 

			// ���������� ����...
			{
				GraphicsCircle3D* circle3D = new GraphicsCircle3D();
				circle3D->Set(Vector3D(0, 0, 0), 40.0f);
				circle3D->BuildCircle(22);
				
				// �������������
				circle3D->SetPosition(Vector3D(290, 150, -1)); 
				circle3D->SetScale(Vector3D(1, 1, 1));
				
				circle3D->Rename("circle3D");
				scene->AddObject(circle3D);// �������� � �����
				scene->AddGraphicsObject(circle3D);
			}

			// ���������� �����...
			{  
				GraphicsMesh2D* mesh2D = new GraphicsMesh2D();
				SimpleList<Vector2D> poly;// ��������� ������, ����������� ��������

				// ����
				// ��������� ������� ������ ������� �������, � ���������� ���������� ������� ���������
				poly.AddToTail(Vector2D(20, 0));
				poly.AddToTail(Vector2D(0, -20));
				poly.AddToTail(Vector2D(-20, 0));

				poly.AddToTail(Vector2D(-20, 0));
				poly.AddToTail(Vector2D(0, 20));
				poly.AddToTail(Vector2D(20, 0));

				mesh2D->CreateShape(poly);// ������� ����� �� �������������, �������� �������� poly
				
			
				//mesh2D->AutoResizeColors();
				mesh2D->SetColor(mesh2D->color);


				// �������������
				mesh2D->SetPosition(Vector2D(380, 250)); 
				mesh2D->SetScale(Vector2D(2, 2));
				mesh2D->SetRotation(Angle::HALF_PI);
				


				mesh2D->Rename("mesh2D.1");
				scene->AddObject(mesh2D);// �������� � �����
				scene->AddGraphicsObject(mesh2D);
			}


			// ���������� �����...
			{
				GraphicsMesh2D* mesh2D = new GraphicsMesh2D();

				mesh2D->CreateCircle2D(Circle<Vector2D>(Vector2D(), 50), 30);// ������� ����������

				//mesh2D->AutoResizeColors();
				mesh2D->SetColor(mesh2D->color);


				// �������������
				mesh2D->SetPosition(Vector2D(580, 450)); 
				mesh2D->SetScale(Vector2D(1, 1));// ������ ������
				mesh2D->SetRotation(Angle::HALF_PI);
				


				mesh2D->Rename("mesh2D.2");
				scene->AddObject(mesh2D);// �������� � �����
				scene->AddGraphicsObject(mesh2D);
			}

			// ���������� �����...
			{
				GraphicsMesh2D* mesh2D = new GraphicsMesh2D();


				mesh2D->CreateQuad2D(ScreenQuad(Vector2D(), Size2D(40, 40)));


				//mesh2D->AutoResizeColors();
				mesh2D->SetColor(mesh2D->color);


				// �������������
				mesh2D->SetPosition(Vector2D(180, 490)); 
				mesh2D->SetScale(Vector2D(1, 1));// ������ ������
				mesh2D->SetRotation(Angle::HALF_PI);
				


				mesh2D->Rename("mesh2D.3");
				scene->AddObject(mesh2D);// �������� � �����
				scene->AddGraphicsObject(mesh2D);
			}

			// ���������� �����...
			{
				GraphicsMesh3D* mesh3D = new GraphicsMesh3D();
				
				// ��������� ������� ����� ������ ������� �������, � ���������� ���������� ������� ���������
				mesh3D->CreateQuad2D(Quad<Vector3D>(Vector3D(), Size3D(40,40,20)));// ������� ����
				mesh3D->TotalPolygonsDivide4();// ����������� ���������� ��������� � 4 ����

				//mesh3D->AutoResizeColors();
				mesh3D->SetColor(mesh3D->color);


				// �������������
				mesh3D->SetPosition(Vector3D(480, 350, -1)); 
				mesh3D->SetScale(Vector3D(2, 2, 0));// ������ ������
				mesh3D->SetRotation(Quaternion(Vector3D::UNIT_Z, Angle::HALF_PI));
				

				mesh3D->Rename("mesh3D.1");
				scene->AddObject(mesh3D);// �������� � �����
				scene->AddGraphicsObject(mesh3D);
			}
			
					
			// ���������� �����...
			{
				GraphicsMesh3D* mesh3D = new GraphicsMesh3D();
				
				// ��������� ������� ����� ������ ������� �������, � ���������� ���������� ������� ���������
				mesh3D->CreateCircle2D(Circle<Vector3D>(Vector3D(), 50), 30);// ������� ����������
				mesh3D->TotalPolygonsDivide4();// ����������� ���������� ��������� � 4 ����

				//mesh3D->AutoResizeColors();
				mesh3D->SetColor(mesh3D->color);


				// �������������
				mesh3D->SetPosition(Vector3D(570, 430, -1)); 
				

				mesh3D->Rename("mesh3D.2");
				scene->AddObject(mesh3D);// �������� � �����
				scene->AddGraphicsObject(mesh3D);
			}

			// ���������� �����...
			{
				GraphicsPoint2D* point2D = new GraphicsPoint2D();
	

				// �������������
				point2D->SetPosition(Vector2D(140, 440)); 


				point2D->Rename("point2D");
				scene->AddObject(point2D);// �������� � �����
				scene->AddGraphicsObject(point2D);
			}


			// ���������� �����...
			{
				GraphicsPoint3D* point3D = new GraphicsPoint3D();
	

				// �������������
				point3D->SetPosition(Vector3D(Renderer::_Picked()->ScreenToClient(Renderer::_Picked()->ClientAreaCenter()).x, Renderer::_Picked()->ScreenToClient(Renderer::_Picked()->ClientAreaCenter()).y, -1)); 


				point3D->Rename("point3D");
				scene->AddObject(point3D);// �������� � �����
				scene->AddGraphicsObject(point3D);
			}

			// ������...
			{
				Camera3D::Define def;
				def.name = "camera";
		
				def.projectionType = Camera3D::ORTHOGRAPHIC_PROJECTION;
				def.rotationEnabled = false;
				camera = new Camera3D();
				camera->Create(def);
				camera->Pick(Title()); 
				scene->AddObject(camera);
				
				
				currentProjectionType = "ORTHOGRAPHIC";
			}

			GraphicsPoint<Vector3D>::_SetPointSize(6);// ������ ����� 
			GraphicsLine<Vector3D>::_SetLineWidth(3);// ������� �����

			//Mouse::_ShowCursor(false);// ������ ������
			MoveToCenter();// ����������� ���� � ����� ������
			Mouse::_MoveCursorToCenter(this);// ����������� ������ � ����� ���������� ������� ����

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

	    // ����� ������...
		camera->BeginScreen2D();
		

		scene->Draw();// ����������� �����

		//camera->End2D();// ������ ����� ���������� �������	
 

		// ����� ������...
		//camera->BeginScreen2D();
		

		//int i = 0;
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "vertices: %d", 0 % 2);
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "indices: %d", 1 % 2);
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "indices: %d", 2 % 2);
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "indices: %d", 3 % 2);
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "indices: %d", 4 % 2);
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "indices: %d", 5 % 2);
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "indices: %s", Bitset::_String(stipple).c_str());
	    

		//camera->End2D();// ������ ����� ���������� �������	
	}

			


	// ������ ������� ����
	public: virtual bool MouseEventFilter(Mouse::Event _event)
	{
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
			else if(_event.key == Keyboard::C)
			{
				SetFrontFaceClockwise(!FrontFaceClockwise());
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
	def.realTimeWorkerDef.windowDef.tittle = "2D Graphics.example_01";
	def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	
	//def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;


	//def.faceDrawMode[Renderer::BACK] = Renderer::POINT;
	//def.faceCullMode[Renderer::FRONT] = false;
	//def.faceCullMode[Renderer::BACK] = false;
	
	//def.frontFaceClockwise = true;
	
	//def.realTimeWorkerDef.windowDef.eventBuffering = true;


	Application* app = new Application;// ������� ����
	
	if(app->Application::Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
