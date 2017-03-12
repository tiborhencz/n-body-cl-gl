#pragma once
#include <OpenCL/OpenCL.h>

struct BodyDescriptions;

class NBodySimulation
{
public:
    NBodySimulation();
    ~NBodySimulation();
    void simulate();
    void loadSimulationDescription(const char* path, int maxBodies);
    
private:
    void initKernel(const char* path, const char* kernelName);
    
private:
    cl_device_id        m_Device;
    cl_context          m_Context;
    cl_kernel           m_Kernel;
    cl_command_queue    m_CommandQueue;
    BodyDescriptions*   m_BodyDescriptions;
    GLuint              m_VBO;
};
