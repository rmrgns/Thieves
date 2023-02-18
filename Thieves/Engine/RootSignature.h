#pragma once

// [계약서 / 결재]

class RootSignature
{
public:
	void Init();

	ComPtr<ID3D12RootSignature>	GetGraphicsRootSignature() { return _graphicsRootSignature; }
	ComPtr<ID3D12RootSignature>	GetComputeRootSignature() { return _computeRootSignature; }

private:
	void CreateGraphicsRootSignature();
	void CreateComputeRootSignature();

private:
	ComPtr<ID3D12RootSignature>		_graphicsRootSignature;
	ComPtr<ID3D12RootSignature>		_computeRootSignature;
	D3D12_STATIC_SAMPLER_DESC		_samplerDesc;
};

