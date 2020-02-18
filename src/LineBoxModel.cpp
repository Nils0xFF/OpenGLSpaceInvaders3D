//
//  LineBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "LineBoxModel.h"

LineBoxModel::LineBoxModel(float Width, float Height, float Depth)
{

	// TODO: Add your code (Exercise 1)
	VB.begin();

	float BeginX = Width / 2.0f;
	float BeginY = Height / 2.0f;
	float BeginZ = Depth / 2.0f;

	// Bottom Left Front Corner
	VB.addVertex(-BeginX, -BeginY, -BeginZ);
	VB.addVertex(-BeginX, -BeginY, BeginZ);
	VB.addVertex(-BeginX, -BeginY, -BeginZ);
	VB.addVertex(-BeginX, BeginY, -BeginZ);
	VB.addVertex(-BeginX, -BeginY, -BeginZ);
	VB.addVertex(BeginX, -BeginY, -BeginZ);

	// Upper Left Back Corner
	VB.addVertex(-BeginX, BeginY, BeginZ);
	VB.addVertex(-BeginX, BeginY, -BeginZ);
	VB.addVertex(-BeginX, BeginY, BeginZ);
	VB.addVertex(-BeginX, -BeginY, BeginZ);
	VB.addVertex(-BeginX, BeginY, BeginZ);
	VB.addVertex(BeginX, BeginY, BeginZ);

	// Lower Right Back Corner
	VB.addVertex(BeginX, -BeginY, BeginZ);
	VB.addVertex(BeginX, -BeginY, -BeginZ);
	VB.addVertex(BeginX, -BeginY, BeginZ);
	VB.addVertex(BeginX, BeginY, BeginZ);
	VB.addVertex(BeginX, -BeginY, BeginZ);
	VB.addVertex(-BeginX, -BeginY, BeginZ);

	// Upper Top Front Corner
	VB.addVertex(BeginX, BeginY, -BeginZ);
	VB.addVertex(-BeginX, BeginY, -BeginZ);
	VB.addVertex(BeginX, BeginY, -BeginZ);
	VB.addVertex(BeginX, -BeginY, -BeginZ);
	VB.addVertex(BeginX, BeginY, -BeginZ);
	VB.addVertex(BeginX, BeginY, BeginZ);

	VB.end();
}

LineBoxModel::LineBoxModel(const Vector& min, const Vector& max)
{
	// TODO: Add your code (Exercise 1)
	VB.begin();


	// Bottom Left Front Corner
	VB.addVertex(min.X, min.Y, min.Z);
	VB.addVertex(min.X, min.Y, max.Z);
	VB.addVertex(min.X, min.Y, min.Z);
	VB.addVertex(min.X, max.Y, min.Z);
	VB.addVertex(min.X, min.Y, min.Z);
	VB.addVertex(max.X, min.Y, min.Z);

	// Upper Left Back Corner
	VB.addVertex(min.X, max.Y, max.Z);
	VB.addVertex(min.X, max.Y, min.Z);
	VB.addVertex(min.X, max.Y, max.Z);
	VB.addVertex(min.X, min.Y, max.Z);
	VB.addVertex(min.X, max.Y, max.Z);
	VB.addVertex(max.X, max.Y, max.Z);

	// Lower Right Back Corner
	VB.addVertex(max.X, min.Y, max.Z);
	VB.addVertex(max.X, min.Y, min.Z);
	VB.addVertex(max.X, min.Y, max.Z);
	VB.addVertex(max.X, max.Y, max.Z);
	VB.addVertex(max.X, min.Y, max.Z);
	VB.addVertex(min.X, min.Y, max.Z);

	// Upper Top Front Corner
	VB.addVertex(max.X, max.Y, min.Z);
	VB.addVertex(min.X, max.Y, min.Z);
	VB.addVertex(max.X, max.Y, min.Z);
	VB.addVertex(max.X, min.Y, min.Z);
	VB.addVertex(max.X, max.Y, min.Z);
	VB.addVertex(max.X, max.Y, max.Z);

	VB.end();

}

LineBoxModel::LineBoxModel(Vector c[8])
{
	VB.begin();

	// Bottom Left Front Corner
	VB.addVertex(c[0]);
	VB.addVertex(c[4]);
	VB.addVertex(c[0]);
	VB.addVertex(c[2]);
	VB.addVertex(c[0]);
	VB.addVertex(c[1]);

	// Upper Left Back Corner
	VB.addVertex(c[6]);
	VB.addVertex(c[2]);
	VB.addVertex(c[6]);
	VB.addVertex(c[4]);
	VB.addVertex(c[6]);
	VB.addVertex(c[7]);

	// Lower Right Back Corner
	VB.addVertex(c[5]);
	VB.addVertex(c[1]);
	VB.addVertex(c[5]);
	VB.addVertex(c[7]);
	VB.addVertex(c[5]);
	VB.addVertex(c[4]);

	// Upper Top Front Corner
	VB.addVertex(c[3]);
	VB.addVertex(c[2]);
	VB.addVertex(c[3]);
	VB.addVertex(c[1]);
	VB.addVertex(c[3]);
	VB.addVertex(c[7]);

	VB.end();
}



void LineBoxModel::draw(const BaseCamera& Cam)
{
	BaseModel::draw(Cam);

	// TODO: Add your code (Exercise 1)
	VB.activate();

	glDrawArrays(GL_LINES, 0, VB.vertexCount());

	VB.deactivate();
}
