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
#include <Engine/Output/Graphics/Shape/Mesh/GraphicsMesh/GraphicsMesh.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Output/Graphics/Material/AnimatedMaterial/AnimatedMaterial.h>
#include <Engine/Core/Time/Timer/SyncTimer/SyncTimer.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/InputHandler/InputHandler.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Helpers/Sprite/AnimatedSprite/AnimatedSprite.h>
#include <Engine/Helpers/Sprite/TexturedSprite/TexturedSprite.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>


// �����, ����������� ����������� �����
class GraphicsPointEx: public TexturedSprite// ���������������� ������
{
	// DATA
	private: bool selected;// ���� true, �� ������� ��������

	// METHODS
	// ����������� ��� ����������
	public: GraphicsPointEx(void): selected(false)
	{}


	// �������� �����
	public: void Select(void)
	{
		selected = true;
		SetColor(RGBA(255, 0, 0, 255));
	}


	// ������� ��������� � �����
	public: void Unselect(void)
	{
		selected = false;
		SetColor(RGBA(255, 255, 255, 255));
	}


	// ���������� true ���� ����� ��������
	public: bool IsSelected(void)const
	{
		return selected;
	}
};

// �����, ����������� ������� ���� � ���������� ������������
class GraphicsQuad: public TexturedSprite// ���������������� ������
{
	// DATA
	private: Quad size;// ������ �����


	// METHODS
	// ����������� ��� ����������
    public: GraphicsQuad(void)
	{}


	// ���������������� �����������
	public: GraphicsQuad(Vector3D _leftBottom, Vector3D _rightTop)
	{
		size = Quad(_leftBottom, _rightTop);// ��������� ������
		CreateQuad2D(size);// ������� ����
	}


	// ����������
    public: virtual ~GraphicsQuad(void)
	{
		Destroying();
	}


	// ���������� ������ (������ � ������) ����� 
	public: Vector2D GetSize(void)const
	{
		return size.GetSize();
	}


	// ���������� true ���� ����� _point ��������� ������ �����
	public: bool IsPointInside(Vector3D _point)
	{
		// ������ ������� ����� ������ ��������� �� ����� �����; ���� ��� �� ���, �� ������ ������� ������� ��������������	
		return size.IsPointInside(_point);
	}

	// ������������ �����
	public: virtual void Draw(void)
	{
		if(IsShowEnabled())
		{
			// ���������� ������ �����...

			BeginTransform();
			TexturedSprite::Draw();
			GraphicsLine::_Draw(GetVertex(0), GetVertex(1), GetColor());
			GraphicsLine::_Draw(GetVertex(1), GetVertex(2), GetColor());
			GraphicsLine::_Draw(GetVertex(2), GetVertex(3), GetColor());
			GraphicsLine::_Draw(GetVertex(3), GetVertex(0), GetColor());
			EndTransform();
		}
	}

	// ������ ����� ������ ������� ��������������
	// ����������: ��������� ������� ����� ������������� �������� ����� �������, ����� ���� �������������� ���������� �������
	public: void SetVertex1(Vector3D _position)
	{
		size.SetVertex1(_position);

		// �������� ������� �����
		SetVertex(0, size.GetVertex1());
		SetVertex(1, size.GetVertex2());
		SetVertex(2, size.GetVertex3());
		SetVertex(3, size.GetVertex4());
	}

				
	// ������ ������ ������ ������� ��������������
	// ����������: ��������� ������� ����� ������������� �������� ����� �������, ����� ���� �������������� ���������� �������
	public: void SetVertex2(Vector3D _position)
	{
		size.SetVertex2(_position);

		// �������� ������� �����
		SetVertex(0, size.GetVertex1());
		SetVertex(1, size.GetVertex2());
		SetVertex(2, size.GetVertex3());
		SetVertex(3, size.GetVertex4());
	}

			
	// ������ ������ ������� ������� ��������������
	// ����������: ��������� ������� ����� ������������� �������� ����� �������, ����� ���� �������������� ���������� �������
	public: void SetVertex3(Vector3D _position)
	{
		size.SetVertex3(_position);

		// �������� ������� �����
		SetVertex(0, size.GetVertex1());
		SetVertex(1, size.GetVertex2());
		SetVertex(2, size.GetVertex3());
		SetVertex(3, size.GetVertex4());
	}

			
	// ������ ����� ������� ������� ��������������
	// ����������: ��������� ������� ����� ������������� �������� ����� �������, ����� ���� �������������� ���������� �������
	public: void SetVertex4(Vector3D _position)
	{
		size.SetVertex4(_position);

		// �������� ������� �����
		SetVertex(0, size.GetVertex1());
		SetVertex(1, size.GetVertex2());
		SetVertex(2, size.GetVertex3());
		SetVertex(3, size.GetVertex4());
	}
};



