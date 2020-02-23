#pragma once
#include <vector>
#include "vector.h"

class PerlinNoiseGenerator
{
public:
	static std::vector<float> generateNoiseMap(int mapWidth, int mapHeight, int seed, float scale, int octaves, float persistance, float lacunarity, Vector& offset);
};

