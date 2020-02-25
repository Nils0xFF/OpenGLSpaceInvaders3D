#pragma once
#include "Collider.h"
#include "Aabb.h"
#include "LineBoxModel.h"
#include "MeshRenderer.h"
#include "CameraManager.h"
#include "ConstantShader.h"
#include"OBB.h"
#include "ShaderManager.h"

class BoxCollider :
	public Collider
{
public:
	OBB collsionBox = OBB(AABB());
	LineBoxModel* debugModel = NULL;

	BoxCollider() {}

	BoxCollider(const BoxCollider& other) {
		debugModel = NULL;
	}

	~BoxCollider() {
		delete debugModel;
		debugModel = NULL;
	}

	BoxCollider* clone() { return new BoxCollider(*this); }

	bool checkCollision(const Collider* other) const;

	void Init() {
	}

	void Start();

	void Update(float deltaTime) {
	}

	void Draw() {
		#if _DEBUG
			if (debugModel != NULL) delete debugModel;
			Vector c[8];
			collsionBox.corners(c);
			debugModel = new LineBoxModel(c);
			debugModel->shader(ShaderManager::getColliderDebugShader(), false);
			debugModel->shadowCaster(false);
			debugModel->draw(*CameraManager::getInstance().activeCamera);
		#endif //_DEBUG
	}

	void updateTransform();


};

