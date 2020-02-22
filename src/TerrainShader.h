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
    int MountainWidth = GameSettings::MOUNTAIN_WIDTH;

    unsigned int TimeLoc = 0;
    unsigned int ModelViewProjLoc = 0;
    unsigned int WorldWidthLoc = 0;
    unsigned int MountainWidthLoc = 0;
};

