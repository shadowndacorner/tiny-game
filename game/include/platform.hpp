#pragma once

namespace tiny
{
	// Platform layer class
	// This should return pointers to any platform-specific
	// interfaces, such as graphics, sound, window, etc.
	class Platform
	{
	public:
		void set_window_title(const char* title);
		void show_error_message(const char* title, const char* message);
		void* load_dll(const char* dll_path);
	};
}