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
	void CreateD3D11On12Device();	// 11on12 ����̽� ����
	void CreateD2DDevice();			// D2D, DWrite ����
	void CreateRenderTarget();		// �ؽ�Ʈ ����Ÿ�� ����
	void Render2D();				// �ؽ�Ʈ ���� �� ���

	// Update�� �߰��ϴ� ȭ�鿡 �ؽ�Ʈ�� �׸��� �Լ���
	// �ؽ�Ʈ ���� (n������ �ؽ�Ʈ ������ ����)
	void SetTextInfo(TEXT_FORMAT infoNumber);
	// �ؽ�Ʈ ���� (�ؽ�Ʈ ����, �ؽ�Ʈ ��ǥx��, �ؽ�Ʈ ��ǥy��, �ؽ�Ʈ ����ũ��, �ؽ�Ʈ ����ũ��, ȭ�� ũ��)
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
	ComPtr<ID2D1SolidColorBrush>	_solidColorBrush = nullptr;	// �ؽ�Ʈ ����
	ComPtr<IDWriteTextFormat>		_writeTextFormat = nullptr;	// �ؽ�Ʈ ��Ʈ
	D2D1_RECT_F	_rect;	// ȭ�� ũ��
};

