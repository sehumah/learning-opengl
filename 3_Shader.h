#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>  // include GLAD to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


/**
 * Writing, compiling and managing shaders can be quite cumbersome. 
 * Let's make our life a bit easier by building a shader class that reads shaders from disk, 
 * compiles and links them, checks for errors and is easy to use. 
 * This also gives you a bit of an idea how we can encapsulate some of the knowledge we learned so far into useful abstract objects.
 * We will create the shader class entirely in a header file, mainly for learning purposes and portability.
 */

class Shader {
public:
    unsigned int ID;  // the shader program ID

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath) {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode, fragmentCode;
        std::ifstream vShaderFile, fShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            // stream objects to hold file contents
            std::stringstream vShaderStream, fShaderStream;
            
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            // close file handlers
            vShaderFile.close();
            fShaderFile.close();

            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch(const std::ifstream::failure& e) {
            // std::cerr << e.what() << '\n';
            std::cout << "ERROR::SHADER::FILE_NOT_READ_SUCCESSFULLY: " << e.what() << std::endl;
        }

        // convert shader src codes into C-style strings
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        
        // 2. compile shaders
        unsigned int vertexShader, fragmentShader;
        
        // vertex shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vShaderCode, NULL);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, "VERTEX");  // print compile errors if any
        
        // fragment shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader, "FRAGMENT");  // print compile errors if any

        // shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");  // print linking errors if any

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    // use/activate the shader program
    void use() {
        glUseProgram(ID);
    }

    // delete the shader program
    void deleteProgram() {
        glDeleteProgram(ID);
    }

    // utility uniform functions to query a uniform location and set its value
    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

private:
    // utility function for checking shader compilation and shader program linking errors
    void checkCompileErrors(unsigned int& shader, const char* type) {
        int success;
        char infoLog[1024];
        if (type == "PROGRAM") {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER::" << type << "::LINKING_FAILED: " << infoLog << std::endl;
            }
        } else {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED: " << infoLog << std::endl;
            }
        }
    }
};


#endif
