#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main(){
    // Glm testing
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    // This is an identity matrix basically
    glm::mat4 trans = glm::mat4(1.0f);
    // Here we are basically creating a translation matrix upon our Identity matrix
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

    // Order matters, we are doing matrix x vector multiplication
    vec = trans * vec;

    std::cout << "Translation \n" << std::endl;


    std::cout << vec.x << std::endl;
    std::cout << vec.y << std::endl;
    std::cout << vec.z << std::endl;

    // Now we do a rotation
    glm::mat4 rot = glm::mat4(1.0f);
    trans = glm::mat4(1.0f);
    // We are creating the translation matrix, and we are applying the rotation to the X component
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // Then we add a scaling
    trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

    vec = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    vec = trans * vec;
    std::cout << "\nRotation + scaling \n" << std::endl;

    std::cout << vec.x << std::endl;
    std::cout << vec.y << std::endl;
    std::cout << vec.z << std::endl;

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

    while (!glfwWindowShouldClose(window)) {
        // Process inputs
        processInput(window);

        // render some colors
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // The gpu generate the next frame at the same time we are showing the current frame.
        // So everytime I generate a new frame, swap so it can be shown
        glfwSwapBuffers(window);
        glfwPollEvents();
        // A single buffer would cause flickering, because you are showing a frame that is being generated.
        // Using a double buffer like so, we avoid this issue because the generation can finish properly before displaying
        // the new frame
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Everytime it's resized, set the viewport
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}