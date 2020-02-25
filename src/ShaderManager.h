#pragma once
#include "PhongShader.h"
#include "ConstantShader.h"
#include "ParticleShader.h"
#include "TextShader.h"
#include "ScreenShader.h"

class ShaderManager
{
public:
	static PhongShader* getPhongShader() {
		static PhongShader phong;
		return &phong;
	}

	static ConstantShader* getDebugShader() {
		static ConstantShader debug(Color(0,1,0));
		return &debug;
	}

	static ConstantShader* getColliderDebugShader() {
		static ConstantShader debug(Color(1, 0, 0));
		return &debug;
	}

	static ParticleShader* getParticleShader() {
		static ParticleShader particle;
		return &particle;
	}

	static TextShader* getTextShader() {
		static TextShader text;
		return &text;
	}

	static ScreenShader* getScreenShader() {
		static ScreenShader screen;
		return &screen;
	}


};

