#include "TerrainShader.h"
#include "GameSettings.h"
#include "rgbimage.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

TerrainShader::TerrainShader()
{
	if (!load(SHADER_DIRECTORY "vertex/vsterrain.glsl", SHADER_DIRECTORY "fragment/fsterrain.glsl"))
		throw new std::exception("Shader not found!");


	TimeLoc = glGetUniformLocation(ShaderProgram, "Time");
	assert(TimeLoc >= 0);
	ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
	assert(ModelViewProjLoc >= 0);
	WorldWidthLoc = glGetUniformLocation(ShaderProgram, "WorldWidth");
	assert(WorldWidthLoc >= 0);
	WorldDepthLoc = glGetUniformLocation(ShaderProgram, "WorldDepth");
	assert(WorldDepthLoc >= 0);
	MountainWidthLoc = glGetUniformLocation(ShaderProgram, "MountainWidth");
	assert(MountainWidthLoc >= 0);
	NoiseMapLoc = getParameterID("NoiseMap");
	assert(NoiseMapLoc >= 0);
	initNoiseTexture();
}

void TerrainShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	glUniform1f(TimeLoc, (GLfloat) glfwGetTime());
	glUniform1f(WorldWidthLoc, WorldWidth);
	glUniform1f(WorldDepthLoc, WorldDepth);
	glUniform1f(MountainWidthLoc, MountainWidth);

	Matrix ModelView = Cam.getViewMatrix() * ModelTransform;
	Matrix ModelViewProj = Cam.getProjectionMatrix() * ModelView;
	glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);

	// glActiveTexture(GL_TEXTURE0);

	NoiseMapTexture->activate(NoiseMapLoc);
	glUniform1i(NoiseMapLoc, 2);

	
}

void TerrainShader::deactivate()
{
	BaseShader::deactivate();
	NoiseMapTexture->deactivate();
}

void TerrainShader::initNoiseTexture()
{
	int sizeX = 200;
	int sizeZ = 200;
	/* float* noiseMap = new float[sizeX * sizeZ];
	for (int z = 0; z < sizeZ; z++) {
		for (int x = 0; x < sizeX; x++) {
			
			noiseMap[z * (WorldWidth + 2 * MountainWidth) + x] = perlinValue;
		}
	}*/

	NoiseMapImage = new RGBImage(sizeX, sizeZ);
	for (int z = 0; z < sizeZ; z++) {
		for (int x = 0; x < sizeX; x++) {
			float perlinValue = siv::PerlinNoise().octaveNoise0_1(x / 20.0f, z / 20.0f, 4);
			NoiseMapImage->setPixelColor(x,z, 
				Color(
					perlinValue, 
					perlinValue,
					perlinValue
				));
		}
	}

	NoiseMapImage->saveToDisk(ASSET_DIRECTORY "NoiseMap.jpg");
	bool loaded = NoiseMapTexture->create(*NoiseMapImage);

	assert(loaded);
	// delete[] noiseMap;
}
