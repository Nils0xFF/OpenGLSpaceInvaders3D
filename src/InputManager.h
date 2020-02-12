#pragma once
#include <GL\glew.h>

class InputManager
{
	public:
		GLboolean Keys[1024];
		static InputManager& getInstance(){
			static InputManager instance;
			return instance;
		}

	private:
			InputManager() {};
			InputManager(const InputManager& im) {};
			InputManager& operator= (const InputManager& mg) {};
		
};

