#pragma once

#include <random>

class Random
{
public:
	static int GetSeed();
	static int GetInt();
	static int GetInt(int max);
	static int GetInt(int min, int max);
	static float GetFloat(float max);
	static float GetFloat(float min, float max);
	static std::mt19937& GetMT();
};