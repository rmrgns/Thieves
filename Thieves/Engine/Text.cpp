#include "pch.h"
#include "Text.h"
#include "Engine.h"

void Text::Init()
{
	CreateD3D11On12Device();
	CreateD2DDevice();
	CreateRenderTarget();
}

void Text::CreateRenderTarget()
{
	// Query the desktop's dpi settings, which will be used to create D2D's render targets.
		float dpiX;
		float dpiY;
	#pragma warning(push)
	#pragma warning(disable : 4996) // GetDesktopDpi is deprecated.
		_d2dFactory->GetDesktopDpi(&dpiX, &dpiY);
	#pragma warning(pop)

	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dpiX,
		dpiY
	);

	//CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle{ _rtvHeap->GetCPUDescriptorHandleForHeapStart() };
	for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
	//	GEngine->GetSwapChain()->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(i)->GetTex2D().GetAddressOf()));
	//	DEVICE->CreateRenderTargetView(GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(i)->GetTex2D().Get(), NULL, rtvHandle);

	//	// Create a wrapped 11On12 resource of this back buffer. Since we are 
	//	// rendering all D3D12 content first and then all D2D content, we specify 
	//	// the In resource state as RENDER_TARGET - because D3D12 will have last 
	//	// used it in this state - and the Out resource state as PRESENT. When 
	//	// ReleaseWrappedResources() is called on the 11On12 device, the resource 
	//	// will be transitioned to the PRESENT state.
		D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
		ThrowIfFailed(_d3d11On12Device->CreateWrappedResource(
			GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(i)->GetTex2D().Get(),
			&d3d11Flags,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT,
			IID_PPV_ARGS(&_wrappedBackBuffer[i])
		));

		// Create a render target for D2D to draw directly to this back buffer.
		ComPtr<IDXGISurface> surface;
		ThrowIfFailed(_wrappedBackBuffer[i].As(&surface));
		ThrowIfFailed(_d2dDeviceContext->CreateBitmapFromDxgiSurface(
			surface.Get(),
			&bitmapProperties,
			&_d2dRenderTargets[i]
		));

		//rtvHandle.Offset(_rtvHeapSize);
	}
}

void Text::CreateD3D11On12Device()
{
	ComPtr<ID3D11Device> d3d11Device = nullptr;
	ThrowIfFailed(D3D11On12CreateDevice(DEVICE.Get(), D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		nullptr, 0, reinterpret_cast<IUnknown**>(GEngine->GetGraphicsCmdQueue()->GetCmdQueue().GetAddressOf()), 1, 0,
		&d3d11Device, &_d3d11DeviceContext, nullptr));

	ThrowIfFailed(d3d11Device.As(&_d3d11On12Device));
}

void Text::CreateD2DDevice()
{
	// Create D2D/DWrite components.
	D2D1_FACTORY_OPTIONS d2dFactoryOptions{};
	D2D1_DEVICE_CONTEXT_OPTIONS deviceOptions = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
	ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3),
		&d2dFactoryOptions, &_d2dFactory));
	ComPtr<IDXGIDevice> dxgiDevice;
	ThrowIfFailed(_d3d11On12Device.As(&dxgiDevice));
	ThrowIfFailed(_d2dFactory->CreateDevice(dxgiDevice.Get(), &_d2dDevice));
	ThrowIfFailed(_d2dDevice->CreateDeviceContext(deviceOptions, &_d2dDeviceContext));
	ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &_dWriteFactory));
}

void Text::Render2D()
{

	// Acquire our wrapped render target resource for the current back buffer.
	_d3d11On12Device->AcquireWrappedResources(&_wrappedBackBuffer[GEngine->GetSwapChain()->GetBackBufferIndex()], 1);

	// Render text directly to the back buffer.
	_d2dDeviceContext->SetTarget(_d2dRenderTargets[GEngine->GetSwapChain()->GetBackBufferIndex()].Get());
	_d2dDeviceContext->BeginDraw();
	//shared_ptr<Scene> m_scene = GET_SINGLE(SceneManager)->GetActiveScene();
	//if (GET_SINGLE(SceneManager)->GetActiveScene()) GET_SINGLE(SceneManager)->GetActiveScene()->Render(GEngine->Getd2dDeviceContext());

	ThrowIfFailed(_d2dDeviceContext->EndDraw());

	// Release our wrapped render target resource. Releasing 
	// transitions the back buffer resource to the state specified
	// as the OutState when the wrapped resource was created.
	_d3d11On12Device->ReleaseWrappedResources(_wrappedBackBuffer[GEngine->GetSwapChain()->GetBackBufferIndex()].GetAddressOf(), 1);

	// Flush to submit the 11 command list to the shared command queue.
	_d3d11DeviceContext->Flush();
}