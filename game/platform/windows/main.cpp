#include <game_core.hpp>
#include <stdio.h>
#include <GLFW/glfw3.h>

#undef main

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main(int argc, char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	auto window = glfwCreateWindow(1280, 720, "TinyGame", NULL, NULL);

	glfwSetKeyCallback(window, key_callback);
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		// game code

		// clear
		// rendering code
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}