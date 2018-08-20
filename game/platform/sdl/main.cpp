#include <game_core.hpp>
#include <platform.hpp>
#include "sdl_platform.h"
#include <stdio.h>
#include <stb_image.h>
#include <chrono>

#include <SDL.h>
#include <SDL_syswm.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "../bgfx/bgfx/imgui/imgui.h"

#include "imgui_impl_sdl.h"

#undef main

inline bool sdlSetWindow(SDL_Window* _window)
{
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	if (!SDL_GetWindowWMInfo(_window, &wmi)) {
		return false;
	}

	bgfx::PlatformData pd;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	pd.ndt = wmi.info.x11.display;
	pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_OSX
	pd.ndt = NULL;
	pd.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
	pd.ndt = NULL;
	pd.nwh = wmi.info.win.window;
#elif BX_PLATFORM_STEAMLINK
	pd.ndt = wmi.info.vivante.display;
	pd.nwh = wmi.info.vivante.window;
#endif // BX_PLATFORM_
	pd.context = NULL;
	pd.backBuffer = NULL;
	pd.backBufferDS = NULL;
	bgfx::setPlatformData(pd);

	return true;
}

namespace chrono = std::chrono;
typedef chrono::high_resolution_clock hr_clock;
typedef chrono::duration<double, std::milli> my_seconds;

WindowsPlatformShared* platform_data;

inline double calculate_time(hr_clock::time_point reference)
{
	return (chrono::duration_cast<my_seconds>(hr_clock::now() - reference)).count() / 1000.0;
}

#include <fstream>

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	auto window = SDL_CreateWindow("TinyGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
	sdlSetWindow(window);


	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	bgfx::Init init;
	init.resolution.width = width;
	init.resolution.height = height;
	init.resolution.reset = BGFX_RESET_VSYNC;
	bgfx::init(init);
	
	bgfx::setDebug(BGFX_DEBUG_STATS);
	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0
		, 1.0f
		, 0
	);

	platform_data = new WindowsPlatformShared;
	platform_data->imgui_context = ImGui::CreateContext();
	ImGui_ImplSDL2_Init(window);

	tiny::Platform win_platform;
	platform_data->window = window;
	platform_data->game_running = true;

	tiny::Game game(&win_platform);
	imguiCreate();

	auto start_time = hr_clock::now();
	double gameTime = calculate_time(start_time);
	double deltaTime = 0;
	char* name = new char[512];

	while (platform_data->game_running)
	{
		auto old = gameTime;
		gameTime = calculate_time(start_time);
		deltaTime = gameTime - old;

		memset(name, 0, sizeof(char) * 512);
		sprintf(name, "TinyGame (%f seconds since startup, %f fps)", float(gameTime), 1.0f / static_cast<float>(deltaTime));

		win_platform.set_window_title(name);

		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
		ImGui_ImplSDL2_NewFrame(window);
		
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			ImGui_ImplSDL2_ProcessEvent(&ev);
			
			// TODO
			switch (ev.type) {
			case SDL_WINDOWEVENT: {
				const SDL_WindowEvent& wev = ev.window;
				switch (wev.event) {
				case SDL_WINDOWEVENT_RESIZED:
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					SDL_GetWindowSize(window, &width, &height);
					bgfx::reset(width, height, BGFX_RESET_VSYNC);
					break;

				case SDL_WINDOWEVENT_CLOSE:
					platform_data->game_running = false;
					break;
				}
			} break;
			}
		}

		if (!platform_data->game_running)
			break;
		
		// game code
		game.update(gameTime, deltaTime);

		// No more UI rendering
		imguiEndFrame();

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));

		// Clear framebuffer
		bgfx::touch(0);
		game.render(gameTime, deltaTime);
		bgfx::frame();
	}

	delete name;
	imguiDestroy();
	bgfx::shutdown();

	while (bgfx::RenderFrame::NoContext != bgfx::renderFrame()) {}

	SDL_DestroyWindow(window);
	SDL_Quit();

	delete platform_data;
	return 0;
}