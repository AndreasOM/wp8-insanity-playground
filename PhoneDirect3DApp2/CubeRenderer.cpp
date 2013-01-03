//#include "pch.h"

#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>

#include "CubeRenderer.h"
#include "CubeMesh.h"
#include "CheckerMesh.h"
#include "D3DTexture.h"
#include "DynamicMesh.h"
#include "Md3.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

CubeRenderer::CubeRenderer() :
	m_loadingComplete(false),
	m_indexCount(0)
	, m_fov( 70.0f )
	, m_pMesh( nullptr )
	, m_pCheckerMesh( nullptr )
{
}

void CubeRenderer::CreateDeviceResources()
{
	Direct3DBase::CreateDeviceResources();


	auto loadVSTask = DX::ReadDataAsync("SimpleVertexShader.cso");
	auto createVSTask = loadVSTask.then([this](Platform::Array<byte>^ fileData) {
		DX::ThrowIfFailed(
			m_d3dDevice->CreateVertexShader(
				fileData->Data,
				fileData->Length,
				nullptr,
				&m_vertexShader
				)
			);

		const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			m_d3dDevice->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				fileData->Data,
				fileData->Length,
				&m_inputLayout
				)
			);
	});

	auto loadPSTask = DX::ReadDataAsync("SimplePixelShader.cso");
	auto createPSTask = loadPSTask.then([this](Platform::Array<byte>^ fileData) {
		DX::ThrowIfFailed(
			m_d3dDevice->CreatePixelShader(
				fileData->Data,
				fileData->Length,
				nullptr,
				&m_pixelShader
				)
			);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_d3dDevice->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
				)
			);
	});


	auto createCubeTask = (createPSTask && createVSTask).then([this] () {
		m_pMesh = new CubeMesh();
		m_pMesh->initialize( m_d3dDevice );
	});

	auto createCheckerTask = (createPSTask && createVSTask).then([this] () {
		m_pCheckerMesh = new CheckerMesh();
		m_pCheckerMesh->initialize( m_d3dDevice );
	});


	auto loadTextureTask = (createPSTask && createVSTask).then([this] () {
		m_pTexture = new D3DTexture( );
		m_pTexture->initialize( 128, 128 );
		m_pTexture->noise( 64, 64 );

		m_pTexture->load( "Assets\\noise.png" );
//		m_pTexture->load( "Assets\\test.png" );

		m_pTexture->storeToHardware( m_d3dDevice );
	});

	auto loadLaraTask = loadTextureTask.then([this]() {
		m_pMd3Lara = new Md3();

		m_pMd3Lara->load( "Assets\\Lara", "upper.md3" );

		m_pLaraMesh = new DynamicMesh();
		m_pLaraMesh->initialize( m_d3dDevice, m_pMd3Lara );
	});


	( createCubeTask && createCheckerTask && loadLaraTask ).then([this] () {
		m_loadingComplete = true;
	});

	// :TODO: m_vertexBuffer
}

void CubeRenderer::updateProjection()
{
	float aspectRatio = m_windowBounds.Width / m_windowBounds.Height;
	float fovAngleY = m_fov* XM_PI / 180.0f;
	if (aspectRatio < 1.0f)
	{
		fovAngleY /= aspectRatio;
	}

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(
			XMMatrixPerspectiveFovRH(
				fovAngleY,
				aspectRatio,
				0.01f,
				100.0f
				)
			)
		);
}

void CubeRenderer::CreateWindowSizeDependentResources()
{
	Direct3DBase::CreateWindowSizeDependentResources();
	updateProjection();
}

void CubeRenderer::Update(float timeTotal, float timeDelta)
{
	(void) timeDelta; // Unused parameter.

	m_fov = 70.0f + sinf( timeTotal*0.5f )*30.0f;
	updateProjection();

	float eyeZ = 2.5f - 1.0f*sinf( timeTotal*0.5f );

	static float eyeY = 0.7f;
	eyeY = 0.7f - 0.7f*sinf( timeTotal*3.0f );
	if( eyeY > 0.5f )
	{
		eyeY = 0.5f;
	}
	eyeY += 1.0f;

	XMVECTOR eye = XMVectorSet(0.0f, eyeY, eyeZ, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, eyeY-0.8f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));

	m_rotX = timeTotal * XM_PIDIV4;
	m_rotX = sinf( m_rotX*1.5f )*2.5f;

	m_rotZ += fabsf( 0.001f*m_rotX );


}

void CubeRenderer::Render()
{
	const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
	const float black[] = { 0.0f, 0.0f, 0.0f, 1.000f };
	m_d3dContext->ClearRenderTargetView(
		m_renderTargetView.Get(),
		black
		);

	m_d3dContext->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);

	// Only draw the cube once it is loaded (loading is asynchronous).
	if (!m_loadingComplete)
	{
		return;
	}

	m_d3dContext->OMSetRenderTargets(
		1,
		m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get()
		);

	XMStoreFloat4x4(
		&m_constantBufferData.model,
		XMMatrixTranspose(
			XMMatrixMultiply(
				XMMatrixMultiply(
					XMMatrixRotationY(m_rotX),
					XMMatrixRotationZ(m_rotZ )
				),
				XMMatrixTranslation( 0.0f, 0.0f, 0.0f )
			)
		)
	);
	m_d3dContext->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);


	m_d3dContext->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);

	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	m_d3dContext->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	m_d3dContext->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);

	m_pTexture->activate( m_d3dContext, 0 );

//	m_pMesh->render( m_d3dContext );


	float x = sinf( m_rotX )*3.0f;
	float y = -1.0f;
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixMultiply( XMMatrixRotationY(0.0f*m_rotX), XMMatrixTranslation( x, y, 0.0f ) ) ) );
	m_d3dContext->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);


	m_d3dContext->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);
	m_pCheckerMesh->render( m_d3dContext );

	static float dummy = 1.5f*3.14f;
//	dummy += 0.001f;
	XMStoreFloat4x4(
		&m_constantBufferData.model,
		XMMatrixTranspose(
			XMMatrixMultiply(
				XMMatrixRotationX( dummy ),
				XMMatrixTranslation( x, y+0.5f, 0.0f )
			)
		)
	);
	m_d3dContext->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);


	m_d3dContext->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);

	m_pLaraMesh->render( m_d3dContext );
}
