#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include "Engine\PrioEngineVars.h"
#include <math.h>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>

class CPerlinNoise
{
public:
	// Constructors.
	CPerlinNoise();
	CPerlinNoise(unsigned int seed);

	// Destructor.
	~CPerlinNoise();

	double Perlin(double x, double y, double z);

private:
	std::vector<int> p;

	double Fade(double time);
	double Lerp(double time, double a, double b);
	double Gradient(int hash, double x, double y, double z);
};

#endif