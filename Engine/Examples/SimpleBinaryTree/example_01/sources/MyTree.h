#ifndef MYTREE_H
#define MYTREE_H

// INCLUDES -----------------------------------------------------------------------------------------
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Math/Vector/Vector3D/Vector3D.h>  
#include <Engine/Output/Graphics/Color/RGBA/RGBA.h>
#include <Engine/Core/Container/Tree/SimpleBinaryTree/SimpleBinaryTree.h>
//#include "Node.h"
// CLASS --------------------------------------------------------------------------------------------


// �������� ���������� ������
class MyTree: public SimpleBinaryTree<int32>
{
	// DATA
	private: float minimumAngleBetweenChilds; // ���������� ���������� ���� ����� ��������� ������ ���� 
	private: Node* selected;// ���������� ����
	private: Node* marked;// ���������� ����


	// METHODS
	// ����������� �� ���������
	public: MyTree(void);




	// �������� ���� _node
	// ����������: ���������� ���� ����� ����� ������� ����
	// ����������: ��������� � ����������� ���� ����� �����, ����� �������, � ���� ������ ������� ����� ���� ������� ������ ���� ����
	public: void Select(Node* _node);


	// ���������� ���������� ����
	public: Node* Selected(void)const;


	// �������� ���� _node
	// ����������: ���������� ���� ����� ����� ������ ����
	// ����������: ����� � ����������� ���� ����� �����, ����� �������, � ���� ������ ������� ����� ���� ������� ������ ���� ����
	public: void SetMark(Node* _node);


	// ���������� ���������� ����
	public: Node* Marked(void)const;


	// ������� ���� _node �� ������
	// ����������: ���� ��������� ���� ������� � �������, �� ����� � ��������� ����� �����
	public: void Del(Node* _node);



	// ������������� �������� ������ 
    public: void Draw(void);



	// ����������� �������, ������� ������������ ������������ ������
    private: void Draw(Node* _node, Vector3D _parentPos, int _deep, float _lineAngle, float _lineAngleDivisor, float _lineLength);
};

#endif
