#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>


/**
 * ==============================================
 *  |                SHADERS                   |
 * ==============================================
 *  | More Attributes & Fragment Interpolation |
 * ==============================================
 * 
 * This program demonstrates adding color data to the vertex data.
 * In addition to the vertex coordinates data, we add color data as 3 floats to the vertices array. 
 * We assign a red, green and blue color to each of the corners of our triangle respectively.
 * 
 *  Compile source code with: g++ glad/src/glad.c 3_shaders_3_fragment_interpolation.cpp -o a.out -ldl -lglfw
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3. Shaders - More Attributes & Shader Interpolation", NULL, NULL);
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

    // 2nd. setup the vertex shader

    // Since we now have more data to send to the vertex shader, it is necessary to adjust the vertex shader to also receive our color 
    // value as a vertex attribute input. Note that we set the location of the aColor attribute to 1 with the layout specifier:
    const char *vertexShaderSource = "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;  // the position variable has attribute position 0\n"
                                    "layout (location = 1) in vec3 aColor;  // the color variable has attribute position 1\n"
                                    "out vec3 ourColor;  // output a color to the fragment shader\n"
                                    "void main () {\n"
                                        "gl_Position = vec4(aPos, 1.0);  // see how we directly give a vec3 to vec4's constructor\n"
                                        "ourColor = aColor;  // set ourColor to the input color we got from the vertex data\n"
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

    // 3rd. setup the fragment shader

    // Since we no longer use a uniform for the fragment's color, but now use the 
    // ourColor output variable we'll have to change the fragment shader as well:
    const char *fragmentShaderSource = "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "in vec3 ourColor;\n"
                                        "void main () {\n"
                                            "FragColor = vec4(ourColor, 1.0);\n"
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

    // 1st setup vertex data with RGB color data for each vertex
    /**
     * Uniforms are a useful tool for setting attributes that may change every frame, or for interchanging data 
     * between your application and your shaders, but what if we want to set a color for each vertex? 
     * In that case we'd have to declare as many uniforms as we have vertices. 
     * A better solution would be to include more data in the vertex attributes.
     * 
     * We saw in the previous chapter how we can fill a VBO, configure vertex attribute pointers and store it all in a VAO. 
     * This time, we also want to add color data to the vertex data. We're going to add color data as 3 floats to the vertices array. 
     * We assign a red, green and blue color to each of the corners of our triangle respectively:
     */
    float vertices[] = {
        // triangle vertices, with color data (RGB values) added so each vertex has its own color
        // coordinates        // colors
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // bottom left
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // bottom right
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  // top
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

    // 4th
    /**
     * Because we added another vertex attribute and updated the VBO's memory we have to re-configure the vertex attribute pointers.
     * The first few arguments of glVertexAttribPointer are relatively straightforward. 
     * This time we are configuring the vertex attribute on attribute location 1. 
     * The color values have a size of 3 floats and we do not normalize the values. 
     * 
     * Since we now have two vertex attributes we have to re-calculate the stride value. To get the next attribute value 
     * (e.g. the next x component of the position vector) in the data array we have to move 6 floats to the right, three for 
     * the position values and three for the color values. This gives us a stride value of 6 times the size of a float in bytes (= 24 bytes). 
     * Also, this time we have to specify an offset. For each vertex, the position vertex attribute is first so we declare an offset of 0. 
     * The color attribute starts after the position data so the offset is 3 * sizeof(float) in bytes (= 12 bytes).
     */
    
    // tell OpenGl how to interpret buffer data & enable the vertex arrays
    // coordinate attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);  // unbind the VAO for later use
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind the VBO
    //////////////////////////////////////////////////////// END ////////////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window)) {
        // process input
        processInput(window);

        // set background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use the shader program, bind the VAO, draw the triangle & unbind the VAO after using it
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