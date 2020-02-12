#pragma once
#include "Camera.h"

class CameraManager
{
public:
	Camera* activeCamera;
	static CameraManager& getInstance() {
		static CameraManager instance;
		return instance;
	}

private:
	CameraManager() {};
	CameraManager(const CameraManager& im) {};
	CameraManager& operator= (const CameraManager& mg) {};
};

