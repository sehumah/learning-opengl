#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


/**
 * 
 * Challenge: Create two shader programs where the second program uses a different fragment shader that 
 *            outputs the color yellow; draw both triangles again where one outputs the color yellow
 * 
 * Compile and execute program with: g++ glad/src/glad.c 2_two_triangles_ex_3.cpp -o a.out -ldl -lglfw; ./a.out
 * 
 */


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);


const int WIDTH = 600;
const int HEIGHT = 400;


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Exercise 3: Two Triangles", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create a GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load GLAD." << std::endl;
    }
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //////////////////////////////////////////////////////////////////

    // setup the vertex shader
    const char *vertexShaderSource = "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"
                                    "void main(){\n"
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

    // setup 1st fragment shader
    const char *firstFragmentShaderSource = "#version 330 core\n"
                                            "out vec4 FragColor;\n"
                                            "void main() {\n"
                                                "FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
                                            "}\n";
    unsigned int firstFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(firstFragmentShader, 1, &firstFragmentShaderSource, NULL);
    glCompileShader(firstFragmentShader);
    glGetShaderiv(firstFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(firstFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::FIRST::COMPILATION FAILED: " << infoLog << std::endl;
    }

    // setup 2nd fragment shader
    const char *secondFragmentShaderSource = "#version 330 core\n"
                                            "out vec4 FragColor;\n"
                                            "void main() {\n"
                                                "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                            "}\n";
    unsigned int secondFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(secondFragmentShader, 1, &secondFragmentShaderSource, NULL);
    glCompileShader(secondFragmentShader);
    glGetShaderiv(secondFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(secondFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::SECOND::COMPILATION FAILED: " << infoLog << std::endl;
    }
    

    // setup the 1st shader program
    unsigned int firstShaderProgram = glCreateProgram();
    glAttachShader(firstShaderProgram, vertexShader);
    glAttachShader(firstShaderProgram, firstFragmentShader);
    glLinkProgram(firstShaderProgram);
    glGetProgramiv(firstShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(firstShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::SHADER::FIRST::LINKING FAILED: " << infoLog << std::endl;
    }

    // setup the 2nd shader program
    unsigned int secondShaderProgram = glCreateProgram();
    glAttachShader(secondShaderProgram, vertexShader);
    glAttachShader(secondShaderProgram, secondFragmentShader);
    glLinkProgram(secondShaderProgram);
    glGetProgramiv(secondShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(secondShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::SHADER::SECOND::LINKING FAILED: " << infoLog << std::endl;
    }

    // delete shaders after they've been linked into the programs
    glDeleteShader(vertexShader);
    glDeleteShader(firstFragmentShader);
    glDeleteShader(secondFragmentShader);
    
    // setup vertex data
    float leftTriangleVertices[] = {  // 2nd (left) triangle
        -0.8f, -0.7f, 0.0f,  // left
        -0.5f, -0.1f, 0.0f,  // top
        -0.2f, -0.7f, 0.0f,  // right
    };

    float rightTriangleVertices[] = {  // 1st (right) triangle
        0.2f, 0.1f, 0.0f,  // left
        0.5f, 0.7f, 0.0f,  // top
        0.8f, 0.1f, 0.0f,  // right
    };

    // generate VAOs & VBOs
    unsigned int VAOs[2], VBOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    
    // setup 1st VAO & VBO
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(leftTriangleVertices), leftTriangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0);  // not necessary here
    
    // setup 2nd VAO & VBO
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rightTriangleVertices), rightTriangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0);  // not necessary here as well

    //////////////////////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        // set the background color
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.9f, 1.0f);

        // render left triangle
        glUseProgram(firstShaderProgram);
        glBindVertexArray(VAOs[0]);  // not necessary since it wasn't unbound
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // render right triangle
        glUseProgram(secondShaderProgram);
        glBindVertexArray(VAOs[1]);  // same. not necessary since it wasn't unbound
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // glBindVertexArray(0);  // no need to unbind every frame

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // deallocate resources after they've outlived their purpose
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(firstShaderProgram);
    glDeleteProgram(secondShaderProgram);

    // deallocate all GLFW resources
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

// Successfully completed this exercise also without any aids. Only looked at the solution after I solved it.
