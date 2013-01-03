#pragma once
#include "model.h"

class Md3;
class D3DTexture;
class DynamicMesh;

class Md3Model : public Model
{
	typedef Model BaseClass;
	public:
							Md3Model(void);
		virtual				~Md3Model(void);
		virtual void		update( float timeStep );
		virtual void		render(  Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dContext );

		int					addMd3( const char* pBasepath, const char* pMd3Filename, const char* pTextureFilename, int parent = -1, const char* pTag = nullptr );
	private:
		enum
		{
			MaxMeshes = 4
		};

		struct Mesh
		{
			int				parent;
			char*			pTag;
			Md3*			pMd3;
			D3DTexture*		pTexture;
			DynamicMesh*	pMesh;
		};

		Mesh			m_meshes[ MaxMeshes ];
		int				m_numberOfMeshes;

};

