#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>


/**
 * ================
 *  |  SHADERS  |
 * ----------------
 *  |  Uniforms  |
 * ================
 * 
 * This program demonstrates the use of a uniform to pass data from the application (CPU) to the shader on the GPU.
 * Here, we set the color of the rendered triangle via a uniform. Instead of passing a single color to the fragment shader, 
 * we spice things up by gradually changing color over time:
 * 
 * Uniforms are slightly different compared to vertex attributes. First, uniforms are global, meaning that a uniform variable 
 * is unique per shader program object, and can be accessed from any shader at any stage in the shader program. 
 * Second, whatever you set the uniform value to, uniforms will keep their values until they're either reset or updated.
 * 
 *  Compile source code with : g++ glad/src/glad.c 3_shaders_2_uniforms.cpp -o a.out -ldl -lglfw
 *      Execute program with : ./a.out
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3. Shaders - Uniforms", NULL, NULL);
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
    //////////////////////////////////////////////////////////// START ////////////////////////////////////////////////////////////

    // When we're talking specifically about the vertex shader each input variable is also known as a vertex attribute. 
    // There is a maximum number of vertex attributes we're allowed to declare limited by the hardware. 
    // OpenGL guarantees there are always at least 16 4-component vertex attributes available, 
    // but some hardware may allow for more which you can retrieve by querying GL_MAX_VERTEX_ATTRIBS:
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;

    // setup the vertex shader
    const char *vertexShaderSource = "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"
                                    "void main () {\n"
                                        "gl_Position = vec4(aPos, 1.0);\n"
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
    
    /**
     * 1st
     * Uniforms
     * Uniforms are global, meaning that a uniform variable is unique per shader program object, and can be accessed from any shader at any stage 
     * in the shader program. Whatever you set the uniform value to, uniforms will keep their values until they're either reset or updated. 
     * To declare a uniform in GLSL we simply add the uniform keyword to a shader with a type and a name. From that point on we can use the 
     * newly declared uniform in the shader. 
     * 
     * Let's set the color of the triangle via a uniform:
     */
    const char *fragmentShaderSource = "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "uniform vec4 ourColor;  // we set this variable in the OpenGL code.\n"
                                        "void main () {\n"
                                            "FragColor = ourColor;\n"
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

    // delete shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // setup vertex data
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  // bottom left
         0.5f, -0.5f, 0.0f,  // bottom right
         0.0f,  0.5f, 0.0f,  // top
    };

    // generate VAOs & VBOs
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
    
    glBindVertexArray(0);  // unbind the VAO for later use
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind the VBO since its data is now in the EBO

    //////////////////////////////////////////////////////// END ////////////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window)) {
        // process input
        processInput(window);

        // set background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // 2nd
        // The uniform is currently empty; we haven't added any data to the uniform yet. 
        // We first need to find the index/location of the uniform attribute in our shader. 
        // Once we have the index/location of the uniform, we can update its values. 
        // Instead of passing a single color to the fragment shader, let's spice things up by gradually changing color over time:
        
        // First, we retrieve the running time in seconds via glfwGetTime().
        float timeValue = glfwGetTime();
        
        // Then we vary the color in the range of 0.0 - 1.0 by using the sin function and store the result in greenValue.
        float greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
        
        // Then we query for the location of the ourColor uniform using glGetUniformLocation. 
        // We supply the shader program and the name of the uniform (that we want to retrieve the location from) to the query function.
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        
        // Note that finding the uniform location does not require you to use the shader program first, but updating a uniform does require 
        // you to first use the program (by calling glUseProgram), because it sets the uniform on the currently active shader program.
        glUseProgram(shaderProgram);

        // Lastly we can set the uniform value using the glUniform4f function.
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        // bind VAO, render the triangle & unbind VAO
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