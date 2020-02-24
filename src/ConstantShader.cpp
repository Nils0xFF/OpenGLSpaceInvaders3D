//
//  ConstantShader.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "ConstantShader.h"
#include "GameSettings.h"

const char *CVertexShaderCode =
"#version 400\n"
"in vec4 VertexPos;"
"uniform mat4 ModelViewProjMat;"
"void main()"
"{"
"    gl_Position = ModelViewProjMat * VertexPos;"
"}";

const char *CFragmentShaderCode =
"#version 400\n"
"uniform vec3 Color;"
"out vec4 FragColor;"
"void main()"
"{"
"    FragColor = vec4(Color,1);"
"}";

ConstantShader::ConstantShader() : Col(1.0f,0.0f,0.0f)
{
    // ShaderProgram = createShaderProgram( CVertexShaderCode, CFragmentShaderCode );
	bool loaded = load(SHADER_DIRECTORY "vertex/vsconstant.glsl", SHADER_DIRECTORY "fragment/fsconstant.glsl");
	if (!loaded)
		throw std::exception();

    ColorLoc = glGetUniformLocation(ShaderProgram, "Color");
    assert(ColorLoc>=0);

    ModelViewProjLoc  = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
    assert(ModelViewProjLoc >=0);

	ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
	// assert(ModelMatLoc >= 0);

	WorldDepthLoc = glGetUniformLocation(ShaderProgram, "WorldDepth");
	// assert(WorldDepthLoc >= 0);

	EyePosLoc = glGetUniformLocation(ShaderProgram, "EyePos");
	// assert(EyePosLoc >= 0);
}
void ConstantShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);

	Vector EyePos = Cam.position();
	glUniform3f(EyePosLoc, EyePos.X, EyePos.Y, EyePos.Z);
	glUniform1i(WorldDepthLoc, GameSettings::WORLD_DEPTH);
    glUniform3f(ColorLoc, Col.R, Col.G, Col.B);
    // always update matrices
    Matrix ModelView = Cam.getViewMatrix() * ModelTransform;
    Matrix ModelViewProj = Cam.getProjectionMatrix() * ModelView;
    glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, modelTransform().m);
}
void ConstantShader::color( const Color& c)
{
    Col = c;
}

