#include <iostream>
#include <iomanip>

#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include"../include/config.h"
#include "ScreenInteractor.h"

using namespace std;

int frame_index = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << "Mouse clicked at: (" << xpos << ", " << ypos << ")" << std::endl;
    }
}

void renderButton() {
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.8f, 0.3f); // Set button color
    glVertex2f(buttonX, buttonY); // Bottom-left corner
    glVertex2f(buttonX + buttonWidth, buttonY); // Bottom-right corner
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight); // Top-right corner
    glVertex2f(buttonX, buttonY + buttonHeight); // Top-left corner
    glEnd();
}

int initApplication()
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    else return 1;
}

int update()
{
    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "4419 Rewind Path App", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Input handling
        processInput(window);


        // Rendering commands
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the button
        renderButton();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up GLFW resources before exiting
    glfwTerminate();
    return 0;
}

int main()
{
    while (true)
    {
        ScreenInteractor::click(100, 100);

    }
    // if(frame_index == 0)
    // {
    //     int init_return = initApplication();
    //     if(init_return == -1) return -1;
    // }

    // int return_val = update();
    // return return_val;
}






