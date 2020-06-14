/*
 *
 * Episode 4: Vertex Buffers and drawing a triangle in OpenGL
 * Episode 5: Vertex Attributes and Layouts in OpenGL
 * Episode 6: How Shaders Work in OpenGL (no new coding)
 * Episode 7: Writing a Shader in OpenGL (lots of pointers)
 *
 * Rendering Pipeline:
 * 1) Create buffer to contain vertex info
 * 2) Draw call
 * 3) Use vertex shader to determine vertex positions in window
 * 4) Use fragment (pixel) shaders to paint (triangle) pixels
 *
 * unsigned int are whole/natural numbers (without sign)
 * Heap -> global variables (fragmented)
 * Stack -> local variabled (continuous)
 * 
 * char* src = string.c_str() returns a pointer to start of string
 * char* buffer = new char[8] allocates 8 bytes of memory to heap
 * char* buffer = (char*) alloca(8) allocates memory to the stack
 * 
 * Rendering pipeline: data -> draw call -> vertex shader -> fragment shader 
 * Erik Giesen Loo, June 2020
 *
 */

#include <GL/glew.h> // glew needs to be imported BEFORE glfw3!!
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int CompileShader(unsigned int type,
                                  const std::string &source)
{
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str(); // pointer to string start
  // pointer is hexadecimal integer holding address.
  // nullptr (NULL pointer) is an invalid pointer address.
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  // Error handling
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (!result) // Same as (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    // Use alloca to allocate memory on the stack (locally)
    // Heap allocation requires deletion further down !!!
    char* message = (char*)alloca(length*sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::string shaderType = (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
    std::cout << "Failed to compile" << shaderType << "shader." << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }
  return id;
}

/*
 * vertexShader and fragmentShader are strings
 * containing the shaders' source code 
 */
static unsigned int CreateShader(const std::string &vertexShader,
                                 const std::string &fragmentShader)
{
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);
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
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW." << std::endl;
    glfwTerminate();
    return -1;
  }

  std::cout << glGetString(GL_VERSION) << std::endl;

  /* Define vertex positions */
  float positions[6] = {
      -0.5f, -0.5f,
      0.0f, 0.5f,
      0.5f, -0.5f};

  /* Define Vertex buffer */
  unsigned int hBuffer;                   // buffer ID
  glGenBuffers(1, &hBuffer);              // Generate buffer ID
  glBindBuffer(GL_ARRAY_BUFFER, hBuffer); // Assign buffer type Array
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  std::string vertexShader =
      "#version 330 core\n"
      "\n"
      "layout(location = 0) in vec4 position;\n" // 0 should match with 0 in glVertexAttribPointer
      "\n"
      "void main()\n"
      "{\n"
      "   gl_Position = position;\n"
      "}\n";

  std::string fragmentShader =
      "#version 330 core\n"
      "\n"
      "layout(location = 0) out vec4 color;\n" // 0 should match with 0 in glVertexAttribPointer
      "\n"
      "void main()\n"
      "{\n"
      "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
      "}\n";

  unsigned int shader = CreateShader(vertexShader, fragmentShader);
  glUseProgram(shader);
  // std::cout << &hBuffer << " " << GL_ARRAY_BUFFER << std::endl;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT); // Clear before drawing

    glDrawArrays(GL_TRIANGLES, 0, 3); // Start from 0th vertex
    // glDrawElements(GL_TRIANGLES, 3, NULL);
    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}