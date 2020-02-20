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
		return (float) std::rand() / RAND_MAX;
	}
};

