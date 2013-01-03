#pragma once
class Vector3
{
	public:
		Vector3(void);
		~Vector3(void);

		Vector3&	clear();

		Vector3&	add( const Vector3& other );

		float	x;
		float	y;
		float	z;
};

