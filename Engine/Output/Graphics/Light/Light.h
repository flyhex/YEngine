#ifndef MATERIAL_H
#define MATERIAL_H

// INCLUDES -----------------------------------------------------------------------------------------
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Core/Numerical/Numerical.h>
// CLASS --------------------------------------------------------------------------------------------


// �����, ����������� �������� �����
class Light: public virtual AbstractObject // ������
{
	// METHODS
	// ����������� ��� ����������
	public: Light(void);



	// ����������
	public: virtual ~Light(void);
};

#endif 