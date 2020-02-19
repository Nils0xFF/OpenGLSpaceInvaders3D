//
//  BaseModel.hpp
//  ogl4
//
//  Created by Philipp Lensing on 19.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef BaseModel_hpp
#define BaseModel_hpp

#include <stdio.h>
#include "camera.h"
#include "matrix.h"
#include "baseshader.h"
#include "Aabb.h"

class BaseModel
{
public:
    BaseModel();
	BaseModel(const BaseModel& other) {
		this->pShader = other.pShader->clone();
		this->DeleteShader = other.DeleteShader;
		this->ShadowCaster = other.ShadowCaster;
		this->ShadowReciver = other.ShadowReciver;
		this->Transform = other.Transform;
	};

	virtual BaseModel* clone() { return new BaseModel(*this); }
    virtual ~BaseModel();
    virtual void draw(const BaseCamera& Cam);
    virtual const Matrix& transform() const { return Transform; }
	virtual void transform(const Matrix& m) { Transform = m; updateBoundingBox(); }
    virtual void shader( BaseShader* shader, bool deleteOnDestruction=false );
    virtual BaseShader* shader() const { return pShader; }
    virtual const AABB& boundingBox() const { return AABB::unitBox(); }
	virtual const AABB& initialBoundingBox() const { return AABB::unitBox(); }
	bool shadowCaster() const { return ShadowCaster; }
	void shadowCaster(bool sc) { ShadowCaster = sc; }
	bool shadowReciver() const { return ShadowReciver; }
	void shadowReciver(bool sr) { ShadowReciver = sr; }

protected:
    Matrix Transform;
    BaseShader* pShader;
    bool DeleteShader;
	bool ShadowCaster;
	bool ShadowReciver;

	virtual void updateBoundingBox() {};
};


#endif /* BaseModel_hpp */
