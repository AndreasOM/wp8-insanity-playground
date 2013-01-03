//#include "pch.h"
#include "Vector3.h"


Vector3::Vector3(void)
{
}


Vector3::~Vector3(void)
{
}

Vector3& Vector3::clear()
{
	x = 0;
	y = 0;
	z = 0;

	return *this;
}

Vector3& Vector3::add( const Vector3& other )
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}


