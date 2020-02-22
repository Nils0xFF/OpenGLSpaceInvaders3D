#pragma once
#include "ScreenShader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class PostProcessing
{	
public:
	static PostProcessing& getInstance() {
		static PostProcessing instance;
		return instance;
	}	

	ScreenShader* shader = NULL;
	void setMSAA(unsigned int samples);

	void Begin();
	void End();

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
	unsigned int textureMulti = 0;
	unsigned int textureColor = 0;
	unsigned int textureDepth = 0;
	unsigned int fbo = 0;
	unsigned int transferFbo = 0;
	unsigned int rbo = 0;

	VertexBuffer VB;
	IndexBuffer IB;
};