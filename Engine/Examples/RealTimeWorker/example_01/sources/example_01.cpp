// Win32.cpp: ���������� ����� ����� ��� ����������.
//


#include "Win32.h"
#include <Engine/Core/Window/Window.h>
#include <Engine/Output/File/File.h>
#include <Engine/Core/RealTimeWorker/RealTimeWorker.h>




// �����, ����������� ���������� ����
class Application: public RealTimeWorker
{ 
	// DATA
	private: float time;// ����� �������
	private: StringANSI str;// ������

	private: PAINTSTRUCT paint; // ��������� ��� ��������� � ����
	private: HDC windowDC;// �������� ���������� � ������
	private: HBITMAP frame; // ���� � ������������, ������� ����� ���������� �� �����
	private: HANDLE oldObject;// ���������� ���������� ������
    private: RECT rect;// ������������� ���������� ������� ����, ������� ������ ���� �������� (�����������)
	private: HBRUSH whiteBrush;// ����� ��� ����



    // METHODS
    // ���������������� �����������
	public: Application(const RealTimeWorker::Define& _define): time(0.0f)
	{
		if(Create(_define))
		{
			CreateBuffer(); // ������� ������ �����
			whiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);// ���� ������� ����� 
			
			Flush(); // ��������� ���������� �������
		}
	}


	// ��������� ���������� �����
	public: void UpdateBuffer(void)
	{
		// ������������� ���������� ������� ����, ������� ����� �������� (������������); �� ���������� ��� ���������� �������
		rect.left = 0;
		rect.top = 0;
		rect.right = ClientAreaSize().width;
		rect.bottom = ClientAreaSize().height;
 
		FillRect(windowDC, &rect, whiteBrush);// ��������� ��� ����� ������

		// ��������� ����� ���� ������
		SetTextColor(windowDC, RGB(100, 100, 100));


		// ������
		str = "timer: " + NumberToString(time);
		::TextOutA(windowDC, 10, 10, str.c_str(), str.size());
				
		// FPS
		str = "FPS: " + NumberToString(UpdateCallController().FPS());
		::TextOutA(windowDC, 10, 30, str.c_str(), str.size());
		
		// 
		str = "FPS mode: " + StringANSI((UpdateCallController().Mode() == CallController::FIXED) ? "fixed" : (UpdateCallController().Mode() == CallController::UNLIM) ? "unlimited" : "none");
		::TextOutA(windowDC, 10, 50, str.c_str(), str.size());
		

		// sleep
		str = "sleep: " + NumberToString(Sleep());
		::TextOutA(windowDC, 10, 70, str.c_str(), str.size());
				
		// unsuspended
		str = "unsuspended: " + NumberToString(Unsuspended());
		::TextOutA(windowDC, 10, 90, str.c_str(), str.size());
		
		// ������� InvalidateRect �������� ��������� WM_PAINT � ��������� �������������� ���������� ������� ����, ������� ������ ���� ��������� (������������)
		::InvalidateRect(Handle(), // ���������� ����, ���������� ������� �������� ������ ���� ���������
					     &rect, // �������������, ������� ����� �������� � ����������� ������� (���� 0, �� ����� ������������ ��� ���������� �������)
					     false); // ���� true, �� ��� ���������� ������� ����� �����, ��� ������ ::BeginPaint; ���� false, �� ��� �������� ����������
	}



 
	// �������������� ���������� ������� ����
	public: void Draw(void)
	{
		// �� ����� ������������ ������� ����������� (��������� � ������, � ����� ����������� ������������� ����������� � ����)

		// HDC - ����������� ��������� ������������ ����������, ������� ��������� ������������ � ������������ �����������
		HDC screen = ::BeginPaint(Handle(), &paint); // ��� ������� BeginPaint �������������� ���� ��� ��������� � ���������� ���������� ��������� ������������ ���������� (��� �� �� ��������� �� ������ paint.hdc)
		
		// �������� �� windowDC � screen
		// paint ������������ ��� �������� ������� ���������� (� ������ ������ ��� ��� ���������� �������)
		StretchBlt(screen, // ����
			       paint.rcPaint.left, 
				   paint.rcPaint.top,
				   paint.rcPaint.right,
				   paint.rcPaint.bottom,
				   windowDC, // ��������
				   paint.rcPaint.left,
				   paint.rcPaint.top,
				   paint.rcPaint.right,
				   paint.rcPaint.bottom,
				   SRCCOPY);
				   
		// ��������� ���������; ���������� �������� ������������ ����������
		EndPaint(Handle(), &paint);
	} 


	// ���������� �������� ���������� � ������, � �����������, ����������� ������ �����
	private: void DestroyBuffer(void)
	{
		// ���������� ����������� �������...
		oldObject = (HBITMAP)SelectObject(windowDC, oldObject);// ����� ��������� � ������� frame, ������� ���������� ������ oldObject
				
		DeleteObject(frame);// ������� ����������� 
		DeleteDC(windowDC);// ������� �������� ����������
	}

	// ������� �������� ���������� � ������, � �����������, ������� ����� ����������� ������ �����
	private: void CreateBuffer(void)
	{
		if(Handle())// ���� ���� ����������
		{
			// ������� ����������� �������...
			HDC dc = GetDC(Handle());// ����������� �������� ����
			
			windowDC = CreateCompatibleDC(dc);// ������� �������� ���������� � ������, ����������� � ���������� ������������ ��������� 
			frame = CreateCompatibleBitmap(dc, ClientAreaSize().width, ClientAreaSize().height);// ������� �����������, ����������� � ���������� ������������ ���������; ������ ����������� ������������� ���������� ������� ����
			
			::ReleaseDC(Handle(), dc);// ���������� �������� ����
					
			oldObject = (HBITMAP)SelectObject(windowDC, frame);// ������� ������ frame ��� ��������� ���������� windowDC; ��������� ���������� ���������� ������, � ��������� ��� � oldObject
		}
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
			if(_event.key == Keyboard::S)
			{
				SetSleep(!Sleep());
			}
			else if(_event.key == Keyboard::U)
			{	
				SetUnsuspended(!Unsuspended());
			}
			else if(_event.key == Keyboard::M)
			{
				if(UpdateCallController().Mode() == CallController::FIXED)
				{
					UpdateCallController().SetMode(CallController::UNLIM);
				}
				else if(UpdateCallController().Mode() == CallController::UNLIM)
				{
					UpdateCallController().SetMode(CallController::FIXED);
				}
			}
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// ���������� ����
			}
		}

		Keyboard().ClearEventList();// �������� ����� ������� 
	}

	// ���������� ��� ��������� �������� ����
	// ����������: � �������� ��������� ����������� ����� ������ ���������� ������� ����
	public: virtual void ResizeEventFilter(Size2D _size)
	{
		// ����������� ������ �����
		DestroyBuffer();
		CreateBuffer();

		// �������� ����
		UpdateBuffer();
	}

    // ���������� ���������
	public: virtual LRESULT Proc(HWND _hWindow, UINT _message, WPARAM _wParam, LPARAM _lParam)
	{
		// ��������� ���������
		switch(_message)
		{ 
			// ���� ������������
			case WM_DESTROY:
			{
				DestroyBuffer();
				break;// �������� �������� switch 
			}

			// ���� ����������������; ��������� ��� �������� ������
			case WM_PAINT:
			{ 
				Draw();// �������� ���������� ������� ����
				break;// �������� �������� switch 
			}

			default: break;// ������������ ��������� ���������
		}
		
		// ������� ������� ����������
		return RealTimeWorker::Proc(_hWindow, _message, _wParam, _lParam);
	}



	// ������� �����
	public: virtual void Update(float _delta)
	{
		time += _delta;// �������� ������ � ������ �������
		UpdateBuffer();// �������� ���������� �����
	}
		
			
	// ������...
	// ���������� ������ (� ������) ������������ ������, �������� �������� ������
	public: virtual int32 DynamicSizeInByte(void)const
	{
		return RealTimeWorker::DynamicSizeInByte();
	}

	// ���������� ������ (� ������) ����������� ������, �������� �������� ������
	public: virtual int32 StaticSizeInByte(void)const
	{
		return sizeof(*this);
	}
};



// ������� ����� ����� � ���������
// ���������� � ������ ������� WINAPI, ���������, ��� ��������� � ������� WinMain ���������� ����� �������
int WINAPI WinMain(HINSTANCE hInstance,// ���������� (�������������), ������������ �������� ��� ������� ����������
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	// �������� ����������� ����
	RealTimeWorker::Define def;
	def.windowDef.SetTemplateStyle(Window::WND_RESIZING);// ����� ����
	def.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	def.windowDef.tittle ="RealTimeWorker";// ��������� ����

	Application* app = new Application(def);// ������� ����
	app->Loop();// ��������� ���� �������
	
	delete app;// ������� ����
	return 0;
}


 
