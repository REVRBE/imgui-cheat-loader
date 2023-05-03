#include "ui.hh"
#include "xenforo.hh"
#include "../globals.hh"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include <shellapi.h>
#include <thread>

bool loginFailed = false; 

void ui::renderSecondPrompt() {
    const std::string& user_rank_local = globals.user_rank;
    ImGui::Text("Welcome, %s!", globals.user_name);
    ImGui::Text("Rank: %s", user_rank_local.c_str()); // Use the local copy of the user rank
	
    if (user_rank_local == "VIP") {
        ImGui::Text("VIP ends at: %s", globals.vip_end_time.c_str()); // Display the VIP end time
    }

    ImGui::Spacing();

    if (user_rank_local == "Registered" || user_rank_local == "Unregistered / Unconfirmed") {
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        if (ImGui::Button("Renew")) {
            ShellExecute(NULL, "open", "http://your-xenforo-forum-link/forum/index.php?register/", NULL, NULL, SW_SHOWNORMAL);
        }
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Click to renew your subscription.");
            ImGui::EndTooltip();
        }
    }
    else {
        ImGui::Dummy(ImVec2(0.0f, 14.0f));

        if (ImGui::Button("Load cheat")) {

            // Add implementation here for loading the cheat
        }
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Click to load cheat!");
            ImGui::Text("Bla bla bla...");
            ImGui::EndTooltip();
        }
        ImGui::SameLine();

        if (ImGui::Button("Forum")) {
            ShellExecute(NULL, "open", "http://your-xenforo-forum-link.com/forum/index.php", NULL, NULL, SW_SHOWNORMAL);
        }
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Click to open the forum.");
            ImGui::EndTooltip();
        }
    }
}

void ui::render() {
    if (!globals.active) return;

    ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
    ImGui::SetNextWindowBgAlpha(1.0f);

    ImGui::Begin(window_title, &globals.active, window_flags);
    {
        if (!globals.isLoggedIn) {
            // Render login interface
            ImGui::Text("Username: ");
            ImGui::SameLine();
            ImGui::InputText("##username", globals.user_name, IM_ARRAYSIZE(globals.user_name));
            ImGui::Text("Password: ");
            ImGui::SameLine();
            ImGui::InputText("##password", globals.pass_word, IM_ARRAYSIZE(globals.pass_word), ImGuiInputTextFlags_Password);

            if (loginFailed) {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Login failed, try again.");
            }

            ImGui::Spacing();
            static int currentColorScheme = 0;
            if (ImGui::Button("Change colors")) {
                currentColorScheme = (currentColorScheme + 1) % 14;

                TCHAR tempPath[MAX_PATH];
                GetTempPath(MAX_PATH, tempPath);
                std::string colorSchemeFile = std::string(tempPath) + "color_scheme.txt";

                std::ofstream outputFile(colorSchemeFile);
                if (outputFile.is_open()) {
                    outputFile << currentColorScheme;
                    outputFile.close();
                }

                applyColorScheme(colorSchemes[currentColorScheme]);
            }
            ImGui::SameLine(0, 60);
            if (ImGui::Button("Login")) {
                bool login = checkLoginCredentials(globals.user_name, globals.pass_word);
                loginFailed = !login;

                if (login) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 6000 + 2000));
                    globals.isLoggedIn = true;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Register")) {
                ShellExecute(NULL, "open", "http://your-xenforo-forum-link.com/forum/index.php?register/", NULL, NULL, SW_SHOWNORMAL);
            }
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Click to open the registration page.");
                ImGui::EndTooltip();
            }

            applyColorScheme(colorSchemes[currentColorScheme]);
        }
        else {
            renderSecondPrompt();
        }
    }
    ImGui::End();
}

void ui::init(LPDIRECT3DDEVICE9 device) {
    dev = device;

    int lastColorSchemeIndex = 0;

    TCHAR tempPath[MAX_PATH];
    GetTempPath(MAX_PATH, tempPath);
    std::string colorSchemeFile = std::string(tempPath) + "color_scheme.txt";

    std::ifstream inputFile(colorSchemeFile);
    if (inputFile.is_open()) {
        inputFile >> lastColorSchemeIndex;
        inputFile.close();
    }

    // colors
    currentColorScheme = lastColorSchemeIndex;
    applyColorScheme(colorSchemes[currentColorScheme]);

    if (window_pos.x == 0) {
        RECT screen_rect{};
        GetWindowRect(GetDesktopWindow(), &screen_rect);
        screen_res = ImVec2(float(screen_rect.right), float(screen_rect.bottom));
        window_pos = (screen_res - window_size) * 0.5f;

        // init images here
    }
}

