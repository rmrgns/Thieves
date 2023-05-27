#pragma once

enum class TEXT_FORMAT {
	DEFALUT,
	LOGIN,
	LOADING,
	INROOM
};

class Text
{
public:
	void Init();
	void Update();
	
public:
	ComPtr<ID2D1Factory> Getd2dFactory() { return _d2dFactory; }
	ComPtr<ID3D11On12Device> Getd3d11On12Device() { return _d3d11On12Device; }
	ComPtr<ID2D1DeviceContext2> Getd2dDeviceContext() { return _d2dDeviceContext; }
	ComPtr<ID3D11DeviceContext> Getd3d11DeviceContext() { return _d3d11DeviceContext; }

public:
	void CreateD3D11On12Device();	// 11on12 디바이스 생성
	void CreateD2DDevice();			// D2D, DWrite 생성
	void CreateRenderTarget();		// 텍스트 렌더타겟 생성
	void Render2D();				// 텍스트 생성 및 출력

	// Update에 추가하는 화면에 텍스트를 그리는 함수들
	// 텍스트 설정 (n값으로 텍스트 설정값 변경)
	void SetTextInfo(TEXT_FORMAT infoNumber);
	// 텍스트 제작 (텍스트 내용, 텍스트 좌표x값, 텍스트 좌표y값, 텍스트 가로크기, 텍스트 세로크기, 화면 크기)
	void SetText(wstring text, float posX, float posY, float scaleX, float scaleY);
private:
	ComPtr<ID3D11On12Device>	_d3d11On12Device = nullptr;
	ComPtr<ID3D11DeviceContext> _d3d11DeviceContext = nullptr;
	ComPtr<ID2D1Factory3>		_d2dFactory = nullptr;
	ComPtr<ID2D1Device2>		_d2dDevice = nullptr;
	ComPtr<ID2D1DeviceContext2> _d2dDeviceContext = nullptr;
	ComPtr<IDWriteFactory>		_dWriteFactory = nullptr;
	ComPtr<ID3D11Resource>		_wrappedBackBuffer[SWAP_CHAIN_BUFFER_COUNT];
	ComPtr<ID2D1Bitmap1>		_d2dRenderTargets[SWAP_CHAIN_BUFFER_COUNT];

private:
	ComPtr<ID2D1SolidColorBrush>	_solidColorBrush = nullptr;	// 텍스트 색깔
	ComPtr<IDWriteTextFormat>		_writeTextFormat = nullptr;	// 텍스트 폰트
	D2D1_RECT_F	_rect;	// 화면 크기
};

