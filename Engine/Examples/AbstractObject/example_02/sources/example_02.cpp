// Console.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include <conio.h>
#include <stdio.h>
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Call/MagicCaller/MagicCaller.h>
#include <Engine/Core/Object/Object.h>
#include <Engine/Core/Object/ObjectBinaryTree/ObjectBinaryTree.h>


class Node: public Object
{};

 
int i = 0;// ���������� ����������

void PrintList(MagicList<Node>& _list)
{
	printf("list:\n");
	MagicList<Node>::LogicAdressDerived* it = _list.HeadAdress();

	i = 0;

	while(it)
	{
		printf("node(%d).name: %s\n", i, it->Node()->GetName().c_str());
		i++;
		it = it->Right();;
	}
}


void PrintTreeNode(Object* _obj)
{
	if(_obj)
	{
		printf("node(%d).name: %s\n", i, _obj->GetName().c_str());
		i++;
	}
}

void PrintBinaryTree(ObjectBinaryTree& _tree, StringANSI _treeName)
{
	printf("%s:\n", _treeName.c_str());
	MagicCaller<void> caller(&PrintTreeNode);

	i = 0;
	_tree.InfixTraverse(caller);// ������ ���� � ������� �����������
}


void RenameNode(Node* _obj, StringANSI _name)
{
	printf("\nRename...\n");
	
	if(_obj)
	{
		StringANSI oldName = _obj->GetName();
		printf("OldName: %s, NewName: %s\n", oldName.c_str(), _name.c_str());
		
		if(_obj->Rename(_name))
		{
			printf("Rename is succeed...\n");
		}
		else 
		{
			printf("Rename is failed...\n");
		}
	}
	printf("\n");
}

int main()
{
	ObjectBinaryTree tree1;
	ObjectBinaryTree tree2;
	MagicList<Node> list;

	Node* node1 = new Node();
	node1->Rename("node1");
	
	Node* node2 = new Node();
	node2->Rename("node2");
	
	Node* testNode = new Node();
	testNode->Rename("testNode");

	Node* node4 = new Node();
	node4->Rename("node4");
	
	//tree1.Add(node1);
	tree1.AddObject(node2);
	tree1.AddObject(testNode);

	tree2.AddObject(node1);
	tree2.AddObject(testNode);

	list.AddToTail(node4);
	list.AddToTail(testNode);

	PrintBinaryTree(tree2, "tree2");
	PrintList(list);
	PrintBinaryTree(tree1, "tree1");

	RenameNode(testNode, "node3");// �������������� �������, �.�. ��� "node3" �� ������ �� ����� �� ����������� ObjectBinaryTree, � ������� ������� ���� testNode

	PrintBinaryTree(tree2, "tree2");
	PrintList(list);
	PrintBinaryTree(tree1, "tree1");

	RenameNode(testNode, "node1");// �������� �������������� ���������� ��-�� ����, ��� � ���������� tree2 ��� ���������� ������ � ����� ������

	PrintBinaryTree(tree2, "tree2");
	PrintList(list);
	PrintBinaryTree(tree1, "tree1");// ���� node3 ����� ��������� ����� ������, ��� ��������������� � ���, ��� �� �� ����� ����� ������� � ������

	RenameNode(testNode, "node4");//�������������� �������, �.�. ����� ��� �� ������ �� ����� �� ����������� ObjectBinaryTree, � ������� ������� ������ testNode

	PrintBinaryTree(tree2, "tree2");
	PrintList(list);
	PrintBinaryTree(tree1, "tree1");

	RenameNode(testNode, "node2");// �������� �������������� ���������� ��-�� ����, ��� � ���������� tree1 ��� ���������� ������ � ����� ������ 

	PrintBinaryTree(tree2, "tree2");
	PrintList(list);
	PrintBinaryTree(tree1, "tree1");
	
	getch();
	return 0;
}