#include "NBodySimulation.h"
#include "NBodyRenderer.h"
#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>

NBodyRenderer* g_Renderer;
bool g_MouseButtonDown;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == 0)
    {
        g_MouseButtonDown = action != 0;
    }
}

int main(int argc, char* argv[])
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(512, 512, "N-Body Simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //NBodySimulation* simulation = new NBodySimulation();
    g_Renderer = new NBodyRenderer();
    g_Renderer->setAspect(1);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    double xpos, ypos = 0, zpos = 0;
    float t = 0;
    while (!glfwWindowShouldClose(window))
    {
        if (g_MouseButtonDown)
        {
            //glfwGetCursorPos(window, &xpos, &ypos);
            t += 0.02;
            xpos = cos(t) * 10;
            zpos = sin(t) * 10;
            g_Renderer->setCameraPosition(xpos, ypos, zpos);
        }
        glClear(GL_COLOR_BUFFER_BIT);
        g_Renderer->draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    delete g_Renderer;
    //delete simulator;
    glfwTerminate();
    return 0;
}
