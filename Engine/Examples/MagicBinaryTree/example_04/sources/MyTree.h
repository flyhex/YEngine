#ifndef MYTREE_H
#define MYTREE_H

// INCLUDES -----------------------------------------------------------------------------------------
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Math/Vector/Vector3D/Vector3D.h>  
#include <Engine/Output/Graphics/Color/ColorRGBA/ColorRGBA.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include "MyNode.h"
// CLASS --------------------------------------------------------------------------------------------


// �������� ���������� ������
class MyTree: public MagicBinaryTree<MyNode, int32>
{
	// DATA
	private: float minimumAngleBetweenChilds; // ���������� ���������� ���� ����� ��������� ������ ���� 
	private: MyNode* selected;// ���������� ����
	private: MyNode* marked;// ���������� ����
	private: SimpleList<MyNode*> animSeq;// ������� ��������
	private: int32 seqIndex; // ������ ����, ������� ����������� � ������ ������
	private: bool play;// ���� true, �� ����������� �������� �������
	private: float timer;//������, �������������� ������������� �������� �� ��������
	private: int32 animFPS;// �������� �������� (���������� ������ � �������)
	private: int32 height;// ������ ������ 
	private: int32 excessHeight; // ������ ������ ������ ������


	// METHODS
	// ����������� �� ���������
	public: MyTree(void);


	// �������� ������� ��������
	public: void ClearAnimSeq(void);


	// ��������� ���� _node � ����� ������� ��������
	public: void AddToAnimSeq(MyNode* _node);



	// ���������� ������ ������� ��������
	public: int32 AnimSeqSize(void)const;


	// ������������ ������� ��������
	// ����������: � �������� _delta ���������� ���������� ������� (� ��������) ����� ������� � ���������� ������� ������� �����
	// ����������: ������� �������� �� ������ ���� ������
	// ����������: ����� ������������ ���� �������, �������� ����� ��������������
	public: void AnimSeqUpdate(float _delta);


	// ���������� true ���� �������� �������������
	public: bool IsPlaying(void)const;


	// ��������� ������������ ������� ��������
	// ����������: ���� ������� �������� �����, �� ������������ ����� ��������
	// ����������: �� ��������� �������� ���������
	public: void PlayAnim(void);


	// ��������� ������������ ������� ��������
	public: void StopAnim(void);


	// ������ ����� �������� �������� � FPS (���������� ������ � �������)
	public: void SetAnimFPS(int32 _fps);


	// ���������� ������ ������
	public: int32 StaticHeight(void)const;

			
	// �������� ������ ������
	public: void UpdateHeight(void);


	// ���������� ���������� ������ ������� ������ ��� ������� ���������� ����� 
	public: int32 StaticExcessHeight(void)const;


	// �������� ���������� ������ ������� ������ ��� ������� ���������� ����� 
	public: void UpdateExcessHeight(void);


	// ���������� �������� �������� � FPS (���������� ������ � �������)
	public: int32 AnimFPS(void)const;



	// ��������� ����� ���� _node
	// ����������: ���������� ���� ����������� ���������� ���� �� �����; ���������� � ���������� ���� ����������� ���� �� ������ ������� ������ �� �����
	private: void UpdateColors(MyNode* _node);


	// �������� ���� _node
	// ����������: ���������� ���� ����� ����� ������� ����
	// ����������: ��������� � ����������� ���� ����� �����, ����� �������, � ���� ������ ������� ����� ���� ������� ������ ���� ����
	public: void Select(MyNode* _node);


	// ���������� ���������� ����
	public: MyNode* Selected(void)const;


	// �������� ���� _node
	// ����������: ���������� ���� ����� ����� ������ ����
	// ����������: ����� � ����������� ���� ����� �����, ����� �������, � ���� ������ ������� ����� ���� ������� ������ ���� ����
	public: void SetMark(MyNode* _node);


	// ���������� ���������� ����
	public: MyNode* Marked(void)const;


	// ������� ���� _node �� ������
	// ����������: ���� ��������� ���� ������� � �������, �� ����� � ��������� ����� �����
	public: void Del(MyNode* _node);



	// ������������� �������� ������ 
    public: void Draw(void);



	// ����������� �������, ������� ������������ ������������ ������
    private: void Draw(MyNode* _node, int _deep, float _lineAngle, float _lineAngleDivisor, float _lineLength);
};

#endif
