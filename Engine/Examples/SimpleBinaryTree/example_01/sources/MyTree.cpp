// INCLUDES -----------------------------------------------------------------------------------------
#include "MyTree.h"
#include <Engine/Math/Angle/Angle.h> 
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
// CLASS --------------------------------------------------------------------------------------------


MyTree::MyTree(void): selected(NIL), marked(NIL)
{	
	minimumAngleBetweenChilds = 10.0f; // ��� ������� ������ ������ ����� ������ (55.0f), ����� ���������� ����������, ����� ������� �� ������������
}



void MyTree::Select(MyTree::Node* _node)
{
	selected = _node;// �������� ���������� ����
}



MyTree::Node* MyTree::Selected(void)const
{
	return selected;
}



void MyTree::SetMark(MyTree::Node* _node)
{
	marked = _node;// �������� ���������� ����
}



MyTree::Node* MyTree::Marked(void)const
{
	return marked;
}


void MyTree::Del(MyTree::Node* _node)
{
	if(_node)// ���� ���� ���������� ����
	{
		if(_node == marked) { SetMark(NIL); } // ����� ����� � ���������� ����
		if(_node == selected) { Select(NIL); } // ����� ��������� � ���������� ����

		Remove(_node);//��������� ���������� ����; ������������� ����������
	}
}



void MyTree::Draw(void)
{
	Draw(Root(), // ����� ������� ������������ 
		 Vector3D(), 
		 1, // ������� ������� ������
		 0.0f, // ��������� ���� ����� ��� �������� ���� 
		 1.0f, // ����������� ������� ���� �� ������ ���� ��������; ��� �������� ���� (1.0f = ����, > 1.0f = ������, �������� �����)  
		 70.0f/100.0f); // ��������� ����� ����� ��� �������� ���� (50 ����������)
}

 

void MyTree::Draw(MyTree::Node* _node, Vector3D _parentPos, int _deep, float _lineAngle, float _lineAngleDivisor, float _lineLength)
{
	/* ����������: ������ ���� ���������� ������ ���� ���; ���� ���������� �� ����, ��� ���������� �� ������� */ 
	if(_node) // ���� ���� ���������� 
	{
		RGBA color = RGBA(RGBA::WHITE_COLOR); // ���� ��� �����
		Vector3D nodePos;// ���������� ���� _node
		
		// �������� ���� ����
		if(_node)// ���� ���� ����������
		{
			if(_node == marked) { color = RGBA(RGBA::YELLOW_COLOR); } // ���� ��� ���������� �����
			if(_node == selected) { color = RGBA(RGBA::GREEN_COLOR); } // ���� ��� ���������� �����
		}

		if(_node->Parent())// ���� ���� ��������, ������ ������� ���� �� ������
		{
			RGBA colorLine; // ���� �����, ������� ��������� ������� ���� � ���������� 

			// ������ ������������ ����
			float nodeAngleRelativeToTheParent = 360.0f / pow(2.0f, _deep); // ���� � �������� ��� �������� ����, ������������ �����; ����� ���������� (360 ��������) �� 2 � ������� _deep
			 
			// ������� ����� �� ������ ���� 
			if((nodeAngleRelativeToTheParent / _lineAngleDivisor) < minimumAngleBetweenChilds)// �� ���� ������������ ���������� ���� minimumAngleBetweenChilds
			{
				nodeAngleRelativeToTheParent = minimumAngleBetweenChilds;
			}
				
			nodeAngleRelativeToTheParent /= _lineAngleDivisor; 
				
	
			if(_node->Parent()->Child(1) == _node)// ���� �� ������ ������� 
			{
				_lineAngle += nodeAngleRelativeToTheParent;// �������� ���� ��� �������� ����
				colorLine = RGBA(RGBA::RED_COLOR); // ���� ����� ��� ������ ����� 
			}
			else// ����� �� ����� ������� 
			{
				_lineAngle -= nodeAngleRelativeToTheParent; // �������� ���� ��� �������� ���� 
				colorLine =RGBA(RGBA::BLUE_COLOR); // ���� ����� ��� ����� ����� 
			}
				
			// ��������� ���������� ���� 
			nodePos = Vector3D(_parentPos.x + sinf((180.0f - _lineAngle) * Angle::PI / 180.0f) * _lineLength,
				               _parentPos.y + cosf((180.0f - _lineAngle) * Angle::PI / 180.0f) * _lineLength,
							   0.0f);

			// ����� ����� 
			GraphicsLine<Vector3D>::_Draw(_parentPos, nodePos, colorLine);
	

			GraphicsPoint<Vector3D>::_Draw(nodePos, color);// ����� ���� �� ����� 
		}
		else // ����� ������� ���� - ������ ������ 
		{	
			// ������ ���������� ������ ��� ����� 
			nodePos = Vector3D(0.0f, 0.0f, 0.0f);
	
			GraphicsPoint<Vector3D>::_Draw(nodePos, color);// ����� ���� �� ����� 
		}

		++_deep; // ������� ���������� ������

		// ���� ���������� �� ����, ��� ���������� �� �������
		Draw(_node->Child(0), nodePos, _deep, _lineAngle, _lineAngleDivisor, _lineLength); 
		Draw(_node->Child(1), nodePos, _deep, _lineAngle, _lineAngleDivisor, _lineLength);
	} 
}