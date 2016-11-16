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

	double amplification = 1.0;
	float frequency = 100.0f;
	mpPerlinNoise->SetFrequency(frequency);

	for (float y = 0; y < mHeight / 10.0f; y += 0.1f)
	{
		indexX = 0;
		for (float x = 0; x < mWidth / 10.0f; x += 0.1f)
		{
			if (indexX < mWidth && indexY < mHeight)
			{
				float X = static_cast<float>(x) / mWidth;
				float Y = static_cast<float>(y) / mHeight;

				double noise = 0.0;
				double gain = 1.0;
				for (int octaves = 0; octaves < 8; octaves++)
				{
					noise += mpPerlinNoise->Perlin(x * gain / frequency, y * gain / frequency, 0.0) * amplification / gain;
					gain *= 2.0;
				}
				mpHeightMap[indexY][indexX] = noise;
				indexX++;
			}
		}
		indexY++;
	}


	gLogger->WriteLine("Heightmap created.");
	return;
}
