#include "NBodySimulation.h"
#include <fstream>
#include <iostream>
#include <OpenCL/OpenCL.h>
#include <OpenGL/gl.h>

#define CL_CHK_ERR(err) do { if (err) { printf("Error %i in " __FILE__":%i\n", err, __LINE__); exit(1); } } while (0)
#define CL_CHK_ERR_EXP(err, exp) do { if (err) { printf("Error %i when running "#exp"; in " __FILE__":%i\n", err, __LINE__); exit(1); } } while (0)
#define CL_CHK(exp) do { cl_int err = exp; CL_CHK_ERR_EXP(err, exp); } while (0)

using namespace std;

struct BodyDescriptions
{
    cl_float*   mass;
    cl_float3*  position;
    cl_float3*  velocity;
    
    BodyDescriptions(int size)
    {
        mass = new cl_float[size];
        position = new cl_float3[size];
        velocity = new cl_float3[size];
    }
    
    ~BodyDescriptions()
    {
        delete[] mass;
        delete[] position;
        delete[] velocity;
    }

private:
    BodyDescriptions(const BodyDescriptions&);
    BodyDescriptions& operator=(const BodyDescriptions&);
};

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

    cl_platform_id  platform;
    CL_CHK(clGetPlatformIDs(1, &platform, NULL));
    CL_CHK(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &m_Device, NULL));
    cl_int clErr;
    m_Context = clCreateContext(NULL, 1, &m_Device, NULL, NULL, &clErr);
    CL_CHK_ERR(clErr);
    m_CommandQueue = clCreateCommandQueue(m_Context, m_Device, 0, &clErr);
    CL_CHK_ERR(clErr);
    cout << "found device:" << m_Device << '\n';
    
    //initKernel("", "");
}

NBodySimulation::~NBodySimulation()
{
    clReleaseKernel(m_Kernel);
    clReleaseCommandQueue(m_CommandQueue);
    clReleaseContext(m_Context);
    delete m_BodyDescriptions;
}

void NBodySimulation::initKernel(const char* path, const char* kernelName)
{
    ifstream kernelFile;
    size_t size;
    kernelFile.open(path);
    if (!kernelFile.is_open())
    {
        cerr << "Unable to open kernel at: " << path;
        exit(1);
    }
    kernelFile.seekg(0, ios::end);
    size = kernelFile.tellg();
    kernelFile.seekg(0, ios::beg);
    char* kernelText = new char[size];
    kernelFile.read(kernelText, size);
    kernelFile.close();

    cl_int clErr;
    cl_program program = clCreateProgramWithSource(m_Context, 1, (const char**)&kernelText, &size, &clErr);
    CL_CHK_ERR(clErr);
    delete[] kernelText;
    
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

void NBodySimulation::simulate()
{
    size_t workSize = 1;
    CL_CHK(clEnqueueNDRangeKernel(m_CommandQueue, m_Kernel, 1, NULL, &workSize, NULL, 0, NULL, NULL));
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
    int bodiesRead = 0;
    while (getline(descFile, line) && bodiesRead < maxBodies)
    {
        BodyDescription& desc = descriptions[bodiesRead++];
        sscanf(line.c_str(), "%f %f %f %f %f %f %f", &desc.mass, &desc.px, &desc.py, &desc.pz,
               &desc.vx, &desc.vy, &desc.vz);
    }
    
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BodyDescription) * bodiesRead, descriptions, GL_STATIC_DRAW);
}
