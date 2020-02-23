#include "BlurShader.h"

#ifdef WIN32
#define SHADER_DIRECTORY "../../assets/shader/"
#else
#define SHADER_DIRECTORY "../assets/shader/"
#endif

BlurShader::BlurShader()
{
	bool loaded = load(SHADER_DIRECTORY "vertex/vsblur.glsl", SHADER_DIRECTORY "fragment/fsblur.glsl");
	if (!loaded)
		throw std::exception();

	HorizontalLoc = glGetUniformLocation(ShaderProgram, "horizontal");
}

void BlurShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	glUniform1i(HorizontalLoc, Horizontal);
}
