#include "TextShader.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

TextShader::TextShader()
{
	if (!load(ASSET_DIRECTORY "vstext.glsl", ASSET_DIRECTORY "fstext.glsl"))
		throw new std::exception("Shader not found.");

	textColorLoc = glGetUniformLocation(ShaderProgram, "textColor");
	assert(textColorLoc >= 0);
}

void TextShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	glUniform3f(textColorLoc, TextColor.R, TextColor.G, TextColor.B);
}
