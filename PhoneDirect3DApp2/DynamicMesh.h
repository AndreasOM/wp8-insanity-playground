#pragma once
#include "mesh.h"

class Md3;

class DynamicMesh : public Mesh
{
	typedef Mesh BaseClass;
	public:
						DynamicMesh(void);
		virtual			~DynamicMesh(void);
		void			initialize(  Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice, const Md3* pMd3 );
};

