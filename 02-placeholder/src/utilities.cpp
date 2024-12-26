#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include "utilities/utilities.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Everytime it's resized, set the viewport
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


