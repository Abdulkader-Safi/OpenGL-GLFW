#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
GLFWwindow *window;

// Vertex Shader
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
// Fragment Shader source code
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
                                   "}\n\0";

int main()
{
  // glfw: initialize and configure
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Triangle vertices
  GLfloat vertices[] = {
      -0.5f, -0.5f, float(sqrt(3)) / 3, 0.0f, // Lower left Corner
      0.5f, -0.5f, float(sqrt(3)) / 3, 0.0f,  // Lower right Corner
      0.0f, 0.0f, float(sqrt(3)) * 2, 0.0f,   // Top Corner
  };

  // glfw window creation
  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Specify the viewport of OpenGL in the window
  // In this case the view port goes from x = 0, y = 0, to x = 800, y = 600
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  // Source and compile Vertex Shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Source and compile Fragment Shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Link the Vertex and Fragment shader into a shader program
  GLuint shaderProgram;
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Create reference containers for the Vertex Array Object (VAO) and the Vertex Buffer Object (VBO)
  GLuint VAO, VBO;

  // Generate the VAO and VBO with only 1 object each
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // Make the VAO the current vertex array object by binding it
  glBindVertexArray(VAO);

  // Bind the VBO specifying it's a GL_ARRAY_BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Introduce the vertices into the VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Configure the vertex attributes so that OpenGL knows how to read the VBO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  // Enable Vertex Attribute so that OpenGL knows to use it
  glEnableVertexAttribArray(0);

  // Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Specify the color of the background
  glClearColor(.07f, .13f, .17f, 1.0f);

  // Clean the back buffer and assign the front buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // Swap the back buffer with the front buffer
  glfwSwapBuffers(window);

  // render loop
  while (!glfwWindowShouldClose(window))
  {
    glClearColor(.07f, .13f, .17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Tell OpenGL which Shader Program we want to use
    glUseProgram(shaderProgram);

    // Bind the VAO so OpenGL knows to use it
    glBindVertexArray(VAO);

    // Draw the triangle using the GL_TRIANGLES primitive
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);

    // Take care of all GLFW events
    glfwPollEvents();
  }

  // De-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
