#pragma once
#include <OpenCL/opencl.h>

class NBodyRenderer
{
public:
    NBodyRenderer();
    void draw() const;
    void setAspect(float aspect) { m_InverseAspect = 1 / aspect; }
    void setCameraPosition(float x, float y, float z);

private:
    float       m_InverseAspect;
    cl_float3   m_CameraPosition;
};