void ui::applyColorScheme(const char* colorScheme) {
    ImGuiStyle& style = ImGui::GetStyle();

    if (strcmp(colorScheme, "Standard") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.22f, 0.22f, 0.22f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.40f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.90f, 0.90f, 0.90f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.70f, 0.50f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.70f, 0.50f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.9f);
    }
    else if (strcmp(colorScheme, "Cinder") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
    }
    else if (strcmp(colorScheme, "Green colorway") == 0) {
        ImGui::StyleColorsClassic();
        style.Colors[ImGuiCol_Text] = ImVec4(0.76f, 0.93f, 0.78f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.76f, 0.93f, 0.78f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.18f, 0.92f, 0.29f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.76f, 0.93f, 0.78f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.76f, 0.93f, 0.78f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.18f, 0.92f, 0.29f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
    }
    else if (strcmp(colorScheme, "Blue colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.85f, 0.92f, 0.95f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.85f, 0.92f, 0.95f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.18f, 0.23f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.48f, 0.69f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.32f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.18f, 0.23f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.18f, 0.23f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.18f, 0.23f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.18f, 0.23f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.48f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.31f, 0.48f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.63f, 0.82f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.35f, 0.63f, 0.82f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.44f, 0.60f, 0.75f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.22f, 0.38f, 0.51f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.63f, 0.82f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.85f, 0.92f, 0.95f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.85f, 0.92f, 0.95f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.44f, 0.60f, 0.75f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.18f, 0.23f, 0.9f);
    }
    else if (strcmp(colorScheme, "Purple/orange colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.88f, 0.84f, 0.96f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.88f, 0.84f, 0.96f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.17f, 0.12f, 0.24f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.91f, 0.55f, 0.23f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.30f, 0.20f, 0.40f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.30f, 0.20f, 0.40f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.30f, 0.20f, 0.40f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.30f, 0.20f, 0.40f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.30f, 0.20f, 0.40f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.91f, 0.55f, 0.23f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.91f, 0.55f, 0.23f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.93f, 0.52f, 0.17f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.57f, 0.35f, 0.85f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.84f, 0.36f, 0.14f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.45f, 0.25f, 0.58f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.57f, 0.35f, 0.85f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.88f, 0.84f, 0.96f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.88f, 0.84f, 0.96f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.84f, 0.36f, 0.14f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.22f, 0.16f, 0.28f, 0.9f);
    }
    else if (strcmp(colorScheme, "Dark mode") == 0) {
        ImGui::StyleColorsClassic();
        style.Colors[ImGuiCol_Text] = ImVec4(0.85f, 0.85f, 0.85f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.55f, 0.55f, 0.55f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.22f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.45f, 0.45f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.29f, 0.29f, 0.29f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.55f, 0.55f, 0.55f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.29f, 0.29f, 0.29f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.85f, 0.85f, 0.85f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.85f, 0.85f, 0.85f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.55f, 0.55f, 0.55f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.9f);
    }
    else if (strcmp(colorScheme, "Pink/green/blue colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.71f, 0.24f, 0.53f, 0.65f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.58f, 0.70f, 0.39f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.25f, 0.25f, 0.25f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.25f, 0.25f, 0.25f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.71f, 0.24f, 0.53f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.58f, 0.70f, 0.39f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.77f, 0.44f, 0.90f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.12f, 0.89f, 0.91f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.81f, 0.28f, 0.60f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.81f, 0.28f, 0.60f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.81f, 0.28f, 0.60f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.81f, 0.28f, 0.60f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.06f, 0.53f, 0.55f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.28f, 0.60f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.12f, 0.89f, 0.91f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.12f, 0.89f, 0.91f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.81f, 0.28f, 0.60f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.9f);
    }
    else if (strcmp(colorScheme, "Blue and grey colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.30f, 0.40f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.45f, 0.55f, 0.65f, 0.50f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.35f, 0.45f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.50f, 0.60f, 0.70f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.45f, 0.55f, 0.65f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.30f, 0.40f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.30f, 0.40f, 0.50f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.35f, 0.45f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.28f, 0.36f, 0.46f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.35f, 0.45f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.60f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.65f, 0.75f, 0.85f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.75f, 0.85f, 0.95f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.80f, 0.90f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.95f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.60f, 0.70f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.85f, 0.95f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.50f, 0.60f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.55f, 0.65f, 0.75f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.77f, 0.87f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.60f, 0.70f, 0.50f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.65f, 0.75f, 0.85f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.85f, 0.95f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.90f, 1.00f, 0.20f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.90f, 0.95f, 1.00f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.85f, 0.95f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.50f, 0.70f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.30f, 0.50f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.50f, 0.70f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.30f, 0.50f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.50f, 0.60f, 0.70f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.30f, 0.40f, 0.50f, 0.9f);
    }
    else if (strcmp(colorScheme, "Boring colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.80f, 0.80f, 0.80f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.90f, 0.20f, 0.20f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.80f, 0.80f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.70f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.60f, 0.90f, 0.90f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.80f, 1.00f, 1.00f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.90f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.80f, 0.80f, 0.50f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 1.00f, 1.00f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.90f, 0.20f, 0.20f, 0.20f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.35f, 0.35f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.90f, 0.30f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.80f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.90f, 0.30f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.90f, 0.80f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.50f, 0.80f, 0.80f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.30f, 0.50f, 0.50f, 0.9f);
    }
    else if (strcmp(colorScheme, "Blue/green colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.85f, 0.92f, 0.88f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.85f, 0.92f, 0.88f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.11f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.23f, 0.50f, 0.46f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.24f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.73f, 0.69f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.73f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.24f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.24f, 0.27f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.44f, 0.73f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.24f, 0.27f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.24f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.23f, 0.50f, 0.46f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.44f, 0.73f, 0.69f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.44f, 0.73f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.23f, 0.50f, 0.46f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.21f, 0.47f, 0.44f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.44f, 0.73f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.32f, 0.55f, 0.51f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.05f, 0.38f, 0.42f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.06f, 0.37f, 0.41f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.05f, 0.38f, 0.42f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.11f, 0.32f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.05f, 0.38f, 0.42f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.16f, 0.47f, 0.51f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.05f, 0.38f, 0.42f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.88f, 0.96f, 0.84f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.88f, 0.96f, 0.84f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.05f, 0.38f, 0.42f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.29f, 0.32f, 0.9f);
    }
    else if (strcmp(colorScheme, "Green/yellow colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.88f, 0.84f, 0.96f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.88f, 0.84f, 0.96f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.18f, 0.12f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.84f, 0.89f, 0.36f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.40f, 0.30f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.96f, 0.84f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.96f, 0.84f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.40f, 0.30f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.30f, 0.50f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.96f, 0.84f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.30f, 0.50f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.30f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.84f, 0.89f, 0.36f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.96f, 0.84f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.96f, 0.84f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.84f, 0.89f, 0.36f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.89f, 0.93f, 0.55f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.96f, 0.84f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.28f, 0.41f, 0.14f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.84f, 0.36f, 0.14f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.45f, 0.25f, 0.58f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.57f, 0.35f, 0.85f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.88f, 0.84f, 0.96f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.88f, 0.84f, 0.96f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.84f, 0.36f, 0.14f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.22f, 0.16f, 0.28f, 0.9f);
    }
    else if (strcmp(colorScheme, "Orange/grey colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.60f, 0.60f, 0.60f, 0.50f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.48f, 0.48f, 0.48f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.24f, 0.24f, 0.24f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.64f, 0.32f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.30f, 0.30f, 0.30f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.50f, 0.64f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45f, 0.65f, 0.85f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.55f, 0.75f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.64f, 0.32f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.50f, 0.64f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.55f, 0.75f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.40f, 0.40f, 0.40f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.93f, 0.75f, 0.18f, 0.78f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.93f, 0.75f, 0.18f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.72f, 0.79f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.93f, 0.75f, 0.18f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.75f, 0.18f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.54f, 0.90f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.93f, 0.75f, 0.18f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.93f, 0.75f, 0.18f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.54f, 0.90f, 0.70f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.93f, 0.75f, 0.18f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.93f, 0.75f, 0.18f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.93f, 0.75f, 0.18f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.23f, 0.16f, 0.42f, 0.9f);
    }
    else if (strcmp(colorScheme, "Whitey") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    }
    else if (strcmp(colorScheme, "Colorful") == 0) {
        ImGui::StyleColorsLight();
        style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.15f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.15f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.90f, 0.35f, 0.29f, 0.75f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.60f, 0.38f, 0.80f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.80f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.35f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.35f, 0.80f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.65f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.45f, 0.80f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.35f, 0.80f, 0.53f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00f, 0.80f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.60f, 0.38f, 0.80f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.90f, 0.35f, 0.29f, 0.75f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.80f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.60f, 0.38f, 0.70f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.90f, 0.35f, 0.29f, 0.75f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.35f, 0.80f, 0.70f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.45f, 1.00f, 0.80f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.25f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.15f, 0.35f, 0.80f, 0.45f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.15f, 0.45f, 1.00f, 0.70f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.25f, 0.60f, 0.80f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.15f, 0.35f, 0.80f, 0.20f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.15f, 0.45f, 1.00f, 0.45f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.15f, 0.25f, 0.60f, 0.70f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.15f, 0.35f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.15f, 0.45f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.15f, 0.35f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.15f, 0.45f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.15f, 0.45f, 1.00f, 0.35f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    }
}
