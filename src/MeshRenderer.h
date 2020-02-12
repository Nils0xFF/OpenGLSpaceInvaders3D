#pragma once
#include "Component.h"
#include "BaseModel.h"
#include "CameraManager.h"

class MeshRenderer : public Component
{
	public:
		BaseModel* model;
		
		void Update(float deltaTime) {
			model->transform(gameObject->getTransform());
		}

		void Draw() {
			model->draw(*CameraManager::getInstance().activeCamera);
		}
};

