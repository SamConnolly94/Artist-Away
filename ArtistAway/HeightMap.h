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
public:
	CHeightMap();
	~CHeightMap();

	void InitialiseMap();
	double** GetMap() { return mpHeightMap; };
	unsigned int GetHeight() { return mHeight; };
	unsigned int GetWidth() { return mWidth; };
	void SetHeight(unsigned int value) { mHeight = value; };
	void SetWidth(unsigned int value) { mWidth = value; };
};

#endif