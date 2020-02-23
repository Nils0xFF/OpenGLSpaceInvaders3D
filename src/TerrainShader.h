#pragma once
#include "BaseShader.h"
#include "GameSettings.h"
#include "Texture.h"
#include "PerlinNoise.hpp"
#include "rgbimage.h"

class TerrainShader : public BaseShader
{
public:
	TerrainShader();
	~TerrainShader() { delete NoiseMapTexture; delete NoiseMapImage; }
    TerrainShader* clone() { return new TerrainShader(*this); }
    virtual void activate(const BaseCamera& Cam) const;
	virtual void deactivate();


private:
	RGBImage* NoiseMapImage = NULL;
	void initNoiseTexture();

    float WorldWidth = GameSettings::WORLD_WIDTH;
    float WorldDepth = GameSettings::WORLD_DEPTH;
    float MountainWidth = GameSettings::MOUNTAIN_WIDTH;

	Texture* NoiseMapTexture = Texture::defaultTex();

    unsigned int TimeLoc = 0;
	unsigned int NoiseMapLoc = 0;
    unsigned int ModelViewProjLoc = 0;
    unsigned int WorldWidthLoc = 0;
	unsigned int WorldDepthLoc = 0;
    unsigned int MountainWidthLoc = 0;
};

