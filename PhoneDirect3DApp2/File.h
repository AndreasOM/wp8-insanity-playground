#pragma once
#include <stdio.h>

class File
{
	public:
		File(void);
		File( const char* pFilename );
		~File(void);

		int		getSize() const;
		int		readBytes( void* pBuffer, int count );

		bool	isOpen() const{ return m_pFile != nullptr; }

		bool	readUint8( uint8* pData );
		bool	readSint16( int16* pData );
		bool	readSint32( int32* pData );
		bool	readFloat32( float* pData );

		void	setPosition( int pos );
	private:
		FILE*	m_pFile;
};

