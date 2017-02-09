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

	unsigned int mRequestedHeight;
	unsigned int mRequestedWidth;

	unsigned int mNumberOfOctaves;
	double mPersistence;
	float mFrequency;
	float mAmplitude;
	float mGain;
	bool mUpdating = false;
	void ReleaseHeightmap();
public:
	CHeightMap();
	~CHeightMap();

	void InitialiseMap();
	void UpdateMap();

	// Getters
	double** GetMap() { return mpHeightMap; };
	unsigned int GetHeight() { return mHeight; };
	unsigned int GetRequestedHeight(){ return mRequestedHeight; };
	unsigned int GetWidth() { return mWidth; };
	unsigned int GetRequestedWidth() { return mRequestedWidth; };
	float GetFrequency() { return mFrequency; };
	float GetAmplitude() { return mAmplitude; };
	float GetGain() { return mGain; };
	double GetPersistence() { return mPersistence; };
	unsigned int GetNumberOfOctaves() { return mNumberOfOctaves; };

	// Setters
	void SetHeight(unsigned int value) { mHeight = value; mRequestedHeight = mHeight; };
	void SetRequestedHeight(unsigned int value) { mRequestedHeight = value;	};
	void SetWidth(unsigned int value) { mWidth = value;  mRequestedWidth = mWidth; };
	void SetRequestedWidth(unsigned int value) { mRequestedWidth = value; };
	void SetNumberOfOctaves(unsigned int octaves) { mNumberOfOctaves = octaves; };
	void SetPersistence(double persistence);
	void SetFrequency(float frequency) { mFrequency = frequency; };
	void SetAmplitude(float amplitude) { mAmplitude = amplitude; };
	void SetGain(float gain) { mGain = gain; };

	// Misc
	void WriteMapToFile(std::string fileName);
};

#endif