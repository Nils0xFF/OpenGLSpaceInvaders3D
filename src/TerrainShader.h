#pragma once
#include "BaseShader.h"

class TerrainShader : public BaseShader
{
public:
    TerrainShader();
    TerrainShader* clone() { return new TerrainShader(*this); }
    virtual void activate(const BaseCamera& Cam) const;

private:
    unsigned int TimeLoc;
    unsigned int ModelViewProjLoc;
};

