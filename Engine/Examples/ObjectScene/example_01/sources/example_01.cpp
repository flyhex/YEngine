// Console.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include <conio.h>
#include <stdio.h>
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Call/MagicCaller/MagicCaller.h>
#include <Engine/Core/Object/Object.h>
#include <Engine/Core/ObjectScene/ObjectScene.h>



// ��������������� ������
class MyObject: public Object,//������� �������� �������
	            public MessageHandler<int32>::Receiver// ���������� ���������
{
	// METHODS
	// ����������� �� ���������
	public: MyObject(void)
	{}

	// ����������� ����������
	public: virtual ~MyObject(void)
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
class MyScene: public MessageHandler<int32>::Receiver,// ���������� ���������
	           public ObjectScene// ��������� �����; �.�. ����� ��������� �� ���� ���� (������������ � ����� �� ��������), �� ���������� ObjectScene (���������� �������� �����) ������ ���� ������ �� ����������� Receiver (������� ����������), ����� ������� ����� ����� ��������� ��� ������ ������� �� ��� ���������� (��������� ����� ������������ ������ ��� ����������� �������, �� ������� �� ������ �����������); � ����� � ���, ��� ����������� ���������� � �������: �� ������������ ������� � �������, �� ����� ������� ��������� �������� ����� ��������� �������������� �����
{
	// METHODS
	// ����������� ����������
	public: virtual ~MyScene(void)
	{
		printf("~scene %s\n", GetName().c_str());
		//Destroying();// ������ ���������� ������������
	}

	// ������� �� ���������� ������� � �����
	public: void AddedObjectEvent(Object* _object)
	{
		printf("%s.added: %s\n", GetName().c_str(), _object->GetName().c_str());
	}

				
	// ������� �� ���������� ������� �� �����
	public: void RemovedObjectEvent(Object* _object)
	{
		printf("%s.removed: %s\n", GetName().c_str(), _object->GetName().c_str());
	}
};


// ���������� ��� ������� _object
void PrintObjectName(Object* _object)
{
	printf("object name: %s\n", _object->GetName().c_str());
}


// ���������� ���������� � �����
void PrintScene(MyScene* _scene)
{
	printf("scene %s...\n", _scene->GetName().c_str());
	printf("scene size: %d\n", _scene->AmountOfObjects());
	_scene->GetObjectBinaryTree().InfixTraverse(MagicCaller<>(&PrintObjectName));
	
}

int main()
{
	MyScene* scene1 = new MyScene();// ������� ����� 1
	scene1->Rename("scene1");
	scene1->Connect(ObjectScene::ADD_OBJECT_MESSAGE, scene1, MagicCaller<>(scene1, &MyScene::AddedObjectEvent));// ����������� �� ������� ���������� ������� � �����
	scene1->Connect(ObjectScene::REMOVE_OBJECT_MESSAGE, scene1, MagicCaller<>(scene1, &MyScene::RemovedObjectEvent));// ����������� �� ������� ���������� ������� �� �����


	MyObject* object1 = new MyObject();// ������� ������ 1
	object1->Rename("object1");
	object1->WriteToLogFile("");
	object1->Connect(Object::RENAME_OBJECT_MESSAGE, object1, MagicCaller<>(object1, &MyObject::RenamedObjectEvent));// ����������� �� ������� �������������� �������
	scene1->AddObject(object1);// �������� ������ � �����



	MyObject* object2 = new MyObject();// ������� ������ 2
	object2->Rename("object2");
	object2->WriteToLogFile("");
	object2->Connect(Object::RENAME_OBJECT_MESSAGE, object2, MagicCaller<>(object2, &MyObject::RenamedObjectEvent));// ����������� �� ������� �������������� �������
	scene1->AddObject(object2);// �������� ������ � �����


	MyScene* scene2 = new MyScene();// ������� ����� 2
	scene2->Rename("scene2");
	scene2->WriteToLogFile("");
	scene2->Connect(ObjectScene::ADD_OBJECT_MESSAGE, scene2, MagicCaller<>(scene2, &MyScene::AddedObjectEvent));// ����������� �� ������� ���������� ������� � �����
	scene2->Connect(ObjectScene::REMOVE_OBJECT_MESSAGE, scene2, MagicCaller<>(scene2, &MyScene::RemovedObjectEvent));// ����������� �� ������� ���������� ������� �� �����


	MyObject* object3 = new MyObject();// ������� ������ 3
	object3->Rename("object3");
	object3->WriteToLogFile("");
	object3->Connect(Object::RENAME_OBJECT_MESSAGE, object3, MagicCaller<>(object3, &MyObject::RenamedObjectEvent));// ����������� �� ������� �������������� �������
	scene2->AddObject(object3);// �������� ������ � �����

	scene1->AddObject(scene2);// �������� ����� � ������ �����
	scene1->GetObjectBinaryTree().AddObject(object3);// �������� ������ � �����
	// --------------------------------------- ���������� ��������� ����

	PrintScene(scene1);
	PrintScene(scene2);
	printf("\n");

	// --------------------------------------- ������������� ������ 1
	
	printf("object renaming...\n");
	object1->Rename("object0");
	object2->Rename("object0");// �������������� ����� ���������, �.�. ����� ��� ��� ������
	
	PrintScene(scene1);
	PrintScene(scene2);
	printf("\n");

	// --------------------------------------- ���������� �������� �� �����

	printf("remove object1 from scene1...\n");
	delete object1->GetAddress(&scene1->GetObjectBinaryTree());// ����� ������������� ������� ���������� �� ����������
	object1->Rename("object10");// ����� scene1 ������ �� ������������ ����� ������ �������
	printf("delete object10...\n");
	delete object1;

	printf("remove object2...\n");
	scene1->GetObjectBinaryTree().Remove(object2, MagicContainer::REMOVE_NODE);// ����� ������������� ������� ���������� �� scene1 ������� object2


	printf("scene1.Clear()...\n");
	scene1->Destroy();// ����� ������������� ������� ���������� �� scene2 ������� object3


	getch();
	return 0;
}