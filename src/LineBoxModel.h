//
//  LineBoxModel.hpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef LineCubeModel_hpp
#define LineCubeModel_hpp

#include <stdio.h>
#include "basemodel.h"
#include "vertexbuffer.h"
#include "Aabb.h"

class LineBoxModel : public BaseModel
{
public:
    LineBoxModel( float Width=1, float Height=1, float Depth=1 );
    LineBoxModel( const Vector& min, const Vector& max);
	LineBoxModel(Vector c[8]);
	LineBoxModel* clone() { return new LineBoxModel(*this); }
    virtual ~LineBoxModel() {}
    virtual void draw(const BaseCamera& Cam);

	const AABB& boundingBox() const { return BoundingBox; }

protected:
    VertexBuffer VB;
	AABB BoundingBox;
    
};

#endif /* LineCubeModel_hpp */
