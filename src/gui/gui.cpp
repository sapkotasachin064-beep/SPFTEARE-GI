#include "gui.h"
#include "../core/Logger.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <string>
#include <chrono>

namespace GUI {
    bool isOpen = false;

    void SetupTheme() {
        auto& style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(15, 15);
        style.WindowRounding = 8.0f;
        style.FramePadding = ImVec2(5, 5);
        style.FrameRounding = 4.0f;
        style.ItemSpacing = ImVec2(12, 8);
        style.ItemInnerSpacing = ImVec2(8, 6);
        style.IndentSpacing = 25.0f;
        style.ScrollbarSize = 15.0f;
        style.ScrollbarRounding = 9.0f;
        style.GrabMinSize = 5.0f;
        style.GrabRounding = 3.0f;
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

        ImVec4* colors = style.Colors;
        colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.93f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 0.96f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        colors[ImGuiCol_Border] = ImVec4(0.60f, 0.20f, 1.00f, 0.50f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.60f, 0.20f, 1.00f, 0.40f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.60f, 0.20f, 1.00f, 0.60f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.60f, 0.20f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.60f, 0.20f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.70f, 0.30f, 1.00f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.60f, 0.20f, 1.00f, 0.80f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.60f, 0.20f, 1.00f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.60f, 0.20f, 1.00f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.60f, 0.20f, 1.00f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.20f, 1.00f, 0.50f);
    }

    void Initialize(HWND window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        
        SetupTheme();

        ImGui_ImplWin32_Init(window);
        ImGui_ImplDX11_Init(pDevice, pContext);
    }

    void RenderOverlay() {
        static float fps = 0.0f;
        static int frames = 0;
        static auto lastTime = std::chrono::steady_clock::now();
        
        frames++;
        auto currentTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
        if (duration >= 500) {
            fps = frames * 1000.0f / duration;
            frames = 0;
            lastTime = currentTime;
        }

        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.6f);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
        
        if (ImGui::Begin("Overlay", nullptr, window_flags)) {
            ImGui::TextColored(ImVec4(0.6f, 0.2f, 1.0f, 1.0f), "SPFTEARE GI v1.0");
            ImGui::SameLine();
            ImGui::TextDisabled("| Safe Mode");
            ImGui::SameLine();
            ImGui::Text(" | FPS: %.1f", fps);
            
            if (isOpen) {
                ImGui::Separator();
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "MENU ACTIVE [DELETE]");
            }
            
            ImGui::End();
        }
    }

    void RenderMenu() {
        if (!isOpen) return;

        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("SPFTEARE GI INTERNAL", &isOpen, ImGuiWindowFlags_NoCollapse)) {
            if (ImGui::BeginTabBar("MainTabs")) {
                if (ImGui::BeginTabItem("Home")) {
                    ImGui::Spacing();
                    ImGui::Text("Welcome to SPFTEARE GI");
                    ImGui::Separator();
                    ImGui::Text("Status:   "); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "Undetected");
                    ImGui::Text("Version:  "); ImGui::SameLine(); ImGui::Text("1.0.0");
                    ImGui::Text("Build:    "); ImGui::SameLine(); ImGui::Text("%s", __DATE__);
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Text("Keybinds:");
                    ImGui::BulletText("DELETE - Toggle Menu");
                    ImGui::BulletText("END    - Unload Cheat");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Visuals")) {
                    ImGui::Spacing();
                    static bool esp_box = false;
                    static bool esp_lines = false;
                    static float esp_color[3] = { 0.6f, 0.2f, 1.0f };

                    ImGui::Checkbox("Enable ESP", &esp_box);
                    ImGui::Checkbox("Snaplines", &esp_lines);
                    ImGui::ColorEdit3("ESP Color", esp_color);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Settings")) {
                    ImGui::Spacing();
                    if (ImGui::Button("Unload DLL", ImVec2(120, 35))) {
                        // Handled in main loop
                    }
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }
    }

    void Render() {
    static bool initOnce = true;
    if (initOnce) {
        Logger::Success("GUI Render started!");
        initOnce = false;
    }

    // Toggle with better detection
    if (GetAsyncKeyState(VK_DELETE) & 1) {
        isOpen = !isOpen;
        Logger::Log(isOpen ? "Menu OPENED" : "Menu CLOSED");
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    RenderOverlay();   // Always draw overlay
    RenderMenu();      // Draw menu when open

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

    void Toggle() {
        isOpen = !isOpen;
    }
}
