#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include <tchar.h>
#include "Fonts.h"

#include "imgui_freetype.h"

#include "ImAnim/ImVec2Anim.h"
#include "ImAnim/ImVec4Anim.h"

#include <Pdh.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#include "imgui_settings.h"

#include <string>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")
#include "images.h"
#include <intrin.h>
#include "blur.hpp"

char login[64] = "";
char email[64] = "";

char sign_login[128] = "";
char sign_password[128] = "";

char password[128] = "";
char key[128];
static bool rememberme = false;

using namespace std;

HWND hwnd;
RECT rc;

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

IDirect3DTexture9* auth_pic = nullptr;
IDirect3DTexture9* avatar_pic = nullptr;
IDirect3DTexture9* product_pics[7] = { nullptr };
PDIRECT3DTEXTURE9  inject_pics[6] = { nullptr };

static bool rain_animation;
static bool image_loaded = false;

static ImVec2 menu_size = ImVec2(750, 500);
static int iProduct = 0;

//static int WIDTH = 850; // Loader Size X
//static int HEIGHT = 500; // Loader Size Y
void Particles()
{
    ImVec2 screen_size = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };

    static ImVec2 partile_pos[200]; 
    static ImVec2 partile_target_pos[200];
    static float partile_speed[200];
    static float partile_radius[200];

    for (int i = 0; i < 200; i++) 
    {
        if (partile_pos[i].x == 0 || partile_pos[i].y == 0)
        {
            partile_pos[i].x = rand() % (int)screen_size.x + 1;
            partile_pos[i].y = 15.f;
            partile_speed[i] = 1 + rand() % 25;
            partile_radius[i] = rand() % 4;

            partile_target_pos[i].x = rand() % (int)screen_size.x;
            partile_target_pos[i].y = screen_size.y * 2;
        }

        partile_pos[i] = ImLerp(partile_pos[i], partile_target_pos[i], ImGui::GetIO().DeltaTime * (partile_speed[i] / 60));

        if (partile_pos[i].y > screen_size.y)
        {
            partile_pos[i].x = 0;
            partile_pos[i].y = 0;
        }

        ImGui::GetWindowDrawList()->AddCircleFilled(partile_pos[i], partile_radius[i], ImColor(240, 248, 255, 255 / 2));
    }

}
void Particles2()
{
    ImVec2 screen_size = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };

    static ImVec2 partile_pos[400];
    static ImVec2 partile_target_pos[400];
    static float partile_speed[400];
    static float partile_radius[400];

    for (int i = 0; i < 400; i++)
    {
        if (partile_pos[i].x == 0 || partile_pos[i].y == 0)
        {
            partile_pos[i].x = rand() % (int)screen_size.x + 1;
            partile_pos[i].y = 15.f;
            partile_speed[i] = 1 + rand() % 25;
            partile_radius[i] = rand() % 4;

            partile_target_pos[i].x = rand() % (int)screen_size.x;
            partile_target_pos[i].y = screen_size.y * 2;
        }

        partile_pos[i] = ImLerp(partile_pos[i], partile_target_pos[i], ImGui::GetIO().DeltaTime * (partile_speed[i] / 60));

        if (partile_pos[i].y > screen_size.y)
        {
            partile_pos[i].x = 0;
            partile_pos[i].y = 0;
        }

        ImGui::GetWindowDrawList()->AddCircleFilled(partile_pos[i], partile_radius[i], ImColor(240, 248, 255, 255 / 2));
    }

}

void LoadImages() {
    if (image_loaded)
        return;

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &auth_image, sizeof(auth_image), &auth_pic);

    D3DXCreateTextureFromFileA(g_pd3dDevice, "C:\\loader_images\\1.png", &inject_pics[4]);
    D3DXCreateTextureFromFileA(g_pd3dDevice, "C:\\loader_images\\2.png", &inject_pics[1]);
    D3DXCreateTextureFromFileA(g_pd3dDevice, "C:\\loader_images\\3.png", &inject_pics[2]);
    D3DXCreateTextureFromFileA(g_pd3dDevice, "C:\\loader_images\\4.png", &inject_pics[3]);
    D3DXCreateTextureFromFileA(g_pd3dDevice, "C:\\loader_images\\5.png", &inject_pics[5]);

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &product1img, sizeof(product1img), &product_pics[0]);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &product2img, sizeof(product2img), &product_pics[1]);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &product3img, sizeof(product3img), &product_pics[2]);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &product4img, sizeof(product4img), &product_pics[3]);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &product5img, sizeof(product5img), &product_pics[4]);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &product6img, sizeof(product6img), &product_pics[5]);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &product7img, sizeof(product7img), &product_pics[6]);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &product8img, sizeof(product8img), &product_pics[7]);

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &avatar, sizeof(avatar), &avatar_pic);
    image_loaded = true;
}

