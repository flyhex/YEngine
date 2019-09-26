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
#include <Engine/Core/InputHandler/InputHandler.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Core/Interpolation/QuaternionLerp/QuaternionLerp.h>



// �����, ����������� ����� (������)
class Gizmo: public GraphicsObject// ����������� ������
{
	// ����� ��������
	public: enum RotationMode
	{
		EULER_SCROLLBAR, // ������� ���������� � ������� ����� ������
		SPHERICAL_SWING_TWIST,// ����������� ����������; ����� ��������� �� ����� �������� ���� 
		SPHERICAL_ST_TWO_POLE,// ����������� ����������; ����� ��������� �� ����� �������� ����, �� ���������� ������ �� ��� Z
		SPHERICAL_FROM_TO//����������� ����������; ����� ������������� �� ������ ����� � ����� �� ����������� �����
	};


	// DATA
	private: Vector3 xAxis;// ����������� ��� X
	private: Vector3 yAxis;// ����������� ��� Y
	private: Vector3 zAxis;// ����������� ��� Z
	private: RGBA xColor;// ���� ��� X
	private: RGBA yColor;// ���� ��� Y
    private: RGBA zColor;// ���� ��� Z
	private: ColoredMesh *xAxisArrow, *yAxisArrow, *zAxisArrow;// ��������� (�����������) ���� X/Y/Z

    private: int32 segments;// ���������� ���������, �� ������� ����� �������� ������
	private: Vector3 point1, point2;// ������� �����
	private: float scale;// ������� ��� �����, ����� �� ������ ��� ������ ������� �����, � ��� �� ������������� ��
	private: float step;// ���� ����� ������� � ������ ����� � ��������
	private: Quaternion qX;// ������� ������� �� ���� ��� (step) ������ X
	private: Quaternion qY;// ������� ������� �� ���� ��� (step) ������ ��� Y
	private: Quaternion qZ;// ������� ������� �� ���� ��� (step) ������ ��� Z

	private: Vector3 xAxisBegin, xAxisEnd, yAxisBegin, yAxisEnd, zAxisBegin, zAxisEnd;// ������ � ����� ���� X/Y/Z
	private: Font2D* font;// ����� ��� ����������� �������� ����
	private: Quaternion quaternion;// ���������� (����������), ��� ��� ����� � ������� �����
	private: RotationMode rotationMode;// ������� ����� ��������


	// METHODS
	// ����������� ��� ����������
	public: Gizmo(void): segments(360), scale(1.51f), xAxisArrow(NIL), yAxisArrow(NIL), zAxisArrow(NIL), font(NIL)
	{
		rotationMode = EULER_SCROLLBAR;

		step = Angle::DOUBLE_PI / (float)segments;
		qX = Quaternion(Matrix4x4::_RotationX(step));
		qY = Quaternion(Matrix4x4::_RotationY(step));
		qZ = Quaternion(Matrix4x4::_RotationZ(step));

		xAxis = Vector3::UNIT_X;
		yAxis = Vector3::UNIT_Y;
		zAxis = Vector3::UNIT_Z;

		xColor = RGBA(163, 21, 21, 255);
		yColor = RGBA(118, 185, 0, 255);
		zColor = RGBA(55, 111, 194, 255);


		// �����...
		Font2D::Define def;
		def.fontName = "Arial";// ��� ������
		def.fontWeight = Font2D::WEIGHT_BOLD;// ������ �����
		def.fontSize = 18;// ������ ������
		font = new Font2D();
		font->Create(def);


		// ��� X...
	    xAxisBegin = -xAxis * (scale * 1.5f);
		xAxisEnd = xAxis * (scale * 1.5f);
	
		xAxisArrow = new ColoredMesh();
		xAxisArrow->CreatePyramid3D(0.04f);
		xAxisArrow->SetScale(Vector3(0.5f, 1.0f, 0.5f));// �������� �� ��� Y
		xAxisArrow->SetColor(xColor + RGBA(1, 0, 0, 0));
		xAxisArrow->SetPosition(xAxisEnd);
		xAxisArrow->SetRotation(Matrix4x4::_RotationZ(-Angle::HALF_PI));// ��������� �� -90 �������� ������ ��� Z


		// ��� Y...
	    yAxisBegin = -yAxis * (scale * 1.5f);
		yAxisEnd = yAxis * (scale * 1.5f);
	
		yAxisArrow = new ColoredMesh();
		yAxisArrow->CreatePyramid3D(0.04f);
		yAxisArrow->SetScale(Vector3(0.5f, 1.0f, 0.5f));// �������� �� ��� Y
		yAxisArrow->SetColor(yColor + RGBA(0, 1, 0, 0));
		yAxisArrow->SetPosition(yAxisEnd);
		

		// ��� Z...
	    zAxisBegin = -zAxis * (scale * 1.5f);
		zAxisEnd = zAxis * (scale * 1.5f);
	
		zAxisArrow = new ColoredMesh();
		zAxisArrow->CreatePyramid3D(0.04f);
		zAxisArrow->SetScale(Vector3(0.5f, 1.0f, 0.5f));// �������� �� ��� Y
		zAxisArrow->SetColor(zColor + RGBA(0, 0, 1, 0));
		zAxisArrow->SetPosition(zAxisEnd);
		zAxisArrow->SetRotation(Matrix4x4::_RotationX(Angle::HALF_PI));// ��������� �� 90 �������� ������ ��� X
	}


	// ����������
	public: virtual ~Gizmo(void)
	{
		delete font; 

		delete xAxisArrow;
		delete yAxisArrow;
		delete zAxisArrow;
	}


	// ���������� ���� ��� �������� X
	public: RGBA GetXColor(void)const
	{
		return xColor;
	}


	// ���������� ���� ��� �������� Y
	public: RGBA GetYColor(void)const
	{
		return yColor;
	}


	// ���������� ���� ��� �������� Z
	public: RGBA GetZColor(void)const
	{
		return zColor;
	}


