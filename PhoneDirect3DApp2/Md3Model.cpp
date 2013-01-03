//#include "pch.h"
#include "Md3Model.h"

#include "Md3.h"
#include "D3DTexture.h"
#include "DynamicMesh.h"

Md3Model::Md3Model(void)
	: m_numberOfMeshes( 0 )
{
}


Md3Model::~Md3Model(void)
{
}

void Md3Model::update( float timeStep )
{
}

void Md3Model::render(  Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dContext )
{
	for( int m=0; m<m_numberOfMeshes; ++m )
	{
		m_meshes[ m ].pTexture->activate( d3dContext, 0 );
		m_meshes[ m ].pMesh->render( d3dContext );
	}
}

int Md3Model::addMd3( const char* pBasepath, const char* pMd3Filename, const char* pTextureFilename, int parent /*= -1*/, const char* pTag /*= nullptr */ )
{
	if( m_numberOfMeshes >= MaxMeshes )
	{
		return -1;
	}

	Md3* pMd3 = new Md3();
	pMd3->load( pBasepath, pMd3Filename );

	m_meshes[ m_numberOfMeshes ].pMd3 = pMd3;
	m_meshes[ m_numberOfMeshes ].pMesh = new DynamicMesh();
	m_meshes[ m_numberOfMeshes ].pMesh->initialize( m_d3dDevice, pMd3 );
	m_meshes[ m_numberOfMeshes ].pTexture = new D3DTexture();
	char buffer[ 1024 ];
	sprintf_s( buffer, "%s\\%s", pBasepath, pTextureFilename );
	m_meshes[ m_numberOfMeshes ].pTexture->load( buffer );
	m_meshes[ m_numberOfMeshes ].pTexture->storeToHardware( m_d3dDevice );

	if( pTag != nullptr )
	{
		m_meshes[ m_numberOfMeshes ].pTag = _strdup( pTag );
	}
	else
	{
		m_meshes[ m_numberOfMeshes ].pTag = nullptr;
	}
	m_meshes[ m_numberOfMeshes ].parent = parent;

	return m_numberOfMeshes++;
}