static bool chunk = false;

void chunkUPD(const ImVec2 size) {
    static DWORD dwTickStart = GetTickCount();
    static ImVec2 lastSize = ImVec2(0, 0);

    // Smoothly interpolate the size

    static imanim::ImVec2Anim* ChunkAnim = nullptr;

    if (ChunkAnim == nullptr)
    {
        ChunkAnim = new imanim::ImVec2Anim(&menu_size);
        ChunkAnim->setStartValue(ImVec2(400, 400));
        ChunkAnim->setEndValue(size);
        ChunkAnim->setDuration(2.5);
        ChunkAnim->setLoopCount(-1);
        ChunkAnim->setEasingCurve(imanim::EasingCurve::Type::InOutBack);
        ChunkAnim->start();
    }
    else
    {
        ChunkAnim->update();
    }

    if (chunk) {
        GetWindowRect(hwnd, &rc);
        MoveWindow(hwnd, rc.left + ImGui::GetMouseDragDelta().x, rc.top + ImGui::GetMouseDragDelta().y, menu_size.x, menu_size.y, TRUE);

        if (GetTickCount() - dwTickStart > 2500) {
            chunk = false;
            dwTickStart = GetTickCount();
        }
    }

    // Redraw only if size has changed
    if (lastSize.x != menu_size.x || lastSize.y != menu_size.y) {
        InvalidateRect(hwnd, NULL, FALSE);
        lastSize = menu_size;
    }
}

namespace ImGui
{
    int rotation_start_index;
    void ImRotateStart()
    {
        rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
    }

    ImVec2 ImRotationCenter()
    {
        ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

        const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

        return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
    }


    void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
    {
        float s = sin(rad), c = cos(rad);
        center = ImRotate(center, s, c) - center;

        auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
    }
}

#define min(x,y) ((x)<(y)?x:y)
#define wh(a) ImColor(1.f,1.f,1.f,a)
void FXRain(ImDrawList* d, ImVec2 a, ImVec2 b, ImVec2 sz, ImVec2, float t)
{

    for (int i = 0; i < 2000; ++i) {
        unsigned h = ImGui::GetID(d + i + int(t / 4));
        auto f = fmodf(t + fmodf(h / 777.f, 99), 99);
        auto tx = h % (int)sz.x;
        auto ty = h % (int)sz.y;
        if (f < 1) {
            auto py = ty - 1000 * (1 - f);
            d->AddLine({ a.x + tx, a.y + py }, { a.x + tx, a.y + min(py + 10,ty) }, main_color);
        }
        else if (f < 1.2f)
            d->AddCircle({ a.x + tx, a.y + ty }, (f - 1) * 10 + h % 5, GetColorWithAlpha(main_color, 0.5f));
    }
}

static imanim::ImVec2Anim* NotifAnim = nullptr;

struct Notification {
    std::string icon, message, information;
    ImColor color;
    ImRect bb;
    ImVec2 offset = ImVec2(400, 0);
    ImVec2 target_offset = ImVec2(400, 0);
    float timer;
}notif_state;

class NotificationSystem {
public:

    void Notification(std::string icon, std::string message, std::string information, ImColor color)
    {
        //notif_state.offset.x = 0.f;

        static ImVec2 tab_offset(0.f, menu_size.x);
        notif_state.timer = 0;
        notif_state.color = color;
        notif_state.icon = icon;
        notif_state.message = message;
        notif_state.information = information;

        if (NotifAnim == nullptr)
        {
            NotifAnim = new imanim::ImVec2Anim(&notif_state.offset);
            NotifAnim->setStartValue(ImVec2(400.f, 0.f));
            NotifAnim->setEndValue(ImVec2(0.f, 0.f));
            NotifAnim->setDuration(1.1f);
            NotifAnim->setLoopCount(1);
            NotifAnim->setEasingCurve(imanim::EasingCurve::Type::InOutBack);
            NotifAnim->start();
        }
    }

