#include "TerrainShader.h"
#include "GameSettings.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

TerrainShader::TerrainShader()
{
	if (!load(ASSET_DIRECTORY "vsterrain.glsl", ASSET_DIRECTORY "fsterrain.glsl"))
		throw new std::exception("Shader not found!");

	TimeLoc = glGetUniformLocation(ShaderProgram, "Time");
	assert(TimeLoc >= 0);
	ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
	assert(ModelViewProjLoc >= 0);
}

void TerrainShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	glUniform1f(TimeLoc, glfwGetTime());
	// always update matrices
	Matrix ModelView = Cam.getViewMatrix() * ModelTransform;
	Matrix ModelViewProj = Cam.getProjectionMatrix() * ModelView;
	glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);
}
