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
	double OctavePerlin(double x, double y, double z, unsigned int octaves, double persistence);
private:
	std::vector<int> p;
	int inc(int number);
	double Fade(double time);
	double Lerp(double time, double a, double b);
	double Gradient(int hash, double x, double y, double z);

	int repeat = -1;
};

#endif