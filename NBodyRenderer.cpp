#include "NBodyRenderer.h"
#include <iostream>
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/OpenGL.h>


NBodyRenderer::NBodyRenderer()
{
    m_CameraPosition.x = 0;
    m_CameraPosition.y = 0;
    m_CameraPosition.z = 0;
}

void NBodyRenderer::draw() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, m_InverseAspect * -0.1, m_InverseAspect * 0.1, 0.5, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
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

void NBodyRenderer::setCameraPosition(float x, float y, float z)
{
    m_CameraPosition.x = x;
    m_CameraPosition.y = y;
    m_CameraPosition.z = z;
}
