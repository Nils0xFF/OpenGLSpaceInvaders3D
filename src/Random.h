#pragma once
#include <random>
#include <ctime>

class Random
{
public:
	static void Init() {
		std::srand(std::time(nullptr));
	}

	static float random() {
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}
};

