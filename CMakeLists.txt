cmake_minimum_required(VERSION 3.10)

# Project name and version
project(OpenGLProject VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ---------------------------------------------------------
# Add executable target
# ---------------------------------------------------------
add_executable(main
  src/main.cpp
  src/ShaderClass.cpp
  src/VAO.cpp
  src/VBO.cpp
  src/EBO.cpp
)

# ---------------------------------------------------------
# Include Directories
# ---------------------------------------------------------
include_directories(
  ${CMAKE_SOURCE_DIR}/include # Include local headers
  ${CMAKE_SOURCE_DIR}/shaders # Include local shaders
  ${CMAKE_SOURCE_DIR}/lib/glad/include # GLAD headers
)

# ---------------------------------------------------------
# Copy shader files to the build directory
# ---------------------------------------------------------
file(COPY ${CMAKE_SOURCE_DIR}/shaders/ DESTINATION ${CMAKE_BINARY_DIR}/shaders)

# ---------------------------------------------------------
# GLAD Configuration
# ---------------------------------------------------------
add_library(glad STATIC lib/glad/src/glad.c)
target_include_directories(glad PUBLIC lib/glad/include)
target_link_libraries(main PRIVATE glad)

# ---------------------------------------------------------
# Find and Link GLFW
# ---------------------------------------------------------
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})
target_link_libraries(main PRIVATE ${GLFW_LIBRARIES})

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
