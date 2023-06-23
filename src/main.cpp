// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs
#define GL_SILENCE_DEPRECATION
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <iostream>

#include "shader.h"
#include "buffer.h"


static void glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

static void simulate(std::vector<Point>& points, float dt)
{
	using namespace glm;
	// NBody gravity simulation
	constexpr float G = 6.67408e-6f;
	constexpr float softening = 1e-3f;


	for (auto& p1 : points)
	{
		auto acceleration = vec3(0.0f);
		for (auto& p2 : points)
		{
			if (&p1 == &p2)
				continue;

			auto r = p2.position - p1.position;
			auto distSqr = dot(r, r);
			auto force = G * p2.mass / (distSqr + softening);


			acceleration += force * normalize(r);
		}

		p1.velocity += acceleration * dt;
		p1.position += p1.velocity * dt;
	}


}


// Main code
int main(int, char**)
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(800, 800, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
	if (window == nullptr)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Our state
	bool use_quads = false;
	bool pause = true;
	float mass = 30.0f;
	float ratio = 0.1f;
	int n = 100;
	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

	// Create a shader program
	Shader shader("shaders/nbody.vert", "shaders/nbody.geom", "shaders/nbody.frag");
	Buffer buffer;
	buffer.setLayout(shader);

	auto now = static_cast<float>(glfwGetTime());
	auto last = now;
	float delta;
	while (!glfwWindowShouldClose(window))
	{
		// simulation
		now = static_cast<float>(glfwGetTime());
		delta = now - last;
		last = now;
		if (!pause)
			simulate(buffer.data, delta);


		glfwPollEvents();
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			ImGui::Begin("Simulation options");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Checkbox("Pause", &pause);      // Edit bools storing our window open/close state

			ImGui::Checkbox("View quads?", &use_quads);      // Edit bools storing our window open/close state

			ImGui::SliderInt("Number of points", &n, 0, 1000);
			// Vec2 input
			ImGui::SliderFloat("Special mass", &mass, 1.0f, 100.0f, "%.3f");

			// ImGui::ColorEdit("Special Gravity", &acceleration, 0.1f, 1.0f, "%.3f");
			ImGui::SliderFloat("SM Ratio",
				&ratio,
				0.01f,
				1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("Clear color", (float*)&clear_color); // Edit 3 floats representing a color

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}


		buffer.setSize(n);
		buffer.setSpecialMass(mass);
		buffer.setRatio(ratio);
		buffer.build();

		// Rendering
		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(clear_color.x * clear_color.w,
			clear_color.y * clear_color.w,
			clear_color.z * clear_color.w,
			clear_color.w);

		shader.bind();

		shader.setUniform1i("useQuads", use_quads);

		buffer.bind();

		glDrawElements(GL_POINTS, n, GL_UNSIGNED_INT, 0);

		buffer.unbind();
		shader.unbind();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}