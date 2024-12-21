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

    // Shaders
    // Compile vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexShaderSource = loadShaderSrc("../assets/vertex_core.glsl");
    const GLchar* vShaderSource = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Catch if an error happens -> is everything going according to plan?
    // Note that success is an integer defined at the start of the main function
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, info);
        std::cout << "Error compiling vertex shader: " << info << std::endl;
    }

    // Compile fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentShaderSource = loadShaderSrc("../assets/fragment_core.glsl");
    const GLchar* pShaderSource = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &pShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, info);
        std::cout << "Error compiling fragment shader: " << info << std::endl;
    }

    // Linking the shaders
    // This will be called by openGL so it can pipeline all the stuff needed
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, info);
        std::cout << "Error linking shader program: " << info << std::endl;
    }

    // Cleaning stuff
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex array -> uses Normalize Device Coordinate.
    // Bottom left is (-1,-1), Top right is (1,1). So (0,0) is the center
    /*
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
    };
    */
    // Right now we have just data, they don't have meaning. We want to give to this data the meaning to be a triangle.
    // We need an attribute pointer and an index. So the idea with our data is tha each 3 adjacent points
    // Correspond to a 3D point. Each coordinate is composed by n bytes . So a 3D point is n*3 bytes.
    // We also want a stride. A stride is the distance between the start of a vertex and the start of the next vertex.
    // In our case, the stride is 3 * sizeof(float). Also we need an offset, in our case 0.

    // Look at include/vertex_core.glsl -> layout (location = 0) in vec3 aPos;
    // We are telling that, starting from 0, we have a vec3

    // What if we wanted to make a rectangle? GPU are only able to create triangle...
    // Let's join two triangles

    float vertices[] = {
        0.5f,  0.5f, 0.0f, //top right
        -0.5f,  0.5f, 0.0f, // top left
        -0.5f, -0.5f, 0.0f, // bottom left

        //second triangle
        -0.5f, -0.5f, 0.0f, //bottom left
        0.5f, -0.5f, 0.0f, //bottom right
        0.5f,  0.5f, 0.0f //top right
    }; // Kinda inefficient... so many vertices are shared...

    // There is another kind of buffer (element buffer object), which stores the indexes of the triangles

    float vertices_2[] = {
        0.5f,  0.5f, 0.0f, //top right
        -0.5f, 0.5f, 0.0f, //top left
        -0.5f, -0.5f, 0.0f, //bottom left
        0.5f, -0.5f, 0.0f, //bottom right
    };

    unsigned int indices[] = {
        0, 1, 2, // First triangle
        2, 3, 0 // Second triangle
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Set attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Using GL_STATIC_DRAW we are telling openGL that we will not modify this array of vertices that much,
    // So it can store them in a convenient position. Other options are DYNAMIC, when you change the shape frame by frame
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);

    // set up EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window)) {
        // Process inputs
        processInput(window);

        // render some colors
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw shapes
        // Which VAO should I look at?
        glBindVertexArray(VAO);
        // Which program?
        glUseProgram(shaderProgram);

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // The gpu generate the next frame at the same time we are showing the current frame.
        // So everytime I generate a new frame, swap so it can be shown
        glfwSwapBuffers(window);
        glfwPollEvents();
        // A single buffer would cause flickering, because you are showing a frame that is being generated.
        // Using a double buffer like so, we avoid this issue because the generation can finish properly before displaying
        // the new frame
    }

    // delete stuff
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}


