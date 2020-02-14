#pragma once
#include "ScreenShader.h"

class PostProcessing
{
public:
	PostProcessing(float width, float height, float samples);
	~PostProcessing();

	ScreenShader* shader;

	void setMSAA(unsigned int samples);
	void setResolution(float width, float height);

protected:
	void Init();
	void Begin();
	void End(float time);
	void GenerateScreenQuad();

private:	
	float Width, Height = 0.0f;
	unsigned int MSAA = 1;
	unsigned int quad = 0;
	unsigned int textureMulti = 0;
	unsigned int texture = 0;
	unsigned int fbo = 0;
	unsigned int transferFbo = 0;
	unsigned int rbo = 0;	
};

