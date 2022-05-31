#include "game/ui/text.hpp"

KK_COMPONENT_IMPL_BEGIN(Text)

    KK_FIELD_PUSH(Text, text, EDataType::STRING)

    KK_FIELD_PUSH(Text, color, EDataType::FLOAT)
    KK_FIELD_COUNT(4)
    KK_FIELD_RANGE(0.f, 1.f)

KK_COMPONENT_END

void Text::Render(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize) const
{
    ImGui::SetWindowFontScale(Maths::Max(screenScale.x, screenScale.y));
    io_dl.AddText(ImVec2(screenPosition.x, screenPosition.y), ImColor(color.x, color.y, color.z, color.w), text.c_str());
}