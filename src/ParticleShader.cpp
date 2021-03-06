#include "ParticleShader.h"
#include "GameSettings.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

ParticleShader::ParticleShader(): Col(0.0f, 0.0f, 0.0f), Alpha(1.0f)
{
	if (!load(ASSET_DIRECTORY "shader/vertex/vsparticle.glsl", ASSET_DIRECTORY "shader/fragment/fsparticle.glsl"))
		throw new std::exception("Shader not found.");

	ColorLoc = glGetUniformLocation(ShaderProgram, "color");
	assert(ColorLoc >= 0);
	AlphaLoc = glGetUniformLocation(ShaderProgram, "alpha");
	assert(AlphaLoc >= 0);
	ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
	assert(ModelViewProjLoc >= 0);

	WorldDepthLoc = glGetUniformLocation(ShaderProgram, "WorldDepth");
}

void ParticleShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	glUniform3f(ColorLoc, Col.R, Col.G, Col.B);
	glUniform1f(AlphaLoc, Alpha);
	glUniform1i(WorldDepthLoc, GameSettings::WORLD_DEPTH);

	Matrix View = Cam.getViewMatrix();
	Matrix Model = ModelTransform;
	// Transposing rotation
	Model.m00 = View.m00;
	Model.m10 = View.m01;
	Model.m20 = View.m02;
	Model.m01 = View.m10;
	Model.m11 = View.m11;
	Model.m21 = View.m12;
	Model.m02 = View.m20;
	Model.m12 = View.m21;
	Model.m22 = View.m22;
	Matrix ModelView = View * Model;

	Matrix ModelViewProj = Cam.getProjectionMatrix() * ModelView * Matrix().translation(ModelTransform.translation()).invert() * ModelTransform;
	glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);
}
