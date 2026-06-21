#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


/**
 * =============================================
 * | Fan-logic / Clockwise / Full loop Hexagon |
 * =============================================
 * Here, I learned the proper way to draw hexagons in OpenGL by properly ordering the vertices in
 * a fan-logic / clockwise / full loop manner, and anchoring the indices at one specific vertex
 * and using 2 adjacent vertices to draw the full triangle.
 * 
 * Compile and execute code with: g++ hexagon.cpp glad/src/glad.c -ldl -lglfw -o a.out; ./a.out
 * 
 */


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);


int main() {
    const int WIDTH = 600;
    const int HEIGHT = 400;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Hexagon", NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to load GLAD" << std::endl;
        return -1;
    }
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // the vertex shader
    const char *vertexShaderSource =    "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "void main(){\n"
                                        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                        "}\n";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "SHADER::VERTEX::COMPILE_STATUS::ERROR: " << infoLog << std::endl;
    }
    // the fragment shader
    const char *fragmentShaderSource = "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "void main(){\n"
                                        "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
                                        "}\n";
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "SHADER::FRAGMENT::COMPILE_STATUS::ERROR: " << infoLog << std::endl;
    }
    // create the shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "PROGRAM::SHADER::LINK_STATUS::ERROR: " << infoLog << std::endl;
    }
    // delete linked shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // setup vertex input
    float vertices[] = {
        // triangle
        // -0.5f, -0.5f, 0.0f,  // left
        //  0.0f,  0.5f, 0.0f,  // top
        //  0.5f, -0.5f, 0.0f,  // right

        // hexagon - vertex ordering should follow the fan-logic i.e. go around the hexagon in order 0 -> 1 -> 2 -> 3 -> 4 -> 5 (clockwise, one full loop)
         0.2f,  0.5f, 0.0f,  // top-right
         0.5f,  0.0f, 0.0f,  // right
         0.2f, -0.5f, 0.0f,  // bottom-right
        -0.2f, -0.5f, 0.0f,  // bottom-left
        -0.5f,  0.0f, 0.0f,  // left
        -0.2f,  0.5f, 0.0f,  // top-left
    };

    unsigned int indices[] = {
        // No need to reorder the vertex array, all that's needed is just a correct triangle fan from one vertex.
        // For a convex polygon with vertices in order v0, v1, v2, v3, v4, v5, the standard fan triangulation (anchored at v0) is:
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        // so basically, every triangle shares vertex 0 and uses two consecutive perimeter vertices.
        // No need to jump around the vertices, instead anchor triangles at one vertex and use 2 adjacent points to draw a full triangle which then draws the hexagon
    };

    // configure VAO, VBO & EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while(!glfwWindowShouldClose(window)){
        processInput(window);
        glClearColor(0.5f, 0.2f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}