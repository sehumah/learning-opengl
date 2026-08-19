#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "3_Shader.h"
#include <iostream>


/**
 *  Exercise 3:
 *  Output the vertex position to the fragment shader using the out keyword and set the fragment's color equal to this vertex position 
 *  (see how even the vertex position values are interpolated across the triangle). 
 *  Once you managed to do this; try to answer the following question: why is the bottom-left side of our triangle black?:
 * 
 * 
 * Compile and run program with:  g++ glad/src/glad.c 3_shaders_ex_3.cpp -o a.out -ldl -lglfw; ./a.out
 * 
 * Solved the challenge before looking at the solution and my solution produced the same result as the author's. On to the next!
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3. Ex. 3 - Vertex Coordinates as Triangle's Vertex Colors", NULL, NULL);
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

    // setup shader and everything else
    Shader shader("3_shaders_ex_3.vs", "3_shaders_ex_3.fs");

    // vertex & color data
    float vertices[] = {
        // coordinates        // colors
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // bottom left
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // bottom right
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  // top
    };

    // generate & bind VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // generate & bind VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // copy vertices data into the bound buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ////////// Tell OpenGL how to interpret vertex data //////////
    
    // position data interpretation
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // color data interpretation
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // can now safely unbind the VBO & VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use the program, bind VAO and render the triangle
        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0);  // no need to unbind it everytime
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