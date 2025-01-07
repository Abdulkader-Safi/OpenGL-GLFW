#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const char *image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
  type = texType;
  // Stores the width, height, and the number of color channels of the image
  int widthImg, heightImg, numColCh;

  // Flips the image so it appears right side up
  stbi_set_flip_vertically_on_load(true);

  // Reads the image from a file and stores it in bytes
  unsigned char *bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

  std::cout << "Loaded image: " << image << " (" << widthImg << "x" << heightImg << ", " << numColCh << " channels)" << std::endl;

  if (!bytes)
  {
    std::cerr << "Error: Failed to load texture: " << image << std::endl;
    exit(EXIT_FAILURE);
  }

  if (widthImg <= 0 || heightImg <= 0 || numColCh <= 0)
  {
    std::cerr << "Error: Invalid texture dimensions for " << image << ": "
              << widthImg << "x" << heightImg << " with " << numColCh << " channels." << std::endl;
    stbi_image_free(bytes);
    exit(EXIT_FAILURE);
  }

  // Generates an OpenGL texture object
  glGenTextures(1, &ID);
  if (glGetError() != GL_NO_ERROR)
  {
    std::cerr << "Error: Failed to generate texture object for " << image << std::endl;
    stbi_image_free(bytes);
    exit(EXIT_FAILURE);
  }

  std::cout << "Binding texture to slot: " << slot << std::endl;

  if (slot < GL_TEXTURE0 || slot > GL_TEXTURE31)
  {
    std::cerr << "Error: Invalid texture slot " << slot << ". Must be between GL_TEXTURE0 and GL_TEXTURE31." << std::endl;
    exit(EXIT_FAILURE);
  }
  // Assigns the texture to a Texture Unit
  glActiveTexture(slot);
  if (glGetError() != GL_NO_ERROR)
  {
    std::cerr << "Error: Failed to activate texture slot for " << image << std::endl;
    stbi_image_free(bytes);
    exit(EXIT_FAILURE);
  }

  glBindTexture(texType, ID);
  if (glGetError() != GL_NO_ERROR)
  {
    std::cerr << "Error: Failed to bind texture for " << image << std::endl;
    stbi_image_free(bytes);
    exit(EXIT_FAILURE);
  }

  // Configures the type of algorithm that is used to make the image smaller or bigger
  glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  if (glGetError() != GL_NO_ERROR)
  {
    std::cerr << "Error: Failed to set texture parameters for " << image << std::endl;
    stbi_image_free(bytes);
    exit(EXIT_FAILURE);
  }

  // Configures the way the texture repeats (if it does at all)
  glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
  if (glGetError() != GL_NO_ERROR)
  {
    std::cerr << "Error: Failed to configure texture wrapping for " << image << std::endl;
    stbi_image_free(bytes);
    exit(EXIT_FAILURE);
  }

  // Assigns the image to the OpenGL Texture object
  GLenum internalFormat = (numColCh == 4) ? GL_RGBA : GL_RGB;
  GLenum imageFormat = (numColCh == 4) ? GL_RGBA : GL_RGB;

  glTexImage2D(texType, 0, internalFormat, widthImg, heightImg, 0, imageFormat, pixelType, bytes);
  if (glGetError() != GL_NO_ERROR)
  {
    std::cerr << "Error: Failed to upload texture data for " << image << std::endl;
    stbi_image_free(bytes);
    exit(EXIT_FAILURE);
  }

  // Generates MipMaps
  glGenerateMipmap(texType);
  if (glGetError() != GL_NO_ERROR)
  {
    std::cerr << "Error: Failed to generate mipmaps for " << image << std::endl;
    stbi_image_free(bytes);
    exit(EXIT_FAILURE);
  }

  // Deletes the image data as it is already in the OpenGL Texture object
  stbi_image_free(bytes);

  // Unbinds the OpenGL Texture object so that it can't accidentally be modified
  glBindTexture(texType, 0);
  if (glGetError() != GL_NO_ERROR)
  {
    std::cerr << "Error: Failed to unbind texture for " << image << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Texture::texUnit(Shader &shader, const char *uniform, GLuint unit)
{
  // Gets the location of the uniform
  GLuint texUni = glGetUniformLocation(shader.ID, uniform);
  if (texUni == -1)
  {
    std::cerr << "Error: Uniform " << uniform << " not found in shader program." << std::endl;
    exit(EXIT_FAILURE);
  }

  // Shader needs to be activated before changing the value of a uniform
  shader.Activate();
  glUniform1i(texUni, unit);
  if (glGetError() != GL_NO_ERROR)
  {
    std::cerr << "Error: Failed to set uniform " << uniform << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Texture::Bind()
{
  glBindTexture(type, ID);
  if (glGetError() != GL_NO_ERROR)
  {
    std::cerr << "Error: Failed to bind texture with ID " << ID << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Texture::Unbind()
{
  glBindTexture(type, 0);
  if (glGetError() != GL_NO_ERROR)
  {
    std::cerr << "Error: Failed to unbind texture with ID " << ID << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Texture::Delete()
{
  glDeleteTextures(1, &ID);
  if (glGetError() != GL_NO_ERROR)
  {
    std::cerr << "Error: Failed to delete texture with ID " << ID << std::endl;
    exit(EXIT_FAILURE);
  }
}