#pragma once
#include "Camera.h"

class CameraManager
{
public:
	Camera* activeCamera = NULL;
	static CameraManager& getInstance() {
		static CameraManager instance;
		return instance;
	}

private:
	CameraManager(): activeCamera(NULL) {};
	CameraManager(const CameraManager& im) {};
	CameraManager& operator= (const CameraManager& mg) {};
};

