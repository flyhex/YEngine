// Win32.cpp: ���������� ����� ����� ��� ����������.
//


#include "Win32.h"
#include <Engine/Core/Window/Window.h>
#include <Engine/Output/File/File.h>


// �����, ����������� ���������������� ����
class MyWindow: public Window,
			    public MessageHandler<int32>::Receiver
{
	// DATA
	private: StringANSI str;// ������
	private: PAINTSTRUCT paint; // ��������� ��� ��������� � ����
	private: HDC windowDC;// �������� ���������� � ������
	private: HBITMAP frame; // ���� � ������������, ������� ����� ���������� �� �����
	private: HANDLE oldObject;// ���������� ���������� ������
    private: RECT rect;// ������������� ���������� ������� ����, ������� ������ ���� �������� (�����������)
	private: HBRUSH whiteBrush;// ����� ��� ����
	private: static int32 windowCount;// ������� ����, ������� ������������ ��� ��������� ����


    // METHODS
    // ���������������� �����������
	public: MyWindow(const Window::Define& _define)
	{
		//Connect(ResizeEvent::TYPE, this, MagicCaller<>(this, &MyWindow::MyResize));
		//Connect(MoveEvent::TYPE, this, MagicCaller<>(this, &MyWindow::MyMove));
		//Connect(MinimizedEvent::TYPE, this, MagicCaller<>(this, &MyWindow::MyMinimized));
		//Connect(MaximizedEvent::TYPE, this, MagicCaller<>(this, &MyWindow::MyMaximized));

		// ������� ����
		if(Create(_define))
		{
			// ��������� ������� ��������� ����
			windowCount++;
			SetTittle("Window" + NumberToString(windowCount));//��������� ����

			CreateBuffer(); // ������� ������ �����
			whiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);// ���� ������� ����� 
			
			Flush(); // ��������� ���������� �������
		}
	}


	// ������� �����
    public: virtual int32 Frame(void)
	{
		int32 result = Window::Frame();// ������� ���������� ���������

		// ���� ���� ����������
		if(result)
		{
			//Flush(); // ��������� ���������� �������
			UpdateBuffer();// �������� ���������� ����� � ������� �� �����
			//Sleep(60);// ������� �� 60 �����������
		}

		return result;
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

		int32 j = 0;

		// ���� �����
		str = "log file: " + LogFile();
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());
				
		// ����� ����
		str = "focus: " + NumberToString(IsKeyboardFocus());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());
				
		// ���������� ���������� ������� ����
		str = "client area position: " + NumberToString(ClientAreaPosition().x) + ", " + NumberToString(ClientAreaPosition().y);
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// ���������� ����� message � ����������� 100, 100
		
		// ���������� ����
		str = "window position: " + NumberToString(WindowPosition().x) + ", " + NumberToString(WindowPosition().y);
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// ���������� ����� message � ����������� 100, 100


		// ������ ���������� �������
		str = "client area size: " + NumberToString(ClientAreaSize().width) + ", " + NumberToString(ClientAreaSize().height);
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// ���������� ����� message � ����������� 100, 100
	
		// ������ ����� ����
		str = "window size: " + NumberToString(WindowSize().width) + ", " + NumberToString(WindowSize().height);
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// ���������� ����� message � ����������� 100, 100
		

		// ������� �����
		str = "windowed: " + NumberToString(IsWindowed());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// ���������� ����� message � ����������� 100, 100
		

		// ���������� ����
		str = "amount of windows: " + NumberToString(Window::_WindowList().Size());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// ���������� ����� message � ����������� 100, 100


		// ���� ������������ ����
		str = "is parent: " + NumberToString(IsParentWindow());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// ���������� ����� message � ����������� 100, 100
		
		if(IsParentWindow())// ���� ��� ������������ ����
		{
			SimpleList<Window*> childs = ChildWindows();// ����� ������ �������� ����
			
			// ����������� ��� �������� ����
			for(int i = 0; i < ChildWindows().Size(); ++i)
			{
				str = "child_" + NumberToString(i+1) + ": " + NumberToString(childs[i]->data->Tittle());
				::TextOutA(windowDC, 10 + (i+1) * 130, j, str.c_str(), str.size());// ���������� ����� message � ����������� 100, 100
			}
		}

		// ���� �������� ����
		str = "is child: " + NumberToString(IsChildWindow());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// ���������� ����� message � ����������� 100, 100
		
		if(IsChildWindow())// ���� ��� �������� ����
		{
			str = ", parent: " + NumberToString(ParentWindow()->Tittle());
			::TextOutA(windowDC, 10 + 70, j, str.c_str(), str.size());// ���������� ����� message � ����������� 100, 100
		}

		// ����������� �������
		str = "event buffering: " + NumberToString(IsEventBuffering());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// ���������� ����� message � ����������� 100, 100
		
		// ��������� �����
		str = "elapsed time: " + NumberToString(WorkingTimeSec());
		::TextOutA(windowDC, 10, j+=20, str.c_str(), str.size());// ���������� ����� message � ����������� 100, 100
		
	
		// ������� InvalidateRect �������� ��������� WM_PAINT � ��������� �������������� ���������� ������� ����, ������� ������ ���� ��������� (������������)
		::InvalidateRect(Handle(), // ���������� ����, ���������� ������� �������� ������ ���� ���������
					     &rect, // �������������, ������� ����� �������� � ����������� ������� (���� 0, �� ����� ������������ ��� ���������� �������)
					     false); // ���� true, �� ��� ���������� ������� ����� �����, ��� ������ ::BeginPaint; ���� false, �� ��� �������� ����������
	}


	// ������� �������� ����
	public: void CreateChildWindow(void)
	{
		// �������� ����
		Window::Define def;
		def.SetTemplateStyle(Window::WND_RESIZING);
		def.parentWindow = Handle();// ������������ ������� ���� � �������� �������������
        def.hSmallIcon = LoadIcon(Instance(), MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	
		MyWindow* window = new MyWindow(def);// ������� ����
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


	// ������ ������� ����
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		Mouse().ClearEventList();// �������� ����� �������
	}

	
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
				SetWindowStyle(Window::WND_VIEWPORT);
				SetClientAreaSize(Size2D(640, 480));
			}
			else if(_event.key == Keyboard::NUM_3)
			{
				SetWindowStyle(Window::WND_TITTLE);
				SetClientAreaSize(Size2D(640, 480));
			}
			else if(_event.key == Keyboard::NUM_4)
			{
				SetWindowStyle(Window::WND_LIGHT);
				SetClientAreaSize(Size2D(640, 480));
			}
			else if(_event.key == Keyboard::NUM_5)
			{
				SetWindowStyle(Window::WND_CLASSIC);
				SetClientAreaSize(Size2D(640, 480));
			}
			else if(_event.key == Keyboard::NUM_6)
			{
				SetWindowStyle(Window::WND_RESIZING);
				SetClientAreaSize(Size2D(640, 480));
			}
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// ���������� ����
			}
			else if(_event.key == Keyboard::C)
			{
				CreateChildWindow();// ������� �������� ����
			}
			else if(_event.key == Keyboard::M)
			{
				MoveToCenter();// ����������� ���� � ����� ������
			}
		}

		Keyboard().ClearEventList();// �������� ����� ������� 
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
	


	
	public: void MyResize(Size2D _size)
	{
		WinAPI::_MessageBox("resize","");

		// ����������� ������ �����
		DestroyBuffer();
		CreateBuffer();

		// �������� ����
		UpdateBuffer();
	}


    public: void MyMove(Vector2D _position)
	{
		WinAPI::_MessageBox("move","");
	}



    public: void MyMinimized(void)
	{
		WinAPI::_MessageBox("Minimized","");
	}

    public: void MyMaximized(void)
	{
		WinAPI::_MessageBox("Maximized","");
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
		return Window::Proc(_hWindow, _message, _wParam, _lParam); 
	}
	
	
				
	// ������...
	// ���������� ������ (� ������) ������������ ������, �������� �������� ������
	public: virtual int32 DynamicSizeInByte(void)const
	{
		return Window::DynamicSizeInByte() + MessageHandler<int32>::Receiver::DynamicSizeInByte();
	}

	// ���������� ������ (� ������) ����������� ������, �������� �������� ������
	public: virtual int32 StaticSizeInByte(void)const
	{
		return sizeof(*this);
	}
};

// ����������� ������
int32 MyWindow::windowCount = 0;



// ������� ����� ����� � ���������
// ���������� � ������ ������� WINAPI, ���������, ��� ��������� � ������� WinMain ���������� ����� �������
int WINAPI WinMain(HINSTANCE hInstance,// ���������� (�������������), ������������ �������� ��� ������� ����������
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	// �������� ����
	Window::Define def;
	def.SetTemplateStyle(Window::WND_RESIZING);// ����� ����
	def.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	//def.eventBuffering = true;

	MyWindow* window = new MyWindow(def);// ������� ����
	

	// ���� ���� �� ���� ���� ����������
	while(Window::_WindowList().IsNotEmpty())
	{
		// ������ ��� ������������ ����
		for(int i = 0; i < Window::_WindowList().Size(); ++i)
		{
			Window::_WindowList()[i]->Frame();// ��������� ���������
		}
	}

	delete window;
	return 0;
}


 
