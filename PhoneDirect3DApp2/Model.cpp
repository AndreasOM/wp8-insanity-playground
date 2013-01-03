//#include "pch.h"
#include "Model.h"


Model::Model(void)
{
}


Model::~Model(void)
{
}
/*
bool Model::loadMD3( const char* pFilename )
{
	return false;
}
*/

void Model::initialize( Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice )
{
	m_d3dDevice = d3dDevice;
}


