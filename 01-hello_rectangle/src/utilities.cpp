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

std::string loadShaderSrc(const char* filename) {
    std::ifstream file;
    std::stringstream buffer;
    std::string ret = "";

    file.open(filename);

    if (file.is_open()) {
        buffer << file.rdbuf();
        ret = buffer.str();
    }
    else {
        std::cout << "Failed to open file " << filename << std::endl;
    }
    file.close();
    return ret;
}
