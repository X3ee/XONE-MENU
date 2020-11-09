// dear imgui: standalone example application for DirectX 9
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include <string>
#include "../../misc/fonts/FontAwesomeTttf.h"
#include "../../misc/fonts/Icons.h"


// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
ImFont* TabsFont;
ImFont* MenuFont;
ImFont* NameFont;
ImFont* IconsFont;
// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    TabsFont = io.Fonts->AddFontFromFileTTF("../../misc/fonts/arial3.ttf", 13.f);
    NameFont = io.Fonts->AddFontFromFileTTF("../../misc/fonts/arial1.ttf", 17.f);
    MenuFont = io.Fonts->AddFontFromFileTTF("../../misc/fonts/arial2.ttf", 14.f);
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    IconsFont = io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesomeTttf_compressed_data, FontAwesomeTttf_compressed_size, 15.f, &icons_config, icons_ranges);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    auto clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

		//styles
        ImGuiStyle& style = ImGui::GetStyle();
        style.FrameRounding = 5.f;
        style.ChildBorderSize = 1.f;
        style.ScrollbarSize = 1.5f;
        style.ScrollbarRounding = 3.f;
        //colors
        auto colors = style.Colors;
        colors[ImGuiCol_FrameBg] = ImColor(40, 40, 50);
        colors[ImGuiCol_FrameBgHovered] = ImColor(30, 30, 40);
        colors[ImGuiCol_FrameBgActive] = ImColor(240, 32, 65);
        colors[ImGuiCol_Separator] = ImColor(240, 32, 65);
        colors[ImGuiCol_SliderGrab] = ImColor(240, 32, 65);
        colors[ImGuiCol_SliderGrabActive] = ImColor(240, 32, 65);
        colors[ImGuiCol_PopupBg] = ImColor(40, 40, 50);
        colors[ImGuiCol_ScrollbarBg] = ImColor(0, 0, 0, 0);
        colors[ImGuiCol_ScrollbarGrab] = ImColor(240, 32, 65);
        colors[ImGuiCol_ScrollbarGrabActive] = ImColor(240, 32, 65);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(250, 35, 70);
        colors[ImGuiCol_Border] = ImColor(40, 40, 50);
        colors[ImGuiCol_ChildBg] = ImColor(50, 50, 60);
        colors[ImGuiCol_Header] = ImColor(30, 30, 40);
        colors[ImGuiCol_HeaderHovered] = ImColor(250, 35, 70);
        colors[ImGuiCol_HeaderActive] = ImColor(240, 32, 65);
        colors[ImGuiCol_Button] = ImColor(40, 40, 50);
        colors[ImGuiCol_ButtonHovered] = ImColor(250, 35, 70);
        colors[ImGuiCol_ButtonActive] = ImColor(240, 32, 65);
        colors[ImGuiCol_Text] = ImColor(255, 255, 255);
        //menu
        {
            ImVec2 p;
            ImDrawList* d;
            std::string text = "X";
            std::string text2 = "ONE";
            std::string text3 = "xone.fun";
            ImVec2 size = ImGui::CalcTextSize(text.c_str());
            ImVec2 size2 = ImGui::CalcTextSize(text2.c_str());
            ImVec2 size3 = ImGui::CalcTextSize(text3.c_str());
            ImGui::SetNextWindowSize(ImVec2(540, 650));
            if (ImGui::Begin("XONE", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground))
            {
                static int tabs = 0;
                p = ImGui::GetWindowPos();
                d = ImGui::GetWindowDrawList();
                
                d->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 540, p.y + 650), ImColor(40, 40, 50), 15.f);
                ImGui::BeginGroup();
                {
                    d->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 90, p.y + 650), ImColor(50, 50, 60), 15.f);
                    d->AddRectFilled(ImVec2(p.x + 80, p.y), ImVec2(p.x + 90, p.y + 650), ImColor(50, 50, 60));
                    ImGui::PushFont(NameFont);
                    d->AddText(ImVec2(p.x + (90 - size.x) * 0.34f, p.y + 20), ImColor(240, 32, 65), text.c_str());
                    d->AddText(ImVec2(p.x + (90 - size2.x) * 0.58f, p.y + 20), ImColor(255, 255, 255), text2.c_str());
                    ImGui::PopFont();
                    ImGui::PushFont(MenuFont);
                    d->AddText(ImVec2(p.x + (90 - size3.x) * 0.55f, p.y + 50), ImColor(255, 255, 255), text3.c_str());
                    ImGui::PopFont();
                    ImGui::BeginGroup();
                    {
                        d->AddRectFilled(ImVec2(p.x + 93, p.y), ImVec2(p.x + 540, p.y + 650), ImColor(50, 50, 60), 15.f);
                        d->AddRectFilled(ImVec2(p.x + 93, p.y), ImVec2(p.x + 100, p.y + 650), ImColor(50, 50, 60));
                    }
                    ImGui::EndGroup();
                }
                ImGui::EndGroup();
                ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 540, ImGui::GetWindowHeight() - 580));
                ImGui::BeginGroup();
                {
                    if (ImGui::Tab("Ragebot", ICON_FA_STREET_VIEW, tabs == 0, ImVec2(90, 40)))
                        tabs = 0;
                    if (ImGui::Tab("Legitbot", ICON_FA_CROSSHAIRS, tabs == 1, ImVec2(90, 40)))
                        tabs = 1;
                    if (ImGui::Tab("Visuals", ICON_FA_LOW_VISION, tabs == 2, ImVec2(90, 40)))
                        tabs = 2;
                    if (ImGui::Tab("Misc", ICON_FA_BUG, tabs == 3, ImVec2(90, 40)))
                        tabs = 3;
                    if (ImGui::Tab("Radar", ICON_FA_BULLSEYE, tabs == 4, ImVec2(90, 40)))
                        tabs = 4;
                    if (ImGui::Tab("Grenade Helper", ICON_FA_BOMB, tabs == 5, ImVec2(90, 40)))
                        tabs = 5;
                    if (ImGui::Tab("Changers", ICON_FA_MAGIC, tabs == 6, ImVec2(90, 40)))
                        tabs = 6;
                    if (ImGui::Tab("Find", ICON_FA_SEARCH, tabs == 7, ImVec2(90, 40)))
                        tabs = 7;
                    if (ImGui::Tab("Chat", ICON_FA_COMMENT, tabs == 8, ImVec2(90, 40)))
                        tabs = 8;
                    if (ImGui::Tab("Settings", ICON_FA_COG, tabs == 9, ImVec2(90, 40)))
                        tabs = 9;
                    ImGui::SameLine();
                }
                ImGui::EndGroup();
                ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 450 + style.WindowPadding.x, ImGui::GetWindowHeight() - 650 + style.WindowPadding.y));
                ImGui::BeginGroup();
                {
                    if (tabs == 0)
                    {
                        static int rage_tabs = 0;
                        if (ImGui::ButtonActive("General", rage_tabs == 0, ImVec2(139, 20)))
                            rage_tabs = 0;
                        ImGui::SameLine();
                        if (ImGui::ButtonActive("Aim-bot", rage_tabs == 1, ImVec2(139, 20)))
                            rage_tabs = 1;
                        ImGui::SameLine();
                        if (ImGui::ButtonActive("Anti-Aim", rage_tabs == 2, ImVec2(139, 20)))
                            rage_tabs = 2;
                        if (rage_tabs == 0)
                        {
                            static int Int = 0;
                            static float Float = 0.f;
                            static bool Bool = false;
                            static int Char = 0;
                            static char* Chars[] = { "Test", "Test1" };
                            ImGui::Text("General");
                            ImGui::SliderInt("SliderInt", &Int, 0, 25, "%d");
                            ImGui::SliderFloat("SliderFloat", &Float, 0.f, 25.f, "%.1f");
                            ImGui::Combo("Combo", &Char, Chars, IM_ARRAYSIZE(Chars));
                            ImGui::Checkbox("Checkbox", &Bool);
                            ImGui::Button("Button");
                        }
                        if (rage_tabs == 1)
                        {
                            ImGui::Text("Aim-bot");
                        }
                        if (rage_tabs == 2)
                        {
                            ImGui::Text("Anti-Aim");
                        }
                    }
                    else if (tabs == 1)
                    {

                    }
                    else if (tabs == 2)
                    {

                    }
                    else if (tabs == 3)
                    {

                    }
                    else if (tabs == 4)
                    {

                    }
                    else if (tabs == 5)
                    {

                    }
                    else if (tabs == 6)
                    {

                    }
                    else if (tabs == 7)
                    {

                    }
                    else if (tabs == 8)
                    {

                    }
                    else if (tabs == 9)
                    {

                    }
                }
                ImGui::EndGroup();
                
            }
            ImGui::End();
        }

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*255.0f), (int)(clear_color.y*255.0f), (int)(clear_color.z*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
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
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
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
