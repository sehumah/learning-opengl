#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


/**
 * 
 * Challenge: Draw 2 triangles next to each other using glDrawArrays by adding more vertices to your data.
 * 
 * Compile and execute program with: g++ glad/src/glad.c 2_two_triangles_ex_1.cpp -o a.out -ldl -lglfw; ./a.out
 * 
 */


const int WIDTH = 600;
const int HEIGHT = 400;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Exercise 1: Two Triangles", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create a GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load GLAD." << std::endl;
        return -1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //////////////////////////////////////////////////  setup functions to render 2 triangles  //////////////////////////////////////////////////
    
    // setup the vertex shader
    const char *vertexShaderSource = "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"
                                    "void main() {\n"
                                        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                    "}\n";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION FAILED: " << infoLog << std::endl;
    }

    // setup the fragment shader
    const char *fragmentShaderSource =  "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "void main() {\n"
                                            "FragColor = vec4(1.0f, 0.6f, 0.0f, 1.0f);\n"
                                        "}\n";
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION FAILED: " << infoLog << std::endl;
    }

    // setup the shader program, link & check for linking errors
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING FAILED: " << infoLog << std::endl;
    }

    // delete shaders after linking them into the shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // setup vertex data
    float vertices[] = {
        // right triangle
        0.8f, 0.6f, 0.0f,  // top right
        0.8f, 0.2f, 0.0f,  // bottom right
        0.4f, 0.2f, 0.0f,  // bottom left

        // left triangle
        0.1f, 0.6f, 0.0f,  // top left
        0.1f, 0.2f, 0.0f,  // bottom left
        0.5f, 0.6f, 0.0f,  // top right
    };

    // create & bind VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // setup & configure the VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // tell OpenGL how to interprete the vertex data & enable vertex array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind VAO for later use
    glBindVertexArray(0);

    // uncomment to draw in wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //////////////////////////////////////////////////  END  //////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.8f, 1.0f);

        // use the shader program, bind the VAO, draw the triangle & unbind the VAO
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);  // 1st triangle. my solution (still works)
        // glDrawArrays(GL_TRIANGLES, 3, 3);  // 2nd triangle. my solution (still works)

        glDrawArrays(GL_TRIANGLES, 0, 6);  // set the count to 6 since we're drawing 6 vertices now (2 triangles); not 3! (right solution)
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // de-allocate resources once they've outlive their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// This exercise was completed all from my understanding. No reference materials were looked at until after completing the task.
