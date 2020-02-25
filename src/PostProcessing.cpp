#include "PostProcessing.h"

#include <GL/glew.h>
#include <exception>
#include "CameraManager.h"
#include "ShaderManager.h"

PostProcessing::PostProcessing()
{
	shader = ShaderManager::getScreenShader();
	Init();
#ifdef _DEBUG
	shader->on(false);
#endif // _DEBUG	
}

PostProcessing::~PostProcessing()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteFramebuffers(1, &transferFbo);
}

void PostProcessing::Begin()
{
	Update();

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(0.9f, 0.9f, 0.9f, 0.9f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void PostProcessing::End()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, transferFbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glReadBuffer(GL_COLOR_ATTACHMENT2);
	glDrawBuffer(GL_COLOR_ATTACHMENT2);
	glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColor);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureBrightness);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureDepth);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTrueDepth);

	shader->setTime((float) glfwGetTime());
	shader->activate(*CameraManager::getInstance().activeCamera);
		
	RenderScreenQuad();

	shader->deactivate();
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
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureMulti, 0);

	glGenTextures(1, &textureMultiBrightness);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureMultiBrightness);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA, GL_RGB, Width, Height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, textureMultiBrightness, 0);

	glGenTextures(1, &textureMultiDepth);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureMultiDepth);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA, GL_RGB, Width, Height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D_MULTISAMPLE, textureMultiDepth, 0);

	// Renderbuffer to sample depth- & stencil-texture
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA, GL_DEPTH24_STENCIL8, Width, Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// Check for complete framebuffer init
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::exception("Not able to create framebuffer.");;
	}

	glGenFramebuffers(1, &transferFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, transferFbo);

	// create a color attachment texture
	glGenTextures(1, &textureColor);
	glBindTexture(GL_TEXTURE_2D, textureColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColor, 0);

	glGenTextures(1, &textureBrightness);
	glBindTexture(GL_TEXTURE_2D, textureBrightness);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, textureBrightness, 0);

	glGenTextures(1, &textureDepth);
	glBindTexture(GL_TEXTURE_2D, textureDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, textureDepth, 0);

	glGenTextures(1, &textureTrueDepth);
	glBindTexture(GL_TEXTURE_2D, textureTrueDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureTrueDepth, 0);

	glDrawBuffers(3, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::exception("Not able to create framebuffer.");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GenerateScreenQuad();
}

void PostProcessing::Update()
{
	Width = CameraManager::getInstance().activeCamera->WindowWidth;
	Height = CameraManager::getInstance().activeCamera->WindowHeight;

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureMulti);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA, GL_RGB, Width, Height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureMultiBrightness);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA, GL_RGB, Width, Height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureMultiDepth);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA, GL_RGB, Width, Height, GL_TRUE);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA, GL_DEPTH24_STENCIL8, Width, Height);
	glBindTexture(GL_TEXTURE_2D, textureColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, textureBrightness);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, textureDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, textureTrueDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
}

void PostProcessing::GenerateScreenQuad()
{
	VB.begin();
	VB.addTexcoord0(0.0f, 0.0f);
	VB.addVertex(-1.0f, -1.0f, 0.0f);
	VB.addTexcoord0(0.0f, 1.0f);
	VB.addVertex(-1.0f, 1.0f, 0.0f);
	VB.addTexcoord0(1.0f, 0.0f);
	VB.addVertex(1.0f, -1.0f, 0.0f);
	VB.addTexcoord0(1.0f, 1.0f);
	VB.addVertex(1.0f, 1.0f, 0.0f);
	VB.end();

	IB.begin();
	IB.addIndex(1);
	IB.addIndex(0);
	IB.addIndex(2);
	IB.addIndex(1);
	IB.addIndex(2);
	IB.addIndex(3);
	IB.end();
}

void PostProcessing::RenderScreenQuad()
{
	VB.activate();
	IB.activate();

	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);

	IB.deactivate();
	VB.deactivate();

	glEnable(GL_DEPTH_TEST);
}

void PostProcessing::setMSAA(unsigned int samples)
{
	if (samples < 1) samples = 1;
	if (samples > 16) samples = 16;
	MSAA = samples;

	Update();
}
