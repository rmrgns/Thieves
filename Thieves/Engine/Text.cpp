#include "pch.h"
#include "Text.h"
#include "Engine.h"
#include "Scene.h"
#include "SceneManager.h"

void Text::Init()
{
	CreateD3D11On12Device();
	CreateD2DDevice();
	CreateTextInfo();
	_rect = D2D1::RectF(0.0f, 0.0f, GEngine->GetWindow().width, GEngine->GetWindow().height);
}

void Text::Update()
{
	// �ؽ�Ʈ ���
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::LOGIN)
	{

		SetTextInfo(0);
		wstring text = L"���ϵ� �α��� ȭ��";
		SetText(text, 400.f, 200.f, 1.f, 1.f);
		
		SetTextInfo(1);
		wstring text1 = L"���m D3D11On12 ������Ʈ �Դϴ�.";
		SetText(text1, 0.f, 0.f, 1.f, 1.f);
	
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		Vec3 pos = GEngine->GetThievesPacketManager()->GetVec();
		wstring text2 = L"x:" + to_wstring(int(pos.x)) + L" y:" + to_wstring(int(pos.y)) + L" z:" + to_wstring(int(pos.z));
		SetText(text2, 0.f, 0.f, 1.f, 1.f);
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

void Text::CreateTextInfo()
{
	// �ؽ�Ʈ ����
	ThrowIfFailed(_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), _solidColorBrush.GetAddressOf()));
	
	// �ؽ�Ʈ ��Ʈ ����
	ThrowIfFailed(_dWriteFactory->CreateTextFormat(L"Verdana", nullptr,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		10, L"en-us", _writeTextFormat.GetAddressOf()));

	//_writeTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	//_writeTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
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

void Text::Render2D()
{
	int8 backIndex = GEngine->GetSwapChain()->GetBackBufferIndex();

	// Render text directly to the back buffer.
	_d2dDeviceContext->SetTarget(_d2dRenderTargets[backIndex].Get());
	_d2dDeviceContext->BeginDraw();

	Update();

	ThrowIfFailed(_d2dDeviceContext->EndDraw());

	// Flush to submit the 11 command list to the shared command queue.
	_d3d11DeviceContext->Flush();
}

void Text::SetTextInfo(int infoNumber)
{
	switch (infoNumber)
	{
	case 0:
		// �ؽ�Ʈ ����
		ThrowIfFailed(_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Crimson), _solidColorBrush.GetAddressOf()));

		// �ؽ�Ʈ ��Ʈ ����
		ThrowIfFailed(_dWriteFactory->CreateTextFormat(L"Verdana", nullptr,
			DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_ITALIC, DWRITE_FONT_STRETCH_NORMAL,
			25, L"en-us", _writeTextFormat.GetAddressOf()));

		//_writeTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		//_writeTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case 1:
		// �ؽ�Ʈ ����
		ThrowIfFailed(_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), _solidColorBrush.GetAddressOf()));

		// �ؽ�Ʈ ��Ʈ ����
		ThrowIfFailed(_dWriteFactory->CreateTextFormat(L"Verdana", nullptr,
			DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_ITALIC, DWRITE_FONT_STRETCH_NORMAL,
			25, L"en-us", _writeTextFormat.GetAddressOf()));

		break;
	default:
		break;
	}
	
}

void Text::SetText(wstring text, float posX, float posY, float scaleX, float scaleY)
{
	wstring _text = text;
	D2D1::Matrix3x2F matrix{};

	matrix.SetProduct(D2D1::Matrix3x2F::Scale(scaleX, scaleY, { _rect.right, _rect.bottom }),
		D2D1::Matrix3x2F::Translation(posX, posY));
	_d2dDeviceContext->SetTransform(matrix);
	_d2dDeviceContext->DrawTextW(text.c_str(), static_cast<UINT32>(text.size()), _writeTextFormat.Get(), &_rect, _solidColorBrush.Get());

}

