#pragma once
#include "texture.h"
#include "DirectXHelper.h"
#include <d3d11_1.h>

class D3DTexture : public Texture
{
	typedef Texture BaseClass;
	public:
		D3DTexture(void);
		virtual ~D3DTexture(void);
		void storeToHardware( Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice );
		void activate( int samplerIndex );
};

