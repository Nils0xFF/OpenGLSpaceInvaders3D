#pragma once
#include "Collider.h"
#include "Aabb.h"
#include "LineBoxModel.h"
#include "MeshRenderer.h"
#include "CameraManager.h"
#include "ConstantShader.h"
#include"OBB.h"

class BoxCollider :
	public Collider
{
public:
	OBB* collsionBox = NULL;
	LineBoxModel* debugModel = NULL;
	ConstantShader* debugShader = NULL;

	BoxCollider() {}

	BoxCollider(const BoxCollider& other) {
		collsionBox = NULL;
		debugModel = NULL;
		debugShader = NULL;
	}

	~BoxCollider() {
		delete collsionBox;
		collsionBox = NULL;
		delete debugModel;
		debugModel = NULL;
		delete debugShader;
		debugShader = NULL;
	}

	BoxCollider* clone() { return new BoxCollider(*this); }

	bool checkCollision(const Collider* other) const;

	void Init() {
		debugShader = new ConstantShader();
		debugShader->color(Color(1.0f,0,1.0f));
	}

	void Start() {
		collsionBox = new OBB(gameObject->getRenderer()->model->initialBoundingBox());
		updateTransform();
	}

	void Update(float deltaTime) {
	}

	void Draw() {
		#if _DEBUG
			if (debugModel != NULL) delete debugModel;
			Vector c[8];
			collsionBox->corners(c);
			debugModel = new LineBoxModel(c);
			debugModel->shader(debugShader, false);
			debugModel->shadowCaster(false);
			debugModel->draw(*CameraManager::getInstance().activeCamera);
		#endif //_DEBUG
	}

	void updateTransform() {
		collsionBox->transform(gameObject->getTransform());
	}


};

