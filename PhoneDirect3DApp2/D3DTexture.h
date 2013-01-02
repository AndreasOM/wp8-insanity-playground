#pragma once
#include "texture.h"
#include "DirectXHelper.h"
#include <d3d11_1.h>

class D3DTexture : public Texture
{
	typedef Texture BaseClass;
	public:
									D3DTexture(void);
		virtual						~D3DTexture(void);
		void						storeToHardware( Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice );
		void						activate( Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext, int samplerIndex );
	private:
		ID3D11ShaderResourceView*				m_pHardwareTextures[ 2 ];

};

