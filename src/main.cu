// Based on Imgui example for GLFW + OpenGL3
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
#include <vector>

#include "shader.h"
#include "buffer.h"


#define GRAVITY 6.67408e-6f
#define SOFTENING 1e-3f

#define BLOCK_SIZE 512

static void glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

static void simulate(std::vector<Point>& points, float dt)
{
	using namespace glm;

	for (auto& p1 : points)
	{
		auto acceleration = vec3(0.0f);
		for (auto& p2 : points)
		{
			if (&p1 == &p2)
				continue;

			auto r = p2.position - p1.position;
			auto distSqr = dot(r, r);
			auto force = GRAVITY * p2.mass / (distSqr + SOFTENING);


			acceleration += force * normalize(r);
		}

		p1.velocity += acceleration * dt;
	}


}

static void update(std::vector<Point>& points, float dt)
{
	for (auto& p : points)
	{
		p.position += p.velocity * dt;
	}
}

__global__ void simulate_cuda(float* points, float dt, int n)
{
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if (idx >= n)
		return;
	float p_x = points[idx * 7];
	float p_y = points[idx * 7 + 1];
	float p_z = points[idx * 7 + 2];


	float a_x = 0.0f;
	float a_y = 0.0f;
	float a_z = 0.0f;

	for (int i = 0; i < n; i++)
	{
		if (i == idx)
			continue;


		float r[3] = { points[i * 7] - p_x, points[i * 7 + 1] - p_y, points[i * 7 + 2] - p_z };
		float distSqr = r[0] * r[0] + r[1] * r[1] + r[2] * r[2];
		float dist = sqrtf(distSqr);
		float force = GRAVITY * points[7*i + 6] / (distSqr + SOFTENING);

		float r_unit[3] = { r[0] / dist, r[1] / dist, r[2] / dist };

		a_x += force * r_unit[0];
		a_y += force * r_unit[1];
		a_z += force * r_unit[2];
	}

	points[idx*7 + 3] += a_x * dt;
	points[idx*7 + 4] += a_y * dt;
	points[idx*7 + 5] += a_z * dt;

}

__global__ void update_cuda(float* points, float dt, int n)
{
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if (idx >= n)
		return;
	points[idx * 7] += points[idx * 7 + 3] * dt;
	points[idx * 7 + 1] += points[idx * 7 + 4] * dt;
	points[idx * 7 + 2] += points[idx * 7 + 5] * dt;
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
	GLFWwindow* window = glfwCreateWindow(800, 800, "Tarea 3", nullptr, nullptr);
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
	bool only_points = true;
	bool pause = true;
	bool use_gpu = false;

	float mass = 30.0f;
	float ratio = 0.1f;
	int n = 100;
	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

	// Create a shader program
	Shader shader("shaders/nbody.vert", "shaders/nbody.geom", "shaders/nbody.frag");
	Shader shader_points("shaders/pnbody.vert", "shaders/nbody.frag");
	Buffer buffer;
	buffer.setLayout(shader);
	buffer.setLayout(shader_points);

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
		{
			if (use_gpu)
			{
				buffer.map();
				simulate_cuda << < (n + BLOCK_SIZE - 1) / BLOCK_SIZE, BLOCK_SIZE >> > (buffer.getCudaPoints(), delta, n);
				update_cuda << < (n + BLOCK_SIZE - 1) / BLOCK_SIZE, BLOCK_SIZE >> > (buffer.getCudaPoints(), delta, n);
				buffer.unmap();
			}
			else
			{
				simulate(buffer.data, delta);
				update(buffer.data, delta);
			}

		}



		glfwPollEvents();
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			ImGui::Begin("Simulation options");                          // Create a window called "Hello, world!" and append into it.

//			ImGui::Checkbox("Pause", &pause);      // Edit bools storing our window open/close state

			ImGui::Checkbox("Only points?", &only_points);      // Edit bools storing our window open/close state
			ImGui::SameLine();
			ImGui::Checkbox("View quads?", &use_quads);      // Edit bools storing our window open/close state
			ImGui::SameLine();
			ImGui::Checkbox("Use GPU?", &use_gpu);      // Edit bools storing our window open/close state

			ImGui::SliderInt("Number of points", &n, 0, 5000);
			// Vec2 input
			ImGui::SliderFloat("Special mass", &mass, 1.0f, 500.0f, "%.3f");

			// ImGui::ColorEdit("Special Gravity", &acceleration, 0.1f, 1.0f, "%.3f");
			ImGui::SliderFloat("SM Ratio",
				&ratio,
				0.0f,
				1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("Clear color", (float*)&clear_color); // Edit 3 floats representing a color

			const std::string& label = pause ? "Play" : "Pause";
			if (ImGui::Button(label.c_str()))
				pause = !pause;

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

		if (only_points)
		{
			shader_points.bind();
			buffer.bind();

			glDrawElements(GL_POINTS, n, GL_UNSIGNED_INT, 0);

			buffer.unbind();
			shader_points.unbind();
		}
		else
		{
			shader.bind();
			shader.setUniform1i("useQuads", use_quads);
			buffer.bind();

			glDrawElements(GL_POINTS, n, GL_UNSIGNED_INT, 0);

			buffer.unbind();
			shader.unbind();
		}



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