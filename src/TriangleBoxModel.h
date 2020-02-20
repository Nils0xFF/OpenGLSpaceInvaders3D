//
//  TriangleBoxModel.hpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef TriangleBoxModel_hpp
#define TriangleBoxModel_hpp

#include <stdio.h>

#include <stdio.h>
#include "basemodel.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class TriangleBoxModel : public BaseModel
{
public:
    TriangleBoxModel(float Width=1, float Height=1, float Depth=1 );
    virtual ~TriangleBoxModel() {}
    virtual void draw(const BaseCamera& Cam);
	void updateBoundingBox() { this->BoundingBox = this->InitialBoundingBox.transform(Transform); }
	const AABB& initialBoundingBox() const { return InitialBoundingBox; }
	const AABB& boundingBox() const { return BoundingBox; }

	TriangleBoxModel* clone() { return new TriangleBoxModel(*this); }

protected:
    VertexBuffer VB;
    IndexBuffer IB;

	AABB BoundingBox;
	AABB InitialBoundingBox;
};


#endif /* TriangleBoxModel_hpp */
