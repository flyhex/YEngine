// Win32.cpp: ���������� ����� ����� ��� ����������.
//


#include "Win32.h"
#include <Engine/Output/File/File.h>
#include <Engine/Math/Vector/Vector2D/Vector2D.h>


#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <glaux.h>// ��� �������

#include <Engine/Math/Ratio/Ratio.h>

#include <Engine/Output/Graphics/Shape/Box/GraphicsBox3DFixed/GraphicsBox3DFixed.h> 
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>

#include <Engine/Output/Graphics/GraphicsScene/GraphicsScene.h>


// �����, ����������� ����������
class Application: public Renderer
{
	// DATA
	private: float zoom; // ���������� �� ����������� �� ����
	private: float spinX, spinY, spinZ;// ���� �������� ���� �� ���� X,Y,Z
	private: float step;// ������ ���� ���� ��������
    
	private: GLuint filter; // ������ �������
	private: GLuint texture[3]; // �������������� �������

   
	private: float fov;// �������� FOV ��� �����������
	private: float aspect; // ������ �����������
	private: float zFar, zNear;// ������� � ������� ��������� ���������

	private: GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion, Size3D>* box;// ���������� �������������, ����������� �������
	private: Camera3D* camera;// ������
	private: GraphicsScene<Vector3D, Quaternion, Size3D>* scene;// ����������� �����
	private: Font2D* font2d;// �����


    // METHODS
    // ���������������� �����������
	public: Application(void): zoom(-10.0f), filter(0), spinX(0.0f), spinY(0.0f), spinZ(0.0f), step(15.0f), fov(0.0f), aspect(0.0f), zFar(0.0f), zNear(0.0f), box(NIL), scene(NIL), camera(NIL), font2d(NIL)
	{}


	// ����������� ����������
	public: virtual ~Application(void)
	{
		delete scene;
	}

	// ������� ����������� ����
	public: bool Create(Renderer::Define _define)
	{
		// ������� ����������� ����
		if(Renderer::Create(_define))
		{
			// ��������� ��������
			LoadTexture();

			
			// �������� ������� �������� (���������������� �� ��������)
            //glShadeModel(GL_SMOOTH);
			

			// �����...
			{
				GraphicsScene<Vector3D, Quaternion, Size3D>::Define def;
				def.logicSceneDef.scale = Size3D(1, 1, 1);
				
				scene = new GraphicsScene<Vector3D, Quaternion, Size3D>();
				scene->Create(def);
			}

			// ���...
			{
				GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion, Size3D>::Define def;
				def.box = Box3D(Vector3D(0, 0, 0), Size3D(2, 2, 2));
				def.graphicsMeshDef.name = "box3d";
				
				box = new GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion, Size3D>();
				box->Create(def);
				

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
				scene->AddObject(box, true, true);// �������� � �����
			} 

			
			// ������...
			{
				Camera3D::Define def;
				zoom = 13.0f;
				def.position = Vector3D(0.0f, 0.0f, zoom);
				def.name = "camera";

				camera = new Camera3D();
				camera->Create(def);
							
				fov = camera->FOV();
				aspect = camera->Aspect();
				zFar = camera->ZFar();
				zNear = camera->ZNear();
				scene->AddObject(camera, true, false);
				camera->ConnectTo(ResizeEvent::TYPE, this);// ����������� �� ������� ��������� �������� ����
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
			}
			Flush();// ��������� ���������� �������
			return true;// ������� �����
		} 
		// ����� ������� �������
		return false;
	}
	
	// ��������� �������� ��� �������� ������
	private: void LoadTexture(void)
	{
		AUX_RGBImageRec* tex = auxDIBImageLoad("phoenix.bmp"); // ��������� ����������� ��� ������ ������ �� ���� ���������: ��������, �������� � ������
 
		glGenTextures(3, &texture[0]);// ������� ��� ��������
				
		// �������� ������ ��������
		glBindTexture(GL_TEXTURE_2D, texture[0]); // ���������� ��������
				
				
		// ���������� �������; �� ������������ ����������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	 
		glTexImage2D(GL_TEXTURE_2D, 
					 0, 
					 3, 
					 tex->sizeX, tex->sizeY,
					 0, 
					 GL_RGB, 
					 GL_UNSIGNED_BYTE, 
					 tex->data);


		// �������� ������ ��������
		glBindTexture(GL_TEXTURE_2D, texture[1]);
				
		// �������� ����������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 
			         0, 
					 3, 
					 tex->sizeX, tex->sizeY,
					 0, 
					 GL_RGB, 
					 GL_UNSIGNED_BYTE, 
					 tex->data);


