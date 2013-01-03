//#include "pch.h"

#include "DirectXHelper.h"
#include "DynamicMesh.h"

#include "Md3.h"

DynamicMesh::DynamicMesh(void)
{
}


DynamicMesh::~DynamicMesh(void)
{
}

void DynamicMesh::initialize(  Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice, const Md3* pMd3, const Matrix43* pMatrix /*= nullptr*/ )
{
	BaseClass::initialize( d3dDevice );

	if( pMatrix != nullptr )
	{
		m_matrix = *pMatrix;
	}
	else
	{
		m_matrix.rot.createUnit();
		m_matrix.pos.clear();
	}

	// :HACK:

//	const Md3::Surface* pSurface = pMd3->getSurface( pMd3->getNumberOfSurfaces() - 1  );
	const Md3::Surface* pSurface = pMd3->getSurface( 0  );
	if( pSurface == nullptr )
	{
		return;
	}

	VertexPositionColorTexCoord*	pVertices = new VertexPositionColorTexCoord[ pSurface->numberOfVertices ];

	Vector3 vec;
	for( int v=0; v<pSurface->numberOfVertices; ++v )
	{
		VertexPositionColorTexCoord& vert = pVertices[ v ];
		vec.x = pSurface->pVertices[ v ].coord[ 0 ];
		vec.y = pSurface->pVertices[ v ].coord[ 1 ];
		vec.z = pSurface->pVertices[ v ].coord[ 2 ];
		m_matrix.multiply( vec, vec );
		vert.pos.x = vec.x; 
		vert.pos.y = vec.y; 
		vert.pos.z = vec.z;
		vert.color.x = 1.0f;
		vert.color.y = 1.0f;
		vert.color.z = 1.0f;
		vert.texCoord.x = pSurface->pTexCoords[ v ].s;
		vert.texCoord.y = pSurface->pTexCoords[ v ].t;
	}

	D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
	vertexBufferData.pSysMem = pVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(pSurface->numberOfVertices*sizeof( VertexPositionColorTexCoord ), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		d3dDevice->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&m_vertexBuffer
			)
		);

	m_indexCount = pSurface->numberOfTriangles*3;

	unsigned short* pIndices = new unsigned short[ pSurface->numberOfTriangles*3 ];

	for( int i=0; i<pSurface->numberOfTriangles; ++i )
	{
		pIndices[ i*3 + 0 ] = pSurface->pTriangles[ i ].vertexIndices[ 0 ];
		pIndices[ i*3 + 1 ] = pSurface->pTriangles[ i ].vertexIndices[ 1 ];
		pIndices[ i*3 + 2 ] = pSurface->pTriangles[ i ].vertexIndices[ 2 ];
	}

	D3D11_SUBRESOURCE_DATA indexBufferData = {0};
	indexBufferData.pSysMem = pIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc( pSurface->numberOfTriangles*3*sizeof( unsigned short ), D3D11_BIND_INDEX_BUFFER );
	DX::ThrowIfFailed(
		d3dDevice->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&m_indexBuffer
			)
		);

}
