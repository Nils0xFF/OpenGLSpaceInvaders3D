#include "TerrainShader.h"
#include "GameSettings.h"

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
	MountainWidthLoc = glGetUniformLocation(ShaderProgram, "MountainWidth");
	assert(MountainWidthLoc >= 0);
}

void TerrainShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	glUniform1f(TimeLoc, (GLfloat) glfwGetTime());
	glUniform1i(WorldWidthLoc, WorldWidth);
	glUniform1i(MountainWidthLoc, MountainWidth);

	Matrix ModelView = Cam.getViewMatrix() * ModelTransform;
	Matrix ModelViewProj = Cam.getProjectionMatrix() * ModelView;
	glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);
}
