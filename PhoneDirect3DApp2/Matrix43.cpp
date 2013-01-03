//#include "pch.h"
#include "Matrix43.h"


Matrix43::Matrix43(void)
{
}


Matrix43::~Matrix43(void)
{
}

Matrix43 Matrix43::operator=( const Matrix43& other )
{
	rot = other.rot;
	pos = other.pos;

	return *this;
}

Vector3& Matrix43::multiply( Vector3& t, const Vector3& v ) const
{
	float vX = v.x;
	float vY = v.y;
	float vZ = v.z;

	// :TODO: verify this is correkt
	t.x = rot.x.x * vX + rot.y.x * vY + rot.z.x * vZ + pos.x;
	t.y = rot.x.y * vX + rot.y.y * vY + rot.z.y * vZ + pos.y;
	t.z = rot.x.z * vX + rot.y.z * vY + rot.z.z * vZ + pos.z;

	return t;
}


