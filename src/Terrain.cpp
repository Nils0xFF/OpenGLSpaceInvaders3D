#include "Terrain.h"

Terrain::Terrain(unsigned int Width, unsigned int Height, unsigned int SegX, unsigned int SegZ)
{
    ++SegX; ++SegZ;
    float startX = Width * -0.5f;
    float startZ = Height * -0.5f;
    float stepX = Width / (float)(SegX - 1);
    float stepZ = Height / (float)(SegZ - 1);

    VB.begin();

    for (unsigned int i = 0; i < SegZ; i++) {
        for (unsigned int j = 0; j < SegX; j++) {
            VB.addNormal(0, 1, 0);
            float s0 = (float)j / (float)(SegX - 1);
            float t0 = (float)i / (float)(SegZ - 1);
            VB.addTexcoord0(s0, t0);
            VB.addVertex(startX + j * stepX, 0, startZ + i * stepZ);
        }
    }

    VB.end();

    IB.begin();

    for (unsigned int i = 0; i < SegZ - 1; i++) {
        for (unsigned int j = 0; j < SegX - 1; j++)
        {
            unsigned int idx = i * SegX + j;
            IB.addIndex(idx + SegX);
            IB.addIndex(idx + 1);
            IB.addIndex(idx);

            IB.addIndex((idx + 1) + SegX);
            IB.addIndex(idx + 1);
            IB.addIndex(idx + SegX);
        }
    }

    IB.end();    
}

void Terrain::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);

    VB.activate();
    IB.activate();

    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);

    IB.deactivate();
    VB.deactivate();
}
