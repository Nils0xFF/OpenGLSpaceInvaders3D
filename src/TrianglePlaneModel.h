//
//  TrianglePlaneModel.hpp
//  ogl4
//
//  Created by Philipp Lensing on 20.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef TrianglePlaneModel_hpp
#define TrianglePlaneModel_hpp

#include <stdio.h>
#include "basemodel.h"
#include "constantshader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "Aabb.h"

class TrianglePlaneModel : public BaseModel
{
public:
    TrianglePlaneModel( float DimX, float DimZ, int NumSegX, int NumSegZ );
	TrianglePlaneModel* clone() { return new TrianglePlaneModel(*this); }
    virtual ~TrianglePlaneModel() {}
    virtual void draw(const BaseCamera& Cam);

	const AABB& boundingBox() const { return BoundingBox; }

	virtual void transform(const Matrix& m) {
		Transform = m;
		updateBoundingBox();
	}

	const Matrix& transform() const { return Transform; }

protected:
    VertexBuffer VB;
    IndexBuffer IB;
	AABB BoundingBox;
	AABB initalBoundingBox;

	void updateBoundingBox() { BoundingBox = initalBoundingBox.transform(Transform); };
};

#endif /* TrianglePlaneModel_hpp */
