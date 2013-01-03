//#include "pch.h"
#include "Md3.h"

#include "File.h"

#include <string.h>

Md3::Md3(void)
{
}


Md3::~Md3(void)
{
}


bool Md3::load( const char* pBasepath, const char* pFilename )
{
	char fullname[ 1024 ];

	sprintf_s( fullname, "%s\\%s", pBasepath, pFilename );
	File file( fullname );
	if( !file.isOpen() )
	{
		return false;
	}

	int32 ident;

	file.readSint32( &ident );
	if( ident != 0x33504449 )
	{
		// ident mismatch (might be endian swapped)
		return false;
	}

	int32 version;

	file.readSint32( &version );

	file.readBytes( m_pName, 64 );

	int32 flags;
	file.readSint32( &flags );

	file.readSint32( &m_numberOfFrames );
	file.readSint32( &m_numberOfTags );
	file.readSint32( &m_numberOfSurfaces );
	file.readSint32( &m_numberOfSkins );

	int32 frameOffset;
	int32 tagOffset;
	int32 surfaceOffset;
	int32 eofOffset;

	file.readSint32( &frameOffset );
	file.readSint32( &tagOffset );
	file.readSint32( &surfaceOffset );
	file.readSint32( &eofOffset );

	file.setPosition( frameOffset );

	m_pFrames = new Frame[ m_numberOfFrames ];

	for( int f = 0; f<m_numberOfFrames; ++f )
	{
		Frame& frame = m_pFrames[ f ];

		file.readFloat32( &frame.minBounds[ 0 ] );
		file.readFloat32( &frame.minBounds[ 1 ] );
		file.readFloat32( &frame.minBounds[ 2 ] );
		file.readFloat32( &frame.maxBounds[ 0 ] );
		file.readFloat32( &frame.maxBounds[ 1 ] );
		file.readFloat32( &frame.maxBounds[ 2 ] );
		file.readFloat32( &frame.origin[ 0 ] );
		file.readFloat32( &frame.origin[ 1 ] );
		file.readFloat32( &frame.origin[ 2 ] );
		file.readFloat32( &frame.radius );

		file.readBytes( &frame.pName, sizeof( frame.pName ) );
	}

	m_pTags = new Tag[ m_numberOfTags ];

	file.setPosition( tagOffset );

	for( int t = 0; t<m_numberOfTags; ++t )
	{
		Tag& tag = m_pTags[ t ];

		file.readBytes( tag.pName, sizeof( tag.pName ) );
		file.readFloat32( &tag.origin[ 0 ] );
		file.readFloat32( &tag.origin[ 1 ] );
		file.readFloat32( &tag.origin[ 2 ] );
		file.readFloat32( &tag.axis[ 0 ] );
		file.readFloat32( &tag.axis[ 1 ] );
		file.readFloat32( &tag.axis[ 2 ] );
		file.readFloat32( &tag.axis[ 3 ] );
		file.readFloat32( &tag.axis[ 4 ] );
		file.readFloat32( &tag.axis[ 5 ] );
		file.readFloat32( &tag.axis[ 6 ] );
		file.readFloat32( &tag.axis[ 7 ] );
		file.readFloat32( &tag.axis[ 8 ] );
	}

	m_pSurfaces = new Surface[ m_numberOfSurfaces ];


	int currentSurfaceOffset = surfaceOffset;
	for( int s = 0; s<m_numberOfSurfaces; ++s )
	{
		file.setPosition( currentSurfaceOffset );
		Surface& surface = m_pSurfaces[ s ];
		file.readSint32( &ident );
		if( ident != 0x33504449 )
		{
			return false;
		}

		file.readBytes( surface.pName, sizeof( surface.pName ) );
		int32 flags;
		file.readSint32( &flags );
		int32 numberOfFrames;
		file.readSint32( &numberOfFrames );
		file.readSint32( &surface.numberOfShaders );
		file.readSint32( &surface.numberOfVertices );
		file.readSint32( &surface.numberOfTriangles );
		int32 triangleOffset;
		int32 shaderOffset;
		int32 stOffset;
		int32 xyznormalOffset;
		int32 endOffset;

		file.readSint32( &triangleOffset );
		file.readSint32( &shaderOffset );
		file.readSint32( &stOffset );
		file.readSint32( &xyznormalOffset );
		file.readSint32( &endOffset );

		// :TODO: load shaders

		surface.pTriangles = new Triangle[ surface.numberOfTriangles * numberOfFrames ];
		file.setPosition( currentSurfaceOffset+triangleOffset );

		for( int t=0; t<surface.numberOfTriangles*numberOfFrames; ++t )
		{
			// md3 is ordered clockwise, so invert winding
			file.readSint32( &surface.pTriangles[ t ].vertexIndices[ 0 ] );
			file.readSint32( &surface.pTriangles[ t ].vertexIndices[ 1 ] );
			file.readSint32( &surface.pTriangles[ t ].vertexIndices[ 2 ] );
//			file.readSint32( &surface.pTriangles[ t ].vertexIndices[ 1 ] );
		}

		surface.pTexCoords = new TexCoord[ surface.numberOfVertices ];
		file.setPosition( currentSurfaceOffset+stOffset );
		
		for( int t=0; t<surface.numberOfVertices; ++t )
		{
			file.readFloat32( &surface.pTexCoords[ t ].s );
			file.readFloat32( &surface.pTexCoords[ t ].t );
		}

		surface.pVertices = new Vertex[ surface.numberOfVertices * numberOfFrames ];
		file.setPosition( currentSurfaceOffset+xyznormalOffset );

		for( int v=0; v<surface.numberOfVertices * numberOfFrames; ++v )
		{
			Vertex& vertex = surface.pVertices[ v ];
			int16 p;
			file.readSint16( &p );
			vertex.coord[ 0 ] = p*( 1/64.0f );
			file.readSint16( &p );
			vertex.coord[ 1 ] = p*( 1/64.0f );
			file.readSint16( &p );
			vertex.coord[ 2 ] = p*( 1/64.0f );
			uint8 a;
			uint8 z;
			file.readUint8( &a );
			file.readUint8( &z );
			vertex.normal[ 0 ] = 0.0f;
			vertex.normal[ 1 ] = 0.0f;
			vertex.normal[ 2 ] = 0.0f;
		}

		currentSurfaceOffset += endOffset;
	}

	return true;
}

const Md3::Surface* Md3::getSurface( int index ) const
{
	if( index >= m_numberOfSurfaces )
	{
		return nullptr;
	}

	return &m_pSurfaces[ index ];
};

const Md3::Tag* Md3::findTag( const char* pTag ) const
{
	for( int t=0; t<m_numberOfTags; ++t )
	{
		if( strcmp( ( const char* )( m_pTags[ t ].pName ), pTag ) == 0 )
		{
			return &m_pTags[ t ];
		}
	}

	return nullptr;
}

