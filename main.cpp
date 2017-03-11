#include "NBodySimulation.h"
#include "NBodyRenderer.h"
#include <GLUT/GLUT.h>

NBodyRenderer* g_Renderer = NULL;

void display()
{
    g_Renderer->draw();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    g_Renderer->setAspect(w / (float)h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



int main(int argc, char* argv[])
{
    //NBodySimulation* simulation = new NBodySimulation();
    g_Renderer = new NBodyRenderer();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("N-Body Simulation");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glutMainLoop();
    return 0;
}
