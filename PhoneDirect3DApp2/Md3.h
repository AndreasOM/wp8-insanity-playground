#pragma once

// solely based on https://en.wikipedia.org/wiki/MD3_%28file_format%29
// no other documents used!

class Md3
{
	public:
		struct Frame
		{
			float			minBounds[ 3 ];
			float			maxBounds[ 3 ];
			float			origin[ 3 ];
			float			radius;
			unsigned char	pName[ 16 ];
		};
		struct Tag
		{
			unsigned char	pName[ 64 ];
			float			origin[ 3 ];
			float			axis[ 3*3 ];
		};
		struct Shader
		{
			unsigned char	pName[ 64 ];
			int				index;
		};
		struct Triangle
		{
			int				vertexIndices[ 3 ];
		};
		struct Vertex
		{
			float			coord[ 3 ];
			float			normal[ 3 ];
		};

		struct Surface
		{
			unsigned char	pName[ 64 ];
			int				numberOfShaders;
			int				numberOfVertices;
			int				numberOfTriangles;
			Shader*			pShaders;
			Triangle*		pTriangles;
			Vertex*			pVertices;
		};

		Md3(void);
		~Md3(void);
		bool		load( const char* pBasepath, const char* pFilename );

		int			getNumberOfSurfaces( ) const{ return m_numberOfSurfaces; }
		const Surface*	getSurface( int index ) const;
	private:
		char		m_pName[ 64 ];
		int			m_numberOfFrames;
		int			m_numberOfTags;
		int			m_numberOfSurfaces;
		int			m_numberOfSkins;

		Frame*		m_pFrames;
		Tag*		m_pTags;
		Surface*	m_pSurfaces;

};

