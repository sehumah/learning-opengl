#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


/**
 * 
 * Challenge: Create 2 triangles using two different VAOs and VBOs for their data.
 * 
 * Compile and execute program with: g++ 2_two_triangles_ex_2.cpp glad/src/glad.c -ldl -lglfw -o a.out; ./a.out
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
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Exercise 2: Two Triangles", NULL, NULL);
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
    
    // setup & configure the vertex shader
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

    // setup & configure the fragment shader
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

    // setup & configure the shader program
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
    float verticesA[] = {  // 1st (right) triangle
        0.2f, 0.1f, 0.0f,  // left
        0.5f, 0.7f, 0.0f,  // top
        0.8f, 0.1f, 0.0f,  // right
    };

    float verticesB[] = {  // 2nd (left) triangle
        -0.8f, -0.7f, 0.0f,  // left
        -0.5f, -0.1f, 0.0f,  // top
        -0.2f, -0.7f, 0.0f,  // right
    };

    /**
     * How this works is that since each triangle is using its own VAO, VBO and vertex data, we treat each operation as independent i.e. setting up and configuring VAO & VBO
     */

    // generate VAOs & VBOs
    unsigned int VAOs[2], VBOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    /**
     * 1st Triangle Setup
     *  - bind its VAO
     *  - bind its VBO
     *  - copy the vertex data into the bound VBO
     *  - then tell OpenGL how to interprete the vertex data
     *  - enable vertex array, giving the vertex attribute location as its argument
     *  - finally, unbind VAO for later use
     */
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesA), verticesA, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0);  // we could but it's not necessary since the next line binds another VAO


    /**
     * 2nd Triangle Setup (follows same steps as the 1st triangle)
     */
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesB), verticesB, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0);  // also not necessary here as well since it's the final step before the rendering loop

    // uncomment to draw in wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //////////////////////////////////////////////////  END  //////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.8f, 1.0f);

        // use the shader program, bind the VAOs, draw the triangles & unbind the VAOs
        glUseProgram(shaderProgram);

        // draw right triangle
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // draw left triangle
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // unbind VAOs
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // delete allocated resources once they've outlived their purpose
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram);

    // delete all allocated GLFW resources
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
