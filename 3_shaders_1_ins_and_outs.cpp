#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>


/**
 * ==================
 *  |   SHADERS    |
 * ------------------
 *  | Ins and Outs |
 * ==================
 * 
 * This program demonstrates sending data from one shader as output to another shader as input.
 * Specifically, we send output data from the vertex shader which the fragment shader takes as its input.
 * The fragment shader then uses the vertex shader's output data as its input data to color the rendered triangle.
 * ----------------------------------------------------------------------------------------------------------------
 *  Compile source code with : g++ glad/src/glad.c 3_shaders_1_ins_and_outs.cpp -o a.out -ldl -lglfw
 *  Execute program with     : ./a.out
 * 
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Shaders - Ins & Outs", NULL, NULL);
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

    // When we're talking specifically about the vertex shader each input variable is also known as a vertex attribute. 
    // There is a maximum number of vertex attributes we're allowed to declare limited by the hardware. 
    // OpenGL guarantees there are always at least 16 4-component vertex attributes available, 
    // but some hardware may allow for more which you can retrieve by querying GL_MAX_VERTEX_ATTRIBS:
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;

    /**
     * Shader Ins & Outs
     * Shaders are programs on their own, but they are part of a whole and for that reason we want to have 
     * inputs and outputs on the individual shaders so that we can move stuff around. GLSL defined the in and out 
     * keywords specifically for that purpose. Each shader can specify inputs and outputs using those keywords and wherever an 
     * output variable matches with an input variable of the next shader stage they're passed along. The vertex and fragment shader differ a bit.
     * 
     * To show how this works we're going to alter the shaders from the previous 
     * chapter to let the vertex shader decide the color for the fragment shader.
     */

    // setup the vertex shader - the src code sends 'vertexColor' as output to the fragment shader
    const char *vertexShaderSource = "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;  // the position variable has attribute position 0\n"
                                    "out vec4 vertexColor;  // specify a color output to the fragment shader\n"
                                    "void main () {\n"
                                        "gl_Position = vec4(aPos, 1.0);  // see how we directly give a vec3 to vec4's constructor\n"
                                        "vertexColor = vec4(0.5, 0.0, 0.0, 1.0);  // set the output variable to a dark-red color\n"
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

    // setup the fragment shader - this fragment source takes 'vertexColor' as input from the vertexShaderSource
    const char *fragmentShaderSource = "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "in vec4 vertexColor;\n"  // the input variable from the vertex shader (same name and same type) 
                                        "void main () {\n"
                                            "FragColor = vertexColor;\n"
                                        "}\n";
    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION FAILED: " << infoLog << std::endl;
    }

    // setup the shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING FAILED: " << infoLog << std::endl;
    }

    // delete compiled shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // setup vertex data
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // bottom left
         0.5f, -0.5f, 0.0f, // bottom right
         0.0f,  0.5f, 0.0f, // top
    };

    // generate VAO & VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // copy vertex data into buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // tell OpenGl how to interpret buffer (vertex) data & enable the vertex array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    
    // unbind the VAO for later use & the VBO too
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //////////////////////////////////////////////////////// END ////////////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window)) {
        // process input
        processInput(window);

        // set background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use the shader program, bind VAO, render the triangle & unbind VAO
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0);

        // swap buffers & poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(shaderProgram);
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