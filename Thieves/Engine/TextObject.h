#pragma once
#include "Component.h"

class TextObject : public Component
{
public:
	TextObject() : Component(COMPONENT_TYPE::TEXTOBJECT) {}
	virtual ~TextObject() {}

	void RenderText();
	//void RenderText(const ComPtr<ID2D1DeviceContext2>& device);
private:
	Vec2 _position = {100, 100};
	wstring _str = L"헤헿 D3D11On12 프로젝트 입니다.";
	D2D1_RECT_F _rect = D2D1::RectF( 0.f,0.f,200.0f,200.0f );
private:
	ComPtr<ID2D1SolidColorBrush> _solidColorBrush = nullptr;
	ComPtr<IDWriteTextFormat> _writeTextFormat = nullptr;
};

