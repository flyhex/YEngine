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
#include "MyTree.h"




// �����, ����������� ���������������� ����
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: GraphicsScene* scene;// ����������� �����

	private: MyTree tree;// �������� ������

	private: bool testEnable;// ���� true, �� ����� ������������� ����������� � ������������ ���� ������ �� ���������� �������


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), testEnable(false)
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

			 


			// ������...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;
				def.pitchMax = 0.0f;// ��������� ������ ���������� ���� ���� (��������� XZ)
				def.zoomMax = 15;
				def.zoomMin = 6;



				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera);
				camera->Pick("SimpleBinaryTree.example_01"); 
				
				// ������ �������������� ������ ������
				//camera->UpdateTrackballObjectViewer(-Angle::HALF_PI/3.0f, Angle::HALF_PI/3.0f, 0.0f);
			}


			SetBackgroundColor(RGBA::GRAY_COLOR);// ���� ����
			
			GraphicsPoint<Vector3D>::_SetPointSize(10);// ������ ����� 
			GraphicsLine<Vector3D>::_SetLineWidth(3.0f);// ������� �����


			MoveToCenter();// ����������� ���� � ����� �����
			Mouse::_MoveCursorToClientAreaCenter(this);// ����������� ������ � ����� ���������� ������� ����	
			Mouse::_ShowCursor(false);// ������ ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}




		


	// ������� �����
	public: virtual void Update(float _delta)
	{
		if(testEnable) { tree.Test(0, 1000); }

		// ������� ����� �� �������� � �������
		//quad->SetPosition(quad->GetPosition() + Vector3D(0,0,0.01));
	}

	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����


		//scene->Draw();
		tree.Draw();// ���������� ������
	
		// ����� ������...
		camera->BeginScreen2D();// ������ ����� ���������� �������

		int32 i = 0;
		
		
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.position: %.2f/%.2f/%.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "tree.Size: %d", tree.Size()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "tree.Root: %d", tree.Root()); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "tree.IsEmpty: %d", tree.IsEmpty()); 


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "marked node: %d", tree.Marked());
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected node: %d", tree.Selected());

		if(tree.Selected())// ���� ���� ���������� ����
		{
			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.data: %d", tree.Selected()->data);

			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.left: %d", tree.Selected()->Child(0));
	
			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.right: %d", tree.Selected()->Child(1));

			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.parent: %d", tree.Selected()->Parent());
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
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sizeInKbyte: %.5f", Memory::_FromByteToKB(mesh->SizeInByte())); 
 


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
		camera->End2D();// ������ ����� ���������� �������
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
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				if(tree.Selected()) { tree.Select(tree.Selected()->Child(0)); }
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				if(tree.Selected()) { tree.Select(tree.Selected()->Child(1)); }
			}
			else if(_event.key == Keyboard::UP_ARROW)
			{
				if(tree.Selected()) { tree.Select(tree.Selected()->Parent()); } 
			}
			else if(_event.key == Keyboard::S)
			{
				tree.Select(tree.Root());  
			}
			else if(_event.key == Keyboard::M)
			{
				tree.SetMark(tree.Selected());// �������� ���������� ����
			}
			else if(_event.key == Keyboard::NUM_6)
			{
				tree.Select(tree.Extremum(false));
			}
			else if(_event.key == Keyboard::NUM_5)
			{ 
				tree.Select(tree.Extremum(true));
			}
			else if(_event.key == Keyboard::C)
			{
				tree.Select(NIL);// �������� ���������� ����
				tree.SetMark(NIL);// �������� ���������� ����
		
				tree.Clear();
			}
			else if(_event.key == Keyboard::F)
			{
				MyTree::Node* finded = NIL;
				if(tree.Marked()) { finded = tree.Find(tree.Marked()->data); } // ����� ���������� ����
				if(finded) { tree.Select(finded); } // �������� ��������� ����
			}
			else if(_event.key == Keyboard::R)
			{ 
				tree.Del(tree.Selected());//��������� ���������� ����
			}
			else if(_event.key == Keyboard::A)
			{
				int32 key = Random::_RandomInt32(0, 10);

				if(tree.Add(key))
				{
					tree.Select(tree.Find(key));// �������� ����������� ����
				}
			}
			else if(_event.key == Keyboard::T)
			{
				testEnable = !testEnable;
				
				if(testEnable)// ���� ����� ��������� ����
				{
					tree.Select(NIL);
					tree.SetMark(NIL);
				}
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
	def.realTimeWorkerDef.windowDef.tittle = "SimpleBinaryTree.example_01";
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
