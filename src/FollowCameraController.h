#pragma once
#include "Component.h"
#include "Camera.h"
class FollowCameraController :
	public Component
{
public:
	Camera& cam;
	Vector offset;

	FollowCameraController(Camera& cam, const Vector& offset) :cam(cam), offset(offset) {};

	void Update(float deltaTime) {
		cam.update();
	};

	void updateTransform() {
		cam.setPosition(gameObject->getTransform().translation() + offset);
		// cam.setUp(gameObject->getTransform().up());
		cam.setTarget(gameObject->getTransform().translation() + Vector::forward() * 2);
	}

	FollowCameraController* clone() {
		return NULL;
	};
};

