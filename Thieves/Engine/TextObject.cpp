#include "pch.h"
#include "TextObject.h"
#include "Engine.h"

void TextObject::RenderText()
{
    ComPtr<ID2D1DeviceContext2> device = GEngine->GetText()->Getd2dDeviceContext();
    device->SetTransform(D2D1::Matrix3x2F::Translation(_position.x, _position.y));
   // static const WCHAR text[] = L"헤헿 D3D11On12 프로젝트 입니다.";
    device->DrawText(
        _str.c_str(),
        static_cast<UINT32>(_str.size()),
        _writeTextFormat.Get(),
        &_rect,
        _solidColorBrush.Get()
    );
}