    void DrawNotification()
    {
        notif_state.timer += ImGui::GetIO().DeltaTime * 10.f;
        notif_state.target_offset = notif_state.timer > 30.f ? ImVec2(400.f, 0.f) : ImVec2(0.f, 0.f);

        if (NotifAnim != nullptr) {

            if (notif_state.timer > 30.f) {
                NotifAnim->setLoopCount(-1);
                NotifAnim->start();
            }

            NotifAnim->update();
            NotifAnim->setStartValue(notif_state.offset);
            NotifAnim->setEndValue(notif_state.target_offset);
        }

        notif_state.bb = ImRect(ImGui::GetWindowPos() + ImGui::GetWindowSize() - ImVec2(360, 70) + ImVec2(notif_state.offset.x, 0.f), ImGui::GetWindowPos() + ImGui::GetWindowSize() - ImVec2(20, 20));

        blur::add_blur(ImGui::GetForegroundDrawList(), notif_state.bb.Min + ImVec2(notif_state.offset.x, 0.f), notif_state.bb.Max + ImVec2(notif_state.offset.x, 0.f), 1.f);
        
        ImGui::GetForegroundDrawList()->AddRectFilled(notif_state.bb.Min + ImVec2(notif_state.offset.x, 0.f), notif_state.bb.Max + ImVec2(notif_state.offset.x, 0.f), ImColor(23, 25, 30, 240), items_rounding);
        ImGui::GetForegroundDrawList()->AddRect(notif_state.bb.Min + ImVec2(notif_state.offset.x, 0.f), notif_state.bb.Max + ImVec2(notif_state.offset.x, 0.f), ImColor(47, 106, 204, 1 / 4), items_rounding);

        PushFont(semibold_font[2]);
        ImGui::GetForegroundDrawList()->AddText(notif_state.bb.Min + ImVec2(12.5f, 6.f), main_color, notif_state.message.c_str());
        PopFont();

        PushFont(semibold_font[0]);
        ImGui::GetForegroundDrawList()->AddText(ImVec2(notif_state.bb.Min.x + 12.5f, notif_state.bb.Max.y - 9.5f - CalcTextSize(notif_state.information.c_str()).y), text_color[3], notif_state.information.c_str());
        PopFont();
    }
};

NotificationSystem notificationSystem;

static int inject_state = 0;
static int iTabs = 0;

namespace inject {
//
    ImVec2 offset(0, 0);
    ImVec2 menu_offset(0, 0);
    ImVec2 register_offset(0, 0);

    int inject_status = 0;
    int menu_status = 0;
    int register_status = 0;

