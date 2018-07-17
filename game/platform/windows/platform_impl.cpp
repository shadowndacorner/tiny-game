#include <platform.hpp>
#include "win_platform.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

void tiny::Platform::set_window_title(const char * title)
{
	// TODO
	glfwSetWindowTitle(platform_data->window, title);
}

void tiny::Platform::show_error_message(const char * title, const char * message)
{
	// TODO
	auto hwnd = glfwGetWin32Window(platform_data->window);
	MessageBox(hwnd, message, title, MB_OK | MB_ICONERROR);
}
