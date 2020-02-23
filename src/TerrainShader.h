#pragma once
#include "BaseShader.h"
#include "GameSettings.h"

class TerrainShader : public BaseShader
{
public:
	TerrainShader();
    TerrainShader* clone() { return new TerrainShader(*this); }
    virtual void activate(const BaseCamera& Cam) const;


private:
    int WorldWidth = GameSettings::WORLD_WIDTH;
    int WorldDepth = GameSettings::WORLD_DEPTH;
    int MountainWidth = GameSettings::MOUNTAIN_WIDTH;

    unsigned int TimeLoc = 0;
    int ModelViewProjLoc = 0;
    int WorldWidthLoc = 0;
    int WorldDepthLoc = 0;
    int MountainWidthLoc = 0;
};

