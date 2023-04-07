#pragma once
class TextObject
{
public:
	TextObject() {}
	virtual ~TextObject() {}

	void Render(const ComPtr<ID2D1DeviceContext2>& device);

};

