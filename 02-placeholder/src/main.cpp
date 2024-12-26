#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <string>

#include "utilities/utilities.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "utilities/shaders.h"



int main(){

    int success;
    char info[512];

    glfwInit();
    // OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // If on macOS...
# ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World!", nullptr, nullptr);
    if (window == nullptr) {
        // If unable to create the window...
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Where to locate the window? how big?
    glViewport(0, 0, 800, 600);

    // If I resize the window, add a callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shader("../assets/vertex_core.glsl", "../assets/fragment_core.glsl");

    float vertices[] = {
        // Position                 colors
        0.5f,  0.5f, 0.0f,      1.0f, 1.0f, 0.5f,
        -0.5f, 0.5f, 0.0f,      0.5f, 1.0f, 0.75f,
        -0.5f, -0.5f, 0.0f,     0.6f, 1.0f, 0.2f,
        0.5f, -0.5f, 0.0f,      1.0f, 0.2f, 1.0f,
        // Position                 colors
        0.8f,  0.8f, 0.0f,      0.3f, 0.4f, 0.2f,
        -0.2f, 0.8f, 0.0f,      0.2f, 0.8f, 0.15f,
        -0.2f, -0.2f, 0.0f,     0.3f, 0.4f, 0.6f,
        0.8f, -0.2f, 0.0f,      1.0f, 0.2f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2, // First triangle
        2, 3, 0, // Second triangle
        4, 5, 6,
        6, 7, 4,
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Set attribute pointer (position)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set attribute pointer (color)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // Using GL_STATIC_DRAW we are telling openGL that we will not modify this array of vertices that much,
    // So it can store them in a convenient position. Other options are DYNAMIC, when you change the shape frame by frame
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // set up EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(0.5f));

    shader.activate();
    shader.setMat4("transform", trans);

    while (!glfwWindowShouldClose(window)) {
        // Process inputs
        processInput(window);

        // render some colors
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 20.0f), glm::vec3(0.3f, 0.7f, 1.0f));
        shader.setMat4("transform", trans);


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // delete stuff
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

