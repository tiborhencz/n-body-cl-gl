#include "NBodySimulation.h"
#include <fstream>
#include <iostream>
#include <OpenCL/OpenCL.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>

#define CL_CHK_ERR(err) do { if (err) { printf("Error %i in " __FILE__":%i\n", err, __LINE__); exit(1); } } while (0)
#define CL_CHK_ERR_EXP(err, exp) do { if (err) { printf("Error %i when running "#exp"; in " __FILE__":%i\n", err, __LINE__); exit(1); } } while (0)
#define CL_CHK(exp) do { cl_int err = exp; CL_CHK_ERR_EXP(err, exp); } while (0)

using namespace std;

struct BodyDescription
{
    float mass;
    float px, py, pz;
    float vx, vy, vz;
};

NBodySimulation::NBodySimulation()
{
    m_BodyDescriptions = NULL;
    m_VBO = 0;
    m_CLBuffer = 0;

    cl_platform_id  platform;
    CL_CHK(clGetPlatformIDs(1, &platform, NULL));
    CL_CHK(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &m_Device, NULL));
    cl_int clErr;
    CGLContextObj kCGLContext = CGLGetCurrentContext();
    CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
    cl_context_properties props[] =
    {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)kCGLShareGroup,
        0
    };
    m_Context = clCreateContext(props, 1, &m_Device, NULL, NULL, &clErr);
    CL_CHK_ERR(clErr);
    m_CommandQueue = clCreateCommandQueue(m_Context, m_Device, 0, &clErr);
    CL_CHK_ERR(clErr);
    initKernel("kernel.cl", "calculate_forces");

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(BodyDescription), (void*)4);

    cl_int err;
    m_CLBuffer = clCreateFromGLBuffer(m_Context, CL_MEM_READ_WRITE, m_VBO, &err);
    clSetKernelArg(m_Kernel, 0, sizeof(cl_mem), &m_CLBuffer);
    CL_CHK_ERR(err);
}

NBodySimulation::~NBodySimulation()
{
    clReleaseKernel(m_Kernel);
    clReleaseCommandQueue(m_CommandQueue);
    clReleaseContext(m_Context);
    glDeleteBuffers(1, &m_VBO);
    clReleaseMemObject(m_CLBuffer);
}

void NBodySimulation::initKernel(const char* path, const char* kernelName)
{
    ifstream kernelFile;
    kernelFile.open(path);
    if (!kernelFile.is_open())
    {
        cerr << "Unable to open kernel at: " << path;
        exit(1);
    }
    string kernelText((istreambuf_iterator<char>(kernelFile)),
                         istreambuf_iterator<char>());
    size_t size = kernelText.length();
    const char* source = kernelText.c_str();

    cl_int clErr;
    cl_program program = clCreateProgramWithSource(m_Context, 1, &source, &size, &clErr);
    CL_CHK_ERR(clErr);
    
    clErr = clBuildProgram(program, 1, &m_Device, NULL, NULL, NULL);
    if (clErr != CL_SUCCESS)
    {
        char log[2048];
        size_t logLength;
        CL_CHK(clGetProgramBuildInfo(program, m_Device, CL_PROGRAM_BUILD_LOG, 2048, log, &logLength));
        cerr << "Error building kernel: " << log;
        exit(1);
    }
    
    m_Kernel = clCreateKernel(program, kernelName, &clErr);
    CL_CHK_ERR(clErr);
    clReleaseProgram(program);
}

void NBodySimulation::simulate(float time)
{
    clSetKernelArg(m_Kernel, 2, sizeof(float), &time);
    CL_CHK(clEnqueueAcquireGLObjects(m_CommandQueue, 1, &m_CLBuffer, 0, NULL, NULL));
    size_t workSize = m_BodyCount;
    CL_CHK(clEnqueueNDRangeKernel(m_CommandQueue, m_Kernel, 1, NULL, &workSize, NULL, 0, NULL, NULL));
    CL_CHK(clEnqueueReleaseGLObjects(m_CommandQueue, 1, &m_CLBuffer, 0, NULL, NULL));
}

void NBodySimulation::loadSimulationDescription(const char* path, int maxBodies)
{
    ifstream descFile;
    descFile.open(path);
    if (!descFile.is_open())
    {
        cerr << "Unable to open simulation description at: " << path;
        exit(1);
    }
    string line;
    BodyDescription* descriptions = new BodyDescription[maxBodies];
    m_BodyCount = 0;
    while (getline(descFile, line) && m_BodyCount < maxBodies)
    {
        BodyDescription& desc = descriptions[m_BodyCount++];
        sscanf(line.c_str(), "%f %f %f %f %f %f %f", &desc.mass, &desc.px, &desc.py, &desc.pz,
               &desc.vx, &desc.vy, &desc.vz);
    }
    clSetKernelArg(m_Kernel, 1, sizeof(int), &m_BodyCount);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BodyDescription) * m_BodyCount, descriptions, GL_DYNAMIC_DRAW);
    delete[] descriptions;
}
