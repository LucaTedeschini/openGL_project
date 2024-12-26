#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    unsigned int id;
    Shader(const char* vertexPath, const char* fragmentPath);
    void activate();

    //utility functions
    std::string loadShaderSource(const char* path);
    GLuint compileShader(const char* path, GLenum type);

    //uniform functions
    void setMat4(const std::string& name, glm::mat4 val);
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);



};