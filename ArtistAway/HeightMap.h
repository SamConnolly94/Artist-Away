#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "PerlinNoise.h"

class CHeightMap
{
private:
	CPerlinNoise* mpPerlinNoise;

	double** mpHeightMap;

	unsigned int mHeight;
	unsigned int mWidth;
	unsigned int mNumberOfOctaves;
	double mPersistence;
	float mFrequency;
	float mAmplitude;
	float mGain;
public:
	CHeightMap();
	~CHeightMap();

	void InitialiseMap();
	double** GetMap() { return mpHeightMap; };
	unsigned int GetHeight() { return mHeight; };
	unsigned int GetWidth() { return mWidth; };
	void SetHeight(unsigned int value) { mHeight = value; };
	void SetWidth(unsigned int value) { mWidth = value; };
	void SetNumberOfOctaves(unsigned int octaves) { mNumberOfOctaves = octaves; };
	void SetPersistence(double persistence);
	void SetFrequency(float frequency) { mFrequency = frequency; };
	void SetAmplitude(float amplitude) { mAmplitude = amplitude; };
	void SetGain(float gain) { mGain = gain; };
};

#endif