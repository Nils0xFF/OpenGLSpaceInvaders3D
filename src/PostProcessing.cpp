#include "PostProcessing.h"

#include <GL/glew.h>
#include <exception>
#include "CameraManager.h"

PostProcessing::PostProcessing()
{
	Init();
	shader = new ScreenShader();
}

PostProcessing::~PostProcessing()
{
	if (shader) {
		delete shader;		
	}
	shader = NULL;

	glDeleteFramebuffers(1, &fbo);
	glDeleteFramebuffers(1, &transferFbo);
}

void PostProcessing::Begin()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(0.9f, 0.9f, 0.9f, 0.9f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void PostProcessing::End(float time)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, transferFbo);
	glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	shader->setTime(time);
	shader->activate(*CameraManager::getInstance().activeCamera);	
		
	glBindVertexArray(quad);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessing::Init()
{
	// Framebuffer for off-screen rendering
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Texture-attachment for framebuffer
	glGenTextures(1, &textureMulti);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureMulti);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA, GL_RGB, Width, Height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Attaching texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureMulti, 0);

	// Renderbuffer to sample depth- & stencil-texture
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA, GL_DEPTH24_STENCIL8, Width, Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attaching renderbuffer (depth- & stencil-texture)
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Check for complete framebuffer init
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::exception("Not able to create framebuffer.");;
	}

	glGenFramebuffers(1, &transferFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, transferFbo);

	// create a color attachment texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::exception("Not able to create framebuffer.");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GenerateScreenQuad();
}

void PostProcessing::GenerateScreenQuad()
{
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	unsigned int quadVBO;
	glGenVertexArrays(1, &quad);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quad);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void PostProcessing::setMSAA(unsigned int samples)
{
	if (samples < 1) samples = 1;
	if (samples > 16) samples = 16;
	MSAA = samples;
}

void PostProcessing::setResolution(unsigned int width, unsigned int height, unsigned int samples)
{
	Width = width;
	Height = height;
	setMSAA(samples);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureMulti);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA, GL_RGB, Width, Height, GL_TRUE);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA, GL_DEPTH24_STENCIL8, Width, Height);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
}