// ���������������� ����������
class Application: public Engine,// ������
         	       public MessageHandler<int32>::Receiver// ���������� ���������
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D *font;// �����
	private: Scene* scene;// �����

	private: GraphicsPointEx* fantomVertex;// ��������� �������
	private: GraphicsLine* fantomLine;// ��������� �����

	private: VectorLerp* vectorLerp;// ������������ �������
	private: MagicList<GraphicsPointEx> vertices;// ��������� �������
	private: Texture2D* texture;// �������� �����
	private: GraphicsPointEx* lockedVertex;// ������������ �������
    private: TexturedSprite* clenchedHandCursor;// ������ "������ ����"
	private: TexturedSprite* unclenchedHandCursor;// ������ "�������� ����"
	private: bool showControlVertices;// ���� true, �� ���������� ����� ����� ������������ ���������
	private: GraphicsQuad* fantomQuad;// ��������� ����
	private: float step;// ��� �������� ���������� ������ ��� �� �����������
	
 


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font(NIL), scene(NIL), vectorLerp(NIL), fantomVertex(NIL), fantomLine(NIL), texture(NIL), lockedVertex(NIL), clenchedHandCursor(NIL), unclenchedHandCursor(NIL), showControlVertices(true), fantomQuad(NIL), step(0.009f)
	{
		vertices.SetClearMode(MagicContainer::DELETE_NODE);
	}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete camera;
		delete font;

		delete fantomVertex;
		delete fantomLine;

		delete vectorLerp;
		delete texture;

		delete clenchedHandCursor;
		delete unclenchedHandCursor;
		
		delete scene;
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
				AssetLibrary::_GetAssets().AddObject(scene);// �������� ����� � ���������� �������, ����� �� ����������
				scene->Pick();
			}

			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.fontName = "Arial";// ��� ������
				def.fontSize = 16;// ������ ������
				def.fontColor = RGBA(RGBA::WHITE_COLOR);// ���� ������

				font = new Font2D();
				font->Create(def);
				font->Pick();// ������� �����
			}
	


			// ������...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::TRACKBALL_FIRST_PERSON;
				def.rotationEnabled = false;// ��������� �������� ������
			
				camera = new Camera3D();
				camera->Create(def);
				camera->Pick(); // ������� ������
			}
		

			
			{// vectorLerp
				vectorLerp = new VectorLerp();
				vectorLerp->SetAmountOfSegment(30);
				//scene->AddLogicObject(vectorLerp);
			}


			// ������ "�������� ����"...
			{
				unclenchedHandCursor = new TexturedSprite();
				unclenchedHandCursor->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
				unclenchedHandCursor->SetTexturedMaterialEnable(true);
				unclenchedHandCursor->BindTexture(AssetLibrary::_LoadCommonAsset<Texture2D>("unclenched"));

				unclenchedHandCursor->SetShowEnable(true);// �������� �����������
				scene->AddGraphicsObject(unclenchedHandCursor);// �������� � ����������� �����
			}


			// ������ "������ ����"...
			{
				clenchedHandCursor = new TexturedSprite();
				clenchedHandCursor->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
				clenchedHandCursor->SetTexturedMaterialEnable(true);
				clenchedHandCursor->BindTexture(AssetLibrary::_LoadCommonAsset<Texture2D>("clenched"));

				clenchedHandCursor->SetShowEnable(true);// �������� �����������
				scene->AddGraphicsObject(clenchedHandCursor);// �������� � ����������� �����
			}

			// ��������� ����...
			{
				fantomQuad = new GraphicsQuad(Vector3D(0.10f, 0.10f, -0.998f), Vector3D(-0.10f, -0.10f, -0.998f));
				fantomQuad->SetColor(RGBA(0, 255, 0, 50));// ���� �������
				Texture2D* texture = new Texture2D();
				texture->Create(Vector2D(8, 8));
				fantomQuad->SetTexturedMaterialEnable(true);
				fantomQuad->BindTexture(texture);
				fantomQuad->SetShowEnable(false);// ��������� �����������
				//scene->AddGraphicsObject(fantomQuad);// �������� � ����������� �����
			}

			// ��������� �������...
			{
				texture = AssetLibrary::_LoadCommonAsset<Texture2D>("point");
				fantomVertex = new GraphicsPointEx();
				fantomVertex->CreateQuad2D(Quad(Vector3D(), Vector2D(0.010f, 0.010f)));
				fantomVertex->SetTexturedMaterialEnable(true);
				fantomVertex->BindTexture(texture);

				//fantomVertex->SetColor(RGBA(255,0,0,255));// ���� �������
				//GraphicsPointEx::_SetPointSize(6.0f);// ������ ������ �����
				fantomVertex->SetShowEnable(false);// ��������� �����������
				scene->AddGraphicsObject(fantomVertex);// �������� � ����������� �����
			}
						
			// ��������� �����...
			{
				fantomLine = new GraphicsLine();
				
				// ���� �����
				fantomLine->SetColor(RGBA(234, 170, 68, 90));// ���� �����
				GraphicsLine::_SetLineWidth(2.0f);// ������ ������� �����
				fantomLine->SetShowEnable(false);// ��������� �����������
				scene->AddGraphicsObject(fantomLine);// �������� � ����������� �����
			}

			Keyboard().SetKeyPressedPeriodInSec(0.08f);


			// ���� ����...
			SetBackgroundColor(RGBA(67, 67, 67, 255));
			
			Mouse::_ShowCursor(true);// �������� ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}



	// ������� ����������
	public: virtual void Update(float _delta)
	{
		Engine::Update(_delta);
	}



	// ������� ������������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����

		if(Camera3D::_GetPicked())// ���� ���� ���������� ������
		{
			Camera3D::_GetPicked()->BeginPerspective3D();// ����� ���������� �������
		}

		// ��������������� �������� �����
		if(showControlVertices)
		{
			for(SimpleList<Vector3D>::Node* it = vectorLerp->GetControlVertices().GetHead(); it != NIL; it = it->GetRight())
			{
				if(it->GetRight())
				{
					GraphicsLine::_Draw(it->key + Vector3D(0, 0, -0.002), it->GetRight()->key + Vector3D(0, 0, -0.002), RGBA(234, 170, 68, 255));
				}
			}
		}
		

		// ��������������� ����������������� �����
		for(SimpleList<Vector3D>::Node* it = vectorLerp->GetCurveVertices().GetHead(); it != NIL; it = it->GetRight())
		{
			if(it->GetRight())
			{
				GraphicsLine::_Draw(it->key + Vector3D(0, 0, -0.001), it->GetRight()->key + Vector3D(0, 0, -0.001), RGBA(0, 255, 0, 255));
			}
		}

		

		if(Scene::_GetPicked())// ���� ���� ������� �����
		{
			Scene::_GetPicked()->Draw();// ���������� ������� �����
		}

		//Engine::Draw(_delta);
					

		camera->BeginScreen2D();

		
		// ����� ������...
		int32 i = 0;

		
		font->DrawScreenText(Vector3D(10, i+=20, -1), "amount of segment: %d", vectorLerp->GetAmountOfSegment());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "amount of control vertices: %d", vectorLerp->GetControlVertices().GetSize());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "amount of curve vertices: %d", vectorLerp->GetCurveVertices().GetSize());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "lerp mode: %s", vectorLerp->GetLerpMode() == VectorLerp::BAZIER ? "BAZIER" : "LAGRANGE");
	}


			
	// ������ ������� ����
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		// ��������� ������� ������� � ������� �����������...
		Vector3D worldPos;
		camera->SetProjectionType(Camera3D::PERSPECTIVE_PROJECTION);

		Vector3D winPos = Vector3D(Mouse::_CursorPosition(this).x, Mouse::_CursorPosition(this).y, 0.9);// ������ �� ������� ��������� ���������
		winPos.y = ClientAreaSize().y - Mouse::_CursorPosition(this).y - 1; // ������������� ��� y; ������� ������� ������������ � �������� ������� ���������, ��� ��� Y ������ ������ ����

		camera->UnProject(winPos, worldPos);//�������� ������� ���������� �������
					

		if(_event->type == Mouse::KEY_PRESSED)// ���� ��������� ������� ������
		{
			if(_event->key == Mouse::LEFT_BUTTON)// ���� ����� ����� ����
			{
				if(lockedVertex == NIL)// ���� ��� ������������ �������
				{
					// ��������� ������� ������� ����� �������...
					if(!fantomVertex->IsShowEnabled())// ���� �� ��������� ����� �������
					{
						Vector3D intersectionPoint;// ����� ����������� ������� � ����� �� ������� ������ ������

						// ���� ������ ��������� �� ���� �� ������������ ������...
						for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
						{
							if(it->GetNode()->IsUnderCursor(intersectionPoint) != -1)// ���� ������� ������������ � ��������
							{
								// ������ ����������� �������...
								lockedVertex = it->GetNode();// ��������� ������������ �������
								
								// �������� ������������������ ������...	
								unclenchedHandCursor->SetShowEnable(false);// ��������� �����������
								scene->GetDrawList().Remove(unclenchedHandCursor, MagicContainer::REMOVE_NODE);// ��������� �� ����������� �����
								
								if(Mouse::_IsShowCursor()) { Mouse::_ShowCursor(false); }// ������ ������ 
								clenchedHandCursor->SetPosition(Vector3D(worldPos.x, worldPos.y, -0.995f));// �������� �������
								
								clenchedHandCursor->SetShowEnable(true);// �������� �����������
								scene->AddGraphicsObject(clenchedHandCursor);// �������� � ����������� �����
								// �������� ��������� �����...
								//fantomLine->SetShowEnable(false);// ��������� �����������
										
								// �������� ��������� �������...
								//fantomVertex->SetShowEnable(false);// ��������� �����������
								break;// �������� ����
							}
						}
					}

					// ������ �������� ����� �������...
					if(lockedVertex == NIL && !fantomVertex->IsShowEnabled())// ����� ��������� ���� � ������ ������� � ���� �� ��������� ����� �������
					{
						if(!unclenchedHandCursor->IsShowEnabled())// ���� ��� ������ �� ������� (������������� � �������)
						{
							// �������� ������� ��������� �������...
							fantomVertex->SetPosition(Vector3D(worldPos.x, worldPos.y, -1.0f));
							fantomVertex->SetShowEnable(true);// �������� �����������

							// ���������� ��������� �����...
							if(vectorLerp->GetControlVertices().IsNotEmpty())// ���� ���� ������� �� �����
							{
								// ���������� ��������� ����� ����� ��������� �������� ������ � ��������� ��������
								fantomLine->Set(vectorLerp->GetControlVertices().GetTail()->key, fantomVertex->GetPosition());// ������� ��������� �����
								fantomLine->SetShowEnable(true);// �������� �����������
							}
						}
					}
				}
			}
			else if(_event->key == Mouse::RIGHT_BUTTON)// ������ ���������
			{
				if(!fantomQuad->IsShowEnabled())// ��������� ������ ���� ���
				{
					// �������� ��������� ����...
					fantomQuad->SetShowEnable(true);// ����������
					scene->AddGraphicsObject(fantomQuad);// �������� � ����������� �����

					fantomQuad->SetVertex4(Vector3D(worldPos.x, worldPos.y, -0.998f));// ������ ����� ������� �������	
					fantomQuad->SetVertex2(Vector3D(worldPos.x, worldPos.y, -0.998f));// ������ ������ ������ �������
				} 
			}
		}
		else if(_event->type == Mouse::KEY_UP)// ����� ������ ��������
		{
			if(_event->key == Mouse::LEFT_BUTTON)
			{
				if(lockedVertex)// ���� �� ���������� �������
				{
					// ������ ������������������ ������...
					clenchedHandCursor->SetShowEnable(false); // ��������� �����������
					scene->GetDrawList().Remove(clenchedHandCursor, MagicContainer::REMOVE_NODE);// ��������� �� ����������� �����
								
					unclenchedHandCursor->SetPosition(Vector3D(worldPos.x, worldPos.y, -0.995f));// �������� ������� 
					unclenchedHandCursor->SetShowEnable(true);// �������� �����������
					scene->AddGraphicsObject(unclenchedHandCursor);// �������� � ����������� �����
					lockedVertex = NIL;// ��������� ����������� �������
				}
				else// ����� �� ������� ����� �������
				{
					// ����� �������� ����� �������...

					// �������� ��������� �����...
					fantomLine->SetShowEnable(false);// ��������� �����������
							
					// �������� ��������� �������...
					fantomVertex->SetShowEnable(false);// ��������� �����������

					// �������� ������...
					if((vectorLerp->GetLerpMode() == VectorLerp::BAZIER && vectorLerp->GetControlVertices().GetSize() < 10) || vectorLerp->GetLerpMode() == VectorLerp::LAGRANGE)// ������ ������������ �� 10 ������ ��� ������ BAZIER
					{
						// ������� �������� ������� �� ���� ���������
						if(vectorLerp->GetControlVertices().AddUniqueToTail(fantomVertex->GetPosition()))
						{					
							// ������� ����������� ����������� �������...
							GraphicsPointEx* vertex = new GraphicsPointEx();
							vertex->CreateQuad2D(Quad(Vector3D(), Vector2D(0.010f, 0.010f)));
							vertex->SetPosition(fantomVertex->GetPosition());
							vertex->SetTexturedMaterialEnable(true);
							vertex->BindTexture(texture);
							scene->AddGraphicsObject(vertex);// �������� � ����������� �����
							vertices.AddToTail(vertex);

							vectorLerp->Build();// ����������� ������
						}
					}
				}
			}
			else if(_event->key == Mouse::RIGHT_BUTTON)// ����� ���������
			{
				if(fantomQuad->IsShowEnabled()) // ���� �������� ����
				{
					// �������� ��������� ����...
					fantomQuad->SetShowEnable(false);// ������
					scene->GetDrawList().Remove(fantomQuad, MagicContainer::REMOVE_NODE);// ��������� �� ����������� �����

					// ��������� ��� ����������� ������� �� ������� ����������� � ��������� ������...
					for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
					{
						if(fantomQuad->IsPointInside(it->GetNode()->GetPosition()))
						{
							it->GetNode()->Select(); // �������� �����
						}
						else
						{
							it->GetNode()->Unselect();// ����� ���������
						}
					}
				}
			}
		}
		else if(_event->type == Mouse::MOVE)// ���� ������ ��������
		{
			if(fantomQuad->IsShowEnabled())// ���� �������� ����
			{
				fantomQuad->SetVertex2(Vector3D(worldPos.x, worldPos.y, -0.998f));// ������ ������ ������ �������

				if(fantomQuad->IsClockwise())// ���� ������� ����������� �� ������� �������
				{ 
					fantomQuad->InverseVertices();// �������� ����������� ������
				}

				// ��������� ��� ����������� ������� �� ������� ����������� � ��������� ������...
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					if(fantomQuad->IsPointInside(it->GetNode()->GetPosition()))
					{
						it->GetNode()->Select(); // �������� �����
					}
					else
					{
						it->GetNode()->Unselect();// ����� ���������
					}
				}
				
				if(lockedVertex == NIL)// ���� ������ �� ��������� �� ������� �������, � ���� ������� �� ������������
				{
					unclenchedHandCursor->SetShowEnable(false);// ��������� �����������
					if(!Mouse::_IsShowCursor()) { Mouse::_ShowCursor(true); }// �������� ������ 
				}
			}
			else if(lockedVertex)// ���� ���������� �������
			{
				// �������� ������... 
				lockedVertex->SetPosition(Vector3D(worldPos.x, worldPos.y, -1.0f));// �������� ������� ������������ �������
				int32 index = dynamic_cast<MagicList<GraphicsPointEx>::AddressDerived*>(lockedVertex->GetAddress(&vertices))->GetIndex();// ����� ������ ����������� ������� � ������; ���� ������ ��������� � �������� ������� ������� � ������������� vectorLerp
				vectorLerp->GetControlVertices()[index]->key = lockedVertex->GetPosition();// �������� ������� ������� �������
				vectorLerp->Build();// ����������� ������

					
				// �������� ������� ������������������� �������...
				clenchedHandCursor->SetPosition(Vector3D(worldPos.x, worldPos.y, -0.995f));// �������� �������
			}
			else if(lockedVertex == NIL && !fantomVertex->IsShowEnabled())// ���� ��� ������������ ������� � ���� �� ��������� ����� �������
			{
				bool collision = false;

				// ��������� ��� ������� ������� �� ������� ����������� � ��������...
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					Vector3D intersectionPoint;// ����� ����������� ������� � ����� �� ������� ������ ������

					// ���� ������ ��������� �� ���� �� ������������ ������...
					if(it->GetNode()->IsUnderCursor(intersectionPoint) != -1)// ���� ������� ������������ � ��������
					{
						collision = true; 

						// �������� ������������������ ������...
						if(Mouse::_IsShowCursor()) { Mouse::_ShowCursor(false); }// ������ ������ 

						unclenchedHandCursor->SetPosition(Vector3D(worldPos.x, worldPos.y, -0.995f));// �������� ������� 
						unclenchedHandCursor->SetShowEnable(true);// �������� �����������
						break;// �������� ����
					}
				}

				if(!collision && lockedVertex == NIL)// ���� ������ �� ��������� �� ������� �������, � ���� ������� �� ������������
				{
					unclenchedHandCursor->SetShowEnable(false);// ��������� �����������
					if(!Mouse::_IsShowCursor()) { Mouse::_ShowCursor(true); }// �������� ������ 
				}
			}
			else if(lockedVertex == NIL)// ���� ��������� ����� �������
			{
				// �������� ������� ��������� �������...
				fantomVertex->SetPosition(Vector3D(worldPos.x, worldPos.y, -1.0f));

				// ���������� ��������� �����...
				if(vectorLerp->GetControlVertices().IsNotEmpty())// ���� ���� ������� �� �����
				{
					// ���������� ��������� ����� ����� ��������� �������� ������ � ��������� ��������
					fantomLine->Set(vectorLerp->GetControlVertices().GetTail()->key, fantomVertex->GetPosition());// ������� ��������� �����
				}
			}
		}

		return Engine::MouseEventFilter(_event);
	}


	// ��������� ������� ����������
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event)
	{
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
			else if(_event->key == Keyboard::C)
			{
				vectorLerp->GetControlVertices().Clear();
				vectorLerp->GetCurveVertices().Clear();
				vertices.Clear();
				lockedVertex = NIL;// �������� ������������ �������
			}
			else if(_event->key == Keyboard::S)
			{
				showControlVertices = !showControlVertices;
			}
			else if(_event->key == Keyboard::D)
			{
				MagicList<GraphicsPointEx> deletionList;// ������ ��������

				// ������� ��� ��������� ����������� �������...
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetTailAddress(); it != NIL; it = it->GetLeft())// ���� � ����� � ������, �.�. � �������� ������ ���������� ��� ����������� ������ ����� ���������� �� �������� ������������ ������ ����������� ������
				{
					if(it->GetNode()->IsSelected())// ���� ������� ��������
					{
						int32 index = it->GetIndex();// ����� ������ �������
						vectorLerp->GetControlVertices().Remove(vectorLerp->GetControlVertices()[index]);// ������� ����������� ������� �� �������������
						deletionList.AddToTail(it->GetNode());// �������� � ������ ��������
					}
				}
				deletionList.Clear(MagicContainer::DELETE_NODE);

				vectorLerp->Build();// ����������� ������
			}
			if(_event->key == Keyboard::NUM_1)
			{
				vectorLerp->SetLerpMode(VectorLerp::BAZIER);
				vectorLerp->Build();// ����������� ������
			}
			if(_event->key == Keyboard::NUM_2)
			{
				vectorLerp->SetLerpMode(VectorLerp::LAGRANGE);
				vectorLerp->Build();// ����������� ������
			}
		}
		else if(_event->type == Keyboard::KEY_PRESSED)
		{
			if(_event->key == Keyboard::MINUS)
			{
				vectorLerp->SetAmountOfSegment(vectorLerp->GetAmountOfSegment() - 1);
				
				vectorLerp->Build();// ����������� ������
			}
			else if(_event->key == Keyboard::EQUALS)
			{
				vectorLerp->SetAmountOfSegment(vectorLerp->GetAmountOfSegment() + 1);
				
				vectorLerp->Build();// ����������� ������
			}
			else if(_event->key == Keyboard::LEFT_ARROW)
			{
				// ����������� ��� ���������� ������� �� ���� ��� ����� �� ��� X
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					if(it->GetNode()->IsSelected())// ���� ������� ��������
					{
						int32 index = it->GetIndex();// ����� ������ �������
						vectorLerp->GetControlVertices()[index]->key = vectorLerp->GetControlVertices()[index]->key - Vector3D(step, 0.0f, 0.0f);// �������� ����������� �������
						it->GetNode()->SetPosition(it->GetNode()->GetPosition() - Vector3D(step, 0.0f, 0.0f));
					}
				}
				vectorLerp->Build();// ����������� ������
			}
			else if(_event->key == Keyboard::RIGHT_ARROW)
			{
				// ����������� ��� ���������� ������� �� ���� ��� ������ �� ��� X
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					if(it->GetNode()->IsSelected())// ���� ������� ��������
					{
						int32 index = it->GetIndex();// ����� ������ �������
						vectorLerp->GetControlVertices()[index]->key = vectorLerp->GetControlVertices()[index]->key + Vector3D(step, 0.0f, 0.0f);// �������� ����������� �������
						it->GetNode()->SetPosition(it->GetNode()->GetPosition() + Vector3D(step, 0.0f, 0.0f));
					}
				}
				vectorLerp->Build();// ����������� ������
			}
			else if(_event->key == Keyboard::UP_ARROW)
			{
				// ����������� ��� ���������� ������� �� ���� ��� ����� �� ��� Y
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					if(it->GetNode()->IsSelected())// ���� ������� ��������
					{
						int32 index = it->GetIndex();// ����� ������ �������
						vectorLerp->GetControlVertices()[index]->key = vectorLerp->GetControlVertices()[index]->key + Vector3D(0.0f, step, 0.0f);// �������� ����������� �������
						it->GetNode()->SetPosition(it->GetNode()->GetPosition() + Vector3D(0.0f, step, 0.0f));
					}
				}
				vectorLerp->Build();// ����������� ������
			}
			else if(_event->key == Keyboard::DOWN_ARROW)
			{
				// ����������� ��� ���������� ������� �� ���� ��� ���� �� ��� Y
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					if(it->GetNode()->IsSelected())// ���� ������� ��������
					{
						int32 index = it->GetIndex();// ����� ������ �������
						vectorLerp->GetControlVertices()[index]->key = vectorLerp->GetControlVertices()[index]->key - Vector3D(0.0f, step, 0.0f);// �������� ����������� �������
						it->GetNode()->SetPosition(it->GetNode()->GetPosition() - Vector3D(0.0f, step, 0.0f));
					}
				}
				vectorLerp->Build();// ����������� ������
			}
		}


		return Engine::KeyboardEventFilter(_event);
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "VectorLerp.example_01";
	//def.rendererDef.zBufferEnabled = false;
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
