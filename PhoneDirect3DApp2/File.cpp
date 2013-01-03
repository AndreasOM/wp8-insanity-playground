//#include "pch.h"
#include "File.h"

#include <string.h>

File::File(void)
	: m_pFile( nullptr )
{
}

File::File( const char *pFilename )
{
	errno_t err = fopen_s( &m_pFile, pFilename, "rb" );
}

File::~File(void)
{
	if( m_pFile != nullptr )
	{
		fclose( m_pFile );
		m_pFile = nullptr;
	}
}

int File::getSize() const
{
	int pos = ftell( m_pFile );
	fseek( m_pFile, 0, SEEK_END );
	int size = ftell( m_pFile );
	fseek( m_pFile, pos, SEEK_SET );

	return size;
}

void File::setPosition( int pos )
{
	fseek( m_pFile, pos, SEEK_SET );
}

int File::readBytes( void* pBuffer, int count )
{
	int read = fread( pBuffer, 1, count, m_pFile );

	return read;
}

bool File::readUint8( uint8* pData )
{
	int32 buffer;

	int read = fread( &buffer, 1, 1, m_pFile );
	if( read!=1 )
	{
		return false;
	}

	memcpy( pData, &buffer, 1 );

	return true;
};

bool File::readSint16( int16* pData )
{
	int32 buffer;

	int read = fread( &buffer, 1, 2, m_pFile );
	if( read!=2 )
	{
		return false;
	}

	// :TODO: fix endianess

	memcpy( pData, &buffer, 2 );

	return true;
};

bool File::readSint32( int32* pData )
{
	int32 buffer;

	int read = fread( &buffer, 1, 4, m_pFile );
	if( read!=4 )
	{
		return false;
	}

	// :TODO: fix endianess

	memcpy( pData, &buffer, 4 );

	return true;
};

bool File::readFloat32( float* pData )
{
	return readSint32( ( int32* )pData );
};
