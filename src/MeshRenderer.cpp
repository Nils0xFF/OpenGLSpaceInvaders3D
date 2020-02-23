#include "MeshRenderer.h"
#include "GameObject.h"

void MeshRenderer::Start() {
	model->transform(gameObject->getTransform());
}

void MeshRenderer::Draw() {
	model->draw(*CameraManager::getInstance().activeCamera);
}

void MeshRenderer::updateTransform() {
	model->transform(gameObject->getTransform());
}
