#include "main.h"

#include <tchar.h>

#include <tchar.h>
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")

#include <ctime>
#include <iomanip>

#include <fstream>

#include <sstream>

#include <chrono>

#include "includes.h"
#include "Process.hpp"

ImVec2 blur_alpha = ImVec2(0.f, 0.f);

float lastUpdateTime = 0.0f;

static bool inject_swap = false;
static int inject_swap_state = 0;
static float inject_hide_alpha = 0;
int Minhook(int argc, char* argv[])
{
    std::string window_name = "FiveM® by Cfx.re";
    if (argc >= 2)
        window_name = argv[1];

  
    Process p(window_name.c_str());
    if (!p)
        return -1;

    p.disableHooks();

    return 0;
}
int main(int, char**)
{

    WNDCLASSEXW wc;
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = nullptr;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = L"ImGui";
    wc.lpszClassName = L"Example";
    wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

    RegisterClassExW(&wc);
    hwnd = CreateWindowExW(NULL, wc.lpszClassName, L"Example", WS_POPUP, (GetSystemMetrics(SM_CXSCREEN) / 2) - (menu_size.x / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (menu_size.y / 2), menu_size.x, menu_size.y, 0, 0, 0, 0);

    SetWindowLongA(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    SetWindowRgn(hwnd, CreateRoundRectRgn(0, 0, menu_size.x, menu_size.y, 20, 20), TRUE); // Создание скругленных углов


    POINT mouse;
    rc = { 0 };
    GetWindowRect(hwnd, &rc);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(GetConsoleWindow(), SW_HIDE);
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    Minhook(NULL, NULL);
    ImFontConfig cfg;
    cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome; //отключает антиалиасинг и дает строгий алгоритм хинта


    io.Fonts->AddFontFromMemoryTTF(&InterSemiBold, sizeof InterSemiBold, 17, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    default_font = io.Fonts->AddFontFromMemoryTTF(&InterSemiBold, sizeof InterSemiBold, 17, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    semibold_font[0] = io.Fonts->AddFontFromMemoryTTF(&InterSemiBold, sizeof InterSemiBold, 15, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    semibold_font[1] = io.Fonts->AddFontFromMemoryTTF(&InterSemiBold, sizeof InterSemiBold, 13, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    semibold_font[2] = io.Fonts->AddFontFromMemoryTTF(&InterSemiBold, sizeof InterSemiBold, 19, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    semibold_font[4] = io.Fonts->AddFontFromMemoryTTF(&InterSemiBold, sizeof InterSemiBold, 11, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    semibold_font[5] = io.Fonts->AddFontFromMemoryTTF(&InterSemiBold, sizeof InterSemiBold, 31, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    semibold_font[6] = io.Fonts->AddFontFromMemoryTTF(&InterSemiBold, sizeof InterSemiBold, 17, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    //semibold_font[5] = io.Fonts->AddFontFromMemoryTTF(&InterSemiBold, sizeof InterSemiBold, 16, NULL, io.Fonts->GetGlyphRangesCyrillic());

    bold_font[0] = io.Fonts->AddFontFromMemoryTTF(&InterBold, sizeof InterBold, 33, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    bold_font[1] = io.Fonts->AddFontFromMemoryTTF(&InterBold, sizeof InterBold, 29, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    bold_font[2] = io.Fonts->AddFontFromMemoryTTF(&InterBold, sizeof InterBold, 13, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    bold_font[3] = io.Fonts->AddFontFromMemoryTTF(&InterBold, sizeof InterBold, 19, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    bold_font[4] = io.Fonts->AddFontFromMemoryTTF(&InterBold, sizeof InterBold, 21, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    bold_font[5] = io.Fonts->AddFontFromMemoryTTF(&InterBold, sizeof InterBold, 23, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    bold_font[6] = io.Fonts->AddFontFromMemoryTTF(&InterBold, sizeof InterBold, 11, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    icon_font[0] = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 25, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    icon_font[1] = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 20, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    icon_font[2] = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 40, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    icon_font[3] = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 30, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    medium_font[0] = io.Fonts->AddFontFromMemoryTTF(&InterMedium, sizeof InterMedium, 15, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    medium_font[1] = io.Fonts->AddFontFromMemoryTTF(&InterMedium, sizeof InterMedium, 23, &cfg, io.Fonts->GetGlyphRangesCyrillic());




    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImVec4 clear_color = ImColorToImVec4(ImColor(255, 7, 7, 0));

    ImGuiStyle& s = ImGui::GetStyle();

    s.FramePadding = ImVec2(10, 10);
    s.ItemSpacing = ImVec2(10, 10);
    s.FrameRounding = items_rounding;
    s.WindowRounding = items_rounding;
    s.WindowBorderSize = 0.f;
    s.PopupBorderSize = 0.f;
    s.WindowPadding = ImVec2(0, 0);
    s.ChildBorderSize = 0.0001f;
    s.Colors[ImGuiCol_Border] = ImVec4(0.f, 0.f, 0.f, 0.f);
    s.WindowShadowSize = 0;
    s.PopupRounding = 30.f;
    s.PopupBorderSize = 1.3f;
    s.ScrollbarSize = 5.f;
    s.ScrollbarRounding = 10.f;

    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {

            blur::set_device(g_pd3dDevice);
            blur::new_frame();
            LoadImages();

            ImGui::SetNextWindowSize(menu_size);
            ImGui::SetNextWindowPos(ImVec2(0, 0));

            ImGui::Begin("General", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);
            {

                anim_speed = ImGui::GetIO().DeltaTime * 16.f;

                auto draw = ImGui::GetWindowDrawList();
                const auto& p = ImGui::GetWindowPos();

                const char* auth_product_name = "Fairplay?";
                const char* auth_product_description = "FPAC, O novo melhor mecanismo de protecao e seguranca para jogos de PC multiplayer terceirizados ou nao competitivos.Nossa equipe tem anos de experiencia utilizando tecnologias hibridas avancadas nao invasivas para impedir que cheaters atrapalhem seu jogo.";


                draw->AddRectFilled(p, p + menu_size, winbg, 8);

                static ImVec2 auth_offset;
                static ImVec2 auth_alpha;
                static bool sign_up;


                EasingAnimationV2("auth_offset", &auth_offset, sign_up && menu_state == 0 ? ImVec2(400.f, 0.f) : menu_state == 1 ? ImVec2(auth_offset.x, -400.f) : ImVec2(0.f, 0.f), 1.3f, imanim::EasingCurve::Type::InOutQuint, -1);
                EasingAnimationV2("auth_alpha", &auth_alpha, menu_state == 1 ? ImVec2(1.f, 1.f) : ImVec2(0.f, 0.f), 1.3f, imanim::EasingCurve::Type::InOutQuint, -1);

                if (menu_state < 2) {

                    draw->AddCircleFilled(p, 400.f, ImColor(47.f / 255.f, 106.f / 255.f, 204.f / 255.f, 1.f), 30);
                    draw->AddRectFilled(p + ImVec2(menu_size.x / 2, 0), p + menu_size, ImColor(59, 133, 255, 255), 8, ImDrawFlags_RoundCornersBottomRight);
                    draw->AddImageRounded(auth_pic, p + ImVec2(menu_size.x / 2, 0), p + menu_size, ImVec2(0, 0), ImVec2(1, 1), ImColor(1.f, 1.f, 1.f, 1.f), 8.f, ImDrawFlags_RoundCornersBottomRight);


                    PushFont(bold_font[0]);
                    draw->AddText(ImVec2(center_text(p, p + ImVec2(menu_size.x / 2, 0.f), "FairPLay").x, p.y + 20), ImColor(1.f, 1.f, 1.f, 1.f), "Fair^8Play");
                    ImGui::PopFont();

                    PushFont(bold_font[3]);
                    draw->AddText(ImVec2(center_text(p, p + ImVec2(menu_size.x / 2, 0.f), "FIQUE FRIO! FAZEMOS O RESTO").x, p.y + 53), ImColor(1.f, 1.f, 1.f, 1.f / 2.f), "^1FIQUE ^5FRIO^1! FAZEMOS O RESTO");
                    ImGui::PopFont();

                    PushFont(bold_font[0]);
                    draw->AddText(ImVec2(center_text(p + ImVec2(375, 0.f), p + ImVec2(menu_size.x, 0.f), auth_product_name).x, p.y + 160), ImColor(1.f, 1.f, 1.f, 1.f), auth_product_name);
                    ImGui::PopFont();

                    PushFont(bold_font[15]); PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
                    CenterText(auth_product_description, p + ImVec2(375, 207), 375, 5);
                    PopFont(); PopStyleColor();

                    ImGui::SetCursorPos(ImVec2(462.5f, 355));
                    ImGui::Button("Ver mais", ImVec2(200, 35));

                    ImGui::SetCursorPos(ImVec2(62.5f, 162) - auth_offset); ImGui::BeginGroup(); {
                        Particles();

                        PushFont(bold_font[4]); ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(125 - CalcTextSize("LOGIN").x / 2, 0.f));
                        ImGui::Text("LOGIN"); ImGui::PopFont();

                        ImGui::InputTextEx("Nome da conta$8", "9", sign_login, sizeof(sign_login), ImVec2(250, 40), 0);
                        ImGui::InputTextEx("Senha$7", "6", sign_password, sizeof(sign_login), ImVec2(250, 40), 0);
                        ImGui::Checkbox("Lembrar minha conta", &rememberme);
                        if (ImGui::Button("ENTRAR", ImVec2(250, 45))) {
                            menu_state = 1;
                            notificationSystem.Notification("1", "Comunicado", "Voce entrou com sucesso. Seja bem-vindo!. ", ImColor(0.5f, 1.f, 0.5f, 1.f));
                        }
                        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(16, 0));
                        if (TwoTexts("Quer criar uma conta? ", "Cadastre-se"))
                            system("start https://fairplay.com/manager");
                        ImGui::CustomText("Copyright @ 2024 FAIRPLAY. Todos os direitos reservados.", bold_font[2], text_color[1], ImRect(p + ImVec2(148, 480), p + ImVec2(226, 490)));
                    }ImGui::EndGroup();

                    ImGui::SetCursorPos(ImVec2(-337.5f + auth_offset.x, 130 - auth_offset.y)); ImGui::BeginGroup(); {

                        PushFont(bold_font[4]); ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(125 - CalcTextSize("REGISTRATION").x / 2, 0.f));
                        ImGui::Text("REGISTRATION"); ImGui::PopFont();

                        ImGui::InputTextEx("Login$8", "9", login, 128, ImVec2(250, 40), 0);
                        ImGui::InputTextEx("Password$7", "6", password, 128, ImVec2(250, 40), 0);
                        ImGui::InputTextEx("Invite key$5", "4", key, 128, ImVec2(250, 40), 0);
                        ImGui::Checkbox("Remember Me", &rememberme);
                        if (ImGui::Button("SIGN UP", ImVec2(250, 45))) {
                            menu_state = 1;
                            notificationSystem.Notification("1", "Notification", "Your account was successfully registred. ", ImColor(0.5f, 1.f, 0.5f, 1.f));
                        }
                        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(-11.f, 0));
                        if (TwoTexts("Do you already have an account? ", "Sign in"))
                            sign_up = !sign_up;
                        ImGui::CustomText("Copyright @ 2024 FAIRPLAY. Todos os direitos reservados.", bold_font[2], text_color[1], ImRect(p + ImVec2(148, 480), p + ImVec2(226, 490)));
                    }ImGui::EndGroup();
                }

                if (menu_state < 4 && menu_state > 1)
                {
                    Particles2();

                    draw->AddCircleFilled(p, 350.f, ImColor(0.f, 0.f, 0.f, 1.f / 4.f), 64);
                    draw->AddCircleFilled(p + menu_size, 350.f, ImColor(0.f, 0.f, 0.f, 1.f / 4.f), 64);

                    const char* product_name[] = { "CS2", "CS:GO", "Minecraft", "RUST", "GTA V", "Apex Legends", "Garry’s Mod", "TF2" };
                    const char* product_description[] = { "^2UNDETECTED\n^0Till: ^119.02.2024", "^2UNDETECTED", "^3DETECTED", "^2UNDETECTED", "^2UNDETECTED", "^3DETECTED", "^2UNDETECTED", "^2UNDETECTED\n^0Till: ^119.02.2024" };
                    ImColor product_status[] = { ImColor(255, 242, 128), ImColor(255, 128, 128), ImColor(255, 128, 128), ImColor(0.5f, 1.f, 0.5f), ImColor(0.5f, 1.f, 0.5f), ImColor(255, 128, 128), ImColor(0.5f, 1.f, 0.5f), ImColor(0.5f, 1.f, 0.5f) };
                    static ImVec2 product_target_pos[] = { ImVec2(30, 78), ImVec2(210, 78), ImVec2(390, 78), ImVec2(570, 78), ImVec2(30, 308), ImVec2(210, 308), ImVec2(390, 308), ImVec2(570, 308) };
                    static ImVec2 product_pos[] = { ImVec2(30, 578), ImVec2(210, 578), ImVec2(390, 578), ImVec2(570, 578), ImVec2(30, 808), ImVec2(210, 808), ImVec2(390, 808), ImVec2(570, 808) };

                    for (int i = 0; i < IM_ARRAYSIZE(product_name); i++) {

                        if (menu_state == 2)
                            EasingAnimationV2("product_pos" + std::to_string(i), &product_pos[i], menu_state == 2 ? product_target_pos[i] : product_target_pos[i] + ImVec2(0, 500), menu_state == 2 ? 1.f + (0.2f * i) : 0.25f, imanim::EasingCurve::Type::InOutBack, -1);
                        else
                            product_pos[i] = ImLerp(product_pos[i], product_target_pos[i] + ImVec2(0, 500), anim_speed);

                        //product_pos[i] = ImLerp(product_pos[i], product_target_pos[i], anim_speed);

                        ImGui::SetCursorPos(product_pos[i]);
                        if (ImGui::Product(product_name[i], product_description[i], i, product_pics[i], product_pos[i])) {
                            inject_swap = true;
                            inject_swap_state = 2;

                            iProduct = i;
                        }
                    }
                }

                static ImVec2 inject_offset = ImVec2(0, -500);

                EasingAnimationV2("inject_offset", &inject_offset, menu_state >= 2 ? ImVec2(0.f, 0.f) : ImVec2(0.f, -500.f), menu_state == 4 ? 1.f : 0.3f, imanim::EasingCurve::Type::InOutQuad, -1);

                if (menu_state == 4)
                {
                    Particles2();

                    draw->AddCircleFilled(p, 350.f, ImColor(0.f, 0.f, 0.f, 1.f / 4.f), 64);
                    draw->AddCircleFilled(p + menu_size, 350.f, ImColor(0.f, 0.f, 0.f, 1.f / 4.f), 64);
                    const char* product_name[] = { "Counter-Strike 2", "Counter-Strike Go", "Minecraft", "RUST", "Grand Theft Auto V", "Apex Legends", "Garry’s Mod", "Team Fortress 2" };

                    ImGui::SetCursorPos(ImVec2(30, 68 + inject_offset.y));
                    ImGui::BeginGroup(); {

                        PushFont(icon_font[2]);
                        draw->AddText(center_text(p + ImVec2(0, 48), p + ImVec2(100, 148), "3") + ImVec2(0, inject_offset.y - 10), text_color[1], "3");
                        PopFont();

                        draw->AddText(center_text(p + ImVec2(0, 48), p + ImVec2(100, 148), "Back") + ImVec2(0, 15 + inject_offset.y), text_color[1], "Back");


                        if (IsMouseHoveringRect(p + ImVec2(0, 48), p + ImVec2(100, 148)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                            inject_swap = true;
                            inject_swap_state = 1;
                        }

                    }ImGui::EndGroup();

                    ImGui::SetCursorPos(ImVec2(100, 69 + inject_offset.y)); BeginGroup();
                    RenderText(product_name[iProduct], semibold_font[5], text_color[0], ImVec2(2, 2));
                    RenderText("Computer game (Tactical Shooter)", medium_font[1], text_color[1]);
                    EndGroup();

                    ImGui::SetCursorPos(ImVec2(10 + inject_offset.y, 158)); BeginGroup(); {
                        ImGui::InjectImage(inject_pics[4]); ImGui::SameLine(); ImGui::InjectImage(inject_pics[1]);
                        ImGui::InjectImage(inject_pics[2]); ImGui::SameLine(); ImGui::InjectImage(inject_pics[3]);
                        ImGui::InjectImage(inject_pics[5], true); ImGui::SameLine(); ImGui::InjectImage(inject_pics[5], true);
                    } EndGroup();

                    ImGui::SetCursorPos(ImVec2(385 - (inject_offset.y), 158)); BeginGroup(); {
                        ImGui::InformationBlock("This modification has been tested more than 12 thousand times", main_color, "Certainly! However, you didn't specify which modification (mod)\n you're referring to. Mods can vastly vary in nature\n, affecting different aspects of games or software\n - from graphical");
                        ImGui::InformationBlock("Only 1 game lockout was received because of this modification", ImColor(255, 90, 90));
                        ImGui::InformationBlock("There's also some text I don't know about... Will you write it?", ImColor(90, 146, 255));
                    } EndGroup();

                    ImGui::SetCursorPos(ImVec2(500, 73 + inject_offset.y));
                    ImGui::LoadingButton("INJECT", ImVec2(200, 50));
                    {
                        Minhook(NULL, NULL);
                    }
                }

                if (inject_hide_alpha >= 0.98f) {

                    if (inject_swap_state == 1)
                        menu_state = 2;

                    if (inject_swap_state == 2)
                        menu_state = 4;

                    if (inject_hide_alpha >= 0.998f)
                        inject_swap = false;
                }

                inject_hide_alpha = ImLerp(inject_hide_alpha, inject_swap ? 1.f : 0, anim_speed / 1.5f);
                draw->AddRectFilled(p, p + menu_size, ImColor(0.12156862745098039, 0.13333333333333333, 0.1568627450980392, inject_hide_alpha * 1.2f), 8);

                if (injection_state == 3) {
                    notificationSystem.Notification("1", "Notification", "Your injection was successfully administered. ", ImColor(0.5f, 1.f, 0.5f, 1.f));
                    injection_state = 0;
                }

                static int head_offset;
                head_offset = ImLerp(head_offset, menu_state >= 1 ? 255 : 0, anim_speed);

                draw->AddRectFilled(p, p + ImVec2(menu_size.x, 48), ImColor(39, 43, 50, head_offset), ImGuiStyleVar_WindowRounding, ImDrawFlags_RoundCornersTop);

                draw->AddImageRounded(avatar_pic, p + ImVec2(515, 8), p + ImVec2(547, 40), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, head_offset), 360.f);

                PushFont(semibold_font[6]);
                draw->AddText(p + ImVec2(551, 9.5f), ImColor(255, 255, 255, head_offset), "Lyapos");
                PopFont();

                PushFont(semibold_font[0]);
                draw->AddText(p + ImVec2(551, 23.5f), ImColor(255, 255, 255, head_offset), "Reg: ^119.02.2024");
                PopFont();

                PushFont(bold_font[5]);
                draw->AddText(ImVec2(p.x + 13, center_text(p, p + ImVec2(menu_size.x, 48), "Fairplay").y), ImColor(255, 255, 255, head_offset), "Fairplay");
                ImGui::PopFont();

                if (auth_alpha.x >= 0.99f)
                    menu_state = 2;

                draw->AddRectFilled(p, p + menu_size, ImColor(31, 34, 40, int(auth_alpha.x * 255)), 8);

                ImRect close_button_bb = ImRect(ImVec2(p.x + 717, p.y + 20), ImVec2(p.x + 717, p.y + 20) + ImVec2(10, 10));
                ImRect mini_button_bb = ImRect(ImVec2(p.x + 685, p.y + 20), ImVec2(p.x + 685, p.y + 20) + ImVec2(10, 10));

                ImGui::PushFont(icon_font[1]);
                draw->AddText(center_text(close_button_bb.Min, close_button_bb.Max, "2"), IsMouseHoveringRect(close_button_bb.Min, close_button_bb.Max, true) ? main_color : text_color[0], "2");
                draw->AddText(center_text(mini_button_bb.Min, mini_button_bb.Max, "1"), IsMouseHoveringRect(mini_button_bb.Min, mini_button_bb.Max, true) ? main_color : text_color[0], "1");
                ImGui::PopFont();

                if (IsMouseHoveringRect(mini_button_bb.Min, mini_button_bb.Max, true) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    ShowWindow(hwnd, SW_MINIMIZE);
                }

                if (IsMouseHoveringRect(close_button_bb.Min, close_button_bb.Max, true) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    exit(0);
                }

                move_window();
                notificationSystem.DrawNotification();
            }
            ImGui::End();
           // RenderBlur(hwnd);
        }
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 0.0f, 0);

        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
    }
}
