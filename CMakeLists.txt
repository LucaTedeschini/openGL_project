# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(openGL_project VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenGL and GLFW libraries
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Add the executable
add_executable(openGL_project src/main.cpp src/glad.c)

# Include directories for headers
target_include_directories(openGL_project PRIVATE include)

# Link libraries
target_link_libraries(openGL_project PRIVATE ${OPENGL_LIBRARIES} glfw)
