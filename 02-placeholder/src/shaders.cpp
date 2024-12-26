#include "../include/utilities/shaders.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    int success;
    char infoLog[512];

    GLuint vertex = compileShader(vertexPath, GL_VERTEX_SHADER);
    GLuint fragment = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    // Check compiling status
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "Linking error: " << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader::activate() {
    glUseProgram(id);
}

std::string Shader::loadShaderSource(const char* filename) {
    std::ifstream file;
    std::stringstream buffer;
    std::string ret = "";

    file.open(filename);

    if (file.is_open()) {
        buffer << file.rdbuf();
        ret = buffer.str();
        file.close();
        return ret;
    }
    else {
        std::cout << "Failed to open file " << filename << std::endl;
        return "";
    }
}

GLuint Shader::compileShader(const char* shaderPath, GLenum shaderType) {
    int success;
    char infoLog[512];
    GLuint ret = glCreateShader(shaderType);
    std::string shaderSource = loadShaderSource(shaderPath);
    const GLchar* shader = shaderSource.c_str();
    glShaderSource(ret, 1, &shader, nullptr);
    glCompileShader(ret);

    glGetShaderiv(ret, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(ret, 512, nullptr, infoLog);
        std::cout << "Error compiling shader: " << infoLog << std::endl;
    }

    return ret;
}

void Shader::setMat4(const std::string &name, glm::mat4 val) {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}


