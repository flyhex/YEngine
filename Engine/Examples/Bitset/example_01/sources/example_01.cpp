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
#include <Engine/Helpers/Sprite/AnimatedSprite/AnimatedSprite.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>



// �����, ����������� ���������������� ����
class Application: public Engine
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D* font2d;// �����
	private: Scene* scene;// ����������� �����
	private: Bitset bitset; // ������ �����
    private: int32 index1; // ������ ����������� ���� ��� ������� �������
    private: int32 index2; // ������ ����������� ���� ��� ������� �������
	private: int32 builtInType;// ���������� ��� ������
	private: int32 selectedBitset;// ������ ����������� �������
	private: int32 newBitsetSize;// ������ ��� ������������ ������
			 
    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), index1(0), index2(0), builtInType(0), selectedBitset(NIL), newBitsetSize(0)
	{}



	// ������� ���������������� �������
	public: virtual void DestroyUserResources(void)
	{
		//delete scene;
	}


	// ������� ���������������� ������� � ���������� true � ������ ������
	public: virtual bool CreateUserResources(void)
	{
		// �����...
		{
			// ��� �������� �������
			scene = new Scene();
			scene->Rename("scene"); // ��� �����
			AssetLibrary::_GetAssets().AddObject(scene);// �������� ����� � ���������� �������, ����� �� ����������
		}


		// �����...
		{
			Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

			Font2D::Define def;
			def.name = "font";
			def.fontName = "Courier New";// ��� ������
			def.fontSize = 16;// ������ ������
			def.fontColor = RGBA(RGBA::WHITE_COLOR);// ���� ������

			font2d = new Font2D();
			font2d->Create(def);
			scene->AddObject(font2d);// �������� � �����
			font2d->Pick();// ������� �����
		}


		// ������...
		{
			// ������...
			bitset.Resize(8);// ���������� ���
			newBitsetSize = 8;
		}
			
					

		// ������...
		{
			Camera3D::Define def;
			def.name = "camera";
			def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// ����� �������
			def.rotationEnabled = false;


			camera = new Camera3D();
			camera->Create(def);
			camera->Pick(Title()); 

			scene->AddObject(camera);
		}

		

		Keyboard().SetKeyPressedPeriodInSec(0.1);// ������� ��������� ������� �������


		// ���� ����...
		SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));
			

		MoveToCenter();// ����������� ���� � ����� �����
		Mouse::_MoveCursorToCenter(this);// ����������� ������ � ����� ���������� ������� ����	
		return true;// ������� �����
	}


	// ������� �����
	public: virtual void Update(float _delta)
	{
		scene->Update(_delta); 
		Trigger::_ExecuteActions();
	}


	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����

	
		// ����� ������...
		//camera->BeginPerspective3D();// ������ ����� ���������� �������

		//scene->Draw();
	
	
		// ����� ������...
		camera->BeginScreen2D();// ������ ����� ���������� �������

		// ������� ������ ������
		DrawBitset1();

		// ������� ������ ������
		DrawBitset2();

		// ��������� ����� ������...
		if(Keyboard().IsKeyPressed(Keyboard::R))// ���� ������ ������� R
		{
			font2d->SetFontColor(RGBA(RGBA::GREEN_COLOR));// �������
			font2d->DrawScreenText(Vector3D(500, 90, -1), "new size: %d", newBitsetSize);
		}

		int32 i = 0;
		
		
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.zoom: %.2f", camera->DistanceToViewingPoint()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.zoom min/max: %.2f/%.2f", camera->ZoomMin(), camera->ZoomMax()); 

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.zoom speed: %.2f", camera->ZoomSpeed()); 

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sprite.binary state: %s", sprite->GetStringState().c_str()); 
	}


	// ������� ������ ����������� ���� int32
	public: void DrawBitset1(void)
	{
		// ����� �������� �������
		font2d->SetFontColor(RGBA(RGBA::WHITE_COLOR));// �����
		font2d->DrawScreenText(Vector3D(14, 20, -1), "bitset: ");
		
		// ���� ������ ������ �������
		if(selectedBitset == 0)
		{
			// ������� ����...
			font2d->SetFontColor(RGBA(RGBA::GREEN_COLOR));// �������
		}

		font2d->DrawScreenText(Vector3D(14 + (8 * 10), 20, -1), "1");// ��� 8 - ���������� ��������, 10 - ������ ������� � ��������


		font2d->SetFontColor(RGBA(RGBA::WHITE_COLOR));// �����

		font2d->DrawScreenText(Vector3D(14 + (1 * 10) + (8 * 10), 20, -1), ", type: int32, size: %d bits, value: %d\r\n", Bitset::_GetBitSize(builtInType), builtInType); 


		// ����� �������
		font2d->DrawScreenText(Vector3D(14, 40, -1), "%s   - bitset\r\n", Bitset::_ToString(builtInType).c_str()); 


	
		// ����� ����������� ����
		font2d->SetFontColor(RGBA(RGBA::GREEN_COLOR));// �������
		for(int i = 0, line = 0; i < Bitset::_GetBitSize(builtInType); i++, ++line)
		{ 
			if(i == index1 && selectedBitset == 0)
			{
				font2d->DrawScreenText(Vector3D(14 + (line * 10), 40, 0), "%d", Bitset::_GetBit(builtInType, index1));
			}
		}

		
		// ����� �������� �������
		font2d->SetFontColor(RGBA(RGBA::WHITE_COLOR));// �����
		for(int i = 0, line = 0; i < Bitset::_GetBitSize(builtInType); i++, ++line)
		{ 
			if(i == index1)
			{
				font2d->DrawScreenText(Vector3D(14 + (line * 10), 60, -1), "%d", i);
			}
		}
		font2d->DrawScreenText(Vector3D(14 + Bitset::_GetBitSize(builtInType) * 10, 60, -1), "   - indices");
	}



	// ������� ������ ����������������� ���� 
	public: void DrawBitset2(void)
	{
		// ����� �������� �������
		font2d->SetFontColor(RGBA(RGBA::WHITE_COLOR));// �����
		
		font2d->DrawScreenText(Vector3D(14, 90, -1), "bitset: ");
		
		// ���� ������ ������ �������
		if(selectedBitset == 1)
		{
			// ������� ����...
			font2d->SetFontColor(RGBA(RGBA::GREEN_COLOR));// �������
		}

		font2d->DrawScreenText(Vector3D(14 + (8 * 10), 90, -1), "2");// ��� 8 - ���������� ��������, 10 - ������ ������� � ��������


		font2d->SetFontColor(RGBA(RGBA::WHITE_COLOR));// �����
		font2d->DrawScreenText(Vector3D(14 + (1 * 10) + (8 * 10), 90, -1), ", type: user, size: %d, exist: %d\r\n", bitset.GetBitSize(), bitset.IsExist()); 


		// ����� �������
		font2d->DrawScreenText(Vector3D(14, 110, -1), "%s   - bitset\r\n", bitset.ToString().c_str()); 


	
		// ����� ����������� ����
		font2d->SetFontColor(RGBA(RGBA::GREEN_COLOR));// �������
		for(int i = 0, line = 0; i < bitset.GetBitSize(); i++, ++line)
		{ 
			if(i == index2 && selectedBitset == 1)
			{  
				font2d->DrawScreenText(Vector3D(14 + (line * 10), 110, 0), "%d", bitset.GetBit(index2));
			}
		}

		
		// ����� �������� �������
		font2d->SetFontColor(RGBA(RGBA::WHITE_COLOR));// �����
		for(int i = 0, line = 0; i < bitset.GetBitSize(); i++, ++line)
		{ 
			if(i == index2)
			{
				font2d->DrawScreenText(Vector3D(14 + (line * 10), 130, -1), "%d", i);
			}
		}

		font2d->DrawScreenText(Vector3D(14 + (bitset.GetBitSize() * 10), 130, -1), "   - indices");
	}


	// ������ ������� ����
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{ 
		if(Keyboard().IsKeyPressed(Keyboard::R))// ���� ������ ������� R
		{
			// ���� ��������� �������� ����
			if(_event->type == Mouse::WHEEL)
			{
				if(_event->wheel > 0)
				{
					newBitsetSize++;
				}
				else if(_event->wheel < 0)
				{
					newBitsetSize--;
				}
				// �������� ���������
				if(newBitsetSize < 0) { newBitsetSize = 0; }
			}
		}
		if(Keyboard().IsKeyPressed(Keyboard::I))// ���� ������ ������� I
		{
			// ���� ��������� �������� ����
			if(_event->type == Mouse::WHEEL)
			{
				if(selectedBitset == 0)// ���� ��� builtInType
				{
					Bitset::_EnableBits(builtInType, _event->wheel > 0);
				}
				else // ����� ��� ���������������� ������
				{
					bitset.EnableBits(_event->wheel > 0);
				}	
			}
		}
		if(Keyboard().IsKeyPressed(Keyboard::M))// ���� ������ ������� M
		{
			// ���� ��������� �������� ����
			if(_event->type == Mouse::WHEEL)
			{
				if(selectedBitset == 0)// ���� ��� builtInType
				{
					Bitset::_AND(builtInType, index1, _event->wheel > 0);// ������������� ���
				}
				else // ����� ��� ���������������� ������
				{
					bitset.AND(index2, _event->wheel > 0);// ������������� ���
				}
			}
		}
	   if(Keyboard().IsKeyPressed(Keyboard::A))// ���� ������ ������� A
	   {
			// ���� ��������� �������� ����
			if(_event->type == Mouse::WHEEL)
			{
				if(selectedBitset == 0)// ���� ��� builtInType
				{
					Bitset::_OR(builtInType, index1, _event->wheel > 0);// ������������� ���
				}
				else // ����� ��� ���������������� ������
				{
					bitset.OR(index2, _event->wheel > 0);// ������������� ���
				}
			}
		}
       if(Keyboard().IsKeyPressed(Keyboard::D))// ���� ������ ������� D
	   {
			// ���� ��������� �������� ����
			if(_event->type == Mouse::WHEEL)
			{
				if(selectedBitset == 0)// ���� ��� builtInType
				{
					Bitset::_XOR(builtInType, index1, _event->wheel > 0);// ������������� ���
				}
				else // ����� ��� ���������������� ������
				{
					bitset.XOR(index2, _event->wheel > 0);// ������������� ���
				}
			}
		}
		if(Keyboard().IsKeyPressed(Keyboard::E))// ���� ������ ������� E
		{
			// ���� ��������� �������� ����
			if(_event->type == Mouse::WHEEL)
			{
				// ���� ��� builtInType
				if(selectedBitset == 0)
				{
					Bitset::_EQUAL(builtInType, index1, _event->wheel > 0);// ������������� ���
				}
				else // ����� ��� ���������������� ������
				{
					bitset.EQUAL(index2, _event->wheel > 0);// ������������� ���
				}
			}
		}
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
				SetWindowStyle(Window::WND_RESIZING);
			}
			else if(_event->key == Keyboard::ESC)
			{
				Destroy();// ���������� ����
			} 
			else if(_event->key == Keyboard::UP_ARROW)
			{
				selectedBitset = 0;// ������� ������ ������
			}
			else if(_event->key == Keyboard::DOWN_ARROW)
			{
				selectedBitset = 1;// ������� ������ ������
			}
			else if(_event->key == Keyboard::SPACE)
			{
				if(selectedBitset == 0)// ���� ��� builtInType
				{
					Bitset::_NOT(builtInType, index1);// ������������� ���
				}
				else // ����� ��� ���������������� ������
				{
					bitset.NOT(index2);// ������������� ���
				}
			}	
			else if(_event->key == Keyboard::N)
			{
				if(selectedBitset == 0)// ���� ��� builtInType
				{
					Bitset::_NOT(builtInType);// ������������� ������
				}
				else // ����� ��� ���������������� ������
				{
					bitset.NOT();// ������������� ������
				}
			}
		} 
		else if(_event->type == Keyboard::KEY_UP)
		{
			if(_event->key == Keyboard::R)
			{ 
				bitset.Resize(newBitsetSize);
			} 
		}
		else if(_event->type == Keyboard::KEY_PRESSED)
		{
			if(_event->key == Keyboard::LEFT_ARROW)
			{
				if(selectedBitset == 0)// ���� ��� builtInType
				{
					if(index1 > 0) // �������� ������� ���������
					{
						--index1;
					}
				}
				else // ����� ��� ���������������� ������
				{
					if(index2 > 0) // �������� ������� ���������
					{
						--index2;
					}
				}
			}
			else if(_event->key == Keyboard::RIGHT_ARROW)
			{
				if(selectedBitset == 0)// ���� ��� builtInType
				{
					if(index1 < Bitset::_GetBitSize(builtInType) - 1)// �������� �������� ���������
					{
						++index1;
					}
				}
				else // ����� ��� ���������������� ������
				{
					if(index2 < bitset.GetBitSize() - 1)// �������� �������� ���������
					{
						++index2;
					}
				}
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
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_RESIZING);// ����� ���� 
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	def.rendererDef.realTimeWorkerDef.windowDef.tittle = "Bitset.example_01";
	
	
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
