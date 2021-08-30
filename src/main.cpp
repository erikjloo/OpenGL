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
#include "VertexArray.h" // includes VertexBuffer and VertexBufferLayout
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"


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
		/* Define vertex positions (and texture coords) */
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f, // 0
			 50.0f, -50.0f, 1.0f, 0.0f,  // 1
			 50.0f,  50.0f, 1.0f, 1.0f,	  // 2
			-50.0f,  50.0f, 0.0f, 1.0f  // 3
		};

		uint indices[] = {
			0, 1, 2,
			2, 3, 0};

		/* Show alpha channels correctly */
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Define vertex array (abstracts glGenVertexArrays, glBindVertexArrays) */
		VertexArray va;

		/* Define vertices (abstracts glGenBuffer, glBindBuffer, glBufferData) */
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		/* Define buffer layout (abstracts glEnableVertexAttribArray, glVertexAttribPointer) */
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		/* Define elements (abstracts glGenBuffer, glBindBuffer, glBufferData) */
		IndexBuffer ib(indices, 6);

		/* MVP matrix = Model matrix * view matrix * projection matrix */
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		/* Create identity matrix and translate it by 100 to the left */
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		/* Create shader */
		Shader shader{"res/shaders/Basic.shader"};
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		/* Load texture */
		Texture texture{"res/textures/icon.png"};
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		/* Unbind all */
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();
		Renderer renderer;

		/* Setup Dear ImGui context */
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		/* Setup Platform/Renderer backends */
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		glm::vec3 translationA{200, 200, 0};
		glm::vec3 translationB{400, 200, 0};

		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			/* Start the Dear ImGui frame */
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			/* Rebind vertex array, index buffer, shader and draw */
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}

			/* Swap front and back buffers */
			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			{
				ImGui::SliderFloat3("TranslationA", &translationA.x, 0.0f, 960.0f);
				ImGui::SliderFloat3("TranslationB", &translationB.x, 0.0f, 960.0f);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}