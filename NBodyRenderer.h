#pragma once
#include <OpenCL/opencl.h>
#include <OpenGL/OpenGL.h>

class NBodyRenderer
{
public:
    NBodyRenderer();
    void draw() const;
    void setAspect(float aspect) { m_InverseAspect = 1 / aspect; }
    void setCameraPosition(float x, float y, float z);
    void getCameraPosition(float* x, float* y, float* z) const;
    void setVBO(GLuint vbo) { m_VBO = vbo; }
    void setBodyCount(int bodyCount) { m_BodyCount = bodyCount; }

private:
    float       m_InverseAspect;
    cl_float3   m_CameraPosition;
    GLuint      m_VBO;
    int         m_BodyCount;
};
