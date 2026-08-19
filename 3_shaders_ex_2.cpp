#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "3_Shader.h"


/**
 *  Exercise 2:
 *  Specify a horizontal offset via a uniform and move the triangle to the right side of the screen in the vertex shader using this offset value:
 * 
 * 
 * Compile and run program with:  g++ glad/src/glad.c 3_shaders_ex_2.cpp -o a.out -ldl -lglfw; ./a.out
 */ 


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    const int WIDTH = 600, HEIGHT = 400;
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3. Ex. 2 - Horizontal Offset via Uniform", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create a GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // use the custom shader class to load the shaders
    Shader shader("3_shaders_ex_2.vs", "3_shader.fs");

    // vertex data
    float vertices[] = {
        // coordinates        // colors
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // bottom left
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // bottom right
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  // top
    };

    // setup VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // setup VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // copy vertex data into the currently bound VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret vertex data

    // interpreting position data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // interpreting color data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // all steps done. Can now safely unbind VBO & VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use the shader program to render the triangle
        shader.use();

        // set a value on the uniform
        shader.setFloat("hOffset", 0.5f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader.deleteProgram();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}