		// �������� ������� ��������
		glBindTexture(GL_TEXTURE_2D, texture[2]);
				
		// mip-����������; ��������� ��������� ������� ������� �������, ���� �� ������� (�������� ����������) ����� �������������� ��� ����������;
	    // ��� ������������� ������ ������ ����������, ������� �������� ����� ���� ������ (� �� ������ �������� 2, ��� � ������ � ����������� �������� ����������, ������� ����������� �������� �����������)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);  

		gluBuild2DMipmaps(GL_TEXTURE_2D, 
			              3, 
						  tex->sizeX, 
						  tex->sizeY,
						  GL_RGB, 
						  GL_UNSIGNED_BYTE, 
						  tex->data); 


		// ����������� �����������
		if(tex) // ���� �������� ����������
		{
			if(tex->data) // ���� ����������� �������� ����������
			{
				free(tex->data); // ������������ ������ ����������� ��������
			}

			free(tex); // ������������ ������ ��� ���������
		}

		// ����������
		//glColor4f(1.0f,1.0f,1.0f,0.5f);   // ������ �������, ������������ 50%; ����� = 0.0 (������ ������������), 1.0 - ������ ��������������
		//glBlendFunc(GL_SRC_ALPHA,GL_ONE); // ������� ���������� ������������ �� �������� �����
	} 





	// ������� �����
	public: void Update(float _delta)
	{
		spinX += _delta * step;

		// ������������ ����� �� �������� 360 ��������
		if(spinX > 360.0f) { spinX = spinX - 360.0f; }

				
		spinY += _delta * step;

		// ������������ ����� �� �������� 360 ��������
		if(spinY > 360.0f) { spinY = spinY - 360.0f; }


		spinZ += _delta * step;

		// ������������ ����� �� �������� 360 ��������
		if(spinZ > 360.0f) { spinZ = spinZ - 360.0f; }
	}


	// ���������� ���������������� �������
	public: void DrawBox(void)
	{
		glEnable(GL_TEXTURE_2D); // �������� ��������� �������

		glColor3f(1.0f, 1.0f, 1.0f);// ����� ���� ������

		glBindTexture(GL_TEXTURE_2D, texture[filter]);  // ����� �������� ����������� �� filter

		// ���������� ���������������� ���
		glBegin(GL_QUADS);  // ������ ��������� �����������������
		 
		// �������� �����
		//glNormal3f(0.0f, 0.0f, 1.0f);   
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f); 


		// ������ �����
		//glNormal3f(0.0f, 0.0f,-1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); 

  
		// ������� �����
		//glNormal3f(0.0f, 1.0f, 0.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); 

  
		// ������ �����
		//glNormal3f(0.0f,-1.0f, 0.0f); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); 

  
		// ������ �����
		//glNormal3f(1.0f, 0.0f, 0.0f); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); 

  
		// ����� �����
		//glNormal3f(-1.0f, 0.0f, 0.0f); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); 
		glEnd(); 
		glDisable(GL_TEXTURE_2D); // �������� ��������� �������
	}


	// ���������� ��� ��������� �������� ����
	public: virtual void ResizeEventFilter(Size2D _size)
	{ 
		Renderer::Resize(_size);// ����� �������� �����������
	}



	// ������� �������
	public: void Draw(float _delta)
	{
		//MakeCurrent();
		//Flush();
		ClearScene();// �������� �����
		
 
		// �������������...
		Renderer::_SetCurrentStack(Renderer::MODELVIEW); // �������� � �������� ����
		Renderer::_PushMatrix();// ��������� ������� ������� (�������������) � �����

		// ���������� ������������� (�������) ����� � ������� (�������) ������������� �������������� 
		Renderer::_MultMatrix(Matrix4x4(Vector3D::UNIT_X, Angle::_DegreesToRadians(spinX)) * 
			                  Matrix4x4(Vector3D::UNIT_Y, Angle::_DegreesToRadians(spinY)) * 
				              Matrix4x4(Vector3D::UNIT_Z, Angle::_DegreesToRadians(spinZ)));

		if(IsKeyboardFocus() && Mouse::_IsAnyKeyPressed())// ���� ������ ���� �� ������ ����
		{
			scene->Draw();// ���������� ��� ������� �����
		}
		else
		{
			DrawBox();// ���������� �������
		}
		
		// �������������... 
		Renderer::_PopMatrix();// ������������ ���������� ������� (�������������) �� ������

		camera->Begin2D();// ������ ����� ���������� �������


		// ������� ������
		font2d->DrawScreenText(Vector3D(10, 20, -1), "box.Center(X: %.1f, Y:%.1f, Z:%.1f); box.Size(width:%.1f, height:%.1f, length:%.1f)", box->Box().Center().x, box->Box().Center().y, box->Box().Center().z, box->Box().Size().width, box->Box().Size().height, box->Box().Size().length); 

		// ������� ������
		font2d->DrawScreenText(Vector3D(10, 40, -1), "camera.Position(X: %.1f, Y:%.1f, Z:%.1f)", camera->Position().x, camera->Position().y, camera->Position().z); 



		// ���������� �������
		StringANSI filterName;
		if(filter == 0) { filterName = "nearest"; }
		else if(filter == 1) { filterName = "linear"; }
		else if(filter == 2) { filterName = "mipmap"; } 
		font2d->DrawScreenText(Vector3D(10, 60, -1), "texture filtering: %s", filterName.c_str());  
		

		StringANSI fpsMode; 
		if(UpdateCallController().Mode() == CallController::FIXED)
		{
			fpsMode = "fixed";
		}
		else if(UpdateCallController().Mode() == CallController::UNLIM)
		{
			fpsMode = "unlim";
		}

		// ���������� ����
		font2d->DrawScreenText(Vector3D(10, 80, -1), "logic core mode: %s   FPS: %d, max steps: %d", fpsMode.c_str(), UpdateCallController().FPS(), UpdateCallController().MaxSteps()); 
		
		

		if(DrawCallController().Mode() == CallController::FIXED)
		{
			fpsMode = "fixed";
		}
		else if(DrawCallController().Mode() == CallController::UNLIM)
		{
			fpsMode = "unlim";
		}
		

		// ����������� ����
		font2d->DrawScreenText(Vector3D(10, 100, -1), "draw core mode: %s   FPS: %d  VSYNC: %d", fpsMode.c_str(), DrawCallController().FPS(), VSync()); 


		// ����� �������
		font2d->DrawScreenText(Vector3D(10, 120, -1), "z-buffer: %d, z-clear value: %f, z-depth: %d", ZBuffer(), ZBufferClearValue(), ZBufferDepth()); 

	
		// fov
		font2d->DrawScreenText(Vector3D(10, 140, -1), "fov: %f", camera->FOV()); 
	
		// ������
		font2d->DrawScreenText(Vector3D(10, 160, -1), "aspect: %f", camera->Aspect()); 

		// ������� � ������� ��������� ���������
		font2d->DrawScreenText(Vector3D(10, 180, -1), "zNear: %f, zFar: %f", camera->ZNear(), camera->ZFar()); 


		// ������ �����
		StringANSI colorFormat;
		if(ColorFormat() == Renderer::RGBA)
		{ colorFormat = "RGBA"; } 
		font2d->DrawScreenText(Vector3D(10, 200, -1), "color depth: %d, colorFormat: %s", GetColorDepth(), colorFormat.c_str()); 


		// �������
		font2d->DrawScreenText(Vector3D(10, 220, -1), "viewport(auto: %d)", ViewportAutoSize());
		font2d->DrawScreenText(Vector3D(10, 240, -1), "viewport(x: %.0f, y: %.0f, width: %.0f, height: %.0f)", ViewportPosition().x, ViewportPosition().y, ViewportSize().width, ViewportSize().height); 
		
		// ����� ������������ ������
		StringANSI faceDrawMode;
		if(GetFaceDrawMode(Renderer::FRONT) == Renderer::POINT) { faceDrawMode = "point"; }
		if(GetFaceDrawMode(Renderer::FRONT) == Renderer::LINE) { faceDrawMode = "line"; }
		if(GetFaceDrawMode(Renderer::FRONT) == Renderer::FILL) { faceDrawMode = "fill"; }
		font2d->DrawScreenText(Vector3D(10, 260, -1), "front face draw mode: %s", faceDrawMode.c_str()); 
		
		if(GetFaceDrawMode(Renderer::BACK) == Renderer::POINT) { faceDrawMode = "point"; }
		if(GetFaceDrawMode(Renderer::BACK) == Renderer::LINE) { faceDrawMode = "line"; }
		if(GetFaceDrawMode(Renderer::BACK) == Renderer::FILL) { faceDrawMode = "fill"; }
		font2d->DrawScreenText(Vector3D(10, 280, -1), "back face draw mode: %s", faceDrawMode.c_str()); 
		

		// ����� ��������� ������
		font2d->DrawScreenText(Vector3D(10, 300, -1), "front face cull mode: %d", GetFaceCullMode(Renderer::FRONT));
		font2d->DrawScreenText(Vector3D(10, 320, -1), "back face cull mode: %d", GetFaceCullMode(Renderer::BACK)); 

		font2d->DrawScreenText(Vector3D(10, 340, -1), "front face clockwise: %d", FrontFaceClockwise()); 
 
		font2d->DrawScreenText(Vector3D(10, 360, -1), "Application.SizeInByte: %d", SizeInByte() + scene->SizeInByte() + camera->SizeInByte() + font2d->SizeInByte()); 
	
 
		camera->Begin3D();// ������ ����� ���������� �������
	} 
 
	

 
	// ������ ������� ����
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		if(Keyboard::_IsKeyPressed(Keyboard::F))// ����� ������� ��� �������
		{
			// ���� ��������� �������� ����
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0)
				{
					filter++;
				}
				else if(_event.wheel < 0)
				{
					filter--;
				}
				// �������� ���������
				if(filter > 2 || filter < 0) { filter = 0; }
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::O))// ���������� FOV
		{
			// ���� ��������� �������� ����
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0)
				{
					fov+=0.1f;
				}
				else if(_event.wheel < 0)
				{
					fov-=0.1f;
				}
				// �������� ���������
				if(fov > 359.0f || fov < 0.0f) { fov = 0.0f; }

				camera->SetFOV(fov);
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::S))// ���������� ��������
		{
			// ���� ��������� �������� ����
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0)
				{
					aspect+=0.1f;
				}
				else if(_event.wheel < 0)
				{
					aspect-=0.1f;
				}

				camera->SetAspect(aspect);
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::N))// ������� ��������� ���������
		{
			// ���� ��������� �������� ����
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0)
				{
					zNear+=1.0f;
				}
				else if(_event.wheel < 0)
				{
					zNear-=1.0f;
				}

				camera->SetZNear(zNear); 
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::M))// ������� ��������� ��������� 
		{
			// ���� ��������� �������� ����
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0)
				{
					zFar+=1.0f;
				}
				else if(_event.wheel < 0)
				{
					zFar-=1.0f;
				}

				camera->SetZFar(zFar); 
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::L))// ����� ����������� ����
		{
			// ���� ��������� �������� ����
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0 || _event.wheel < 0)
				if(UpdateCallController().Mode() == CallController::FIXED)
				{ UpdateCallController().SetMode(CallController::UNLIM); }
				else { UpdateCallController().SetMode(CallController::FIXED); }
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::R))// ����� ������������ ����
		{
			// ���� ��������� �������� ����
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0 || _event.wheel < 0)
				if(DrawCallController().Mode() == CallController::FIXED)
				{ DrawCallController().SetMode(CallController::UNLIM); }
				else { DrawCallController().SetMode(CallController::FIXED); }
			}
		}
		else if(_event.type == Mouse::WHEEL)
		{
			if(_event.wheel > 0)
			{
				zoom += 1.0f;
			}
			else if(_event.wheel < 0)
			{
				zoom -= 1.0f;
			}	
			camera->SetPosition(Vector3D(camera->Position().x, camera->Position().y, zoom));
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
			else if(_event.key == Keyboard::Z)
			{
				SetZBuffer(!ZBuffer());
			}
			else if(_event.key == Keyboard::V)
			{
				SetVSync(!VSync());
			}	
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// ���������� ����
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
	def.realTimeWorkerDef.windowDef.tittle = "Renderer";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::POINT;
	//def.faceDrawMode[Renderer::BACK] = Renderer::POINT;
	//def.faceCullMode[Renderer::FRONT] = false;
	//def.faceCullMode[Renderer::BACK] = false;
	//def.frontFaceClockwise = true;
	//def.realTimeWorkerDef.windowDef.eventBuffering = true;


	
	Application* app = new Application;// ������� ����
	app->Application::Create(def);
	app->Loop();// ��������� ���� �������

	delete app;
	return 0;
}
