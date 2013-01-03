//#include "pch.h"
#include "Matrix33.h"


Matrix33::Matrix33(void)
{
}


Matrix33::~Matrix33(void)
{
}


Matrix33& Matrix33::createUnit()
{
	x.x = 1.0f; x.y = 0.0f; x.z = 0.0f;
	y.x = 0.0f; y.y = 1.0f; y.z = 0.0f;
	z.x = 0.0f; z.y = 0.0f; z.z = 1.0f;

	return *this;
}

Matrix33& Matrix33::createScale( float s)
{
	x.x = s; x.y = 0.0f; x.z = 0.0f;
	y.x = 0.0f; y.y = s; y.z = 0.0f;
	z.x = 0.0f; z.y = 0.0f; z.z = s;

	return *this;
}
