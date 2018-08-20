#include <platform.hpp>
#include "sdl_platform.h"
#include <SDL.h>
#include <SDL_syswm.h>

void tiny::Platform::set_window_title(const char * title)
{
	// TODO
	SDL_SetWindowTitle(platform_data->window, title);
}

#ifdef __WIN32__
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
void tiny::Platform::show_error_message(const char * title, const char * message)
{
	SDL_SysWMinfo wmInfo;
	SDL_GetWindowWMInfo(platform_data->window, &wmInfo);
	HWND hwnd = wmInfo.info.win.window;
	MessageBox(hwnd, message, title, MB_OK | MB_ICONERROR);
}
#endif