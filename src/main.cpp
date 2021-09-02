/*
 * OpenGL is a state machine
 * Episode 4: Vertex Buffers (Nodes) and drawing a triangle in OpenGL
 * Episode 5: Vertex Attributes and Layouts in OpenGL
 * Episode 6: How Shaders Work in OpenGL (no new coding)
 * Episode 7: Writing a Shader in OpenGL (lots of pointers)
 * Episode 8: Shader parser function (fstream and sstream)
 * Episode 9: Index Buffers (Node indices) to define elements
 * Episode 10: How to debug
 * Episode 11: Uniforms set per draw / attributes set per vertex
 * Episode 12: Vertex Arrays
 * 
 * Rendering Pipeline:
 * 1) Create buffer to contain vertex info
 * 2) Draw call
 * 3) Use vertex shader to determine vertex positions in window
 * 4) Use fragment (pixel) shaders to paint (triangle) pixels
 *
 * Heap -> global variables (fragmented)
 * Stack -> local variables (continuous)
 * 
 * char* src = string.c_str() returns a pointer to start of string
 * char* buffer = new char[8] allocates 8 bytes of memory to heap
 * char* buffer = (char*) alloca(8) allocates memory to the stack
 * 
 * Rendering pipeline: data -> draw call -> vertex shader -> fragment shader 
 * Erik Giesen Loo, Aug 2021
 *
 */

#include <GL/glew.h> // glew needs to be imported BEFORE glfw3!!
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Texture.h"

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestSquare.h"
#include "tests/TestTexture2D.h"

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	const char *glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a window and OpenGL context */
	GLFWwindow *window = glfwCreateWindow(960, 540, "Hello World.", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		Renderer renderer;

		/* Setup Dear ImGui context */
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		/* Setup Platform/Renderer backends */
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::TestSquare>("Square");
		testMenu->RegisterTest<test::TestTexture2D>("2D Texture");

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			renderer.Clear();

			/* Start the Dear ImGui frame */
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (currentTest)
			{
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("<-"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}
			
			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		/* Cleanup */
		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
		currentTest = nullptr;
	}

	/* Cleanup */
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}