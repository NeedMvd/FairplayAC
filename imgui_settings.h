#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <iostream>
#include <map>

#include "ImAnim/ImVec2Anim.h"
#include "ImAnim/ImVec4Anim.h"

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h> 
#include <conio.h>
#include <Windows.h>
#include <random>
#include <vector>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

using namespace ImGui;

inline void rect_glow(ImDrawList* draw, ImVec2 start, ImVec2 end, ImColor col, float rounding, float intensity) {
    while (true) {
        if (col.Value.w < 0.0019f)
            break;

        draw->AddRectFilled(start, end, col, rounding);

        col.Value.w -= col.Value.w / intensity;
        start = ImVec2(start.x - 1, start.y - 1);
        end = ImVec2(end.x + 1, end.y + 1);
    }
}

inline int rotation_start_index;
inline void ImRotateStart()
{
    rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

inline ImVec2 ImRotationCenter()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}


inline void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
{
    float s = sin(rad), c = cos(rad);
    center = ImRotate(center, s, c) - center;

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

inline ImVec2 center_text(ImVec2 min, ImVec2 max, const char* text)
{
    return min + (max - min) / 2 - ImGui::CalcTextSize(text) / 2;
}


inline ImVec4 ImColorToImVec4(const ImColor& color)
{
    return ImVec4(color.Value.x, color.Value.y, color.Value.z, color.Value.w);
}


inline int menu_state = 0;

inline ImColor main_color(255, 255, 255, 255);

inline ImVec2 frame_size = ImVec2(350, 46);

inline ImFont* icon_font[15];
inline ImFont* semibold_font[15];
inline ImFont* medium_font[15];
inline ImFont* bold_font[15];

inline ImFont* default_font;

static int injection_state = 0;

inline ImColor text_color[3]{ ImColor(255, 255, 255, 255), ImColor(214, 214, 214, 200), ImColor(214, 214, 214, 180) };

inline ImColor background_color(136, 181, 255, 25);

inline ImColor second_color(64, 64, 64, 50);

inline ImColor winbg(59, 133, 255, 255);

inline float items_rounding = 6.f;

inline ImVec4 inputtext_color[3] = { background_color, main_color, text_color[0] };

inline float anim_speed;

static ImDrawList* front_draw;

inline float random_float(float min, float max)
{
    return min + float(rand() / float(RAND_MAX)) * (max - min);
}

inline ImColor GetColorWithAlpha(ImColor color, float alpha)
{
    return ImColor(color.Value.x, color.Value.y, color.Value.z, alpha);
}

inline std::vector<std::string> stringToVector(std::string str, std::string token)
{
    std::vector<std::string> result;
    while (str.size())
    {
        int index = str.find(token);
        if (index != std::string::npos)
        {
            result.push_back(str.substr(0, index));
            str = str.substr(index + token.size());
            if (str.size() == 0)
                result.push_back(str);
        }
        else
        {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

constexpr int ypadding = 3; // vertical spacing of text in pixels

inline void DrawCenteredText(ImVec2 position, ImVec2 boundry, ImVec2 charSize, int idx, std::string Text)
{
    ImVec2 renderPos;
    ImVec2 linesize = ImGui::CalcTextSize(Text.c_str());
    renderPos.x = position.x + ((boundry.x / 2) - ((linesize.x + charSize.x) / 2)); // horizontal position centered in boundry x
    renderPos.y = position.y + ((idx * linesize.y) + ypadding);
    ImGui::RenderText(renderPos, Text.c_str(), 0, false);
}

inline void CenterText(std::string text, ImVec2 position, int boundry_width, int lines = 3)
{
    ImVec2 charsize = ImGui::CalcTextSize("X"); // TODO: should do per font, global constexpr
    std::vector<std::string> words = stringToVector(text, " ");

    int linecnt = 0; // 0 is first line
    int wordcnt = 0; // 0 is first word / vector item
    int linex = 0; // space already used on line
    std::string lineText = "";

    for (auto& word : words)
    {
        wordcnt++;
        int wordsize = (word.length() + 1) * charsize.x; // size of this word in pixels, plus one space

        if ((linex + wordsize < boundry_width)) // if word will fit in space available
        {
            lineText += word + " "; // add it
            linex += wordsize;      // reduce the available space
            if (wordcnt == words.size()) // last one
            {
                DrawCenteredText(position, ImVec2(boundry_width, boundry_width), charsize, linecnt, lineText.c_str());
                continue;
            }
        }

        else // word won't fit on this line, so draw current line, then start next line...
        {
            DrawCenteredText(position, ImVec2(boundry_width, boundry_width), charsize, linecnt, lineText.c_str());
            // clear linex and lineText, increment linecnt
            lineText = word + " ";
            linex = 0;
            linecnt++;
            continue;
        }

    }
}

inline ImColor GetDarkColor(const ImColor& color)
{
    float r, g, b, a;
    r = color.Value.x;
    g = color.Value.y;
    b = color.Value.z;
    a = 255;

    float darkPercentage = 0.2f;
    float darkR = r * darkPercentage;
    float darkG = g * darkPercentage;
    float darkB = b * darkPercentage;

    return ImColor(darkR, darkG, darkB, a);
}

struct easingv2_state
{
    imanim::ImVec2Anim* anim = nullptr;
    ImVec2 current_vec;
};

inline void EasingAnimationV2(std::string anim_name, ImVec2* current_vec, ImVec2 target_vec, float duration, imanim::EasingCurve::Type type, int loop)
{
    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiID id = window->GetID(anim_name.c_str());

    static std::map<ImGuiID, easingv2_state> a;
    auto it_a = a.find(id);

    if (it_a == a.end())
    {
        a.insert({ id, easingv2_state() });
        it_a = a.find(id);
    }

    it_a->second.current_vec = *current_vec;

    if (it_a->second.anim == nullptr)
    {
        it_a->second.anim = new imanim::ImVec2Anim(current_vec);
        it_a->second.anim->setStartValue(it_a->second.current_vec);
        it_a->second.anim->setEndValue(target_vec);
        it_a->second.anim->setDuration(duration);
        it_a->second.anim->setLoopCount(loop);
        it_a->second.anim->setEasingCurve(type);
        it_a->second.anim->start();
    }
    else
    {
        it_a->second.anim->update();
        it_a->second.anim->setStartValue(it_a->second.current_vec);
        it_a->second.anim->setEndValue(target_vec); // Обновление целевого цвета
    }
}

struct easingv4_state
{
    imanim::ImVec4Anim* anim = nullptr;
    ImVec4 current_vec;
};

inline void EasingAnimationV4(std::string anim_name, ImVec4 *current_vec, ImVec4 target_vec, float duration, imanim::EasingCurve::Type type, int loop)
{
    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiID id = window->GetID(anim_name.c_str());

    static std::map<ImGuiID, easingv4_state> a;
    auto it_a = a.find(id);

    if (it_a == a.end())
    {
        a.insert({ id, easingv4_state() });
        it_a = a.find(id);
    }

    it_a->second.current_vec = *current_vec;

    if (it_a->second.anim == nullptr)
    {
        it_a->second.anim = new imanim::ImVec4Anim(current_vec);
        it_a->second.anim->setStartValue(it_a->second.current_vec);
        it_a->second.anim->setEndValue(target_vec);
        it_a->second.anim->setDuration(duration);
        it_a->second.anim->setLoopCount(loop);
        it_a->second.anim->setEasingCurve(type);
        it_a->second.anim->start();
    }
    else
    {
        it_a->second.anim->update();
        it_a->second.anim->setStartValue(it_a->second.current_vec);
        it_a->second.anim->setEndValue(target_vec); // Обновление целевого цвета
    }
}

inline void Loading_Circle(ImVec2 pos) {
    // Setup
    auto front = front_draw; // also you can use GetWindowDrawList() or GetBackgroundDrawList()
    static float arc_size = 0.45f; // 0.f < x < 2.f
    static float radius = 45.f;
    static float thickness = 8.f;

    static ImVec2 position;

    //EasingAnimationV2("circle_loading", &position, ImVec2(IM_PI * 2.f, 0.f), 0.8f, imanim::EasingCurve::Type::InBack, -1);

    static imanim::ImVec2Anim* ProductAnim = nullptr;

    if (ProductAnim == nullptr)
    {
        ProductAnim = new imanim::ImVec2Anim(&position);
        ProductAnim->setStartValue(ImVec2(0, 0));
        ProductAnim->setKeyValueAt(0.5, ImVec2(IM_PI * 2.f, 0.f));
        ProductAnim->setEndValue(ImVec2(IM_PI * 4.f, 0.f));
        ProductAnim->setDuration(2.f);
        ProductAnim->setLoopCount(-1);
        ProductAnim->setEasingCurve(imanim::EasingCurve::Type::OutInBack);
        ProductAnim->start();
    }
    else
        ProductAnim->update();

    front->PathClear();
    front->PathArcTo(pos, radius, 0.f, 2.f * IM_PI, 40.f);
    front->PathStroke(GetColorWithAlpha(main_color, 0.3f), 0, thickness);

    float initialAngle = IM_PI * (1.5f + arc_size) + position.x;
    ImVec2 initialPos = ImVec2(pos.x + ImCos(initialAngle) * radius, pos.y + ImSin(initialAngle) * radius);

    float spaceBetweenCircles = 3.f;
    float circleRadius = 4.f;
    ImVec2 currentPos = initialPos;

    for (int i = 0; i < 40; i++)
    {
        front->AddCircleFilled(currentPos, circleRadius, GetColorWithAlpha(main_color, i / 40.f));
        front->AddShadowCircle(currentPos, circleRadius, GetColorWithAlpha(main_color, i / 40.f), 25.f, ImVec2(0,0));

        float nextAngle = initialAngle + i * (spaceBetweenCircles / radius); // расчет угла для следующего круга с учетом минимального расстояния
        currentPos = ImVec2(pos.x + ImCos(nextAngle) * radius, pos.y + ImSin(nextAngle) * radius); // рассчитываем позицию для следующего круга
    }
}

inline bool button_text(const char* first_text, const char* label)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImVec2 pos = window->DC.CursorPos;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    static ImVec4 color_text;
    static ImVec4 color_shadow;

    ImVec2 size = CalcTextSize(label);

    const ImRect bb(pos, pos + size);
    const ImRect bb_text(pos + ImVec2(CalcTextSize(first_text).x, 0), pos + ImVec2(CalcTextSize(first_text).x, 0) + size);
    ItemSize(size, style.FramePadding.y);
    ItemAdd(bb, id);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb_text, id, &hovered, &held, 0);

    color_text = ImLerp(color_text, hovered ? main_color : text_color[0], anim_speed);

    color_shadow = ImLerp(color_shadow, hovered ? ImColor(main_color.Value.x, main_color.Value.y, main_color.Value.z, 0.1f) : ImColor(main_color.Value.x, main_color.Value.y, main_color.Value.z, 0.f), anim_speed);

    rect_glow(window->DrawList, pos + ImVec2(CalcTextSize(first_text).x, 0) + ImVec2(0, 2), pos + ImVec2(CalcTextSize(first_text).x, 0) + size - ImVec2(0, 2), color_shadow, 360.f, 5.f);

    window->DrawList->AddText(pos, text_color[1], first_text);

    window->DrawList->AddText(pos + ImVec2(CalcTextSize(first_text).x, 0), GetColorU32(color_text), label);

    return pressed;
}
