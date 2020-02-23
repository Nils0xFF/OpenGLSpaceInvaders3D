#include "PerlinNoiseGenerator.h"
#include "PerlinNoise.hpp"
#include "vector.h"
#include <limits>
#include <cassert>

using namespace siv;
using namespace std;

std::vector<float> PerlinNoiseGenerator::generateNoiseMap(int mapWidth, int mapHeight, int seed, float scale, int octaves, float persistance, float lacunarity, Vector& offset)
{
	vector<float> noiseMap;
	PerlinNoise noise;
	// noise.reseed(seed);
	vector<Vector> octavesOffsets;
	for (int i = 0; i < octaves; i++) {
		float offsetX = (float) noise.noise(0.25) * 1000 + offset.X;
		float offsetY = (float) noise.noise(0.75) * 1000 + offset.Y;
		octavesOffsets.push_back(Vector(offsetX, offsetY, 0));
	}

	if (scale <= 0) {
		scale = 0.0001f;
	}

	float minNoiseHeight = FLT_MAX;
	float maxNoiseHeight = -FLT_MAX;

	float halfWidth = mapWidth / 2.0f;
	float halfHeight = mapHeight / 2.0f;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			float amplitude = 1;
			float frequency = 1;
			float noiseHeigth = 0;

			for (int i = 0; i < octaves; i++) {
				float sampleX = (x - halfWidth) / scale * frequency + octavesOffsets[i].X;
				float sampleY = (y - halfHeight) / scale * frequency + octavesOffsets[i].Y;

				float perlinValue = (float) noise.noise(sampleX, sampleY) * 2 - 1;
				noiseHeigth += perlinValue * amplitude;

				amplitude *= persistance;
				frequency *= lacunarity;
			}

			if (noiseHeigth > maxNoiseHeight) {
				maxNoiseHeight = noiseHeigth;
			}
			else if (noiseHeigth < minNoiseHeight) {
				minNoiseHeight = noiseHeigth;
			}

			noiseMap.push_back(noiseHeigth);
		}
	}

	for (int i = 0; i < mapHeight * mapWidth; i++) {
		noiseMap[i] = (noiseMap[i] - minNoiseHeight) / (maxNoiseHeight - minNoiseHeight);
	}

	return noiseMap;
}
