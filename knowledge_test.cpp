#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


/**
 * Compile src code with: g++ -std=c++20 knowledge_test.cpp glad.c -ldl -lglfw -o t.out
 * Run executable with: ./t.out
 */

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);


int main () {
    const int WIDTH = 800;
    const int HEIGHT = 600;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Knowledge Test Window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {  // load GLAD
        std::cout << "Failed to load GLAD." << std::endl;
        return -1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);  // now we can set the viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //////////////////////////////////////////////////////// DRAWING THE 1ST TRIANGLE ////////////////////////////////////////////////////////

    /**
     * PHASE 1: The Vertex Shader
     */

    // 1. create the vertex shader source code
    const char *vertexShaderSource = "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"
                                    "void main () {\n"
                                        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                    "}\n";
    
    // 2. create a shader object and store it in an unsigned int variable
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // 3. attach the vertex shader source code to the vertex shader object and compile the vertex shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 4. check if compilation was successful. Print error messages if compilation failed
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION FAILED: " << infoLog << std::endl;
    }

    /**
     * PHASE 2: Creating the Fragment Shader
     */

    // 5. create the fragment shader source code
    const char *fragmentShaderSource = "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "void main () {\n"
                                            "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                        "}\n";

    // 6. create a fragment shader object and store it in an unsigned int variable
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // 7. attach the fragment shader source code to the fragment shader and compile the fragment shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 8. check if compilation was successful. print error messages if compilation failed
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION FAILED: " << infoLog << std::endl;
    }

    /**
     * PHASE 3: Creating the Shader Program to link the shader objects
     */

    // 9. create a shader program object and store it in an unsigned int variable
    unsigned int shaderProgram = glCreateProgram();

    // 10. attach previously compiled shaders with glAttachShader
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // 11. link attached shaders with glLinkProgram
    glLinkProgram(shaderProgram);

    // 12. check if linking was successful. print error messages if linking failed
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING FAILED: " << infoLog << std::endl;
    }

    // 13. delete the shader objects. we no longer need them after linking them into the shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    /**
     * PHASE 4: VAO & VBO
     *  - Vertex Input
     *  - Linking Vertex Attributes
     *  - 
     */

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f,  -0.5f, 0.0f,
        0.0f,   0.5f, 0.0f,
    };

    // Generate & Bind a VAO to use it
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // After that you can bind/configure the corresponding VBO(s) and attribute pointer(s) and then unbind the VAO for later use

    // Configure VBO & attribute pointers
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Linking Vertex Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind the VAO for later use
    glBindVertexArray(0);    

    //////////////////////////////////////////////////////// END OF DRAWING THE 1ST TRIANGLE ////////////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        ////////////////////////////// RENDER STUFF //////////////////////////////
        glClearColor(0.0f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use the shader program, bind the vertex array object and render the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);  // params are: OpenGL primitive we'd like to draw (triangles in this case, starting index of the vertex array we'd like to draw, how many vertices we want to draw)
        glBindVertexArray(0);  // unbind the VAO after using it

        ////////////////////////////// END OF RENDERING //////////////////////////////
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
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
        glfwSetWindowShouldClose(window, true);
    }
}