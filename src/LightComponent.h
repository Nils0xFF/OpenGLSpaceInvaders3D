#pragma once
#include "Component.h"
#include "Lights.h"
#include "ShaderLightMapper.h"
class LightComponent : public Component
{
public:
	BaseLight* light;

	LightComponent(BaseLight* light) :light(light) {};
	LightComponent(const LightComponent& other) {
		this->light = other.light->clone();
	}

	~LightComponent() { Destroy(); }

	LightComponent* clone() {
		return new LightComponent(*this);
	}

	void Start() {
		ShaderLightMapper::instance().addLight(light);
		light->position(gameObject->getTransform().translation());
	};

	void Update(float deltaTime) {};
	void updateTransform() {
		light->position(gameObject->getTransform().translation());
	};

	void Destroy() {
		if (light) {
			ShaderLightMapper::instance().removeLight(light);
			delete light;
		}
	};
};
