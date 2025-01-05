#include "shaderClass.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <glad/glad.h>

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::binary);
  if (in)
  {
    std::stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
  }
  else
  {
    throw std::runtime_error("Failed to open file: " + std::string(filename));
  }
}

// Constructor that builds the Shader Program from 2 different shaders
Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
  // Read vertexFile and fragmentFile and store the strings
  std::string vertexCode = get_file_contents(vertexFile);
  std::string fragmentCode = get_file_contents(fragmentFile);

  // Convert the shader source strings into character arrays
  const char *vertexSource = vertexCode.c_str();
  const char *fragmentSource = fragmentCode.c_str();

  // Create Vertex Shader Object and get its reference
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, nullptr);
  glCompileShader(vertexShader);
  checkCompileErrors(vertexShader, "VERTEX");

  // Create Fragment Shader Object and get its reference
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
  glCompileShader(fragmentShader);
  checkCompileErrors(fragmentShader, "FRAGMENT");

  // Create Shader Program Object and get its reference
  ID = glCreateProgram();
  // Attach the Vertex and Fragment Shaders to the Shader Program
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  // Link all the shaders together into the Shader Program
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");

  // Delete the now useless Vertex and Fragment Shader objects
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

// Activates the Shader Program
void Shader::Activate()
{
  glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
  glDeleteProgram(ID);
}

// Checks for compilation and linking errors
void Shader::checkCompileErrors(GLuint shader, const std::string &type)
{
  GLint success;
  GLchar infoLog[1024];

  if (type != "PROGRAM")
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
      std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
      std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
}