#pragma once
#include "BaseShader.h"

class TerrainShader : public BaseShader
{
public:
    TerrainShader(const int worldWidth, const int mountainWidth);
    TerrainShader* clone() { return new TerrainShader(*this); }
    virtual void activate(const BaseCamera& Cam) const;

    void setWorldWidth(const int width) { WorldWidth = width; }
    void setMountainWidth(const int width) { MountainWidth = width; }

private:
    int WorldWidth = 0;
    int MountainWidth = 0;

    unsigned int TimeLoc = 0;
    unsigned int ModelViewProjLoc = 0;
    unsigned int WorldWidthLoc = 0;
    unsigned int MountainWidthLoc = 0;
};

