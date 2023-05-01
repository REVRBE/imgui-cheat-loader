#include "ui.hh"
#include "xenforo.hh"
#include "../globals.hh"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include <shellapi.h>
#include <thread>

bool loginFailed = false; 
bool loginSuccessful = false;

std::string user_rank;

void ui::renderSecondPrompt() {
    std::string user_rank_local = globals.user_rank; // Create a local copy of the user rank
    ImGui::Text("Welcome, %s!", globals.user_name);
    ImGui::Text("Rank: %s", user_rank_local.c_str()); // Use the local copy of the user rank
    // Render your second prompt interface here

    ImGui::Spacing();

    if (user_rank_local == "Registered" || user_rank_local == "Unregistered / Unconfirmed") {
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

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
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

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
            if (loginSuccessful) {
                ImGui::TextColored(ImVec4(0, 1, 0, 1), "Login successful!");
            }

            ImGui::Spacing();
            static int currentColorScheme = 0;
            if (ImGui::Button("Change colors")) {
                currentColorScheme = (currentColorScheme + 1) % 3;
            }
            ImGui::SameLine(0, 60);
            if (ImGui::Button("Login")) {
                bool login = checkLoginCredentials(globals.user_name, globals.pass_word);
                loginFailed = !login;
                loginSuccessful = login;

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
            // Render second prompt
            renderSecondPrompt();
        }
    }
    ImGui::End();
}

void ui::init(LPDIRECT3DDEVICE9 device) {
    dev = device;
	
    // colors
    applyColorScheme(colorSchemes[0]);

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

    if (strcmp(colorScheme, "Dark") == 0) {
        ImGui::StyleColorsDark();
    }
    else if (strcmp(colorScheme, "Light") == 0) {
        ImGui::StyleColorsLight();
    }
    else if (strcmp(colorScheme, "Classic") == 0) {
        ImGui::StyleColorsClassic();
    }
}