	// ������ ����� ����� ��������
	public: void SetRotationMode(RotationMode _mode)
	{
		rotationMode = _mode;
	}


	// ���������� ������� ����� ��������
	public: RotationMode GetRotationMode(void)const
	{
		return rotationMode;
	}


	// ���������� ������� ����� ��������
	public: StringANSI GetRotationModeString(void)const
	{
		switch(rotationMode)
		{
			case EULER_SCROLLBAR:
			{
				return "EULER_SCROLLBAR";
			}
		
			case SPHERICAL_SWING_TWIST:
			{
				return "SPHERICAL_SWING_TWIST";
			}

			case SPHERICAL_FROM_TO:
			{
				return "SPHERICAL_FROM_TO";
			}

			case SPHERICAL_ST_TWO_POLE:
			{
				return "SPHERICAL_ST_TWO_POLE";
			}
		}
		return "";
	}


	// ���������� ���������� (����������), ��� ��� ����� � ������� �����
	public: Quaternion GetQuaternion(void)const
	{ 
		return quaternion;
	}



	// ������������...
	// ���������� ��� X
	private: void DrawXAxis(void)
	{
		GraphicsLine::_Draw(xAxisBegin, xAxisEnd, xColor + RGBA(1, 0, 0, 0));
					
		xAxisArrow->Draw();

		font->SetColor(xColor + RGBA(1, 0, 0, 0));//������ ���� ������
		font->DrawWorldText(xAxisEnd + Vector3(0.0f, +0.10f, 0.0f), "X");// ���������� ��� ���
	}


	// ���������� ��� Y
	private: void DrawYAxis(void)
	{
		GraphicsLine::_Draw(yAxisBegin, yAxisEnd, yColor + RGBA(0, 1, 0, 0));
					
		yAxisArrow->Draw();

		font->SetColor(yColor + RGBA(0, 1, 0, 0));//������ ���� ������
		font->DrawWorldText(yAxisEnd + Vector3(0.0f, +0.10f, 0.0f), "Y");// ���������� ��� ���
	}


	// ���������� ��� Z
	private: void DrawZAxis(void)
	{
		GraphicsLine::_Draw(zAxisBegin, zAxisEnd, zColor + RGBA(0, 0, 1, 0));
		
		zAxisArrow->Draw();

		font->SetColor(zColor + RGBA(0, 0, 1, 0));//������ ���� ������
		font->DrawWorldText(zAxisEnd + Vector3(0.0f, +0.10f, 0.0f), "Z");// ���������� ��� ���
	}


	// ���������� �����
	public: virtual void Draw(void)
	{
		if(IsShowEnabled())
		{
			BeginTransform();

			//if(rotationMode == EULER_SCROLLBAR)
			{
				GraphicsLine::_SetLineWidth(3.0f);// ������� �����
					
				// ���������� ������ �������� ������ ��� ��� X...
				point1 = yAxis * scale;// �������� ��������� ������ � ������ ��� Y
				for(int32 i = 0; i < segments; i++)
				{
					point2 = qX * point1;// ������������ ���������� ������ �� qX
					GraphicsLine::_Draw(point1, point2, xColor + RGBA(1, 0, 0, 0));
					point1 = point2;
				}

				DrawXAxis();// ���������� ��� X


				// ���������� ������ �������� ������ ��� ��� Y...
				point1 = xAxis * scale;// �������� ��������� ������ � ������ ��� X
				for(int32 i = 0; i < segments; i++)
				{
					point2 = qY * point1;// ������������ ���������� ������ �� qY
					GraphicsLine::_Draw(point1, point2, yColor + RGBA(0, 1, 0, 0));
					point1 = point2;
				}

				DrawYAxis();// ���������� ��� Y
				

				// ���������� ������ �������� ������ ��� ��� Z...
				point1 = xAxis * scale;// �������� ��������� ������ � ������ ��� X
				for(int32 i = 0; i < segments; i++)
				{
					point2 = qZ * point1;// ������������ ���������� ������ �� qZ
					GraphicsLine::_Draw(point1, point2, zColor + RGBA(0, 0, 1, 0));
					point1 = point2;
				}

				DrawZAxis();// ���������� ��� Z
			
				GraphicsLine::_SetLineWidth(6.0f);// ������� �����
			}
			//else if(rotationMode == SPHERICAL*) 
			{}

			EndTransform();
		}
	}
};



