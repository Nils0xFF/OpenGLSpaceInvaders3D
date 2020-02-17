#include "ParticleShader.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

ParticleShader::ParticleShader()
{
	load(ASSET_DIRECTORY "vsparticle.glsl", ASSET_DIRECTORY "fsparticle.glsl");

	ColorLoc = glGetUniformLocation(ShaderProgram, "Color");
	assert(ColorLoc >= 0);
	ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
	assert(ModelViewProjLoc >= 0);
}

void ParticleShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	glUniform3f(ColorLoc, Col.R, Col.G, Col.B);

	Matrix ModelView = Cam.getViewMatrix() * ModelTransform;
	Matrix ModelViewProj = Cam.getProjectionMatrix() * ModelView;
	glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);
}
