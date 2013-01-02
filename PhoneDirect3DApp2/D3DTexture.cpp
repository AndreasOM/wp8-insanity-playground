//#include "pch.h"
#include "D3DTexture.h"


D3DTexture::D3DTexture(void)
{
}


D3DTexture::~D3DTexture(void)
{
}

void D3DTexture::storeToHardware( Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice )
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory( &depthBufferDesc, sizeof( depthBufferDesc ) );

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = getWidth();
	depthBufferDesc.Height = getHeight();
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	depthBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = getData();
	data.SysMemPitch = getWidth()*4*4;
	data.SysMemSlicePitch = 0;

	ID3D11Texture2D* tex = 0;
	// Create the texture for the depth buffer using the filled out description.
	DX::ThrowIfFailed(
		d3dDevice->CreateTexture2D(&depthBufferDesc, &data, &tex)
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	memset( &SRVDesc, 0, sizeof( SRVDesc ) );
	SRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	DX::ThrowIfFailed(
		d3dDevice->CreateShaderResourceView( tex, &SRVDesc, &m_pHardwareTextures[0])
	);
//		m_d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//		m_d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

}

void D3DTexture::activate( Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext, int samplerIndex )
{
	m_d3dContext->PSSetShaderResources( 0, 1, m_pHardwareTextures );
	//deviceContext->PSSetSamplers(0, 1, &m_sampleState);
}

