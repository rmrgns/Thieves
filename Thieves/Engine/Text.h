#pragma once

class Text
{
	

public:
	void Init();
	void Update();

public:
	void CreateRenderTarget();
	
public:
	ComPtr<ID2D1Factory> Getd2dFactory() { return _d2dFactory; }
	ComPtr<ID3D11On12Device> Getd3d11On12Device() { return _d3d11On12Device; }
	ComPtr<ID2D1DeviceContext2> Getd2dDeviceContext() { return _d2dDeviceContext; }
	ComPtr<ID3D11DeviceContext> Getd3d11DeviceContext() { return _d3d11DeviceContext; }
	//ComPtr<ID3D11Resource>	GetWrappedBackBuffer(int8 i) const { return _wrappedBackBuffer[i]; }
	//ComPtr<ID2D1Bitmap1>	GetD2dRenderTargets(int8 i) const { return _d2dRenderTargets[i]; }

public:
	void CreateD3D11On12Device();	// 11on12 叼官捞胶 积己
	void CreateD2DDevice();			// D2D, DWrite 积己
	void Render2D();

private:
	ComPtr<ID3D11On12Device>	_d3d11On12Device = nullptr;
	ComPtr<ID3D11DeviceContext> _d3d11DeviceContext = nullptr;
	ComPtr<ID2D1Factory3>		_d2dFactory = nullptr;
	ComPtr<ID2D1Device2>		_d2dDevice = nullptr;
	ComPtr<ID2D1DeviceContext2> _d2dDeviceContext = nullptr;
	ComPtr<IDWriteFactory>		_dWriteFactory = nullptr;
	ComPtr<ID3D11Resource>	_wrappedBackBuffer[SWAP_CHAIN_BUFFER_COUNT];
	ComPtr<ID2D1Bitmap1>	_d2dRenderTargets[SWAP_CHAIN_BUFFER_COUNT];

};

