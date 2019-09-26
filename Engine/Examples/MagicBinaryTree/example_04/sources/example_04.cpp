

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
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Core/Shape/Sphere/Sphere3D/Sphere3D.h>
#include "MyNode.h"
#include "MyTree.h"


// �����, ����������� ���������������� ����
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: GraphicsScene<Vector3D, Quaternion, Matrix4x4>* scene;// ����������� �����
	
	private: MyTree tree;// ���������� �������� ������
	private: bool testEnabled;// ���� true, �� ����� �������� �������������� ������������ ������
	private: MyTree treeCopy;// ����� ������

    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), testEnabled(false)
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
				GraphicsScene<Vector3D, Quaternion, Matrix4x4>::Define def;
				def.logicSceneDef.scale = Size3D(1.0f, 1.0f, 1.0f);
				scene = new GraphicsScene<Vector3D, Quaternion, Matrix4x4>();
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
				scene->LogicScene::AddObject(font2d);
				font2d->Pick();
			}
	

			// ������...
			{
				Camera3D::Define def;
				def.name = "camera";
				//def.movementType = Camera3D::OVERLAND;
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;
				def.pitchMax = 0.0f;// ��������� ������ ���������� ���� ���� (��������� XZ)
				def.zoomMax = 15;
				def.zoomMin = 6;


				camera = new Camera3D();
				camera->Create(def);
				scene->LogicScene::AddObject(camera, true);
				camera->Pick("MagicBinaryTree.example_04");
			}

			// ������...
			tree.SetAnimFPS(2);

			MoveToCenter();// ���������� ���� � ������ ������
			GraphicsPoint<Vector3D, ColorRGBA>::_SetPointSize(10);// ������ ����� 
			GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(3.0f);// ������� �����

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
		if(testEnabled) { tree.Test(&MyNode::Key, 1, 1000); } 

		tree.UpdateHeight();// �������� ������ ������
		tree.UpdateExcessHeight();// �������� ���������� ������ ������� ������ ������

		// ���� ������������ ���������
		if(!testEnabled)
		{
			// ����������� ������� ��������
			tree.AnimSeqUpdate(_delta);
		}

	}


	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����



		// scene->Draw();// ����������� �����
		
		
		// ����� ������...
		tree.Draw();



		// ����� ������...
		camera->BeginScreen2D();// ������ ����� ���������� �������


		int i = 0;
		
		font2d->SetTextColor(ColorRGBA::WHITE_COLOR);

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "tree.height/perfect height: %d, %d", tree.StaticHeight(), tree.StaticHeight() - tree.StaticExcessHeight());
	    

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "tree.excess height: %d", tree.StaticExcessHeight());
	    
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "tree.Size: %d", tree.Size());

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "root node: %d", tree.Root());
	  
		

		font2d->SetTextColor(MyNode::MARK_COLOR);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "marked node: %d", tree.Marked());
	  

		font2d->SetTextColor(MyNode::SELECT_COLOR);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected node: %d", tree.Selected());




		if(tree.Selected())// ���� ���� ���������� ����
		{
			// �������� �����
			font2d->SetTextColor(ColorRGBA::WHITE_COLOR);
			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.key: %d", tree.Selected()->Key());


			// ����� �������
			if(tree.Marked() && tree.ChildOf(tree.Selected(), 0) == tree.Marked()) 
			{ 
				font2d->SetTextColor(MyNode::MARK_COLOR); 
			}
			else if(tree.Selected() && tree.ChildOf(tree.Selected(), 0) == tree.Selected()) 
			{ 
				font2d->SetTextColor(MyNode::SELECT_COLOR); 
			}
			else 
			{ 
				// ���� ������ �������
				font2d->SetTextColor(MyNode::LEFT_COLOR);
			} 

			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.left: %d", tree.ChildOf(tree.Selected(), 0));


			// ������ �������
			if(tree.Marked() && tree.ChildOf(tree.Selected(), 1) == tree.Marked()) 
			{ 
				font2d->SetTextColor(MyNode::MARK_COLOR); 
			}
			else if(tree.Selected() && tree.ChildOf(tree.Selected(), 1) == tree.Selected()) 
			{ 
				font2d->SetTextColor(MyNode::SELECT_COLOR); 
			}
			else 
			{ 
				// ���� ������� �������
				font2d->SetTextColor(MyNode::RIGHT_COLOR); 
			}

			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.right: %d", tree.ChildOf(tree.Selected(), 1));


			// ��������
			if(tree.Marked() && tree.ParentOf(tree.Selected()) == tree.Marked()) 
			{ 
				font2d->SetTextColor(MyNode::MARK_COLOR); 
			}
			else if(tree.Selected() && tree.ParentOf(tree.Selected()) == tree.Selected()) 
			{ 
				font2d->SetTextColor(MyNode::SELECT_COLOR); 
			}
			else 
			{ 
				font2d->SetTextColor(ColorRGBA::WHITE_COLOR);
			}

			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.parent: %d", tree.ParentOf(tree.Selected()));
		}



		font2d->SetTextColor(ColorRGBA::WHITE_COLOR);

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "anim size: %d", tree.AnimSeqSize());

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "anim fps: %d", tree.AnimFPS());
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "anim playing: %d", tree.IsPlaying());

		
		


		

	  
		
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
				MyNode* n = new MyNode(Random::_RandomInt32(1, 100));

				if(tree.Add(n, &MyNode::Key))// �������� � ������
				{
					tree.Select(n);// �������� ����������� ����
				}
				else { delete n; } // ����� ������� ����-��������
			}
			else if(_event.key == Keyboard::F)
			{
				MyNode* finded = NIL;
				if(tree.Marked()) { finded = tree.Find(tree.Marked()->Key(), &MyNode::Key); } // ����� ���������� ����
				if(finded) { tree.Select(finded); } // �������� ��������� ����
			}
			else if(_event.key == Keyboard::D)
			{ 
				tree.Del(tree.Selected());//��������� ���������� ����
			}
			else if(_event.key == Keyboard::R)
			{ 
				tree.Rebuild(tree.Marked(), 0, &MyNode::Key);
			}
			else if(_event.key == Keyboard::M)
			{
				tree.SetMark(tree.Selected());// �������� ���������� ����
			}
			else if(_event.key == Keyboard::S)
			{
				tree.Select(tree.Root());  
			}
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				if(tree.Selected()) { tree.Select(tree.ChildOf(tree.Selected(), 0)); }
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				if(tree.Selected()) { tree.Select(tree.ChildOf(tree.Selected(), 1)); }
			}
			else if(_event.key == Keyboard::UP_ARROW)
			{
				if(tree.Selected()) { tree.Select(tree.ParentOf(tree.Selected())); } 
			}
			else if(_event.key == Keyboard::T)
			{
				testEnabled = !testEnabled;

				if(testEnabled)// ���� ����� ��������� ����
				{
					tree.Select(NIL);
					tree.SetMark(NIL);
				}
			}
			else if(_event.key == Keyboard::NUM_9)
			{
				tree.ClearAnimSeq();// �������� ������ ��������

				MagicCaller<void> caller;
				caller.Set(&tree, &MyTree::AddToAnimSeq);

				tree.PrefixTraverse(caller, false);

				tree.PlayAnim();// ��������� ������������ ��������
			}
			else if(_event.key == Keyboard::NUM_8)
			{
				tree.ClearAnimSeq();// �������� ������ ��������

				MagicCaller<void> caller;
				caller.Set(&tree, &MyTree::AddToAnimSeq);

				tree.PostfixTraverse(caller, false);

				tree.PlayAnim();// ��������� ������������ ��������
			}
			else if(_event.key == Keyboard::NUM_7)
			{
				tree.ClearAnimSeq();// �������� ������ ��������

				MagicCaller<void> caller;
				caller.Set(&tree, &MyTree::AddToAnimSeq);
		
				tree.InfixTraverse(caller, false);

				tree.PlayAnim();// ��������� ������������ ��������
			}
			else if(_event.key == Keyboard::NUM_6)
			{
				tree.Select(tree.Extremum(false));
			}
			else if(_event.key == Keyboard::NUM_5)
			{
				tree.Select(tree.Extremum(true));
			}
			else if(_event.key == Keyboard::P)
			{
				if(tree.IsPlaying()) { tree.StopAnim(); }
				else { tree.PlayAnim(); } 
			}
			else if(_event.key == Keyboard::C)
			{
				int32 key;
				if(tree.Marked())// ���� ���������� ���� ����������
				{
					// ��������� ���� ���� Marked 
					key = tree.Marked()->Key();
				}

				tree.Clear(tree.Selected(), MagicContainer::DELETE_NODE);// ������� ��������� � ������ Selected
				tree.Select(NIL);// �������� ���������� ����
 
				if(tree.Marked())// ���� ���������� ���� ����������
				{
					// ��������� �� Marked ����� ���� �� ��������, � ���� ���� ��� ��������������� ���� ��� ���� ������
					if(tree.Find(key, &MyNode::Key) == NIL)// ���� ���� � ����� ������ �� ����������, ������ ���� ��� ������
					{
						tree.SetMark(NIL);// �������� ���������� ����
					}
				}
				tree.ClearAnimSeq();
			}
			else if(_event.key == Keyboard::J)// ����������� ��������
			{
				tree.SetMark(NIL);
				tree.Select(NIL); 
				tree.ClearAnimSeq();

				treeCopy = tree;
				tree.Clear(MagicContainer::DELETE_NODE);
				tree.CheckTree(tree.Root(), &MyNode::Key);
			}
			else if(_event.key == Keyboard::K)// ����������� ��������
			{
				tree.SetMark(NIL);
				tree.Select(NIL);
				tree.ClearAnimSeq();
				
				tree = treeCopy;
				tree.CheckTree(tree.Root(), &MyNode::Key);
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
	def.realTimeWorkerDef.windowDef.tittle = "MagicBinaryTree.example_04";
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
