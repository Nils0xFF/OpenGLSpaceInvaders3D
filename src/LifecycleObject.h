#pragma once

class LifecycleObject
{
	public:
		virtual void Init() {};
		virtual void Start() {};
		virtual void Update(float deltaTime) {};
		virtual void Draw() {};
		virtual void Destroy() {};
};
