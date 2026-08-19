#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "3_Shader.h"


/**
 *  ==========================
 *   |       SHADERS        |
 *   ------------------------
 *   | Our own shader class |
 *  ==========================
 * 
 *  This program demonstrates the use of a custom shader class to read shaders from disk, 
 *  compile and link them, check for errors and is easy to use.
 *  
 *  Compile source code with  : g++ glad/src/glad.c 3_shaders_4_shader_class_fragment_interpolation.cpp -o a.out -ldl -lglfw
 *  Execute program with      : ./a.out
 */


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main() {
    const int WIDTH=600, HEIGHT=400;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3. Shaders - Shader class & Fragment Interpolation", NULL, NULL);
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

    // build and compile our shader program
    Shader ourShader("3_shader.vs", "3_shader.fs");

    // setup vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // triangle vertices with color data (RGB values) added so each vertex has its own color
        // coordinates        // colors
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // bottom left
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // bottom right
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  // top
    };

    // generate & bind VAO & VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // copy vertex data into VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //*** Tell OpenGl how to interpret buffer data (vertex data) & enable the vertex attrib arrays ***//
    // The first few arguments of glVertexAttribPointer are relatively straightforward. This time we are configuring the vertex attribute on attribute location 1. 
    // The color values have a size of 3 floats and we do not normalize the values. 
    // 
    // Since we now have two vertex attributes we have to re-calculate the stride value. To get the next attribute value 
    // (e.g. the next x component of the position vector) in the data array we have to move 6 floats to the right, three for 
    // the position values and three for the color values. This gives us a stride value of 6 times the size of a float in bytes (= 24 bytes). 
    // Also, this time we have to specify an offset. For each vertex, the position vertex attribute is first so we declare an offset of 0. 
    // The color attribute starts after the position data so the offset is 3 * sizeof(float) in bytes (= 12 bytes).
    
    // coordinate attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // process input
        processInput(window);

        // set background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use the shader program, bind the VAO, draw the triangle & unbind the VAO after using it
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0);

        // swap buffers & poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // free up resources
    ourShader.deleteProgram();
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