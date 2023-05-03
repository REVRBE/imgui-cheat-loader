#pragma once
#include <d3d9.h>
#include "../imgui/imgui.h"

namespace ui {
	void init(LPDIRECT3DDEVICE9);
	void renderSecondPrompt();
	void render();
	void applyColorScheme(const char* colorScheme);

	enum class ColorScheme {
		Dark,
		Light,
		Classic
	};
}

namespace ui {
	inline LPDIRECT3DDEVICE9 dev;
	inline const char* window_title = "smokescreen.gg";
	inline const char* colorSchemes[] = { "Standard", "Cinder", "Green colorway", "Blue colorway", "Purple/orange colorway", "Dark mode", "Pink/green/blue colorway", "Blue and grey colorway", "Boring colorway", "Blue/green colorway", "Green/yellow colorway", "Orange/grey colorway", "Whitey", "Colorful"};
}

namespace ui {
	inline ImVec2 screen_res{ 000, 000 };
	inline ImVec2 window_pos{ 0, 0 };
	inline ImVec2 window_size{ 300, 125 };
	inline DWORD  window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
}
