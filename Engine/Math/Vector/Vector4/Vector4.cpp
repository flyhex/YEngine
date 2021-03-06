
#include "Vector4.h"


const int32 Vector4::SIZE = 4;



Vector4::Vector4(void): x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{}


Vector4::Vector4(float _x, float _y, float _z, float _w): x(_x), y(_y), z(_z), w(_w)
{}


Vector4::Vector4(Vector3 _vector, float _w)
{
	x = _vector.x;
	y = _vector.y;
	z = _vector.z;
	w = _w;
}



void Vector4::Set(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}


void Vector4::Set(Vector3 _vector, float _w)
{
	x = _vector.x;
	y = _vector.y;
	z = _vector.z;
	w = _w;
}


bool Vector4::operator == (const Vector4& _vector)const 
{ 
	return (x == _vector.x && y == _vector.y && z == _vector.z && w == _vector.w);	
}


bool Vector4::operator != (const Vector4& _vector)const 
{ 
	return !(*this == _vector);
}



float& Vector4::operator [] (int32 _index)
{ 
	return *(&x + _index);
}
	

float Vector4::operator [] (int32 _index)const
{ 
	return *(&x + _index);
}

