#pragma once

class LifecycleObject
{
	virtual void Init() {};
	virtual void Start() {};
	virtual void Update(float deltaTime) {};
	virtual void Draw() {};
	virtual void Destroy() {};
};
