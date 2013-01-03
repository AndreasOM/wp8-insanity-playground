#pragma once

#include "Matrix33.h"

class Matrix43
{
	public:
		Matrix43(void);
		~Matrix43(void);
		Matrix43	operator=( const Matrix43& other );

		Vector3&	multiply( Vector3& t, const Vector3& v ) const;

		Matrix33	rot;
		Vector3		pos;
};