    bool CancelBox(const char* text)
    {
        EasingAnimationV2("cancelbox_offset", &offset, ImVec2(1.f, inject_status == 0 ? 164.f : 0.f), 1.2f, imanim::EasingCurve::Type::InOutBack, -1);

        ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + menu_size.x / 2 - 175, ImGui::GetWindowPos().y + menu_size.y - offset.y));
        ImGui::SetNextWindowSize(ImVec2(350, 110));

        ImGui::Begin("InjectBox", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
        {
            blur::add_blur(ImGui::GetWindowDrawList(), ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), 1.f);
            ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), GetColorWithAlpha(winbg, 0.6f), 5.f);
            ImGui::SetCursorPos(ImVec2(25, 49));
            ImGui::GetWindowDrawList()->AddText(center_text(ImGui::GetWindowPos() + ImVec2(31, 21), ImGui::GetWindowPos() + ImVec2(319, 39), text), text_color[0], text);
            if (ImGui::Button("Cancel", ImVec2(300, 40)))
                inject_status = 1;

            if (inject_status == 1 && offset.y == 0.f) {
                inject_state = 0;
                inject_status = 0;
            }

        }ImGui::End();
        return true;
    }

    bool AuthBox(const char* text)
    {
        EasingAnimationV2("authbox_offset", &menu_offset, ImVec2(1.f, menu_status == 0 && iTabs == 0 && menu_state != 2 ? 400.f : 0.f), 1.3f, imanim::EasingCurve::Type::InOutBack, -1);

        ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + menu_size.x / 2 - 225, ImGui::GetWindowPos().y + menu_size.y - menu_offset.y));
        ImGui::SetNextWindowSize(ImVec2(500, 310));
        Particles();
        ImGui::Begin("AuthBox", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
        {
            blur::add_blur(ImGui::GetWindowDrawList(), ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), 1.f);
            ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), GetColorWithAlpha(winbg, 0.6f), 5.f);

            PushFont(bold_font[1]);
            ImGui::GetWindowDrawList()->AddText(center_text(ImGui::GetWindowPos() + ImVec2(0, 21), ImGui::GetWindowPos() + ImVec2(ImGui::GetWindowSize().x, 39), text), text_color[0], text);
            PopFont();

            ImGui::SetCursorPos(ImVec2(25, 49)); ImGui::BeginGroup(); {
                
                ImGui::InputTextEx("Login", "b", login, sizeof(login), ImVec2(450, 65), 0);

                ImGui::InputTextEx("Password", "b", password, sizeof(password), ImVec2(450, 65), 0);

                ImGui::InputTextEx("Password", "b", password, sizeof(password), ImVec2(450, 65), 0);
                if (ImGui::Button("Autorization", ImVec2(450, 65))) {

                    if (CalcTextSize(login).x > 5.f && CalcTextSize(password).x > 5.f) {
                        menu_status = 1;
                        notificationSystem.Notification("4", "Succesfully!", "You have successfully logged in!", GetColorWithAlpha(main_color, 0.6f));
                    }
                    else
                    {
                        notificationSystem.Notification("4", "Mistake!", "Incorrect username or password!", ImColor(208, 48, 47, 150));
                    }
                }

                if (menu_status == 1 && menu_offset.y == 0.f)
                    menu_state = 2;

            }ImGui::EndGroup();
            

        }ImGui::End();
        return true;
    }

    bool RegisterBox(const char* text)
    {
        Particles();

        EasingAnimationV2("regbox_offset", &register_offset, ImVec2(1.f, register_status == 0 && iTabs == 1 && menu_state != 2 ? 450.f : 0.f), 1.3f, imanim::EasingCurve::Type::InOutBack, -1);

        ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + menu_size.x / 2 - 225, ImGui::GetWindowPos().y + menu_size.y - register_offset.y));
        ImGui::SetNextWindowSize(ImVec2(500, 390));

        ImGui::Begin("RegBox", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
        {
            blur::add_blur(ImGui::GetWindowDrawList(), ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), 1.f);
            ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), GetColorWithAlpha(winbg, 0.6f), 5.f);

            PushFont(bold_font[1]);
            ImGui::GetWindowDrawList()->AddText(center_text(ImGui::GetWindowPos() + ImVec2(0, 21), ImGui::GetWindowPos() + ImVec2(ImGui::GetWindowSize().x, 39), text), text_color[0], text);
            PopFont();

            ImGui::SetCursorPos(ImVec2(25, 49)); ImGui::BeginGroup(); {

                ImGui::InputTextEx("Email", "b", email, 128, ImVec2(450, 65), 0);
                ImGui::InputTextEx("Login", "b", login, sizeof(login), ImVec2(450, 65), 0);
                ImGui::InputTextEx("Password", "b", password, sizeof(password), ImVec2(450, 65), 0);
                if (ImGui::Button("Registration", ImVec2(450, 65))) {
                    register_status = 1;
                }

                if (register_status == 1 && register_offset.y == 0.f) {
                    menu_state = 2;
                    iTabs = 0;
                }

            }ImGui::EndGroup();


        }ImGui::End();
        return true;
    }
};

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    blur::device_reset();
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

void move_window() {
    ImGui::SetCursorPos(ImVec2(0, 0));
    if (ImGui::InvisibleButton("Move_detector", ImVec2(menu_size.x, menu_size.y)));
    if (ImGui::IsItemActive() && !chunk) {

        GetWindowRect(hwnd, &rc);
        MoveWindow(hwnd, rc.left + ImGui::GetMouseDragDelta().x, rc.top + ImGui::GetMouseDragDelta().y, menu_size.x, menu_size.y, TRUE);
    }
}

void RenderBlur(HWND hwnd)
{
    struct ACCENTPOLICY
    {
        int na;
        int nf;
        int nc;
        int nA;
    };
    struct WINCOMPATTRDATA
    {
        int na;
        PVOID pd;
        ULONG ul;
    };

    const HINSTANCE hm = LoadLibrary(L"user32.dll");
    if (hm)
    {
        typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);

        const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hm, "SetWindowCompositionAttribute");
        if (SetWindowCompositionAttribute)
        {
            ACCENTPOLICY policy = { 3, 0, 0, 0 };

            WINCOMPATTRDATA data = { 19, &policy,sizeof(ACCENTPOLICY) };
            SetWindowCompositionAttribute(hwnd, &data);
        }
        FreeLibrary(hm);
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
