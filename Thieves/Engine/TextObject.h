#pragma once
class TextObject
{
public:
	TextObject() {}
	virtual ~TextObject() {}

	void Render(const ComPtr<ID2D1DeviceContext2>& device);
private:
	Vec2 _position = {100, 100};
	wstring _str;
	D2D1_RECT_F _rect = D2D1::RectF( 0.f,0.f,200.0f,200.0f );
private:
	ComPtr<ID2D1SolidColorBrush> _SolidColorBrush = nullptr;
	ComPtr<IDWriteTextFormat> _DWriteTextFormat = nullptr;
};

