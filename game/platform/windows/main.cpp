#include <game_core.hpp>
#include "win_platform.h"
#include <tchar.h>
#include <stdio.h>

// Define these here to minimize typos, or preferably, load them from a
// resource file at the top of the main function
#define MYCLASSNAME    TEXT("MainWndClass")
#define MYWINDOWNAME   TEXT("Main Window")

// Global variable to keep track of your hInstance
HINSTANCE g_hInstance;

static bool shouldDestroy = false;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	printf("Received message %d\n", uMsg);
	switch (uMsg)
	{
	case WM_CLOSE:
		printf("CLOSE ME!\n");
		shouldDestroy = true;
		break;
	}

	// If you don't process any of the messages yourself, you
	// must pass them to DefWindowProc for default handling.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)
{
	// Save the instance handle in a global variable.
	g_hInstance = hInstance;

	// Register your window class.
	// (A full-featured app will probably want to set additional members.)
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(wcex);
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = MYCLASSNAME;
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, TEXT("Call to RegisterClassEx failed!"), NULL, MB_OK);
		return 1;
	}

	// Create your main window.
	HWND hwndMain = CreateWindowEx(0, MYCLASSNAME, MYWINDOWNAME, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL,
		hInstance, NULL);
	if (!hwndMain)
	{
		MessageBox(NULL, TEXT("Call to CreateWindowEx failed!"), NULL, MB_OK);
		return 1;
	}

	// Show your main window.
	ShowWindow(hwndMain, nCmdShow);
	UpdateWindow(hwndMain);

	// Run the main message loop.
	BOOL bRetVal;
	MSG msg;
	while ((bRetVal = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRetVal == -1)
		{
			MessageBox(NULL, TEXT("Error encountered in message loop!"), NULL, MB_OK);
			return 1;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (shouldDestroy)
			break;
	}

	return (int)msg.wParam;
}

int main(int argc, char** argv)
{
	return WinMain(0, 0, GetCommandLine(), argc);
}