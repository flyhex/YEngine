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



// �����, ����������� ���������� �������������� �����������
class MyPixelImage: public PixelImage// ���������� �����������
{
	// METHODS
	// ������� ����������� ��������� ����� �������� _size
	public: void CreateCheckDeskImage(Size2D _size)
	{
		if(Resize(_size, RGBA()))// ������� ���������� ����������� ������� �������
		{
			// ��� ������� �������
			for(int32 i = 0; i < _size.height; i++) 
			{ 
				// ��� ������ ������
				for(int32 j = 0; j < _size.width; j++) 
				{ 
					/* ������ �������� ������ ����, ����� ������� */ 
					int32 c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255; // �������� ��� RGB ���������
					RGBA color; color.SetRGBAb(c,c,c,255); 
					Pixel(Vector2D(j, i)) = color;
				} 
			} 
		}
	}
			
	// ������� ����������� �� ���������� ������� �������� _size
	public: void CreateRandomImage(Size2D _size)
	{
		Resize(_size, RGBA());// ������� ���������� ����������� ������� �������

		// ��� ������� �������
		for(int32 i = 0; i < _size.height; i++) 
		{ 
			// ��� ������ ������
			for(int32 j = 0; j < _size.width; j++) 
			{ 
				/* ������ �������� ������ ����, ����� ������� */ 
				Pixel(Vector2D(j, i)) = Color<RGBA>::_Random(5, 200);
			} 
		} 
	}
};



// �����, ����������� ���������� ����������� ���� (������� �������������)
class Quad2D: public GraphicsMesh<Vector3D, Quaternion, Matrix4x4>,// ���������� ����������� ������������� �����
	          public TexturedMaterial<Vector3D>// ���������������� ��������
{
	// METHODS
	// ����������� �� ���������
	public: Quad2D(void)
	{
		GraphicsMesh::Define def;
		def.meshDef.name = "quad";
		Create(def);
	}


	// ���������� ������������� ����� �� ����� 
	public: virtual void Draw(void)
	{
		BeginTransform();
		GraphicsMesh::Draw();
		EndTransform();
	}


};


