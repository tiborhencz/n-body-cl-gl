#include "NBodyRenderer.h"
#include <OpenGL/gl.h>
#include <iostream>

NBodyRenderer::NBodyRenderer()
{
}

void NBodyRenderer::draw() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glTranslatef(0, 0, 0);
    glFrustum(-0.1, 0.1, m_InverseAspect * -0.1, m_InverseAspect * 0.1, 0.5, 1000.0);
    std::cout << m_InverseAspect << '\n';
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0, 0.0, -10.0);
    glVertex3f(1.0, 0.0, -10.0);
    glVertex3f(1.0, 1.0, -10.0);
    
    glVertex3f(0.0, 0.0, -10.0);
    glVertex3f(0.0, -1.0, -10.0);
    glVertex3f(1.0, -1.0, -10.0);
    
    glVertex3f(0.0, 0.0, -10.0);
    glVertex3f(-1.0, 0.0, -10.0);
    glVertex3f(-1.0, -1.0, -10.0);
    
    glVertex3f(0.0, 0.0, -10.0);
    glVertex3f(0.0, 1.0, -10.0);
    glVertex3f(-1.0, 1.0, -10.0);
    glEnd();
}
