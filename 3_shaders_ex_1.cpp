#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "3_Shader.h"


/**
 *  Exercise 1:
 *  Adjust the vertex shader so that the triangle is upside down:
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3. Ex. 1 - Upside Down Triangle", NULL, NULL);
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
    ///////////////////////////////////////////////////////////////////// START /////////////////////////////////////////////////////////////////////

    // load our custom shader class
    Shader shader { "3_shader.vs", "3_shader.fs" };

    // vertex coordinate & color data
    // my solution was to use the same vertex data from before but with inverted y-coordinates instead 
    // but another correct solution is to invert the y-coordinates in the vertex shader source code
    float vertices[] = {
        // coordinates        // color
        -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // top left
         0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // top right
         0.0f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  // bottom
    };

    // configure VAO & VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //  Tell OpenGL how to interpret the vertex data  //

    // how to get individual coordinate data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // how to get individual color data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // unbind VAO & VBO 
    // glBindVertexArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    ////////////////////////////////////////////////////////////////////// END //////////////////////////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the upside down triangle
        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // swap buffers & poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
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