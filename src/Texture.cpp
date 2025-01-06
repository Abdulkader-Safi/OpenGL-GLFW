#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const char *image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
  type = texType;

  int widthImg, heightImg, numColCh;
  stbi_set_flip_vertically_on_load(true);

  unsigned char *bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
  if (!bytes)
  {
    std::cerr << "Failed to load texture: " << image << std::endl;
    return;
  }

  // Generates an OpenGL texture object
  glGenTextures(1, &ID);
  // Assigns the texture to a Texture Unit
  glActiveTexture(slot);
  glBindTexture(texType, ID);

  // Configures the type of algorithm that is used to make the image smaller or bigger
  glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Configures the way the texture repeats (if it does at all)
  glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Assigns the image to the OpenGL Texture object
  glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
  // Generates MipMaps
  glGenerateMipmap(texType);

  stbi_image_free(bytes);
  glBindTexture(texType, 0);
}

void Texture::texUnit(Shader &shader, const char *uniform, GLuint unit)
{
  // Gets the location of the uniform
  GLuint texUni = glGetUniformLocation(shader.ID, uniform);
  // Shader needs to be activated before changing the value of a uniform
  shader.Activate();
  // Sets the value of the uniform
  glUniform1i(texUni, unit);
}

void Texture::Bind()
{
  glBindTexture(type, ID);
}

void Texture::Unbind()
{
  glBindTexture(type, 0);
}

void Texture::Delete()
{
  glDeleteTextures(1, &ID);
}