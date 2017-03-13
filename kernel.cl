typedef struct BodyDescription
{
    float mass;
    float px, py, pz;
    float vx, vy, vz;
} BodyDescription;

__kernel void calculate_forces(__global BodyDescription* devX)
{
    int i = get_global_id(0);

    float3 r;
    float3 ai = 0;
    
    for (int j = 0; j < 4000; j++)
    {
        float mj = devX[j].mass;
        r.x = devX[j].px - devX[i].px;
        r.y = devX[j].py - devX[i].py;
        r.z = devX[j].pz - devX[i].pz;
        
        float distSqr = r.x * r.x + r.y * r.y + r.z * r.z + 0.01;
        float distSixth = distSqr * distSqr * distSqr;
        float invDistCube = 1.0f/sqrt(distSixth);
        float s = mj * invDistCube * 10;
        
        ai.x += r.x * s;
        ai.y += r.y * s;
        ai.z += r.z * s;
    }
    devX[i].vx += ai.x * 0.016;
    devX[i].vy += ai.y * 0.016;
    devX[i].vz += ai.z * 0.016;
    
    devX[i].px += devX[i].vx * 0.016;
    devX[i].py += devX[i].vy * 0.016;
    devX[i].pz += devX[i].vz * 0.016;
}
