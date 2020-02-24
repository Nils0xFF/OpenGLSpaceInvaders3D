#pragma once
#include "Component.h"
#include "Camera.h"
#include "GameObject.h"

class FollowCameraController :
	public Component
{
public:
	Camera& cam;
	Vector offset;

	FollowCameraController(Camera& cam, const Vector& offset) :cam(cam), offset(offset) {};

	void Update(float deltaTime) {
		//cam.setPosition(Vector::Lerp(cam.position(), gameObject->getTransform().translation() + offset, deltaTime * 0.75f));
		//cam.setTarget(Vector::Lerp(cam.target(), gameObject->getTransform().translation() + Vector::forward() * 2, deltaTime * 0.75f));
		//cam.update();
	};

	void updateTransform() {
		cam.setPosition(gameObject->getTransform().translation() + offset);
		cam.setTarget(gameObject->getTransform().translation() + Vector::forward() * 2);
		cam.update();
	}

	FollowCameraController* clone() {
		return NULL;
	};
};

