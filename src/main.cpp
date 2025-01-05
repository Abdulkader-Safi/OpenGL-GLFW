#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// Vertices coordinates
GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  //
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   //
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,  //
};

// Indices for vertices order
GLuint indices[] = {
    0, 2, 1, //
    0, 3, 2, //
};

int main()
{
  // Initialize GLFW
  glfwInit();

  // Tell GLFW what version of OpenGL we are using
  // In this case we are using OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Tell GLFW we are using the CORE profile
  // So that means we only have the modern functions
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
  GLFWwindow *window = glfwCreateWindow(800, 800, "LearnOpenGL-GLFW", NULL, NULL);
  // Error check if the window fails to create
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  // Introduce the window into the current context
  glfwMakeContextCurrent(window);

  // Load GLAD so it configures OpenGL
  gladLoadGL();
  // Specify the viewport of OpenGL in the Window
  // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
  int framebufferWidth, framebufferHeight;
  glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
  framebuffer_size_callback(window, framebufferWidth, framebufferHeight);

  // Set the callback for when the window resizes
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Call it once to set the viewport to the correct size

  // Generates Shader object using shaders default.vert and default.frag
  Shader shaderProgram("res/shaders/default.vert", "res/shaders/default.frag");

  // Generates Vertex Array Object and binds it
  VAO VAO1;
  VAO1.Bind();

  // Generates Vertex Buffer Object and links it to vertices
  VBO VBO1(vertices, sizeof(vertices));
  // Generates Element Buffer Object and links it to indices
  EBO EBO1(indices, sizeof(indices));

  // Links VBO to VAO
  VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *)0);
  VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  // Unbind all to prevent accidentally modifying them
  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

  GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

  // Texture
  int widthImg, heightImg, numColCh;
  unsigned char *data = stbi_load("res/images/img1.jpg", &widthImg, &heightImg, &numColCh, 0);

  // Main while loop
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // Specify the color of the background
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clean the back buffer and assign the new color to it
    glClear(GL_COLOR_BUFFER_BIT);
    // Tell OpenGL which Shader Program we want to use
    shaderProgram.Activate();

    glUniform1f(uniID, 0.5f);

    // Bind the VAO so OpenGL knows to use it
    VAO1.Bind();
    // Draw primitives, number of indices, datatype of indices, index of indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);
    // Take care of all GLFW events
    glfwPollEvents();
  }

  // Delete all the objects we've created
  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  shaderProgram.Delete();
  // Delete window before ending the program
  glfwDestroyWindow(window);
  // Terminate GLFW before ending the program
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}