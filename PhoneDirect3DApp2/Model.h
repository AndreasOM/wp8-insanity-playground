#pragma once

#include <d3d11_1.h>
#include <wrl/client.h>

// :TODO: we intentionally _don't_ abstract away the renderer and use D3D11
// this will change in the future, but keeps us focused on D3D11 now, which is the topic of this whole thing

class Model
{
	public:
							Model(void);
		virtual				~Model(void);
		void				initialize( Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice );


		virtual void		update( float timeStep ) {}
		virtual void		render(  Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dContext ) {}
//		bool		loadMD3( const char *pFilename );
	protected:
		Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice;
	private:
};

