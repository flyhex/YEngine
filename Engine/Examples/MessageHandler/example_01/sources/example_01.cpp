// example_01.cpp: ���������� ����� ����� ��� ����������� ����������.
//


#include "stdafx.h"
//#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Template/MessageHandler/MessageHandler.h>
#include "conio.h"
#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;


// �����, ����������� �������
class Event
{
	// ���� �������
	public: enum 
	{
		MOUSE,
		KEYBOARD,
		DESTROY
	};

	// DATA
	public: int32 type;// ��� �������
	public: string action;// ��������


	// METHODS
	// ���������������� �����������
	public: Event(int32 _type, string _action): type(_type), action(_action)
	{}

	// ���������� ��� �������
	public: virtual int32 Type(void)const
	{
		return type;
	}
};



 
// �����, ����������� ����������
class Receiver: public MessageHandler<int32>::Receiver
{
	// METHODS
	// ����������� �� ���������
	public: Receiver(void)
	{
		cout<<"Receiver constructor"<<endl;
	}

	// ���������� ������� ����
	public: void MouseMessage(Event _event)
	{
		cout<<"Event processing..."<<endl;
		if(_event.type == Event::MOUSE)
		{
			cout<<"event type: "<<"MOUSE"<<endl;
			cout<<"event action: "<<_event.action.c_str()<<endl;
		}
	}
	

	// ���������� ������� ����������
	public: void KeyboardMessage(Event _event)
	{
		cout<<"Event processing..."<<endl;
		if(_event.type == Event::KEYBOARD)
		{
			cout<<"event type: "<<"KEYBOARD"<<endl;
			cout<<"event action: "<<_event.action.c_str()<<endl;
		}
	}

	// ���������� ������� �����������
	public: void DestroyMessage(Event _event)
	{
		cout<<"Event processing..."<<endl;
		if(_event.type == Event::DESTROY)
		{
			cout<<"event type: "<<"DESTROY"<<endl;
		}
	}
				
	// ����������
	public: ~Receiver(void)
	{
		cout<<"Receiver destructor"<<endl;
	}
};



int main()
{
	MessageHandler<int32> sender;// ������� ����������� ���������
	

	// �������� ������� ����������...
	Receiver* receiver1 = new Receiver;// ������� ����������
	cout<<"sender.Connect(Event::MOUSE, receiver1)..."<<endl;
	sender.Connect(Event::MOUSE, // ���������, �� ������� ������ ����������� ���������
		           receiver1, // ����� ����������
		           MagicCaller<>(receiver1, &Receiver::MouseMessage));// ���������� ���������
	cout<<"AmountOfMessages: "<<sender.AmountOfMessages()<<endl<<endl;
	


	// ��������� ���������...
	sender.SendPackage(Event::MOUSE, Event(Event::MOUSE, "mouse move"));
	sender.SendPackage(Event::KEYBOARD, Event(Event::KEYBOARD, "key pressed"));

	
	// ��������� ����������...
	cout<<endl<<"sender.Disconnect(Event::MOUSE, receiver1)..."<<endl;
	//receiver1->DisconnectFromAll();
	sender.Disconnect(Event::MOUSE, receiver1);// ��������� ���������� �� ������� Event::MOUSE
	cout<<"AmountOfMessages: "<<sender.AmountOfMessages()<<endl;
	
		

	// ��������� ���������...
	sender.SendPackage(Event::MOUSE, Event(Event::MOUSE, "mouse move"));
	sender.SendPackage(Event::KEYBOARD, Event(Event::KEYBOARD, "key pressed"));


	// �������� ������� ����������...
	Receiver* receiver2 = new Receiver;// ������� ����������; �������� ��� ����� �� ��� ������ �������: Event::KEYBOARD � Event::DESTROY
	cout<<"sender.Connect(Event::KEYBOARD, receiver2)..."<<endl;
	sender.Connect(Event::KEYBOARD, // ���������, �� ������� ������ ����������� ���������
		           receiver2, // ����� ����������
		           MagicCaller<>(receiver2, &Receiver::KeyboardMessage));// ���������� ���������


	cout<<"sender.Connect(Event::DESTROY, receiver2)..."<<endl;
	sender.Connect(Event::DESTROY, // ���������, �� ������� ������ ����������� ���������
		           receiver2, // ����� ����������
		           MagicCaller<>(receiver2, &Receiver::DestroyMessage));// ���������� ���������
	cout<<"AmountOfMessages: "<<sender.AmountOfMessages()<<endl;

	cout<<endl<<"sender.Connect(Event::MOUSE, receiver1)..."<<endl;
	sender.Connect(Event::MOUSE, // ���������, �� ������� ������ ����������� ���������
		           receiver1, // ����� ����������
		           MagicCaller<>(receiver1, &Receiver::MouseMessage));// ���������� ���������
	cout<<"AmountOfMessages: "<<sender.AmountOfMessages()<<endl<<endl;
	

	// ��������� ���������...
	sender.SendPackage(Event::MOUSE, Event(Event::MOUSE, "mouse move"));
	sender.SendPackage(Event::KEYBOARD, Event(Event::KEYBOARD, "key pressed"));
	sender.SendPackage(Event::DESTROY, Event(Event::DESTROY, "destroy"));
	 
	// ������� ����������...
	cout<<endl<<"delete receiver2..."<<endl;
	delete receiver2;// ������� ����������
	cout<<"AmountOfMessages: "<<sender.AmountOfMessages()<<endl;
	
		
	// ��������� ���������...
	sender.SendPackage(Event::MOUSE, Event(Event::MOUSE, "mouse move"));
	sender.SendPackage(Event::KEYBOARD, Event(Event::KEYBOARD, "key pressed"));
	sender.SendPackage(Event::DESTROY, Event(Event::DESTROY, "destroy"));
	

	cout<<endl<<"sender.IsConnected(Event::MOUSE, receiver1): "<<sender.IsConnected(Event::MOUSE, receiver1)<<endl;

	// �������� ���� �����������...
	cout<<endl<<"sender.DisconnectAll()..."<<endl;
	sender.DisconnectAll();// �������� ���� �����������
	cout<<"AmountOfMessages: "<<sender.AmountOfMessages()<<endl;

	// ��������� ���������...
	sender.SendPackage(Event::MOUSE, Event(Event::MOUSE, "mouse move"));
	sender.SendPackage(Event::KEYBOARD, Event(Event::KEYBOARD, "key pressed"));
	sender.SendPackage(Event::DESTROY, Event(Event::DESTROY, "destroy"));

	getch();
	return 0;
}