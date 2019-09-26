// INCLUDES -----------------------------------------------------------------------------------------
#include "MyTree.h"
#include <Engine/Math/Angle/Angle.h> 
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
// CLASS --------------------------------------------------------------------------------------------


MyTree::MyTree(void): selected(NIL), marked(NIL), seqIndex(0), play(false), timer(0.0f), animFPS(1), height(0), excessHeight(0)
{	
	minimumAngleBetweenChilds = 10.0f; // ��� ������� ������ ������ ����� ������ (55.0f), ����� ���������� ����������, ����� ������� �� ������������
}


void MyTree::ClearAnimSeq(void)
{
	animSeq.Clear();
}


int32 MyTree::AnimSeqSize(void)const
{
	return animSeq.Size();
}


void MyTree::PlayAnim(void)
{
	if(animSeq.Size())
	{
		play = true;
	}
}


void MyTree::StopAnim(void)
{
	play = false;
}

void MyTree::SetAnimFPS(int32 _fps)
{
	animFPS = _fps;
}

int32 MyTree::AnimFPS(void)const
{
	return animFPS;
}


void MyTree::AnimSeqUpdate(float _delta)
{
	if(play && animFPS && animSeq.Size())// ���� ������������ �������� ���������, � FPS ������ ����, � ������� �������� �� �����
	{
		timer += _delta;// �������� ������

		if(timer >= (1.0f / (float)animFPS))// ���������� � �������� FPS
		{
			timer = 0.0f; // �������� ��������� �������

			// ���� �������� �����������, �� �������� �������
			if(seqIndex >= animSeq.Size()) 
			{ 
				seqIndex = 0; // ������� � ������ ��������
				StopAnim();// ��������� ������������ ��������
				return;// �������� �������
			}
			// ����� �������� ������������...
			Select(animSeq[seqIndex]->data);// �������� ���� ��� �������� seqIndex
			seqIndex++; // ��������� ������ ����
		}
	}
}

bool MyTree::IsPlaying(void)const
{
	return play;
}



void MyTree::AddToAnimSeq(MyNode* _node)
{
	if(_node) { animSeq.AddToTail(_node); }
}

void MyTree::UpdateColors(MyNode* _node)
{
	if(_node)// ���� ���� ����������
	{
		if(_node == marked) { _node->SetColor(MyNode::MARK_COLOR); } // ���� ��� ���������� �����
		if(_node == selected) { _node->SetColor(MyNode::SELECT_COLOR); } // ���� ��� ���������� �����
	}
}


void MyTree::Select(MyNode* _node)
{
	selected = _node;// �������� ���������� ����
}



MyNode* MyTree::Selected(void)const
{
	return selected;
}



void MyTree::SetMark(MyNode* _node)
{
	marked = _node;// �������� ���������� ����
}



MyNode* MyTree::Marked(void)const
{
	return marked;
}

void MyTree::UpdateHeight(void)
{
	height = Height();
}

void MyTree::UpdateExcessHeight(void)
{
	excessHeight = ExcessHeight();
}


int32 MyTree::StaticHeight(void)const
{
	return height;
}


int32 MyTree::StaticExcessHeight(void)const
{
	return excessHeight;
}




void MyTree::Del(MyNode* _node)
{
	if(_node)// ���� ���� ���������� ����
	{
		if(_node == marked) { SetMark(NIL); } // ����� ����� � ���������� ����
		if(_node == selected) { Select(NIL); } // ����� ��������� � ���������� ����

		//Remove(_node, 0);//��������� ���������� ����; ������������� ����������
		delete _node; // ������� ���������� ����
	}
}



void MyTree::Draw(void)
{
	Draw(Root(), // ����� ������� ������������ 
		 1, // ������� ������� ������
		 0.0f, // ��������� ���� ����� ��� �������� ���� 
		 1.0f, // ����������� ������� ���� �� ������ ���� ��������; ��� �������� ���� (1.0f = ����, > 1.0f = ������, �������� �����)  
		 70.0f/100.0f); // ��������� ����� ����� ��� �������� ���� (50 ����������)
}



void MyTree::Draw(MyNode* _node, int _deep, float _lineAngle, float _lineAngleDivisor, float _lineLength)
{
	/* ����������: ������ ���� ���������� ������ ���� ���; ���� ���������� �� ����, ��� ���������� �� ������� */ 
	if(_node) // ���� ���� ���������� 
	{
		// ���������� ���� ����...
		if(_deep > height - excessHeight)// ���� ���� ��������� �� ������ ������ ������
		{
		_node->SetColor(MyNode::EXCESS_COLOR);
		}
		else { _node->SetColor(MyNode::DEFAULT_COLOR); }// ���� ��� ��������� �����
		UpdateColors(_node);// ���� ���� ������� ��� �������
			

		if(ParentOf(_node))// ���� ���� ��������, ������ ������� ���� �� ������
		{
			ColorRGBA colorLine; // ���� �����, ������� ��������� ������� ���� � ���������� 

			// ������ ������������ ����
			float nodeAngleRelativeToTheParent = 360.0f / pow(2.0f, _deep); // ���� � �������� ��� �������� ����, ������������ �����; ����� ���������� (360 ��������) �� 2 � ������� _deep
			 
			// ������� ����� �� ������ ���� 
			if((nodeAngleRelativeToTheParent / _lineAngleDivisor) < minimumAngleBetweenChilds)// �� ���� ������������ ���������� ���� minimumAngleBetweenChilds
			{
				nodeAngleRelativeToTheParent = minimumAngleBetweenChilds;
			}
				
			nodeAngleRelativeToTheParent /= _lineAngleDivisor; 
				
	
			if(ChildOf(ParentOf(_node), 1) == _node)// ���� �� ������ ������� 
			{
				_lineAngle += nodeAngleRelativeToTheParent;// �������� ���� ��� �������� ����
				colorLine = MyNode::RIGHT_COLOR; // ���� ����� ��� ������ ����� 
			}
			else// ����� �� ����� ������� 
			{
				_lineAngle -= nodeAngleRelativeToTheParent; // �������� ���� ��� �������� ���� 
				colorLine = MyNode::LEFT_COLOR; // ���� ����� ��� ����� ����� 
			}
				
			// ��������� ���������� ���� 
			_node->SetPosition(Vector3D(ParentOf(_node)->GetPosition().x + sinf((180.0f - _lineAngle) * Angle::PI / 180.0f) * _lineLength,
				                        ParentOf(_node)->GetPosition().y + cosf((180.0f - _lineAngle) * Angle::PI / 180.0f) * _lineLength,
								        0.0f));

			// ����� ����� 
			GraphicsLine<Vector3D, ColorRGBA>::_Draw(ParentOf(_node)->GetPosition(), _node->GetPosition(), colorLine);
	
			_node->Draw(); // ����� ���� �� ����� 
		}
		else // ����� ������� ���� - ������ ������ 
		{	
			// ������ ���������� ������ ��� ����� 
			_node->SetPosition(Vector3D(0.0f, 0.0f, 0.0f));
	
			_node->Draw(); // ����� ���� �� ����� 
		}

		++_deep; // ������� ���������� ������

		// ���� ���������� �� ����, ��� ���������� �� �������
		Draw(ChildOf(_node, 0), _deep, _lineAngle, _lineAngleDivisor, _lineLength); 
		Draw(ChildOf(_node, 1), _deep, _lineAngle, _lineAngleDivisor, _lineLength);
	} 
}