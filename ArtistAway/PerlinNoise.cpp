#include "PerlinNoise.h"



CPerlinNoise::CPerlinNoise()
{
	gLogger->MemoryAllocWriteLine(typeid(this).name());
}


CPerlinNoise::~CPerlinNoise()
{
	gLogger->MemoryDeallocWriteLine(typeid(this).name());
}
