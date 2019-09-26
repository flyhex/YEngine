#ifndef MYNODE_H
#define MYNODE_H

// INCLUDES -----------------------------------------------------------------------------------------
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Math/Vector/Vector3D/Vector3D.h>  
#include <Engine/Output/Graphics/Color/ColorRGBA/ColorRGBA.h> 
#include <Engine/Core/Container/MagicNode.h>
// CLASS --------------------------------------------------------------------------------------------


// ���� ��������� ������
class MyNode: public GraphicsPoint<Vector3D, ColorRGBA> // ����������� �����
{
	// DATA
	private: int32 key;

	public: static ColorRGBA DEFAULT_COLOR;// ���� ������� ������
	public: static ColorRGBA SELECT_COLOR; // ���� ���������� ����� 
	public: static ColorRGBA MARK_COLOR; // ���� ���������� ����� 
    public: static ColorRGBA LEFT_COLOR; // ���� ������ �������
	public: static ColorRGBA RIGHT_COLOR; // ���� ������� �������
	public: static ColorRGBA EXCESS_COLOR;// ���� ��� ���� �� ������ ������ ������

	// METHODS
	// ����������� �� ���������
	public: MyNode(void);


	// ���������������� �����������
	public: MyNode(int32 _key);


	// ���������� ���� ������� ����
	public: int32 Key(void);
};




#endif
