#pragma once
#include <OpenCL/OpenCL.h>

class NBodySimulation
{
public:
    NBodySimulation();
    ~NBodySimulation();
    void simulate();
    
private:
    void initKernel(const char* path, const char* kernelName);
    
private:
    cl_device_id        m_Device;
    cl_context          m_Context;
    cl_kernel           m_Kernel;
    cl_command_queue    m_CommandQueue;
};
