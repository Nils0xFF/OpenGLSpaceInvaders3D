//
//  TriangleBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "TriangleBoxModel.h"

TriangleBoxModel::TriangleBoxModel(float Width, float Height, float Depth)
{
    /// TODO: Add your code (Exercise 2)
    float BeginX = Width*-0.5f;
    float BeginY = Height*-0.5f;
    float BeginZ = Depth*-0.5f;
    
    // 1. setup vertex buffer
    VB.begin();

    //back
    VB.addNormal(0, 0, -1.0f);
    VB.addTexcoord0(1.0f, 1.0f);
    VB.addVertex(BeginX, BeginY, BeginZ);

    VB.addNormal(0, 0, -1.0f);
    VB.addTexcoord0(0.0f, 1.0f);
    VB.addVertex(-BeginX, BeginY, BeginZ);

    VB.addNormal(0, 0, -1.0f);
    VB.addTexcoord0(0.0f, 0.0f);
    VB.addVertex(-BeginX, -BeginY, BeginZ);

    VB.addNormal(0, 0, -1.0f);
    VB.addTexcoord0(1.0f, 0.0f);
    VB.addVertex(BeginX, -BeginY, BeginZ);
    
    
    //right
    VB.addNormal(1.0f, 0, 0);
    VB.addTexcoord0(1.0f, 1.0f);
    VB.addVertex(-BeginX, BeginY, BeginZ);
    
    VB.addNormal(1.0f, 0, 0);
    VB.addTexcoord0(0.0f, 1.0f);
    VB.addVertex(-BeginX, BeginY, -BeginZ);
    
    VB.addNormal(1.0f, 0, 0);
    VB.addTexcoord0(0.0f, 0.0f);
    VB.addVertex(-BeginX, -BeginY, -BeginZ);
    
    VB.addNormal(1.0f, 0, 0);
    VB.addTexcoord0(1.0f, 0.0f);
    VB.addVertex(-BeginX, -BeginY, BeginZ);
    
    
    //front
    VB.addNormal(0, 0, 1.0f);
    VB.addTexcoord0(1.0f, 1.0f);
    VB.addVertex(-BeginX, BeginY, -BeginZ);
    
    VB.addNormal(0, 0, 1.0f);
    VB.addTexcoord0(0.0f, 1.0f);
    VB.addVertex(BeginX, BeginY, -BeginZ);
    
    VB.addNormal(0, 0, 1.0f);
    VB.addTexcoord0(0.0f, 0.0f);
    VB.addVertex(BeginX, -BeginY, -BeginZ);
    
    VB.addNormal(0, 0, 1.0f);
    VB.addTexcoord0(1.0f, 0.0f);
    VB.addVertex(-BeginX, -BeginY, -BeginZ);
    
    
    //left
    VB.addNormal(-1.0f, 0, 0);
    VB.addTexcoord0(1.0f, 1.0f);
    VB.addVertex(BeginX, BeginY, -BeginZ);
    
    VB.addNormal(-1.0f, 0, 0);
    VB.addTexcoord0(0.0f, 1.0f);
    VB.addVertex(BeginX, BeginY, BeginZ);
    
    VB.addNormal(-1.0f, 0, 0);
    VB.addTexcoord0(0.0f, 0.0f);
    VB.addVertex(BeginX, -BeginY, BeginZ);
    
    VB.addNormal(-1.0f, 0, 0);
    VB.addTexcoord0(1.0f, 0.0f);
    VB.addVertex(BeginX, -BeginY, -BeginZ);
    
    
    //top
    VB.addNormal(0, 1.0f, 0);
    VB.addTexcoord0(1.0f, 1.0f);
    VB.addVertex(-BeginX, -BeginY, -BeginZ);
    
    VB.addNormal(0, 1.0f, 0);
    VB.addTexcoord0(0.0f, 1.0f);
    VB.addVertex(BeginX, -BeginY, -BeginZ);
    
    VB.addNormal(0, 1.0f, 0);
    VB.addTexcoord0(0.0f, 0.0f);
    VB.addVertex(BeginX, -BeginY, BeginZ);
    
    VB.addNormal(0, 1.0f, 0);
    VB.addTexcoord0(1.0f, 0.0f);
    VB.addVertex(-BeginX, -BeginY, BeginZ);
    
    
    //bottom
    VB.addNormal(0, -1.0f, 0);
    VB.addTexcoord0(1.0f, 1.0f);
    VB.addVertex(-BeginX, BeginY, BeginZ);
    
    VB.addNormal(0, -1.0f, 0);
    VB.addTexcoord0(0.0f, 1.0f);
    VB.addVertex(BeginX, BeginY, BeginZ);
    
    VB.addNormal(0, -1.0f, 0);
    VB.addTexcoord0(0.0f, 0.0f);
    VB.addVertex(BeginX, BeginY, -BeginZ);
    
    VB.addNormal(0, -1.0f, 0);
    VB.addTexcoord0(1.0f, 0.0f);
    VB.addVertex(-BeginX, BeginY, -BeginZ);

    VB.end();
    int indecies[] = {0,3,2,2,1,0,  4,7,6,6,5,4, 8,11,10,10,9,8,  12,15,14,14,13,12, 16,19,18,18,17,16,  20,23,22,22,21,20};
    IB.begin();
    for(int k=0; k< sizeof(indecies)/sizeof(indecies[0]); k++)
    {
        IB.addIndex(indecies[k]);
    }
    IB.end();
}

void TriangleBoxModel::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);
    
    // TODO: Add your code (Exercise 2)
    VB.activate();
    IB.activate();
    
    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);
    
    IB.deactivate();
    VB.deactivate();
}