// �����, ����������� ���������������� ����
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: Scene* scene;// ����������� �����
	private: Quad2D* quad;// ���������� ����������� ����
	private: Texture2D *texture1, *texture2, *texture3;// ��������� �� ��������



    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), quad(NIL), texture1(NIL), texture2(NIL), texture3(NIL)
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
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;
				def.textColor = RGBA(RGBA::WHITE_COLOR);

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d);
				font2d->Pick();
			}


			// ��������...
			{
				texture1 = new Texture2D();

				texture1->Create(Size2D(64, 64));
				texture1->Rename("texture1");

				MyPixelImage image;
				image.CreateCheckDeskImage(Size2D(64, 64));

				texture1->SetImage(Vector2D(), image);
				scene->AddObject(texture1);// �������� � �����	
			}
			

			// ��������...
			{
				texture2 = new Texture2D();

				texture2->Create(Size2D(68, 64));
				texture2->Rename("texture2");

				MyPixelImage image;
				image.CreateRandomImage(Size2D(68, 64));

				texture2->SetImage(image);
 


				scene->AddObject(texture2);// �������� � �����	 
			}

		
			// ��������...
			{
				texture3 = new Texture2D();

				texture3->Create(Size2D(76, 64));
				texture3->Rename("texture3");

				MyPixelImage image;
				image.LoadImageFromPNGFile("image.png");

				texture3->SetImage(image);
 
				scene->AddObject(texture3);// �������� � �����	 
			}
	

						
			// ������������� �����...
			{
				// ������� ������������� �����, ����������� ����
				quad = new Quad2D();  
				quad->CreateQuad2D(Quad<Vector3D>(Vector3D(),Size2D(1,1)));

				// ���������� ��������
				quad->BindTexture(texture1);// ��������� �������� texture � ������������� �����
				//quad->ComputeTexelsForQuadShape();// ��������� ������� ��� �����
				quad->SetTexturedMaterialEnable(true);// �������� ��������
	
				//quad->SetColor(RGBA::RED_COLOR);// ���� �������
				scene->AddObject(quad);// �������� � �����	 
				scene->AddGraphicsObject(quad);
			}
			
						
			// ������...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// ����� �������
				def.viewingPoint = quad->GetPosition();
				def.zoomMin = 2;
				def.zoomMax = 10;


				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera);
				camera->Pick(Title()); 
								
				// ������ �������������� ������ ������
				//camera->UpdateTrackballObjectViewer(-Angle::HALF_PI/3.0f, Angle::HALF_PI/3.0f, 0.0f);
			}

			//AUX_RGBImageRec* tex = auxDIBImageLoad("phoenix.bmp"); // ��������� ����������� ��� ������ ������ �� ���� ���������: ��������, �������� � ������
 

			SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));// ���� ����
			

			MoveToCenter();// ����������� ���� � ����� �����
			Mouse::_MoveCursorToCenter(this);// ����������� ������ � ����� ���������� ������� ����	
			Mouse::_ShowCursor(false);// ������ ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}




		


	// ������� �����
	public: virtual void Update(float _delta)
	{
	}

	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����


		camera->BeginPerspective3D();

		scene->Draw();
	
		// ����� ������...
		camera->BeginScreen2D();// ������ ����� ���������� �������

		int32 i = 0;
		
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.position: %.2f/%.2f/%.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.scale(x,y): %.2f/%.2f", quad->GetScale().x, quad->GetScale().x); 


		if(quad->GetTexture())
		{
			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.texture.sizeOfTexture: %.2f/%.2f", quad->GetTexture()->TextureSize().width, quad->GetTexture()->TextureSize().height); 
		
			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.texture.IsExist: %d", quad->GetTexture()->IsExist()); 
			//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.texture.IsAutoDestroy: %d", quad->GetTexture()->IsAutoDestroyEnabled()); 
			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.texture.TextureMaxSize: %d", Texture2D::_TextureMaxSize()); 

			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.texture.SizeOfImage: %.2f/%.2f", quad->GetTexture()->ImageSize().width, quad->GetTexture()->ImageSize().height); 
		}

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.AmountOfResources: %d", quad->AmountOfResources()); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quad.TexturedMaterial.Enable: %d", quad->IsTexturedMaterialEnabled()); 

	

		
		//int32 val;
		

		//uint64 uu = 0;
		//Bitset::_SetBit(uu, 0, 1);
	
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
		//camera->End2D();// ������ ����� ���������� �������
	}


	// ������ ������� ����
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Mouse().ClearEventList();// �������� ����� �������
		return _event;
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
				SetWindowStyle(Window::WND_CLASSIC);
			}
			else if(_event->key == Keyboard::ESC)
			{
				Destroy();// ���������� ����
			} 
			else if(_event->key == Keyboard::D)
			{
				delete quad->GetTexture();
			}
			else if(_event->key == Keyboard::N)
			{
				quad->BindTexture(NIL);
			} 
			else if(_event->key == Keyboard::LEFT_ARROW)
			{
				quad->SetScale(Vector3D(quad->GetScale().x - 0.01, quad->GetScale().y, quad->GetScale().z));
			}
			else if(_event->key == Keyboard::RIGHT_ARROW)
			{
				quad->SetScale(Vector3D(quad->GetScale().x + 0.01, quad->GetScale().y, quad->GetScale().z));
			}
			else if(_event->key == Keyboard::UP_ARROW)
			{
				quad->SetScale(Vector3D(quad->GetScale().x, quad->GetScale().y + 0.01, quad->GetScale().z));
			}
		    else if(_event->key == Keyboard::DOWN_ARROW)
			{
				quad->SetScale(Vector3D(quad->GetScale().x, quad->GetScale().y - 0.01, quad->GetScale().z));
			}
			else if(_event->key == Keyboard::NUM_1)
			{
				quad->BindTexture(texture1);// ��������� �������� 
				//quad->ComputeTexelsForQuadShape();// ��������� ������� ��� �����
			}
			else if(_event->key == Keyboard::NUM_2)
			{
				quad->BindTexture(texture2);// ��������� �������� 
				//quad->ComputeTexelsForQuadShape();// ��������� ������� ��� �����
			}
			else if(_event->key == Keyboard::NUM_3)
			{
				quad->BindTexture(texture3);// ��������� �������� 
				//quad->ComputeTexelsForQuadShape();// ��������� ������� ��� �����
			}
			else if(_event->key == Keyboard::W)
			{
				quad->GetTexture()->SetPixel(Vector2D(0,0), RGBA(RGBA::RED_COLOR));// ����� ������ ����
			    quad->GetTexture()->SetPixel(Vector2D(0,quad->GetTexture()->ImageSize().height-1), RGBA(RGBA::RED_COLOR));// ����� ������� ����

				quad->GetTexture()->SetPixel(Vector2D(quad->GetTexture()->ImageSize().width-1,quad->GetTexture()->ImageSize().height-1), RGBA(RGBA::RED_COLOR));// ������ ������� ����
				quad->GetTexture()->SetPixel(Vector2D(quad->GetTexture()->ImageSize().width-1, 0), RGBA(RGBA::RED_COLOR));// ������ ������ ����
			}
			else if(_event->key == Keyboard::E)
			{
				for(int32 i = 0; i < quad->GetTexture()->ImageSize().height; ++i)
				{
					quad->GetTexture()->SetPixel(Vector2D(quad->GetTexture()->ImageSize().width/2, i), RGBA(RGBA::INVISIBLE_COLOR));
				}
			}
			else if(_event->key == Keyboard::R)
			{
				PixelImage img(Size2D(16,16), RGBA(RGBA::GREEN_COLOR));
				quad->GetTexture()->SetImage(Vector2D(0,0), img);
			}
			else if(_event->key == Keyboard::T)
			{
				// �������� ������ �������� � ������
				PixelImage image;
				texture1->GetImage(image);	
				texture3->SetImage(Vector2D(), image);
			}

		}

		Keyboard().ClearEventList();// �������� ����� �������
		return _event;
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
	def.realTimeWorkerDef.windowDef.tittle = "Texture2D.example_01";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;
	//def.faceDrawMode[Renderer::BACK] = Renderer::LINE;
	//def.faceCullMode[Renderer::FRONT] = false;
	//def.faceCullMode[Renderer::BACK] = false;
	
//	def.blendEnable = true;// �������� ����������
	
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
