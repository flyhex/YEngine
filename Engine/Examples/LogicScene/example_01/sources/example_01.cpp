// Console.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include <conio.h>
#include <stdio.h>
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Call/MagicCaller/MagicCaller.h>
#include <Engine/Core/LogicScene/LogicScene.h>



// ��������������� ������
class MyLogicObject: public LogicObject,//������� �������� �������
	                 public MessageHandler<int32>::Receiver// ���������� ���������
{
	// METHODS
	// ����������� �� ���������
	public: MyLogicObject(void)
	{}

	// ����������� ����������
	public: virtual ~MyLogicObject(void)
	{
		printf("~object %s\n", GetName().c_str());
		Destroying();// ������ ���������� ������������
	}
				
			
	// ������� �� �������������� �������
	public: void RenamedObjectEvent(Object::ObjectRenameInfo* _info)
	{
		printf("old name: %s, new name: %s\n", _info->oldName.c_str(), _info->newName.c_str());
	}
};

// ���������������� �����
class MyLogicScene: public MessageHandler<int32>::Receiver,// ���������� ���������
	                public LogicScene// �.�. ����� ��������� �� ���� ���� (������������ � ����� �� ��������), �� ���������� LogicScene (���������� �������� �����) ������ ���� ������ �� ����������� Receiver (������� ����������), ����� ������� ����� ����� ��������� ��� ������ ������� �� ��� ���������� (��������� ����� ������������ ������ ��� ����������� �������, �� ������� �� ������ �����������); � ����� � ���, ��� ����������� ���������� � �������: �� ������������ ������� � �������, �� ����� ������� ��������� �������� ����� ��������� �������������� �����
{
	// METHODS
	// ����������� ����������
	public: virtual ~MyLogicScene(void)
	{
		printf("~scene %s\n", GetName().c_str());
		//Destroying();// ������ ���������� ������������
	}

	// ������� �� ���������� ������� � �����
	public: void AddedLogicObjectEvent(LogicObject* _object)
	{
		printf("%s.added: %s\n", GetName().c_str(), _object->GetName().c_str());
	}

				
	// ������� �� ���������� ������� �� �����
	public: void RemovedLogicObjectEvent(LogicObject* _object)
	{
		printf("%s.removed: %s\n", GetName().c_str(), _object->GetName().c_str());
	}
};


// ���������� ��� ����������� ������� _object
void PrintLogicObjectName(LogicObject* _object)
{
	printf("object name: %s\n", _object->GetName().c_str());
}


// ���������� ���������� � ���������� �����
void PrintLogicScene(MyLogicScene* _scene)
{
	printf("scene %s...\n", _scene->GetName().c_str());
	printf("scene size: %d\n", _scene->AmountOfLogicObjects());
	_scene->GetUpdateList().PrefixTraverse(MagicCaller<>(&PrintLogicObjectName));
	
}

int main()
{
	MyLogicScene* scene1 = new MyLogicScene();// ������� ����� 1
	scene1->Rename("scene1");
	scene1->Connect(LogicScene::ADD_LOGIC_OBJECT_MESSAGE, scene1, MagicCaller<>(scene1, &MyLogicScene::AddedLogicObjectEvent));// ����������� �� ������� ���������� ������� � �����
	scene1->Connect(LogicScene::REMOVE_LOGIC_OBJECT_MESSAGE, scene1, MagicCaller<>(scene1, &MyLogicScene::RemovedLogicObjectEvent));// ����������� �� ������� ���������� ������� �� �����


	MyLogicObject* object1 = new MyLogicObject();// ������� ������ 1
	object1->Rename("object1");
	object1->Connect(Object::RENAME_OBJECT_MESSAGE, object1, MagicCaller<>(object1, &MyLogicObject::RenamedObjectEvent));// ����������� �� ������� �������������� �������
	scene1->AddObject(object1);
	scene1->AddLogicObject(object1);// �������� ������ � �����



	MyLogicObject* object2 = new MyLogicObject();// ������� ������ 2
	object2->Rename("object2");
	object2->Connect(Object::RENAME_OBJECT_MESSAGE, object2, MagicCaller<>(object2, &MyLogicObject::RenamedObjectEvent));// ����������� �� ������� �������������� �������
	scene1->AddObject(object2);
	scene1->AddLogicObject(object2);// �������� ������ � �����


	MyLogicScene* scene2 = new MyLogicScene();// ������� ����� 2
	scene2->Rename("scene2");
	scene2->Connect(LogicScene::ADD_LOGIC_OBJECT_MESSAGE, scene2, MagicCaller<>(scene2, &MyLogicScene::AddedLogicObjectEvent));// ����������� �� ������� ���������� ������� � �����
	scene2->Connect(LogicScene::REMOVE_LOGIC_OBJECT_MESSAGE, scene2, MagicCaller<>(scene2, &MyLogicScene::RemovedLogicObjectEvent));// ����������� �� ������� ���������� ������� �� �����


	MyLogicObject* object3 = new MyLogicObject();// ������� ������ 3
	object3->Rename("object3");
	object3->Connect(Object::RENAME_OBJECT_MESSAGE, object3, MagicCaller<>(object3, &MyLogicObject::RenamedObjectEvent));// ����������� �� ������� �������������� �������
	
	scene2->AddObject(object3);
	scene2->AddLogicObject(object3);// �������� ������ � �����

	scene1->AddObject(scene2);
	scene1->AddLogicObject(scene2);// �������� ������ � �����
	
	scene1->AddObject(object3);
	scene1->AddLogicObject(object3);// �������� ������ � �����

	
	// --------------------------------------- ���������� ��������� ����

	PrintLogicScene(scene1);
	PrintLogicScene(scene2);
	printf("\n");

		
	// --------------------------------------- ������������� ������ 1
	
	printf("object renaming...\n");
	object1->Rename("object0");
	object2->Rename("object0");// �������������� ����� ���������, �.�. ����� ��� ��� ������
	
	PrintLogicScene(scene1);
	PrintLogicScene(scene2);
	printf("\n");
	// --------------------------------------- ���������� �������� �� �����


	printf("remove object0...\n");
	delete object1->GetAddress(&scene1->GetUpdateList());// ����� ������������� ������� ���������� �� scene1 ������� object1
	
	object1->Rename("object10");// ����� scene1 ������ �� ������������ ����� ������ �������

	printf("delete object10...\n");
	delete object1;

	printf("remove object2...\n");
	scene1->GetUpdateList().Remove(object2, MagicContainer::REMOVE_NODE);// ����� ������������� ������� ���������� �� scene1 ������� object2


	printf("scene1.Clear()...\n");
	scene1->Destroy();// ����� ������������� ������� ���������� �� scene2 ������� object3


	getch();
	return 0;
}