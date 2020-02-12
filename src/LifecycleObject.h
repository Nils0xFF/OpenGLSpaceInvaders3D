#pragma once

class LifecycleObject
{
	virtual void Init() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void Destroy() {};
};
