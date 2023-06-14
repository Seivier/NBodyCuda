#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>



int simulate(int n, float ratio, float gravity)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Tarea 3: N Body Problem", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor( 0.4f, 0.3f, 0.4f, 0.0f );

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		std::cout << "Usage: " << argv[0] << " <n> <ratio> <gravity>" << std::endl;
		return -1;
	}
	int n = std::stoi(argv[1]);
	float ratio = std::stof(argv[2]);
	float gravity = std::stof(argv[3]);
	return simulate(n, ratio, gravity);
}