// �����, ����������� ����������� �����
class GraphicsSphere: public ColoredMesh,// ����������� ������������� �����
	                  public LogicObject// ���������� ������
{
	// �������������� ���������� ��� ��������
	public: enum
	{
		NO_SELECTED,
		CIRCLE_X,
		CIRCLE_Y,
		CIRCLE_Z
	};

	// DATA
	private: Vector3 xAxis;// ����������� ��� X
	private: Vector3 yAxis;// ����������� ��� Y
	private: Vector3 zAxis;// ����������� ��� Z
	private: RGBA xColor;// ���� ��� X
	private: RGBA yColor;// ���� ��� Y
    private: RGBA zColor;// ���� ��� Z
	private: int32 segments;// ���������� ���������, �� ������� ����� �������� ������
	private: Vector3 point1, point2;// ������� �����
	private: float scale;// ������� ��� �����, ����� �� ������ ��� ������ ������� �����, � ��� �� ������������� ��
	private: float step;// ���� ����� ������� � ������ ����� � ��������
	private: Quaternion qX;// ������� ������� �� ���� ��� (step) ������ X
	private: Quaternion qY;// ������� ������� �� ���� ��� (step) ������ ��� Y
	private: Quaternion qZ;// ������� ������� �� ���� ��� (step) ������ ��� Z
	private: ColoredMesh* face;// ���������� ������� �� ���������
	private: RGBA faceColor;// ���� ������� ������� ����������
	private: int32 selected;// ������������� ���������� (�������) ��� ��������
	private: SimpleList<Vector3> pathList;// ������ ������, ����������� ���������� ������������
	private: ColoredMesh* mesh;// ������� ����, ������� ������������ ��� ������� ��������� � ������� ������� ������������
	private: Vector3 startLerp, targetLerp;// ��������� � ������� ���������� ��� ������������
	private: bool startLerpValid, targetLerpValid;// ���� true, �� ���������/������� ������� ��� ������������ ���� ������
	private: SimpleList<Vector3> vList;// ������ ������, ����������� ����
	private: bool showPath;// ���� true, �� ���������� ���������� ������������


	// METHODS
	// ����������� ��� ����������
	public: GraphicsSphere(void): segments(360 / 5), scale(1.01f), selected(NO_SELECTED), mesh(NIL), startLerpValid(false), targetLerpValid(false), showPath(false)
	{
		mesh = new ColoredMesh();
		mesh->CreateCircle2D(Circle(Vector3(), 0.07f), 16);
		mesh->SetColor(RGBA(200, 200, 200, 255));

		// ����� ������ ������, ����������� ����
		for(int32 i = 1; i < mesh->GetAmountOfVertices(); i++)// ���������� ������ �������, � �������� ������� ��������� ����� �����
		{
			vList.AddToTail(mesh->GetVertex(i));
		}
		vList.AddToTail(mesh->GetVertex(1));// �������� ���������� �����


		step = Angle::DOUBLE_PI / (float)segments;
		qX = Quaternion(Matrix4x4::_RotationX(step));
		qY = Quaternion(Matrix4x4::_RotationY(step));
		qZ = Quaternion(Matrix4x4::_RotationZ(step));

		xAxis = Vector3::UNIT_X;
		yAxis = Vector3::UNIT_Y;
		zAxis = Vector3::UNIT_Z;

		xColor = RGBA(163, 21, 21, 255);
		yColor = RGBA(118, 185, 0, 255);
		zColor = RGBA(55, 111, 194, 255);

		faceColor = RGBA(RGBA::BLACK_COLOR);

		// ������� ������ ������� ����������
		face = new ColoredMesh();
		face->CreatePyramid3D(0.08f);
		face->SetPosition(zAxis);
		face->SetRotation(Matrix4x4::_RotationX(Angle::HALF_PI));// ��������� �� 90 �������� ������ ��� X
		face->SetColor(faceColor);
	}

	// ����������
	public: virtual ~GraphicsSphere(void)
	{
		delete face;
		delete mesh;
	}


	// ��������� ������� ���������� � �������� ��������� ������� ������������
	public: void SetStartQ(void)
	{
		startLerp = GetRotation() * Vector3::UNIT_Z;
		startLerpValid = true;
	}


	// ��������� ������� ���������� � �������� ������� ������� ������������
	public: void SetTargetQ(void)
	{
		targetLerp = GetRotation() * Vector3::UNIT_Z;
		targetLerpValid = true;
	}


	// ��������� ��������� �����
	public: virtual void Update(float _delta)
	{
		face->SetColor(RGBA::_GetRandom());
	}


	
	// ���������� ������ ������, ����������� ���������� ������������
	public: SimpleList<Vector3>& GetPathList(void)
	{
		return pathList;
	}
	


	// ���������� �����
	public: virtual void Draw(void)
	{
		if(IsShowEnabled())
		{
			ColoredMesh::Draw();

			BeginTransform();

			/* ����� ����� ������ 1; ������� ���� ���� ��������������� � ����� ������ 1, ����� ������� ��� ���������� 
			   ������������ ������� ����� �������� ����� �� ����������� ����� */ 

			// ���������� ������ �������� ������ ��� ��� X...
			point1 = yAxis * scale;// �������� ��������� ������ � ������ ��� 
			SimpleList<Vector3> xList;
			xList.AddToTail(point1);

			for(int32 i = 0; i < segments; i++)
			{  
				point2 = qX * point1;// ������������ ���������� ������ �� qX
				
				xList.AddToTail(point2);
				//GraphicsLine::_Draw(point1, point2 + Vector3::_FromTo(point1, point2) * 0.11f, selected == CIRCLE_X ? RGBA(RGBA::YELLOW_COLOR) : xColor);
				point1 = point2;
			}
			GraphicsLine::_Draw(xList, selected == CIRCLE_X ? RGBA(RGBA::YELLOW_COLOR) : xColor);
		
			// ���������� ������ �������� ������ ��� ��� Y...
			point1 = xAxis * scale;// �������� ��������� ������ � ������ ��� 
			SimpleList<Vector3> yList;
			yList.AddToTail(point1);

			for(int32 i = 0; i < segments; i++)
			{
				point2 = qY * point1;// ������������ ���������� ������ �� qY
				yList.AddToTail(point2);
				//GraphicsLine::_Draw(point1, point2 + Vector3::_FromTo(point1, point2) * 0.11f, selected == CIRCLE_Y ? RGBA(RGBA::YELLOW_COLOR) : yColor);
				point1 = point2;
			}
			GraphicsLine::_Draw(yList, selected == CIRCLE_Y ? RGBA(RGBA::YELLOW_COLOR) : yColor);


			// ���������� ������ �������� ������ ��� ��� Z...
			point1 = xAxis * scale;// �������� ��������� ������ � ������ ��� 
			SimpleList<Vector3> zList;
			zList.AddToTail(point1);

			for(int32 i = 0; i < segments; i++)
			{
				point2 = qZ * point1;// ������������ ���������� ������ �� qZ
				zList.AddToTail(point2);
				//GraphicsLine::_Draw(point1, point2 + Vector3::_FromTo(point1, point2) * 0.11f, selected == CIRCLE_Z ? RGBA(RGBA::YELLOW_COLOR) : zColor);
				point1 = point2;
			}
			GraphicsLine::_Draw(zList, selected == CIRCLE_Z ? RGBA(RGBA::YELLOW_COLOR) : zColor);

			// ���������� ��� Up � ��� Right ��� ������� ����� ���� ���� ��� �� ������������ �� ��� Z
			GraphicsLine::_SetLineWidth(3.0f);// ������� �����
			face->BeginTransform();
			
			GraphicsLine::_Draw(Vector3() + Vector3(0, 0.04f, 0), (xAxis / 4.0f) + Vector3(0, 0.04f, 0), xColor + RGBA(1, 0, 0, 0));
			GraphicsLine::_Draw(Vector3() + Vector3(0, 0.04f, 0), (-zAxis / 4.0f) + Vector3(0, 0.04f, 0), yColor + RGBA(0, 1, 0, 0));// �.�. �� ��������� ������ face, �� �� �� ���������� �������� ������ yAxis ��� ������������� ��� Y
			
			face->EndTransform();
			GraphicsLine::_SetLineWidth(6.0f);// ������� �����

			face->Draw();// ���������� ��������� ���������� �����

			EndTransform();

					
			// ������ ����� ��� ��������� � ������� ������� ������������...	
			if(startLerpValid)// ���� ��������� ������� ���� ������
			{
				// �������� ���� ������ �����
				Vector3 v = Vector3::_FromTo(Vector3(), startLerp);
				v.Normalize();
				v.z *= 1.005f;

				mesh->SetPosition(v);
				mesh->SetRotation(Quaternion(Vector3::UNIT_Z, startLerp));// ����������� ���� �� ������ ����� �� ����������� �����
				//mesh->Draw(); 


				GraphicsLine::_SetLineWidth(2.0f);// ������� �����
				mesh->BeginTransform();
				GraphicsLine::_Draw(vList, mesh->GetColor());

				// ������ ���������� ��� ����������� �������� �� ��� Z
				//GraphicsLine::_SetLineWidth(4.0f);// ������� �����
				Vector3 v1(0, 0.07f, 0);
				Vector3 v2(0, -0.07f, 0);
				GraphicsLine::_Draw(v1, v2, mesh->GetColor());

				Vector3 v3(-0.07f, 0, 0);
				Vector3 v4(0.07f, 0, 0);
				GraphicsLine::_Draw(v3, v4, mesh->GetColor());
				//GraphicsList::_Draw(v1, v2, mesh->GetColor());
				
				mesh->EndTransform();
			}

			if(targetLerpValid)// ���� ������� ������� ���� ������
			{
				// �������� ���� ������ �����
				Vector3 v = Vector3::_FromTo(Vector3(), targetLerp);
				v.Normalize();
				v.z *= 1.005f;

				mesh->SetPosition(v);
				mesh->SetRotation(Quaternion(Vector3::UNIT_Z, targetLerp));// ����������� ���� �� ������ ����� �� ����������� �����
				//mesh->Draw(); 

				GraphicsLine::_SetLineWidth(2.0f);// ������� �����
				mesh->BeginTransform();
				GraphicsLine::_Draw(vList, mesh->GetColor());
				
				// ������ ���������� ��� ����������� �������� �� ��� Z
				//GraphicsLine::_SetLineWidth(4.0f);// ������� �����
				Vector3 v1(0, 0.07f, 0);
				Vector3 v2(0, -0.07f, 0);
				GraphicsLine::_Draw(v1, v2, mesh->GetColor());

				Vector3 v3(-0.07f, 0, 0);
				Vector3 v4(0.07f, 0, 0);
				GraphicsLine::_Draw(v3, v4, mesh->GetColor());
				//GraphicsList::_Draw(v1, v2, mesh->GetColor());
				
				mesh->EndTransform();
			}

			//if(showPath)// ���� ����� ���������� ���� ������������
			{
				GraphicsLine::_Draw(pathList, RGBA(RGBA::GREEN_COLOR));
			}
		}
	}
 
		
	// ���������� ���� ��������� ��� �������� X
	public: RGBA GetXColor(void)const
	{
		return xColor;
	}

	// ���������� ���� ��������� ��� �������� Y
	public: RGBA GetYColor(void)const
	{
		return yColor;
	}


	// ���������� ���� ��������� ��� �������� Z
	public: RGBA GetZColor(void)const
	{
		return zColor;
	}


	// �������� �������� ������ ��� X � �������� ��������
	public: void SelectX(void)
	{
		selected = CIRCLE_X;
	}


	// �������� �������� ������ ��� Y � �������� ��������
	public: void SelectY(void)
	{
		selected = CIRCLE_Y;
	}


	// �������� �������� ������ ��� Z � �������� ��������
	public: void SelectZ(void)
	{
		selected = CIRCLE_Z;
	}


	// ���������� ������������� ������� ��� ��������
	public: int32 GetSelected(void)const
	{
		return selected;
	}

	// ������� ��������� � ������� ��� ��������
	public: void Unselect(void)
	{
		selected = NO_SELECTED;
	}


	// ���������� � ��������...
	// ��������� ������ � ����� ����� _file, � ���������� true � ������ ������
	// ����������: ���� ���� �� ������ �� ������, �� ������������ false
	public: virtual bool SaveToFile(File& _file)
	{
		return true;
	}

	
	// ��������� ������ �� ����� _file, � ���������� true � ������ ������
	// ����������: ���� ���� �� ���������� ��� �� ������ �� ������, �� �������� false
	protected: virtual bool LoadFromFile(File& _file)
	{
		return true;
	}
};



