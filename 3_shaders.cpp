#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


/**
 * =============
 *  | SHADERS |
 * =============
 *  Compile source code with: g++ shaders.cpp glad/src/glad.c -ldl -lglfw -o a.out
 *      Execute program with: ./a.out
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ch. 3 - Shaders", NULL, NULL);
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
     * To show how this works we're going to alter the shaders from the previous chapter to let the vertex shader decide the color for the fragment shader.
     */

    // setup the vertex shader
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

    // setup the fragment shader
    const char *fragmentShaderSource = "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "in vec4 vertexColor;\n"
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

    // delete shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // setup vertex data
    float vertices[] = {
        // vertices from tutorial (https://youtu.be/69f6W-vO67E?list=PLtqEJYsBa5uoYFuKT9cjfaH39ibLdxzqc)
		 0.3f,  0.5f, 0.0f,  // top right
		 0.5f,  0.0f, 0.0f,  // middle right
		 0.3f, -0.5f, 0.0f,  // bottom right
		-0.3f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.0f, 0.0f,  // middle left
		-0.3f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {
        // indices from tutorial (https://youtu.be/69f6W-vO67E?list=PLtqEJYsBa5uoYFuKT9cjfaH39ibLdxzqc)
		0, 5, 4, // first triangle
		0, 4, 1, // second triangle
		1, 4, 3, // third triangle
		1, 3, 2,  // fourth triangle
    };

    // print to verify vertex count (expecting: 12, actual: 11. not sure why)
    // std::cout << "Vertices: " << sizeof(vertices) / (3 * sizeof(float)) << '\n';

    // generate VAOs & VBOs
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    /**
     * bind VAOs
     * bind VBOs
     * copy vertex data into the buffers
     * tell OpenGL how to interpret vertex data
     * enable vertex attribe arrays
     */

    // bind buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    // copy data into buffers
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // tell OpenGl how to interpret buffer data & enable the vertex array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);  // unbind the VAO for later use
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind the VBO since its data is now in the EBO

    // switch to wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //////////////////////////////////////////////////////// END ////////////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window)) {
        // process input
        processInput(window);

        // set background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use the shader program, bind the VAO & draw the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);  // unbind the VAO after using it

        // swap buffers & poll events
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


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}