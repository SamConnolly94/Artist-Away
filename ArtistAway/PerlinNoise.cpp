#include "PerlinNoise.h"



CPerlinNoise::CPerlinNoise()
{
	gLogger->MemoryAllocWriteLine(typeid(this).name());
}


CPerlinNoise::~CPerlinNoise()
{
	gLogger->MemoryDeallocWriteLine(typeid(this).name());
}

double CPerlinNoise::Perlin(double x, double y, double z)
{
	// Calculate the length of the vector.	
	double length = sqrt(PrioEngine::Math::Square(x) + PrioEngine::Math::Square(y) + PrioEngine::Math::Square(z));

	// Normalise the individual components of the vector.
	double normX = x / length;
	double normY = y / length;
	double normz = z / length;

	return 0.0;
}
