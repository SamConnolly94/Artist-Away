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

	double amplification = 4.0;
	double frequency = 25;
	unsigned int numberOfOctaves = 8;
	mpPerlinNoise->SetFrequency(frequency);
	mpPerlinNoise->SetAmplitude(amplification);

	for (unsigned int y = 0; y < mHeight; y++)
	{
		for (unsigned int x = 0; x < mWidth; x++)
		{
			double X = (double)x / ((double)mWidth);
			double Y = (double)y / ((double)mHeight);
			 
			// Typical Perlin noise
			double n = 0.0; /*mpPerlinNoise->OctavePerlin(X, Y, 0.8, numberOfOctaves, 0.3); */
			for (unsigned int i = 0; i < numberOfOctaves; i++)
			{
				n += mpPerlinNoise->Perlin(frequency * X, frequency * Y, 0.8);
			}

			mpHeightMap[y][x] = n;
		}
	}


	gLogger->WriteLine("Heightmap created.");
	return;
}