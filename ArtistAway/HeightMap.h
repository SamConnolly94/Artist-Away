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
	void UpdateMap();

	// Getters
	double** GetMap() { return mpHeightMap; };
	unsigned int GetHeight() { return mHeight; };
	unsigned int GetWidth() { return mWidth; };
	float GetFrequency() { return mFrequency; };
	float GetAmplitude() { return mAmplitude; };
	float GetGain() { return mGain; };
	double GetPersistence() { return mPersistence; };

	// Setters
	void SetHeight(unsigned int value) { mHeight = value; };
	void SetWidth(unsigned int value) { mWidth = value; };
	void SetNumberOfOctaves(unsigned int octaves) { mNumberOfOctaves = octaves; };
	void SetPersistence(double persistence);
	void SetFrequency(float frequency) { mFrequency = frequency; };
	void SetAmplitude(float amplitude) { mAmplitude = amplitude; };
	void SetGain(float gain) { mGain = gain; };

	// Misc
	void WriteMapToFile(std::string fileName);
};

#endif