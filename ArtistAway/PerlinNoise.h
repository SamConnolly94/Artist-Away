#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include "Engine\PrioEngineVars.h"
#include <math.h>

class CPerlinNoise
{
public:
	CPerlinNoise();
	~CPerlinNoise();

	double Perlin(double x, double y, double z);
};

#endif