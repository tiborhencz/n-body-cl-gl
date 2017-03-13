struct BodyDescription
{
    float mass;
    float px, py, pz;
    float vx, vy, vz;
};

__kernel void calculate_forces(__global struct BodyDescription* devX, const int bodyCount, const float time)
{
    int i = get_global_id(0);

    float3 r;
    float3 ai = 0;
    
    #pragma unroll 128
    for (int j = 0; j < bodyCount; j++)
    {
        float mj = devX[j].mass;
        r.x = devX[j].px - devX[i].px;
        r.y = devX[j].py - devX[i].py;
        r.z = devX[j].pz - devX[i].pz;
        
        float distSqr = mad(r.x, r.x, mad(r.y, r.y, r.z * r.z)) + 0.01;
        float distSixth = distSqr * distSqr * distSqr;
        float invDistCube = native_rsqrt(distSixth);
        float s = mj * invDistCube;
        
        ai.x += r.x * s;
        ai.y += r.y * s;
        ai.z += r.z * s;
    }
    
    devX[i].vx += ai.x * time;
    devX[i].vy += ai.y * time;
    devX[i].vz += ai.z * time;

    devX[i].px += devX[i].vx * time;
    devX[i].py += devX[i].vy * time;
    devX[i].pz += devX[i].vz * time;
}
