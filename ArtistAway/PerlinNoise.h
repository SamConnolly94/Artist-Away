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
private:
	CLogger* logger;
public:
	// Constructors.
	CPerlinNoise(int repeatVal = -1);
	CPerlinNoise(unsigned int seed, int repeatVal = -1);

	// Destructor.
	~CPerlinNoise();

	double Perlin(double x, double y, double z);
	void SetFrequency(float value) { mFrequency = value; };
	void SetAmplitude(float value) { mAmplitude = value; };

	double OctavePerlin(double x, double y, double z, unsigned int octaves, double persistence);
private:
	std::vector<int> p;
	//int inc(int number);
	double Fade(double time);
	double Lerp(double time, double a, double b);
	double Gradient(int hash, double x, double y, double z);

	int repeat = -1;
	float mFrequency;
	float mAmplitude;


	int inc(int number);

};

#endif