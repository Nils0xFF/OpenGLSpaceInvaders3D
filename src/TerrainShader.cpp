#include "TerrainShader.h"
#include "GameSettings.h"
#include "rgbimage.h"
#include "PerlinNoiseGenerator.h"

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
	glUniform1f(WorldWidthLoc, (float) WorldWidth);
	glUniform1f(WorldDepthLoc, (float) WorldDepth);
	glUniform1f(MountainWidthLoc, (float) MountainWidth);

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
	int sizeX = 500;
	int sizeZ = 500;

	std::vector<float> noiseMap = PerlinNoiseGenerator::generateNoiseMap(sizeX, sizeZ, 554532, 35.0f, 5, 0.35f, 1.75f, Vector(0,0,0));


	NoiseMapImage = new RGBImage(sizeX, sizeZ);
	for (int z = 0; z < sizeZ; z++) {
		for (int x = 0; x < sizeX; x++) {
			float perlinValue = noiseMap[z * sizeX + x];
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
