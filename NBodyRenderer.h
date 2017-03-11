#pragma once

class NBodyRenderer
{
public:
    NBodyRenderer();
    void draw() const;
    void setAspect(float aspect) { m_InverseAspect = 1 / aspect; }

private:
    float   m_InverseAspect;
};
