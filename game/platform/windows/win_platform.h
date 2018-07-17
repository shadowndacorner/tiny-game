#pragma once

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

struct WindowsPlatformShared
{
	HWND hWnd;
};

extern WindowsPlatformShared* platform_data;