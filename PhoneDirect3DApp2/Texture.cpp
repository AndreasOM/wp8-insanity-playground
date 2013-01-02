//#include "pch.h"
#include "Texture.h"

#include <stdlib.h>

Texture::Texture(void)
	: m_width( 0 )
	, m_height( 0 )
	, m_pData( nullptr )
{
}


Texture::~Texture(void)
{
}

void Texture::initialize( int width, int height )
{
//	ASSERT( this->m_pData == nullptr );

	m_width = width;
	m_height = height;

	m_pData = ( void* )new unsigned char[ m_width*m_height*4*4 ];

	noise( 8, 8 );

}

void Texture::shutdown()
{
}

void Texture::noise( int rw, int rh ) // :TODO: move to generator
{
	float* pRandom = new float[ rw*rh ];

	for( int y=0; y<rh; ++y )
	{
		for( int x=0; x<rw; ++x )
		{
			pRandom[ y*rw + x ] = ( float )rand()/RAND_MAX;
		}
	}

	float* pData = ( float* )m_pData;
	for( int y=0; y<128; ++y )
	{
		for( int x=0; x<128; ++x )
		{
			float rx = ( x*( rw-1 ) )/128.0f;
			float ry = ( y*( rh-1 ) )/128.0f;

			int rix = ( int )rx;
			float rfx = rx-rix;

			int riy = (int)ry;
			float rfy = ry-riy;

			float corners[ 4 ];
			corners[ 0 ] = pRandom[ ( int )riy*rw + rix ];
			corners[ 1 ] = pRandom[ ( int )riy*rw + rix+1 ];
			corners[ 2 ] = pRandom[ ( int )( riy+1 )*rw + rix ];
			corners[ 3 ] = pRandom[ ( int )( riy+1 )*rw + rix+1 ];

			float rl = pRandom[ ( int )ry*rw + rix ];
			float rr = pRandom[ ( int )ry*rw + rix+1 ];
//			float r = ( 1-rfx )*rl+( rfx )*rr;

			float upper = ( 1-rfx )*corners[ 0 ]+( rfx )*corners[ 1 ];
			float lower = ( 1-rfx )*corners[ 2 ]+( rfx )*corners[ 3 ];
			float r = ( 1-rfy )*upper+( rfy )*lower;

			float u = ( x-64 )/128.0f;
			float v = ( y-64 )/128.0f;
			int i = 128*y+x;
//			pData[ i*4+0 ] = 1*( sinf( 2*3.14f*u )+sinf( 2*3.14f*v ) );
			float c = ( 0.25f*( u*u+v*v )+0.75f )*( 0.5f*r+0.5f );
//			unsigned char byteColor = ( unsigned char )( c*255.0f );
			pData[ i*4+0 ] = c;
			pData[ i*4+1 ] = c;
			pData[ i*4+2 ] = c;
//			pData[ i*4+0 ] = 1*( (x+y)%256 )/256.0f;
//			pData[ i*4+1 ] = 1*( (x+y)%256 )/256.0f;
//			pData[ i*4+2 ] = 1*( (x+y)%256 )/256.0f;
			pData[ i*4+3 ] = 1.0f;
		}
	}
	/*
	for( int i=0; i<128*128; ++i )
	{
		pData[ i*4+0 ] = ( i%128 )/128.0f;
		pData[ i*4+1 ] = ( i%128 )/128.0f;
		pData[ i*4+2 ] = ( i%128 )/128.0f;
		pData[ i*4+3 ] = 1.0;
	}
	*/

}
