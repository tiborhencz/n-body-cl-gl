#include "NBodySimulation.h"
#include "NBodyRenderer.h"
#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>

#define N_BODIES 8000
#define DATASET_FILE "dubinski.tab"

NBodySimulation*    g_Simulation;
NBodyRenderer*      g_Renderer;
bool                g_QuitPressed = false;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R && action == 1)
    {
        g_Simulation->loadSimulationDescription(DATASET_FILE, N_BODIES);
    }
    else if (key == GLFW_KEY_Q)
    {
        g_QuitPressed = true;
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
    g_Simulation = new NBodySimulation();
    g_Simulation->loadSimulationDescription(DATASET_FILE, N_BODIES);
    g_Renderer = new NBodyRenderer();
    g_Renderer->setVBO(g_Simulation->getVBO());
    g_Renderer->setBodyCount(g_Simulation->getBodyCount());
    g_Renderer->setAspect(1);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glfwSetKeyCallback(window, keyCallback);

    double lastTime = glfwGetTime();
    double previousFrameTime = lastTime;
    int nbFrames = 0;
    while (!glfwWindowShouldClose(window) && !g_QuitPressed)
    {
        nbFrames++;
        double currentTime = glfwGetTime();
        g_Simulation->simulate(currentTime - previousFrameTime);
        glClear(GL_COLOR_BUFFER_BIT);
        g_Renderer->draw();
        if (currentTime - lastTime >= 1.0)
        {
            std::cout << nbFrames << "FPS\n";
            nbFrames = 0;
            lastTime += 1.0;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
        previousFrameTime = currentTime;
    }
    delete g_Renderer;
    delete g_Simulation;

    glfwTerminate();
    return 0;
}
