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

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

int main(void)
{
	GLFWwindow *window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World.", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window." << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW." << std::endl;
		glfwTerminate();
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		/* Define vertex positions (and texture coords) */
		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, // 0
			0.5f, -0.5f, 1.0f, 0.0f,  // 1
			0.5f, 0.5f, 1.0f, 1.0f,	  // 2
			-0.5f, 0.5f, 0.0f, 1.0f  // 3
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
		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			/* Rebind vertex array, index buffer, and shader - no need to rebind vertex buffer */
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			/* Bind and draw */
			renderer.Draw(va, ib, shader);

			/* Swap front and back buffers */
			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}