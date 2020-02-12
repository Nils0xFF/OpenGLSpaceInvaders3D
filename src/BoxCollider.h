#pragma once
#include "Collider.h"
#include "Aabb.h"
#include "LineBoxModel.h"
#include "MeshRenderer.h"
#include "CameraManager.h"
#include "ConstantShader.h"

class BoxCollider :
	public Collider
{
public:
	const AABB* collsionBox;
	LineBoxModel* debugModel;

	~BoxCollider() {
		delete debugModel;
	}

	bool checkCollision(const Collider& other) const;


	void Start() {
		collsionBox = &(gameObject->getRenderer()->model->boundingBox());
		debugModel = new LineBoxModel(collsionBox->Min, collsionBox->Max);
		ConstantShader* shader = new ConstantShader();
		shader->color(Color(0,1,0));
		debugModel->shader(shader, true);
	}

	void Update(float deltaTime) {
		// debugModel->transform(Matrix().translation(gameObject->getTransform().translation()));
	}

	void Draw() {
		if (debugModel != NULL) debugModel->draw(*CameraManager::getInstance().activeCamera);
	}
};

