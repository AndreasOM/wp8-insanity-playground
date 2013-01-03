#pragma once
#include "Vector3.h"

class Matrix33
{
	public:
		Matrix33(void);
		~Matrix33(void);
		Matrix33&		createUnit();
		Matrix33&		createScale( float s );

		Vector3		x;
		Vector3		y;
		Vector3		z;
};

