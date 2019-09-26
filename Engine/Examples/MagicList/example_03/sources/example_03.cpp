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
	// ���������� ���� � ������ � ������ ������������
	{
		MagicList<MyNode> list;
		MagicBinaryTree<MyNode> tree;

		MyNode* node = new MyNode(1);
		list.AddToTail(node);
		PrintMagicListState(list, "list");

		tree.Add(node, &MyNode::Key);
		PrintMagicBinaryTreeState(tree, "tree");
		
		printf("\ndelete node...");
		delete node;
		PrintMagicListState(list, "list");
		PrintMagicBinaryTreeState(tree, "tree");

		node = new MyNode(1);
		list.AddToTail(node);
		tree.Add(node, &MyNode::Key);
		PrintMagicListState(list, "list");
		PrintMagicBinaryTreeState(tree, "tree");
		
		printf("\ndelete node->Adress(&list)...");
		delete node->Adress(&list);
		PrintMagicListState(list, "list");
		PrintMagicBinaryTreeState(tree, "tree");
	}

	getch();
	return 0;
}

