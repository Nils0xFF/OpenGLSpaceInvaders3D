#pragma once
#include "Component.h"
#include "BaseModel.h"
#include "CameraManager.h"

class MeshRenderer : public Component
{
	public:
		BaseModel* model;

		~MeshRenderer() {
			delete model;
		}

		void Init() {
			model->transform(gameObject->getTransform());
		}

		void Update(float deltaTime) {
		}

		void Draw() {
			model->draw(*CameraManager::getInstance().activeCamera);
		}

		void updateTransform() {
			model->transform(gameObject->getTransform());
		}
};