// ���������������� ����������
class Application: public Engine// ������
{
	// DATA
	private: Camera3D* camera, *camera2d;// ������
    private: Font2D *font;// �����
	private: Scene* scene;// �����

	private: bool rightPressed;// ���� true, �� ������ ������ ���� ������
	private: GraphicsSphere* sphere;// ����������� �����
	private: Gizmo* gizmo;// ����� (������� ����������� ������� ���������)

	private: Vector3 previousCursorPosition;// ������� ������� �� ����� � ���������� ������ �������
	private: Vector3 startCursorPosition;// ������� ������� �� ����� � ������ ������ �������� �����
	private: bool startSphereRotation;// ���� true, �� �������������� �������� �����
    
    // �������
    private: TexturedMesh* clenchedHandCursor;// ������ "������ ����"
	private: TexturedMesh* unclenchedHandCursor;// ������ "�������� ����"
	private: TexturedMesh* arrowCursor;// ������ "�������"
	private: StringANSI relativeRotateString;// ������������� �������� ����� � ���� ������
	private: FloatLerp* fLerp;// �������� ������������
	private: QuaternionLerp* qLerp;// ������������� ������������
	private: IAction* action1;// ��������
	private: Trigger* updateRotation;// ���������, �������������� ����� ����� ������ � ������������� �������������


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), camera2d(NIL), font(NIL), scene(NIL), rightPressed(false), sphere(NIL), gizmo(NIL), startSphereRotation(false), clenchedHandCursor(NIL), unclenchedHandCursor(NIL), arrowCursor(NIL), fLerp(NIL), qLerp(NIL), action1(NIL), updateRotation(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete camera;
		delete camera2d;

		delete font;
		delete gizmo;
		delete sphere;

		delete clenchedHandCursor;
		delete unclenchedHandCursor;
		delete arrowCursor;
		delete fLerp;
		delete qLerp;
		
		//delete action1;
		delete updateRotation;

		delete scene;
	} 


	// ���������� ������������� ���� �������� pitch/yaw/roll � ���� ������
	public: StringANSI GetRelativeRotateString(void)
	{
		return "relative rotate: " + relativeRotateString;
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
				//scene->Rename("scene");// ��� �����
				//AssetLibrary::_GetAssets().AddObject(scene);// �������� ����� � ���������� �������, ����� �� ����������
				scene->Pick();
			}


			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.fontName = "Arial";// ��� ������
				def.fontSize = 16;// ������ ������
				def.color = RGBA(RGBA::GRAY_COLOR);// ���� ������

				font = new Font2D();
				font->Create(def);
				font->Pick();// ������� �����
			}


			// ������...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::SCROLL_OBJECT_VIEWER;
				def.rotationEnable = false;// ��������� �������� ������
				def.nearZoom = def.nearZoom * 18.0f;
				def.farZoom = def.farZoom * 5.0f;
				def.distanceToViewingPoint = 4.3f;
				def.zoomEnable = true;
				def.projectionMode = Camera3D::PERSPECTIVE_PROJECTION;

				def.pitchClampEnable = false;// ��������� ���������� �������� ������ ��� X
				


				camera = new Camera3D();
				camera->Create(def);
				
				// ������ ����������� � ������������ ��� ������
				//camera->SetNearZoom(camera->GetNearZoom() * 18.0f);
				//camera->SetFarZoom(camera->GetFarZoom() * 5.0f);
				//camera->SetDistanceToViewingPoint(170.0f); 

				camera->Pick(); // ������� ������
				//camera->SetRotation(Quaternion(Matrix4x4::_RotationY(Angle::PI)));// ��������� ������ � ����, � ��������������� �������
				scene->BindCamera(camera);
			}
		
			// ������ ��� ���������� �������...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;

				def.pitchClampEnable = false;// ��������� ���������� �������� ������ ��� X
				
				camera2d = new Camera3D();
				camera2d->Create(def);
			}

			{// ����������� �����...
				sphere = new GraphicsSphere();
				sphere->SetColor(RGBA(230,230,230,255));// ����� ����
				sphere->CreateIcosphere3D(4);// ������� ����� �� ���� ���������
				sphere->Rename("sphere");

				scene->AddGraphicsObject(sphere);// �������� � ����������� �����
				scene->AddLogicObject(sphere);// �������� � ���������� �����
				
				AssetLibrary::_GetAssets().AddObject(sphere);
				//sphere->SetRotation(Quaternion(Matrix4x4::_RotationX(Angle::PI)));// ������ ���������� "����� �������"
			}

			// �����..
			{
				gizmo = new Gizmo();
				gizmo->SetRotation(Quaternion());
				gizmo->SetShowEnable(false);// ������
				scene->AddGraphicsObject(gizmo);// �������� � ����������� �����
			}

			
			// �������� ������������...
			{
				fLerp = new FloatLerp();
				fLerp->Rename("FIXED_TIME");// ������ ��� 
				fLerp->SetPlayingSpeed(3.0f);// �������� �������� � ��������
				fLerp->SetTarget(10.0f);// ���������� �� ����
				//fLerp->Play();
				fLerp->SetLerpMode(FloatLerp::FIXED_TIME);//������� � �������
				scene->AddLogicObject(fLerp);
			}


			// ������������� ������������...
			{
				qLerp = new QuaternionLerp();
				qLerp->Rename("qLerp");
				qLerp->BindFloatLerp(fLerp);
				qLerp->SetFloatLerpBuildInEnable(true);
				AssetLibrary::_GetAssets().AddObject(qLerp);
			}


			
			// �������, ����������� ���������� �����...
			{
				updateRotation = new Trigger();
				//updateRotation->Rename("update_rotation");
				updateRotation->SetActionExpr("compute_quaternion_current");// �������
			
				// ��������...
				action1 = Trigger::_CreateAction(QuaternionLerp::COMPUTE_QUATERNION_CURRENT_MESSAGE, "qLerp", "sphere");
				action1->Rename("compute_quaternion_current");
				AssetLibrary::_GetAssets().AddObject(action1);

				updateRotation->SetEventExpr("");// �������
			}
			

			// ������ "�������� ����"...
			{
				TexturedAnimation* anim = AssetLibrary::_LoadCommonAsset<TexturedAnimation>("unclenched");

				unclenchedHandCursor = new TexturedMesh();
				unclenchedHandCursor->CreateQuad2D(Quad(Vector3(), Vector2(24, 29)));
				unclenchedHandCursor->BindAnimation(anim);
				
				unclenchedHandCursor->ConvertTo2DObject();
				unclenchedHandCursor->SetShowEnable(true);// �������� �����������
				//scene->AddGraphicsObject(unclenchedHandCursor);// �������� � ����������� �����
			}


			// ������ "������ ����"...
			{
				TexturedAnimation* anim = AssetLibrary::_LoadCommonAsset<TexturedAnimation>("clenched");

				clenchedHandCursor = new TexturedMesh();
				clenchedHandCursor->CreateQuad2D(Quad(Vector3(), Vector2(22, 24)));
				clenchedHandCursor->BindAnimation(anim);

				clenchedHandCursor->SetShowEnable(true);// �������� �����������
				clenchedHandCursor->ConvertTo2DObject();
			}

			
			// ������ "�������"...
			{
				TexturedAnimation* anim = AssetLibrary::_LoadCommonAsset<TexturedAnimation>("cursor");

				arrowCursor = new TexturedMesh();
				arrowCursor->CreateQuad2D(Quad(Vector3(14/2, 24/2, 0), Vector2(14, 24)));
				arrowCursor->BindAnimation(anim);

				arrowCursor->SetShowEnable(true);// �������� �����������
				arrowCursor->ConvertTo2DObject();
				Mouse::_PickCursor(arrowCursor);// �������� ������
			}


			// ����...
			Mouse::_SetShowSystemCursor(false);// ������ ��������� ������
			Mouse().SetKeyPressedPeriodInSec(0.04f);// ��������� ���������������� ����

	
			GraphicsLine::_SetLineWidth(6.0f);// ������� �����
			GraphicsPoint::_SetPointSize(3.0f);// ������ �����


			// ���� ����...
			SetBackgroundColor(RGBA(RGBA::WHITE_COLOR));// ����� ����

			MoveToCenter();// ����������� ���� � ����� ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}



	// ������� ����������
	public: virtual void Update(float _delta)
	{
		Engine::Update(_delta);

		if(fLerp->IsPlaying())
		{
			Vector3 v = sphere->GetTransformation() * Vector3::UNIT_Z;
			v *= 1.005f;
			sphere->GetPathList().AddToTail(v);
		}
	}



	// ������� ������������
	public: virtual void Draw(float _delta)
	{
		Engine::Draw(_delta);
		

		// ������ ����� ������������� �����...
		camera2d->BeginTransform(Renderer::PROJECTION);
		camera2d->BeginTransform(Renderer::MODELVIEW);


		// ����� ������...
		int32 i = 0;

		font->DrawScreenText(Vector3(10, i+=20, -1), "gizmo.edit mode: %s", gizmo->GetRotationModeString().c_str());

		if(sphere->GetSelected() != GraphicsSphere::NO_SELECTED)
		{
			font->DrawScreenText(Vector3(10, i+=20, -1), "%s", GetRelativeRotateString().c_str());
		}

		
			
		font->DrawScreenText(Vector3(10, i+=20, -1), "quaternion: %.3f/%.3f/%.3f/%.3f", sphere->GetRotation().x, sphere->GetRotation().y, sphere->GetRotation().z, sphere->GetRotation().w);

		/*
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.buildIn: %d", qLerp->IsFloatLerpBuildInEnabled());

		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.mode: %d", qLerp->GetFloatLerp()->GetLerpMode());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.playing duration: %f", qLerp->GetFloatLerp()->GetPlayingDurationInSec());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.playing speed: %f", qLerp->GetFloatLerp()->GetPlayingSpeed());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.playing: %d", qLerp->GetFloatLerp()->IsPlaying());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.target: %f", qLerp->GetFloatLerp()->GetTarget());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.current: %f", qLerp->GetFloatLerp()->GetCurrent());


		*/ 
		// ������������ ���������� �������������...
		camera2d->EndTransform(Renderer::PROJECTION);
		camera2d->EndTransform(Renderer::MODELVIEW);
	}


	// ������ ������� ����
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Engine::MouseEventFilter(_event);

		if(_event->type == Mouse::KEY_DOWN)// ���� ������ ������
		{
			if(_event->key == Mouse::LEFT_BUTTON)// ���� ����� ����
			{
				if(gizmo->GetRotationMode() == Gizmo::EULER_SCROLLBAR)
				{
					if(!rightPressed)// ���� ������ �� ���������
					{
						// ������ �������� �����...
						if(!startSphereRotation && sphere->GetSelected() != GraphicsSphere::NO_SELECTED)
						{
							Mouse::_PickCursor(clenchedHandCursor);// �������� ������

							Vector3 intersectedPoint;// ����� ����������� 

							if(sphere->IsIntersected(camera->GetSelectRay(), intersectedPoint) != -1)// ���� ���� ����������� ������� �� ������
							{
								startSphereRotation = true;
								startCursorPosition = intersectedPoint;// ��������� ��������� ������� �������
								previousCursorPosition = intersectedPoint;// ����� ������� �������
							}
						}
					}
				}
			}
		}
		else if(_event->type == Mouse::KEY_PRESSED)// ���� ������ ������
		{
			if(_event->key == Mouse::LEFT_BUTTON)// ���� ����� ����
			{
				if(gizmo->GetRotationMode() == Gizmo::SPHERICAL_ST_TWO_POLE)
				{
					Vector3 intersectedPoint;

					if(sphere->IsUnderCursor(intersectedPoint) != -1)// ���� �������� ���� �� �����
					{
						Vector3 v1(Vector3::UNIT_Z);
						
						Vector3 v2 = Vector3::_FromTo(sphere->GetPosition(), intersectedPoint);
					
						if(Vector3::_GetDot(v1, v2) < 0.0f)// ���� ������� ��������� � ������ �����������
						{
							v1 = -Vector3::UNIT_Z;// ������� ������� ����� ����� ������������� ����� ������������� ��� Z
							sphere->SetRotation(Quaternion(v1, v2));
							sphere->SetRotation(sphere->GetRotation() * Quaternion(Euler(0.0f, Angle::PI, 0.0f)));

							//sphere->GetControlPoints().AddToTail(sphere->GetRotation() * Vector3::UNIT_Z);
						}
						else
						{
							sphere->SetRotation(Quaternion(v1, v2));
							//sphere->GetControlPoints().AddToTail(sphere->GetRotation() * Vector3::UNIT_Z);
						}
					}
				}
				else if(gizmo->GetRotationMode() == Gizmo::SPHERICAL_SWING_TWIST)
				{
					Vector3 intersectedPoint;

					if(sphere->IsUnderCursor(intersectedPoint) != -1)// ���� �������� ���� �� �����
					{
						Vector3 v1(Vector3::UNIT_Z);
						
						Vector3 v2 = Vector3::_FromTo(sphere->GetPosition(), intersectedPoint);	
						
						sphere->SetRotation(Quaternion(v1, v2));
					}
				}
				else if(gizmo->GetRotationMode() == Gizmo::SPHERICAL_FROM_TO)
				{					
					Vector3 intersectedPoint;

					if(sphere->IsUnderCursor(intersectedPoint) != -1)// ���� �������� ���� �� �����
					{
						sphere->SetRotation(Quaternion(Matrix3x3<Vector3>::_LookAt(sphere->GetPosition(), intersectedPoint, Vector3::UNIT_Y)));
					}
				}
			}
			else if(_event->key == Mouse::RIGHT_BUTTON)// ������ ����
			{
				// ������ �������� ������...
				if(!rightPressed)// ��������� ������ ���� ���
				{
					Mouse::_MoveCursorToCenter(Renderer::_Picked());// ���������� ���� � ����� ���������� ������� ���� 
					camera->SetRotationEnable(true);// ��������� �������� ������
					rightPressed = true;
					Mouse::_PickCursor(NIL);
					sphere->Unselect();// ����� ���������
					//camera->SetZoomEnable(false);// ��������� ������
				}
			}
		} 
		else if(_event->type == Mouse::KEY_UP)// ���� ������ ��������
		{
			if(_event->key == Mouse::LEFT_BUTTON)// ���� ����� ����
			{
				if(gizmo->GetRotationMode() == Gizmo::EULER_SCROLLBAR)
				{
					// ����� �������� �����...
					if(startSphereRotation) 
					{ 
						startSphereRotation = false; // ��������� ��������
						Mouse::_PickCursor(unclenchedHandCursor);// �������� ������
					}
				}
			}
			else if(_event->key == Mouse::RIGHT_BUTTON)// ���� ������ ����
			{
				// ����� �������� ������...
				if(rightPressed)// ��������� ������ ���� ���
				{
					camera->SetRotationEnable(false); // ��������� �������� ������
					rightPressed = false;
					Mouse::_PickCursor(arrowCursor);
				}
			}
		}	
		else if(_event->type == Mouse::WHEEL)// �������� �������� ����
		{
			if(!rightPressed)// ���� ������ �� ���������
			{
				Mouse::_PickCursor(arrowCursor);// �������� ������
				sphere->Unselect();// ����� ���������
			}
		}
		else if(_event->type == Mouse::MOVE)
		{
			if(gizmo->GetRotationMode() == Gizmo::EULER_SCROLLBAR)
			{
				if(!startSphereRotation && !rightPressed)// ���� ������ �� ��������� � ����� �� ���������
				{
					if(sphere->GetXColor() == GetPixelColorUnderCursor())// ���� ������ ��������� �� ��������� ��� �������� X
					{
						sphere->SelectX();
						gizmo->SetShowEnable(true);

						Mouse::_PickCursor(unclenchedHandCursor);// �������� ������
					}
					else if(sphere->GetYColor() == GetPixelColorUnderCursor())// ���� ������ ��������� �� ��������� ��� �������� Y
					{
						sphere->SelectY();
						gizmo->SetShowEnable(true);

						Mouse::_PickCursor(unclenchedHandCursor);// �������� ������
					}
					else if(sphere->GetZColor() == GetPixelColorUnderCursor())// ���� ������ ��������� �� ��������� ��� �������� Z
					{
						sphere->SelectZ();
						gizmo->SetShowEnable(true);

						Mouse::_PickCursor(unclenchedHandCursor);// �������� ������
					}
					else
					{
						// ��������� �� ����� (���� �����)� ����� ����� (���� ����)
						if(GetPixelColorUnderCursor() == RGBA(230,230,230,255) || GetPixelColorUnderCursor() == RGBA(RGBA::WHITE_COLOR))
						{
							Mouse::_PickCursor(arrowCursor);// �������� ������

							sphere->Unselect();// ����� ���������
							gizmo->SetShowEnable(false);
						}
					}
				}
				else if(startSphereRotation && !rightPressed)// ���� ������ �� ��������� � ����� ���������
				{
					Vector3 intersectedPoint;// ����� ����������� 

					if(sphere->IsIntersected(camera->GetSelectRay(), intersectedPoint) != -1)// ���� ���� ����������� ������� �� ������
					{
						Vector3 currentCursorPosition = intersectedPoint;// ����� ������� �������

						Vector3 v1 = Vector3::_FromTo(sphere->GetPosition(), previousCursorPosition);
						v1.Normalize();// ��������������� ������ ����� ��� ��� ��� �������
						v1 = Quaternion::_GetInverse(sphere->GetRotation()) * v1;// ���������� ������������� �����, ������� ����������� ����� �� �����, �� ��� ����� ������������� ��������; ������ ���� ����� ��������� ������������ ���������� ���� X/Y/Z ����� ����������� ���������� �� ������������� ��������, ����������� �� �����

						Vector3 v2 = Vector3::_FromTo(sphere->GetPosition(), currentCursorPosition);
						v2.Normalize();
						v2 = Quaternion::_GetInverse(sphere->GetRotation()) * v2;

						Vector3 v3 = Vector3::_FromTo(sphere->GetPosition(), startCursorPosition);
						v3.Normalize();// ��������������� ������ ����� ��� ��� ��� �������
						v3 = Quaternion::_GetInverse(sphere->GetRotation()) * v3;// ���������� ������������� �����, ������� ����������� ����� �� �����, �� ��� ����� ������������� ��������; ������ ���� ����� ��������� ������������ ���������� ���� X/Y/Z ����� ����������� ���������� �� ������������� ��������, ����������� �� �����

						switch(sphere->GetSelected())
						{
							case GraphicsSphere::CIRCLE_X:
							{
								float x1 = v1.AngleX();
								float x2 = v2.AngleX();
								
								sphere->SetRotation(sphere->GetRotation() * Quaternion(Matrix4x4::_RotationX(x2 - x1)));
								previousCursorPosition = currentCursorPosition;
								
			
								float x3 = v3.AngleX();
								relativeRotateString = "[" + NumberToString((int32)Angle::_RadiansToDegrees(x2 - x3)) + ", 0, 0]";
								break;
							}

							case GraphicsSphere::CIRCLE_Y:
							{
								float y1 = v1.AngleY();
								float y2 = v2.AngleY();

								sphere->SetRotation(sphere->GetRotation() * Quaternion(Matrix4x4::_RotationY(y2 - y1)));
								previousCursorPosition = currentCursorPosition;
								

								float y3 = v3.AngleY();
								relativeRotateString = "[0, " + NumberToString((int32)Angle::_RadiansToDegrees(y2 - y3)) + ", 0]";
								break;
							}

							case GraphicsSphere::CIRCLE_Z:
							{
								float z1 = v1.AngleZ();
								float z2 = v2.AngleZ();

								sphere->SetRotation(sphere->GetRotation() * Quaternion(Matrix4x4::_RotationZ(z2 - z1)));
								previousCursorPosition = currentCursorPosition;
								
								
								float z3 = v3.AngleZ();
								relativeRotateString = "[0, 0, " + NumberToString((int32)Angle::_RadiansToDegrees(z2 - z3)) + "]";
								break;
							}
						}
					}
				}
			}
		}

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
			else if(_event->key == Keyboard::X)
			{
				//if(gizmo->GetRotationMode() == Gizmo::EULER_SCROLLBAR)
				{
					camera->SetRotation(Quaternion(Matrix4x4::_RotationY(Angle::HALF_PI)));// ��������� ����������
					gizmo->SetShowEnable(true);
					
					// ����������� �������� ��� ����������� ����������
					camera->SetReverseScrollY(false);
					camera->SetViewingPointYAxis(Vector3::UNIT_Y);
				}
			} 
			else if(_event->key == Keyboard::Y)
			{
				//if(gizmo->GetRotationMode() == Gizmo::EULER_SCROLLBAR)
				{
					camera->SetRotation(Quaternion(Euler(-Angle::HALF_PI, Angle::HALF_PI, 0)));// ��������� ������ ������
					gizmo->SetShowEnable(true);
					
					// ����������� �������� ��� ����������� ����������
					camera->SetReverseScrollY(true);
					camera->SetViewingPointYAxis(Vector3::UNIT_X);
				}
			} 
			else if(_event->key == Keyboard::Z)
			{
				//if(gizmo->GetRotationMode() == Gizmo::EULER_SCROLLBAR)
				{
					camera->SetRotation(Quaternion(Euler(0.0f, 0.0f, 0.0f)));// ��������� ������ � ����, � ��������������� �������
					gizmo->SetShowEnable(true);
					
					// ����������� �������� ��� ����������� ����������
					camera->SetViewingPointYAxis(Vector3::UNIT_Y);
				}
			}
			else if(_event->key == Keyboard::NUM_1)
			{
				gizmo->SetRotationMode(Gizmo::EULER_SCROLLBAR);
			}
			else if(_event->key == Keyboard::NUM_2)
			{
				gizmo->SetRotationMode(Gizmo::SPHERICAL_SWING_TWIST);
				gizmo->SetShowEnable(true);
				Mouse::_PickCursor(arrowCursor);
			}
			else if(_event->key == Keyboard::NUM_3)
			{
				gizmo->SetRotationMode(Gizmo::SPHERICAL_ST_TWO_POLE);
				gizmo->SetShowEnable(true);
				Mouse::_PickCursor(arrowCursor);
			}
			else if(_event->key == Keyboard::NUM_4)
			{
				gizmo->SetRotationMode(Gizmo::SPHERICAL_FROM_TO);
				gizmo->SetShowEnable(true);
				Mouse::_PickCursor(arrowCursor);
			}
			else if(_event->key == Keyboard::H)
			{
				sphere->SetRotation(Quaternion());
				//qLerp->Set(Quaternion(), Quaternion());
			}
			else if(_event->key == Keyboard::A)
			{
				qLerp->SetFrom(sphere->GetRotation());
				sphere->SetStartQ();
			}
			else if(_event->key == Keyboard::B)
			{
				qLerp->SetTo(sphere->GetRotation());
				sphere->SetTargetQ();
			}
			else if(_event->key == Keyboard::SPACE)
			{
				qLerp->GetFloatLerp()->Stop();
				qLerp->GetFloatLerp()->Play();
				sphere->GetPathList().Clear();
			}
			else if(_event->key == Keyboard::C)
			{
				delete action1;
			}
			/*
			else if(_event->key == Keyboard::S)
			{
				qLerp->SaveToFile();
				delete qLerp;
				delete fLerp;
			}
			else if(_event->key == Keyboard::L)
			{
				qLerp = new QuaternionLerp();
				//qLerp->Destroy();
				qLerp->LoadFromFile("qLerp");
				AssetLibrary::_GetAssets().AddObject(qLerp);

				sphere->SetRotation(qLerp->GetFrom());
				sphere->SetStartQ();

				sphere->SetRotation(qLerp->GetTo());
				sphere->SetTargetQ();

				scene->AddLogicObject(qLerp->GetFloatLerp());

				updateRotation->SetValue(true);
			}
			*/ 
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "QuaternionLerp.example_01";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
