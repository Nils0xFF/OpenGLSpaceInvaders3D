#pragma once
#include "Component.h"
#include "BaseModel.h"

class MeshRenderer : public Component
{
	public:
		BaseModel* model;
		void Draw() {}
};

