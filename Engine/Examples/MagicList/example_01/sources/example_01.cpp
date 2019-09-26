// example_03.cpp: ���������� ����� ����� ��� ����������� ����������.
//


#include "stdafx.h"
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include "conio.h"
#include <Engine/Core/Container/Tree/MagicBinaryTree/MagicBinaryTree.h>
#include <string>
using std::string;



class MyNode: public MagicNode
{
	// DATA
	public: int32 key;

	// METHODS
	// ����������� �� ���������
	public: MyNode(void): key(0)
	{
		printf("\nMyNode const");
	}

	// ���������������� �����������
	public: MyNode(int32 _key): key(_key)
	{
		printf("\nMyNode const");
	}


	
	// ����������
	public: ~MyNode(void)
	{
		printf("\n~MyNode");
	}

	// �������� ���������
	public: bool operator == (const MyNode& _node)
	{
		if(key == _node.key)
		{
			return true;
		}
		return false;
	}


	public: int32 Key(void)const
	{
		return key;
	}
};




// ������� ��������� ������ _list
void PrintMagicListState(MagicList<MyNode>& _list, StringANSI _string)
{
	printf("\n%s.Head: %d", _string.c_str(), _list.Head());
	printf("\n%s.Tail: %d", _string.c_str(), _list.Tail());
	printf("\n%s.IsEmpty: %d", _string.c_str(), _list.IsEmpty());
	printf("\n%s.Size: %d", _string.c_str(), _list.Size());
}

// ������� ��������� ������ _tree
void PrintMagicBinaryTreeState(MagicBinaryTree<MyNode>& _tree, StringANSI _string)
{
	printf("\n%s.Root: %d", _string.c_str(), _tree.Root());
	printf("\n%s.IsEmpty: %d", _string.c_str(), _tree.IsEmpty());
	printf("\n%s.Size: %d", _string.c_str(), _tree.Size());
}


// ������� ��������� ���� _node
void PrintNodeState(MyNode* _node, MagicList<MyNode>& _list, StringANSI _string)
{
	if(_node)
	{
		printf("\n%s[%d]: ", _string.c_str(), _list.IndexOf(_node));

		printf("\n(%d).Key: %d", _node, _node->Key());
		printf("\n(%d).Left: %d", _node, _list.LeftOf(_node));
		printf("\n(%d).Right: %d", _node, _list.RightOf(_node));
	}
}

// ������� ��������� ���� ����� ������ _list
void PrintAllNodes(MagicList<MyNode>& _list, StringANSI _string)
{
	// ������ ���� ������
	MyNode* it = _list.Head();
	while(it)
	{
		PrintNodeState(static_cast<MyNode*>(it), _list, _string);// ������� ��������� ����
		it = _list.RightOf(it);// ����� ������� ������
	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	// ������� ���� � �������� � ������ + ������� ���� ����� delete
	{
		MagicList<MyNode> list1;

		MyNode* node = new MyNode(1);
		list1.AddToTail(node);
		PrintMagicListState(list1, "list1");
		printf("\ndelete node...");
		delete node;
		PrintMagicListState(list1, "list1");
	}
	printf("\n");

	// ������� ��� ���� + ��������� ���� ���� + ��������� ��� ���� ����� Clear
	{
		MagicList<MyNode> list1;

		MyNode* node = new MyNode(1);
		list1.AddToTail(node);
		list1.AddToTail(node = new MyNode(2));
		list1.AddToTail(node = new MyNode(3));
		PrintMagicListState(list1, "list1");
		printf("\nlist1.Remove(tail)...");
		list1.Remove(node);
		PrintMagicListState(list1, "list1");
		printf("\nlist1.Clear(MagicContainer::REMOVE_NODE)...");
		list1.Clear(MagicContainer::REMOVE_NODE);
		PrintMagicListState(list1, "list1");
	}
	printf("\n");

	// ������� ��� ���� + ������� ��������� ����� + ������� ��� ���� ����� Clear
	{
		MagicList<MyNode> list1;

		MyNode* node = new MyNode(1);
		list1.AddToHead(node);
		list1.AddToHead(node = new MyNode(2));
		PrintMagicListState(list1, "list1");
		
		PrintAllNodes(list1, "list1");


		printf("\nlist1.Clear(MagicContainer::DELETE_NODE)...");
		list1.Clear(MagicContainer::DELETE_NODE);
		PrintMagicListState(list1, "list1");
	}
	printf("\n");


	// �������� ���� ����� + ����� ���� + ��������� ���� �� ������� + ����� ���������� + ���������� ���� �����
	{
		MagicList<MyNode> list1;

		MyNode* node = new MyNode(3);
		list1.AddToHead(node);
		list1.AddToHead(node = new MyNode(5));
		PrintMagicListState(list1, "list1");
		
		PrintAllNodes(list1, "list1");


		printf("\nlist1.Find(3): %d", list1.Find(3, &MyNode::Key));
		printf("\nlist1.Get(1): %d", list1.Get(1));
		printf("\nlist1.Extremum(true): %d", list1.Extremum(&MyNode::Key, true));

		list1.AddToRight(list1.Head(), node = new MyNode(0));
		list1.AddToLeft(list1.Tail(), node = new MyNode(0));
		PrintAllNodes(list1, "list1");
	}
	printf("\n");


	// AddTo, AddRandom, Distance, GetFrom
	{
		MagicList<MyNode> list1;
		MagicList<MyNode> list2;

		MyNode* node = new MyNode(1);
		list1.AddToTail(node);
		list1.AddToTail(node = new MyNode(3));
		
		PrintMagicListState(list1, "list1");
		PrintAllNodes(list1, "list1");

		list1.AddTo(1, node = new MyNode(2));
		PrintAllNodes(list1, "list1");

		list1.AddRandom(node = new MyNode(10));
		PrintMagicListState(list1, "list1");
		PrintAllNodes(list1, "list1");

		printf("\nlist1.Distance(head, tail): %d", list1.Distance(list1.Head(), list1.Tail()));
		printf("\nlist1.GetFrom(head, 1): %d", list1.GetFrom(list1.Head(), 1));	
	}
	printf("\n");


	// RemoveRange, MoveTo, Share
	{
		MagicList<MyNode> list1;
		MagicList<MyNode> list2;

		MyNode* node = new MyNode(1);
		list1.AddToTail(node);
		list1.AddToTail(node = new MyNode(2));
		list1.AddToTail(node = new MyNode(3));
		list1.AddToTail(node = new MyNode(4));

		printf("\nlist1.MoveTo(0, 2, list2, 0)...");
		list1.MoveTo(0, 2, list2, 0);
		PrintMagicListState(list1, "list1");
		PrintMagicListState(list2, "list2");

		printf("\nlist1.RemoveRange(0, 1)...");
		list1.RemoveRange(0, 0, true);
		PrintMagicListState(list1, "list1");

		printf("\nlist2.Share(0, 2, list1)...");
		list2.Share(0, 2, list1, 0);
		PrintMagicListState(list1, "list1");
		PrintMagicListState(list2, "list2");
	}
	printf("\n");



	getch();
	return 0;
}

