#pragma once
#include "BaseModel.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Terrain :	public BaseModel
{
public:
	Terrain(unsigned int Width, unsigned int Height, unsigned int SegX = 1, unsigned int SegZ = 1);

	virtual void draw(const BaseCamera& Cam);

protected:
	VertexBuffer VB;
	IndexBuffer IB;

private:
};

