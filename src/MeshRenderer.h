#pragma once
#include "Component.h"
#include "BaseModel.h"
#include "CameraManager.h"

class MeshRenderer : public Component
{
	public:
		BaseModel* model;

		MeshRenderer(BaseModel* model, BaseShader* shader, bool deleteShader)
			:model(model) {
			assert(model);
			assert(shader);
			this->model->shader(shader, deleteShader);
		};

		MeshRenderer(const MeshRenderer& other ) {
			this->model = other.model->clone();
		};

		~MeshRenderer() {
			delete model;
		}

		MeshRenderer* clone() { return new MeshRenderer(*this); }

		void Init() {
		}

		void Start() {
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

