#include "HeightMap.h"


CHeightMap::CHeightMap()
{
	mpPerlinNoise = new CPerlinNoise();
	gLogger->MemoryAllocWriteLine(typeid(mpPerlinNoise).name());
	
	mpHeightMap = nullptr;

	mWidth = 100;
	mHeight = 100;
}


CHeightMap::~CHeightMap()
{
	delete mpPerlinNoise;
	gLogger->MemoryDeallocWriteLine(typeid(mpPerlinNoise).name());

	if (mpHeightMap != nullptr)
	{
		for (int i = 0; i < mHeight; ++i) {
			delete[] mpHeightMap[i];
			gLogger->MemoryDeallocWriteLine(typeid(mpHeightMap[i]).name());
		}
		delete[] mpHeightMap;
		gLogger->MemoryDeallocWriteLine(typeid(mpHeightMap).name());
	}
}

void CHeightMap::InitialiseMap()
{
	if (mpHeightMap == nullptr)
	{
		// Allocate row space.
		mpHeightMap = new double*[mHeight];

		// Iterate through all the rows.
		for (int x = 0; x < mHeight; x++)
		{
			// Allocate space for the columns.
			mpHeightMap[x] = new double[mWidth];
		}
	}
	else 
	{
		// If mpHeightMap wasn't a nullptr, don't do any of this as we'll cause memory leaks.
		gLogger->WriteLine("The InitialiseMap function has been called twice by the same object of HeightMap. Avoiding creating a new instance to avoid memory leaks.");
	}

	int indexY = 0;
	int indexX = 0;
	float freq = 1.0f;

	for (float y = 0; y < 10.0f; y += 0.01f) 
	{
		if (indexY < mHeight && indexX < mWidth)
		{
			indexY = 0;
			for (float x = 0; x < 10.0f; x += 0.1f) {
				double nx = (x / 100 - 0.5);
				double ny = (y / 100 - 0.5);

				mpHeightMap[indexY][indexX] = mpPerlinNoise->OctavePerlin(nx * freq, ny * freq, 0.0, 2, 1);
				indexY++;
			}
			indexX++;
		}
	}

	return;
}
