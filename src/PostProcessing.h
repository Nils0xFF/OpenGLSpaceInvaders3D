#pragma once
#include "ScreenShader.h"

class PostProcessing
{	
public:
	static PostProcessing& getInstance() {
		static PostProcessing instance;
		return instance;
	}	

	ScreenShader* shader;
	void setMSAA(unsigned int samples);
	void setResolution(unsigned int width, unsigned int height);

	void Begin();
	void End(float time);

protected:
	void Init();
	void Update();
	void GenerateScreenQuad();

private:	
	PostProcessing();
	~PostProcessing();
	PostProcessing(const PostProcessing& post) {};
	PostProcessing& operator= (const PostProcessing& post) {};

	unsigned int Width = 960;
	unsigned int Height = 540;
	unsigned int MSAA = 1;
	unsigned int quad = 0;
	unsigned int textureMulti = 0;
	unsigned int texture = 0;
	unsigned int fbo = 0;
	unsigned int transferFbo = 0;
	unsigned int rbo = 0;
};

