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
#include <iostream>
#include <fstream> // file reader
#include <string>
#include <sstream> // string stream

#include "Renderer.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string &filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return {ss[0].str(), ss[1].str()};
}

static uint CompileShader(uint type, const std::string &source)
{
	GLCall(uint id = glCreateShader(type));
	const char *src = source.c_str(); // pointer to string start (same as &source[0])
	// pointer is hexadecimal integer holding address.
	GLCall(glShaderSource(id, 1, &src, nullptr)); // 1 source code, pointer to pointer
	// nullptr can be used if the string is null terminated.
	GLCall(glCompileShader(id));

	// Error handling
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); // iv = integer 'vector' i.e. array pointer
	if (!result)								   // Same as (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		// Use alloca to allocate memory on the stack (locally)
		// Heap allocation requires deletion further down !!!
		char *message = (char *)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		// Print error message to console
		std::string shaderType = (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		std::cout << "Failed to compile " << shaderType << " shader." << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
    }
	return id;
}

/*
 * vertexShader and fragmentShader are strings
 * containing the shaders' source code 
 */
static uint CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
	GLCall(uint program = glCreateProgram());
	uint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	uint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Think of this as compiling a C++ program
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	// No need to call glDetachShader (Minimum performance gain & helps debugging)

	// Delete intermediate shaders (Already linked to program)
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

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

	/* Define vertex positions */
	float positions[] = {
		-0.5f, -0.5f, // 0
		0.5f, -0.5f,  // 1
		0.5f, 0.5f,   // 2
        -0.5f, 0.5f   // 3
        };

    uint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

	/* Define Vertex array */
	uint vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	/* Define Vertex buffer - aka nodes */
	uint buffer;					        // vertex buffer ID
	GLCall(glGenBuffers(1, &buffer));				// Generate vertex buffer ID
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer)); // Assign buffer type Array
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));
	// glBufferData(type, size in bytes, data, hint)
    GLCall(glEnableVertexAttribArray(0)); // 0 is the index
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
    // glVertexAttribPointer(index, size, type, normalized, stride, pointer)

    /* Define Index buffer - aka node indices */
    uint ibo;                                   // index buffer ID
    GLCall(glGenBuffers(1, &ibo));                      // Generate index buffer ID
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); // Assign buffer type Array
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint), indices, GL_STATIC_DRAW));
    // glBufferData(type, size in bytes, data, hint)

	/* Create shader */
	ShaderProgramSource source = ParseShader("Basic.shader");
	uint shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

	/* Unbind all */
	GLCall(glUseProgram(0));
	GLCall(glBindVertexArray(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT)); // Clear before drawing

		/* Rebind vao and ibo - no need to rebind buffer */
		GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
		GLCall(glBindVertexArray(vao));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

        // glDrawArrays(GL_TRIANGLES, 0, 3); // Start from 0th vertex, 3 is number of vertices
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // 6 is number of indices

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

	GLCall(glDeleteProgram(shader));

	glfwTerminate();
	return 0;
}