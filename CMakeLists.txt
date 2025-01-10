cmake_minimum_required(VERSION 3.10)

# Project name and version
project(OpenGLProject VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------
# Add executable target
# ---------------------------------------------------------
add_executable(main
  src/main.cpp

  src/ShaderClass.cpp

  src/Texture.cpp

  src/VAO.cpp
  src/VBO.cpp
  src/EBO.cpp
)

# ---------------------------------------------------------
# Include Directories
# ---------------------------------------------------------
target_include_directories(main PRIVATE
  ${CMAKE_SOURCE_DIR}/include # Local headers
  ${CMAKE_SOURCE_DIR}/shaders # Local shaders
  ${CMAKE_SOURCE_DIR}/lib/glad/include # GLAD headers
  ${CMAKE_SOURCE_DIR}/lib/stb # stb_image headers
)

# ---------------------------------------------------------
# GLM Configuration (Using Homebrew)
# ---------------------------------------------------------
find_path(GLM_INCLUDE_DIR glm/glm.hpp PATHS /opt/homebrew/include /usr/local/include)

if(GLM_INCLUDE_DIR)
  message(STATUS "Found GLM at ${GLM_INCLUDE_DIR}")
  target_include_directories(main PRIVATE ${GLM_INCLUDE_DIR})
else()
  message(FATAL_ERROR "GLM not found. Install it via Homebrew using 'brew install glm'.")
endif()

# ---------------------------------------------------------
# Copy Resource Files
# ---------------------------------------------------------
file(COPY ${CMAKE_SOURCE_DIR}/res/shaders/ DESTINATION ${CMAKE_BINARY_DIR}/res/shaders)
file(COPY ${CMAKE_SOURCE_DIR}/res/images/ DESTINATION ${CMAKE_BINARY_DIR}/res/images)

# ---------------------------------------------------------
# GLAD Configuration
# ---------------------------------------------------------
add_library(glad STATIC lib/glad/src/glad.c)
target_include_directories(glad PUBLIC lib/glad/include)
target_link_libraries(main PRIVATE glad)

# ---------------------------------------------------------
# GLFW Configuration
# ---------------------------------------------------------
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

if(GLFW_FOUND)
  message(STATUS "Found GLFW: ${GLFW_LIBRARIES}")
  target_include_directories(main PRIVATE ${GLFW_INCLUDE_DIRS})
  target_link_libraries(main PRIVATE ${GLFW_LIBRARIES})
else()
  message(FATAL_ERROR "GLFW not found. Install it via Homebrew using 'brew install glfw'.")
endif()

# ---------------------------------------------------------
# Link OpenGL (macOS)
# ---------------------------------------------------------
find_library(OpenGL_LIBRARY OpenGL)
if(OpenGL_LIBRARY)
  message(STATUS "Found OpenGL framework: ${OpenGL_LIBRARY}")
  target_link_libraries(main PRIVATE ${OpenGL_LIBRARY})
else()
  message(FATAL_ERROR "OpenGL framework not found. Ensure macOS development tools are installed.")
endif()
