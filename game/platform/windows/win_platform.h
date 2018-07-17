#pragma once

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <GLFW/glfw3.h>

#define WINDOW_TYPE GLFWwindow*
struct WindowsPlatformShared
{
	WINDOW_TYPE window;
};

extern WindowsPlatformShared* platform_data;