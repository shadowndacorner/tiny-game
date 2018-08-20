#pragma once

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
#endif

//#include <GLFW/glfw3.h>
#include <SDL.h>
#include <dear-imgui/imgui.h>

#define WINDOW_TYPE SDL_Window*
struct WindowsPlatformShared
{
	WINDOW_TYPE window;
	bool game_running;
	ImGuiContext* imgui_context;
};

extern WindowsPlatformShared* platform_data;