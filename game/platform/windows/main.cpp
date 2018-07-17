#include "../opengl/glad.h"
#include <game_core.hpp>
#include <platform.hpp>
#include "win_platform.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#undef main

WindowsPlatformShared* platform_data;
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main(int argc, char** argv)
{
	platform_data = new WindowsPlatformShared;

	tiny::Platform win_platform;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	auto window = glfwCreateWindow(1280, 720, "TinyGame", NULL, NULL);

	platform_data->window = window;

	glfwSetKeyCallback(window, key_callback);
	
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	tiny::Game game(&win_platform);
	
	double gameTime = glfwGetTime();
	double deltaTime = 0;
	char* name = new char[512];
	while (!glfwWindowShouldClose(window))
	{
		auto old = gameTime;
		gameTime = glfwGetTime();
		deltaTime = gameTime - old;

		memset(name, 0, sizeof(char) * 512);
		sprintf(name, "TinyGame (%f seconds since startup, %f fps)", float(gameTime), 1.0f / static_cast<float>(deltaTime));

		win_platform.set_window_title(name);

		glfwPollEvents();
		
		// game code
		game.update(gameTime, deltaTime);

		glClearColor(1, 0, 0, 1);
		// clear
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		game.render(gameTime, deltaTime);

		glfwSwapBuffers(window);
	}
	delete name;
	glfwTerminate();

	delete platform_data;
	return 0;
}