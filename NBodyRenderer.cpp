#include "NBodyRenderer.h"
#include <iostream>
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/OpenGL.h>


NBodyRenderer::NBodyRenderer()
{
    m_CameraPosition.x = -1.7;
    m_CameraPosition.y = 19;
    m_CameraPosition.z = -200;
}

void NBodyRenderer::draw() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, m_InverseAspect * -0.1, m_InverseAspect * 0.1, 0.5, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPointSize(3);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_POINTS, 0, m_BodyCount);
}

void NBodyRenderer::setCameraPosition(float x, float y, float z)
{
    m_CameraPosition.x = x;
    m_CameraPosition.y = y;
    m_CameraPosition.z = z;
}

void NBodyRenderer::getCameraPosition(float* x, float* y, float* z) const
{
    if (x != NULL)
    {
        *x = m_CameraPosition.x;
    }
    if (y != NULL)
    {
        *y = m_CameraPosition.y;
    }
    if (z != NULL)
    {
        *z = m_CameraPosition.z;
    }
}
