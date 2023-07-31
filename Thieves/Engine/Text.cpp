#include "pch.h"
#include "Text.h"
#include "Engine.h"
#include "Scene.h"
#include "LobbyScene.h"
#include "RoomScene.h"
#include "InGameScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "server\main\network.h"

void Text::Init()
{
	CreateD3D11On12Device();
	CreateD2DDevice();
	SetTextInfo(TEXT_FORMAT::DEFALUT);
	_rect = D2D1::RectF(0.0f, 0.0f, GEngine->GetWindow().width, GEngine->GetWindow().height);

}

void Text::Update()
{
	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	// 텍스트 출력
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::LOGIN)
	{
		SetTextInfo(TEXT_FORMAT::LOGIN);
		wstring text = L"LOGIN";
		SetText(text, 1060.f, 620.f, 1.f, 1.f);
		//wstring ID = INPUT->GetUserID();
		//SetText(ID, 500.f, 450.f, 1.f, 1.f);

		/*wstring text1 = L"Thieves Password";
		SetText(text1, 500.f, 500.f, 1.f, 1.f);
		wstring Password = INPUT->GetUserPassword();
		SetText(Password, 500.f, 550.f, 1.f, 1.f);*/
		//wstring test = L"한글 테스트";
		//SetText(test, 100.f, 500.f, 1.f, 1.f);

	}
	else if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		/*Vec3 pos = GEngine->GetThievesPacketManager()->GetVec();
		wstring text2 = L"x:" + to_wstring(int(pos.x)) + L" y:" + to_wstring(int(pos.y)) + L" z:" + to_wstring(int(pos.z));
		SetText(text2, 0.f, 0.f, 1.f, 1.f);*/

		/*if (GEngine->GetThievesPacketManager()->GetRecv()) {
			SetTextInfo(TEXT_FORMAT::DEFALUT);
			wstring text5 = L"TRUE";
			SetText(text5, 0.f, 0.f, 1.f, 1.f);
		}
		else {
			SetTextInfo(TEXT_FORMAT::DEFALUT);
			wstring text6 = L"FALSE";
			SetText(text6, 0.f, 0.f, 1.f, 1.f);
		}*/

		shared_ptr<InGameScene> iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());

		if (!iScene->GetIsGameEnd())
		{
			if (!iScene->GetIsTimerStart())
			{
				if (iScene->IsGetTime())
				{
					SetTextInfo(TEXT_FORMAT::INFO);
					auto time = iScene->GetStartTime();
					auto sec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - time);
					wstring timerText;
					timerText.append(to_wstring(5 - sec.count()));

					SetText(timerText, width / 2 - 50.f, height / 2 - 50.f, 1.f, 1.f);
				}
			}
			else
			{
				if (iScene->IsGetTime())
				{
					SetTextInfo(TEXT_FORMAT::TIMER);
					auto time = iScene->GetStartTime();
					auto sec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - time);

					int seconds = sec.count() - 5;

					int min = seconds / 60;
					int nSec = seconds % 60;

					wstring timerText;
					timerText.append(to_wstring(min));
					timerText.append(L":");
					timerText.append(to_wstring(nSec));

					SetText(timerText, width / 2, 50.f, 1.f, 1.f);
				}
			}

			if (iScene->GetIsActiveEscape())
			{
				if (std::chrono::duration_cast<std::chrono::seconds>
					(std::chrono::system_clock::now() - iScene->GetActiveEscapeTime()).count() < 3)
				{
					SetTextInfo(TEXT_FORMAT::INFO);
					wstring mText = L"Escape Zone Activated.";
					SetText(mText, width / 2 - 300.f, 300.f, 1.f, 1.f);
				}
			}

			if (iScene->GetIsActiveSpecialEscape())
			{
				if (std::chrono::duration_cast<std::chrono::seconds>
					(std::chrono::system_clock::now() - iScene->GetActiveSpecialEscapeTime()).count() < 3)
				{
					SetTextInfo(TEXT_FORMAT::INFO);
					wstring mText = L"Special Escape Zone Activated.";
					SetText(mText, width / 2 - 300.f, 300.f, 1.f, 1.f);
				}
			}

			if (iScene->GetIsOpenSafe())
			{
				if (std::chrono::duration_cast<std::chrono::seconds>
					(std::chrono::system_clock::now() - iScene->GetOpenSafeTime()).count() < 3)
				{
					SetTextInfo(TEXT_FORMAT::INFO);
					wstring mText = L"Safe Opened.";
					SetText(mText, width / 2 - 200.f, 300.f, 1.f, 1.f);
				}
			}

			if (iScene->GetPhaseChanged())
			{
				if (std::chrono::duration_cast<std::chrono::seconds>
					(std::chrono::system_clock::now() - iScene->GetPhaseChangedTime()).count() < 3)
				{
					SetTextInfo(TEXT_FORMAT::INFO);
					wstring mText = L"Someone Stole The Diamond!";
					SetText(mText, width / 2 - 300.f, 300.f, 1.f, 1.f);
				}
			}

			if (iScene->GetIsInteractOn() && iScene->GetIsOpenSafe() && Network::GetInst()->GetPacketManager()->GetDiamondPlayer() == -1)
			{
				SetTextInfo(TEXT_FORMAT::INFO);
				wstring mText = L"Interaction ON [F]";
				SetText(mText, width / 2 - 300.f, 400.f, 1.f, 1.f);
			}

			int netID = Network::GetInst()->GetPacketManager()->GetGameInfo().GetNetworkID();

			if (Network::GetInst()->GetNetworkObjMap().find(netID)->second->GetIsStun())
			{
				SetTextInfo(TEXT_FORMAT::INFO);
				wstring mText = L"You've Stunned!";
				SetText(mText, width / 2 - 300.f, height / 2, 1.f, 1.f);
			}

			if (Network::GetInst()->GetNetworkObjMap().find(netID)->second->GetIsInvincible())
			{
				SetTextInfo(TEXT_FORMAT::INFO);
				wstring mText = L"Now Invincible!";
				SetText(mText, width / 2 - 300.f, height / 2, 1.f, 1.f);
			}
		}
		else
		{
			if (iScene->GetIsPlayerWin())
			{
				SetTextInfo(TEXT_FORMAT::INFO);
				wstring mText = L"WIN!";
				SetText(mText, width / 2 - 100.f, 300.f, 1.f, 1.f);
			}
			else
			{
				SetTextInfo(TEXT_FORMAT::INFO);
				wstring mText = L"Lose...";
				SetText(mText, width / 2 - 150.f, 300.f, 1.f, 1.f);
			}
		}
		
		
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::LOBBY)
	{
		/*SetTextInfo(TEXT_FORMAT::DEFALUT);
		wstring text7 = L"This Is LOBBY!";
		SetText(text7, 0.f, 10.f, 1.f, 1.f);
		*/
		shared_ptr<LobbyScene> lScene = static_pointer_cast<LobbyScene>(GET_SINGLE(SceneManager)->GetActiveScene());

		SetTextInfo(TEXT_FORMAT::INROOM);
		int count = 0;

		/*if (lScene->GetRoomsData().empty()) {
			wstring text8 = L"Room Empty";
			SetText(text8, 200.f, 100.f, 1.f, 1.f);
		}*/

		for (auto& data : lScene->GetRoomsData())
		{

			wstring tempText = L"Room No.";
			tempText.append(to_wstring(data.second.id + 1));
			tempText.append(L" ");

			wstring tempText2 = L"";
			tempText2.append(to_wstring(data.second.nPlayer));
			tempText2.append(L"/8 ");
			if (data.second.isInGame) tempText.append(L"Game Playing!");

			if (count % 2 == 0)
			{
				SetText(tempText, width * 390.f / 1600.f, height * (145.f + 90.f * count) / 900.f, 1.f, 1.f);
				SetText(tempText2, width * 745.f / 1600.f, height * (185.f + 90.f * count) / 900.f, 1.f, 1.f);
			}
			else
			{
				SetText(tempText, width * 1025.f / 1600.f, height * (145.f + 90.f * (count - 1)) / 900.f, 1.f, 1.f);
				SetText(tempText2, width * 1380.f / 1600.f, height * (185.f + 90.f * (count - 1)) / 900.f , 1.f, 1.f);
			}
			count += 1;
		}
		
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::ROOM)
	{
		SetTextInfo(TEXT_FORMAT::DEFALUT);
		wstring text = L"This Is ROOM!";
		SetText(text, 0.f, 0.f, 1.f, 1.f);

		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetActiveScene());


		SetTextInfo(TEXT_FORMAT::INROOM);
		int count = 0;
		for (auto& data : rScene->GetRoomData())
		{
			if (data.second.id == rScene->GetRoomMasterId())
			{
				wstring masterText = L"Master : ID : Ready";
				SetText(masterText, width * 950.f / 1600.f, height * (115.f + 45.f * count) / 900.f, 1.f, 1.f);
			}
			else {
				wstring tempText = L"temp";
				tempText.append(data.second.name);
				SetText(tempText, width * 950.f / 1600.f, height * (115.f + 45.f * count) / 900.f, 1.f, 1.f);
			}

			wstring tempText2 = L"";

			if (data.second.isReady) {
				tempText2.append(L"Ready!");
				SetText(tempText2, width * 1315.f / 1600.f, height* (115.f + 45.f * count) / 900.f, 1.f, 1.f);
			}
			count += 1;
		}
		for (int i = 0; i < 7; i++)
		{
			wstring tempText = L"temp : ID : Ready";
			//tempText.append(data.second.name);
			SetText(tempText, width * 950.f / 1600.f, height* (115.f + 80.f * count) / 900.f, 1.f, 1.f);
			count += 1;
		
		}

	}
	else if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::LOADING)
	{
		SetTextInfo(TEXT_FORMAT::LOADING);
		SetText(GET_SINGLE(SceneManager)->GetLoadText(), width / 2.f, height / 2.f, 1.f, 1.f);
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

void Text::SetTextInfo(TEXT_FORMAT infoNumber)
{
	float width = static_cast<float>(GEngine->GetWindow().width);

	switch (infoNumber)
	{
	case TEXT_FORMAT::LOGIN:
		// 텍스트 색깔
		ThrowIfFailed(_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), _solidColorBrush.GetAddressOf()));

		// 텍스트 폰트 등
		ThrowIfFailed(_dWriteFactory->CreateTextFormat(L"상주경천섬체", nullptr,
			DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			width * 75.f / 1600.f, L"en-us", _writeTextFormat.GetAddressOf()));

		//_writeTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		//_writeTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case TEXT_FORMAT::DEFALUT:
		// 텍스트 색깔
		ThrowIfFailed(_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), _solidColorBrush.GetAddressOf()));

		// 텍스트 폰트 등
		ThrowIfFailed(_dWriteFactory->CreateTextFormat(L"상주경천섬체", nullptr,
			DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			width * 25.f / 1600.f, L"en-us", _writeTextFormat.GetAddressOf()));

		break;
	case TEXT_FORMAT::LOADING:
		// 텍스트 색깔
		ThrowIfFailed(_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), _solidColorBrush.GetAddressOf()));

		// 텍스트 폰트 등
		ThrowIfFailed(_dWriteFactory->CreateTextFormat(L"상주경천섬체", nullptr,
			DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			width * 25.f / 1600.f, L"en-us", _writeTextFormat.GetAddressOf()));
		break;
	case TEXT_FORMAT::INROOM:
		// 텍스트 색깔
		ThrowIfFailed(_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), _solidColorBrush.GetAddressOf()));

		// 텍스트 폰트 등
		ThrowIfFailed(_dWriteFactory->CreateTextFormat(L"상주경천섬체", nullptr,
			DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			width * 25.f / 1600.f, L"en-us", _writeTextFormat.GetAddressOf()));
		break;
	case TEXT_FORMAT::TIMER:
		// 텍스트 색깔
		ThrowIfFailed(_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), _solidColorBrush.GetAddressOf()));

		// 텍스트 폰트 등
		ThrowIfFailed(_dWriteFactory->CreateTextFormat(L"상주경천섬체", nullptr,
			DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			width * 50.f / 1600.f, L"en-us", _writeTextFormat.GetAddressOf()));
		break;
	case TEXT_FORMAT::INFO:
		// 텍스트 색깔
		ThrowIfFailed(_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), _solidColorBrush.GetAddressOf()));

		// 텍스트 폰트 등
		ThrowIfFailed(_dWriteFactory->CreateTextFormat(L"상주경천섬체", nullptr,
			DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			width * 50.f / 1600.f, L"en-us", _writeTextFormat.GetAddressOf()));
		break;
	case TEXT_FORMAT::INFO_BIG:
		// 텍스트 색깔
		ThrowIfFailed(_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), _solidColorBrush.GetAddressOf()));

		// 텍스트 폰트 등
		ThrowIfFailed(_dWriteFactory->CreateTextFormat(L"상주경천섬체", nullptr,
			DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			width * 100.f / 1600.f, L"en-us", _writeTextFormat.GetAddressOf()));
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

