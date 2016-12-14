#include "random.h"

namespace {
	std::random_device rd;
	int seed = rd();
	std::mt19937 mt(seed);
}

int Random::GetSeed()
{
	return seed;
}

int Random::GetInt()
{
	return mt();
}

int Random::GetInt(int max)
{
	std::uniform_int_distribution<int> dist(0, max);
	return dist(mt);
}

int Random::GetInt(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(mt);
}

float Random::GetFloat(float max)
{
	std::uniform_real_distribution<float> dist(0.f, max);
	return dist(mt);
}

float Random::GetFloat(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mt);
}

std::mt19937& Random::GetMT()
{
	return mt;
}
