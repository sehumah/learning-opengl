#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


/**
 * 
 * Compile and execute OpenGL prgram with: g++ 1_hello_window.cpp glad/src/glad.c -ldl -lglfw -o a.out; ./a.out
 * 
 */


/////*** FUNCTION PROTOTYPES ***/////

/**
 * Callback function on the window that gets called each time the GLFW window is resized.
 * It takes a GLFWwindow as its first argument and two integers indicating the new window dimensions. 
 * Whenever the window changes in size, GLFW calls this function and fills in the proper arguments for you to process.
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


/**
 * We also want to have some form of input control in GLFW and we can achieve this with several of GLFW's input functions. 
 * We'll be using GLFW's glfwGetKey function that takes the window as input together with a key. 
 * The function returns whether this key is currently being pressed. 
 * This processInput function with keep all input code organized.
 */
void processInput(GLFWwindow* window);



int main () {  // here, we will instantiate & configure the GLFW window
    
    // 1st. Initialize GLFW
    glfwInit();

    /**
     * 2nd. Configure GLFW using glfwWindowHint
     * The first argument of glfwWindowHint tells us what option we want to configure, 
     * where we can select the option from a large enum of possible options prefixed with GLFW_. 
     * The second argument is an integer that sets the value of our option.
     * 
     * 
     * Since the focus of this book is on OpenGL version 3.3 we'd like to tell GLFW that 3.3 is the OpenGL version we want to use. 
     * This way GLFW can make the proper arrangements when creating the OpenGL context. 
     * This ensures that when a user does not have the proper OpenGL version GLFW fails to run. 
     * We set the major and minor version both to 3. We also tell GLFW we want to explicitly use the core-profile. 
     * Telling GLFW we want to use the core-profile means we'll get access to a smaller subset of OpenGL features 
     * without backwards-compatible features we no longer need.
     */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // for MacOS
    #endif

    // 3rd. Next, we're required to create a window object which holds all the windowing data and is required by most of GLFWs other functions.
    //      glfwCreateWindow returns a GLFWwindow object that we'll later need for other GLFW operations.
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window - LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 4th. Tell GLFW to make the context of our window the main context on the current thread
    glfwMakeContextCurrent(window);


    /**
     * 
     * GLAD
     * 
     * 
     * GLAD manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function
     * 
     * We pass GLAD the function to load the address of the OpenGL function pointers which is OS-specific. 
     * GLFW gives us glfwGetProcAddress that defines the correct function based on which OS we're compiling for.
     */

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // print OpenGL version after loading the function pointers
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;


    /**
     * 
     * Viewport
     * 
     * 
     * Before we can start rendering we have to tell OpenGL the size of the rendering window so OpenGL knows how we want to display 
     * the data and coordinates with respect to the window. We can set those dimensions via the glViewport function:
     * 
     * The first two parameters of glViewport set the location of the lower left corner of the window. 
     * The third and fourth parameter set the width and height of the rendering window in pixels, which we set equal to GLFW's window size.
     */
    glViewport(0, 0, 800, 600);

    // Tell GLFW that we want to call the framebuffer_size_callback function on every window resize by registering it:
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



    /////***** We register the callback functions after we've created the window and before the render loop is initiated. *****/////



    /**
     * 
     * Ready your engines
     * 
     * 
     * We don't want the application to draw a single image and then immediately quit and close the window. 
     * We want the application to keep drawing images and handling user input until the program has been explicitly told to stop. 
     * For this reason we have to create a while loop, that we now call the render loop, that keeps on running until we tell GLFW to stop.
     * The following code shows a very simple render loop:
     */

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);  // call processInput every iteration of the render loop

        /**
         * 
         * Rendering
         * 
         * We want to place all the rendering commands in the render loop, since we want to execute 
         * all the rendering commands each iteration or frame of the loop.
         */

        // rendering commands here .....

        /**
         * Just to test if things actually work we want to clear the screen with a color of our choice. 
         * At the start of frame we want to clear the screen. 
         * Otherwise we would still see the results from the previous frame (this could be the effect you're looking for, but usually you don't). 
         * We can clear the screen's color buffer using glClear where we pass in buffer bits to specify which buffer we would like to clear. 
         * The possible bits we can set are GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT. 
         * Right now we only care about the color values so we only clear the color buffer.
         */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    /**
     * 
     * One last thing
     * 
     * As soon as we exit the render loop we would like to properly clean/delete all of GLFW's resources that were allocated. 
     * We can do this via the glfwTerminate function that we call at the end of the main function.
     * This will clean up all the resources and properly exit the application.
     */
    glfwTerminate();
    return 0;
}





/////*** FUNCTION DEFINITIONS ***/////

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
