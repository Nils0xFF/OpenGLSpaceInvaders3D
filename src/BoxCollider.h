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
	OBB* collsionBox;
	LineBoxModel* debugModel;
	ConstantShader* debugShader;

	~BoxCollider() {
		delete collsionBox;
		delete debugModel;
		delete debugShader;
	}

	bool checkCollision(const Collider* other) const;

	void Init() {
		debugShader = new ConstantShader();
		debugShader->color(Color(1.0f,0,0));
	}

	void Start() {
		collsionBox = new OBB(gameObject->getRenderer()->model->boundingBox());
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
			if (debugModel != NULL) debugModel->draw(*CameraManager::getInstance().activeCamera);
		#endif //_DEBUG
	}

	void updateBoundingVolume() {
		collsionBox->transform(gameObject->getTransform());
	}

};

