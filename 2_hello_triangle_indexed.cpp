#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


/**
 * 
 * Compile and execute program with: g++ 2_hello_triangle_indexed.cpp glad/src/glad.c -ldl -lglfw -o a.out; ./a.out
 * 
 * This program builds on 2_hello_triangle.cpp and uses an Element Buffer Object (EBO) to draw a rectangle using 2 triangles and index drawing.
 * 
 */


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main () {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle (Indexed) - LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load GLAD." << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    ////////////////////////////// building and compiling our shader program //////////////////////////////
    
    /**
     * PHASE 1: The Vertex Shader
     */

    // 1. create the shader source code
    const char *vertexShaderSource =    "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "void main() {\n"
                                        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                        "}\n";

    // 2. create the shader object with the glCreateShader function passing in the appropriate gl shader enum and then store it in a uint variable
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // 3. attach the shader source code to the shader object and compile the shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 4. check for shader compilation errors and if any were found, print the info log
    int success;  // integer to indicate success
    char infoLog[512];  // storage container for the error messages (if any)

    // 5th. check if compilation was successful with glGetShaderiv
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    // 6th. if compilation failed, retrieve the error message with glGetShaderInfoLog and print the error message
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    /**
     * 
     * PHASE 2: The Fragment Shader
     * 
     */

    // create the fragment shader source code
    const char *fragmentShaderSource =  "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "void main () {\n"
                                        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                        "}\n";

    // The process for compiling a fragment shader is similar to the vertex shader, 
    // although this time we use the GL_FRAGMENT_SHADER constant as the shader type for glCreateShader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for compile errors here as well
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    /**
     * PHASE 3: The Shader Program
     * ===========================
     * A shader program links compiled shaders into a shader program object. We activate the shader program when rendering objects.
     */

    // 7. create shader program object with glCreateProgram and store it in a uint variable
    unsigned int shaderProgram = glCreateProgram();

    // 8. Now we need to attach the previously compiled shaders to the program object
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // 9. then link them with glLinkProgram. only link after attaching all shaders
    glLinkProgram(shaderProgram);

    // 10. Just like shader compilation we also check if linking a shader program failed and retrieve the corresponding log.
    // However, instead of using glGetShaderiv and glGetShaderInfoLog we now use glGetProgramiv and glGetProgramInfoLog
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // 11. Every shader and rendering call after glUseProgram will now use the program object (and thus the shaders). 
    // We can now safely delete the shader objects after we've linked them into the program object; we no longer need them:
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    
    /**
     * PHASE 4: The Vertex Input
     */

    // 12. setup vertex data
    
    /*
    // vertices for one triangle (the first triangle we rendered)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 
    */
    
    /*
    // vertices before using a EBO. this draws a rectangle using 2 triangles
    float vertices[] = {
        // first triangle
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f,  0.5f, 0.0f,  // top left 
        // second triangle
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    */

    // vertices for the EBO. this draws a rectangle using triangles
    float vertices[] = {
        0.5f,  0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, // bottom left
       -0.5f,  0.5f, 0.0f, // top left
    };

    // indices used by the EBO for index drawing
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3,  // second Triangle
    };

    //***** Configure VAO, VBO & EBO *****//

    // 13. define & bind the vertex array object (VAO)
    /**
     * Vertex Array Object (VAO)
     * Generate & bind the VAO first, before generating and configuring the VBO(s) and EBO(s)
     */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);  // bind the Vertex Array Object first

    // 14. copy the vertices array in a vertex buffer (VBO) for OpenGL to use
    /**
     * Vertex Buffer Object (VBO)
     * then bind and set the vertex buffer(s), and then configure vertex attributes(s)
     */
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // 15. copy our index array in a element buffer for OpenGL to use
    /**
     * Element Buffer Object (EBO) setup
     * ------------------------------------------------------------------------------------------
     * EBOs eliminate the problem of overlapping vertices. We instead only need to store the unique vertices in a float array.
     * Then we declare a uint array that stores the indices of the vertices to draw, basically how OpenGL knows to draw the shapes.
     * ------------------------------------------------------------------------------------------
     * Similar to the VBO we bind the EBO and copy the indices into the buffer with glBufferData.
     * And just like the VBO we place those calls between a VAO bind and an unbind call, 
     * although this time we specify GL_ELEMENT_ARRAY_BUFFER as the buffer type.
     */
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // 16. set the vertex attributes pointers
    /**
     * Linking Vertex Attributes  (telling OpenGL how to interpret the vertex data)
     * ============================================================================
     * The function (glVertexAttribPointer) has quite a few parameters so let's carefully walk through them:
     *   - The 1st parameter specifies which vertex attribute we want to configure. Remember that we specified the location of the position vertex attribute in the vertex shader with layout (location = 0). This sets the location of the vertex attribute to 0 and since we want to pass data to this vertex attribute, we pass in 0.
     *   - The 2nd argument specifies the size of the vertex attribute. The vertex attribute is a vec3 so it is composed of 3 values.
     *   - The 3rd argument specifies the type of the data which is GL_FLOAT (a vec* in GLSL consists of floating point values).
     *   - The 4th argument specifies if we want the data to be normalized. If we're inputting integer data types (int, byte) and we've set this to GL_TRUE, the integer data is normalized to 0 (or -1 for signed data) and 1 when converted to float. This is not relevant for us so we'll leave this at GL_FALSE.
     *   - The 5th argument is known as the stride and tells us the space between consecutive vertex attributes. Since the next set of position data is located exactly 3 times the size of a float away we specify that value as the stride. Note that since we know that the array is tightly packed (there is no space between the next vertex attribute value) we could've also specified the stride as 0 to let OpenGL determine the stride (this only works when values are tightly packed). Whenever we have more vertex attributes we have to carefully define the spacing between each vertex attribute but we'll get to see more examples of that later on.
     *   - The last parameter is of type void* and thus requires that weird cast. This is the offset of where the position data begins in the buffer. Since the position data is at the start of the data array this value is just 0.
     * 
     * After specifying how OpenGL should interpret the vertex data we enable the vertex attribute with glEnableVertexAttribArray 
     * giving the vertex attribute location as its argument; vertex attributes are disabled by default. 
     * From that point on we have everything set up: we initialized the vertex data in a buffer using a vertex buffer object, 
     * set up a vertex and fragment shader and told OpenGL how to link the vertex data to the vertex shader's vertex attributes.
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // tells OpenGL how to interpret the vertex data
    glEnableVertexAttribArray(0);  // enables the vertex attribute, giving the vertex attribute location (from the vertexShaderSource, location = 0) as its argument

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // REMEMBER: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // ***** uncomment this call to draw in wireframe polygons *****//
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // default mode

    ////////////////////////////////////////////  END OF TRIANGLE RENDERING PROCESS ////////////////////////////////////////////
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // render stuff
        glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /**
         * Drawing the first triangle
         * --------------------------
         * As soon as we want to draw an object, we simply bind the VAO with the preferred settings before calling the function to draw the object and that is it.
         */
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);  // seeing as we only have a single VAO there's no need to bind it everytime, but we'll do so to keep things a bit more organized
        /**
         * glDrawArrays(mode, first, count)
         *      - mode specifies the type of primitive to render
         *      - first specifies the starting index in the enabled arrays
         *      - count specifies the number of vertices to render (aka how many vertices we want to draw)
         */
        // glDrawArrays(GL_TRIANGLES, 0, 3);  // used when there's only a VBO and no EBO defined

        /**
         * With the EBO defined, replace the glDrawArrays call with glDrawElements to indicate we want to render the triangles from an index buffer.
         * When using glDrawElements we're going to draw using indices provided in the element buffer object currently bound.
         * glDrawElements for when an EBO is defined, to indicate we want to render the triangles from an index buffer.
         * -----------------------------------------------------------------------------------------------------------------------------------------
         * The glDrawElements function takes its indices from the EBO currently bound to the GL_ELEMENT_ARRAY_BUFFER target. 
         * This means we have to bind the corresponding EBO each time we want to render an object with indices which again is a bit cumbersome. 
         * It just so happens that a vertex array object also keeps track of element buffer object bindings. 
         * The last element buffer object that gets bound while a VAO is bound, is stored as the VAO's element buffer object. 
         * Binding to a VAO then also automatically binds that EBO.
         */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);  // no need to unbind it everytime. but i'll do it anyway cos why not???

        // swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // deallocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // clear all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
