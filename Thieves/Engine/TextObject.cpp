#include "pch.h"
#include "TextObject.h"
#include "Engine.h"

void TextObject::Render(const ComPtr<ID2D1DeviceContext2>& device)
{
    device->SetTransform(D2D1::Matrix3x2F::Translation(m_position.x, m_position.y));
    device->DrawText(
        m_text.c_str(),
        static_cast<UINT32>(m_text.size()),
        s_formats[m_format].Get(),
        &m_rect,
        s_brushes[m_brush].Get()
    );
}