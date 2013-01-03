#pragma once
#include "mesh.h"

class Md3;

#include "Matrix43.h"

class DynamicMesh : public Mesh
{
	typedef Mesh BaseClass;
	public:
						DynamicMesh(void);
		virtual			~DynamicMesh(void);
		void			initialize(  Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice, const Md3* pMd3, const Matrix43* pMatrix = nullptr );

	private:
		Matrix43		m_matrix;
};

