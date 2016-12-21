#include "HeightMap.h"


void CHeightMap::ReleaseHeightmap()
{
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

CHeightMap::CHeightMap()
{
	mpPerlinNoise = new CPerlinNoise();
	gLogger->MemoryAllocWriteLine(typeid(mpPerlinNoise).name());
	
	mpHeightMap = nullptr;

	mWidth = 100;
	mHeight = 100;
	mOldHeight = 100;
	mOldWidth = 100;

	SetPersistence(0.6);
	SetAmplitude(1.0f);
	SetFrequency(1.0f);
	SetNumberOfOctaves(6);
	// Default gain quite high, great for terrain maps.
	SetGain(150.0f);

}

CHeightMap::~CHeightMap()
{
	delete mpPerlinNoise;
	gLogger->MemoryDeallocWriteLine(typeid(mpPerlinNoise).name());

	ReleaseHeightmap();
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

	for (unsigned int y = 0; y < mHeight; y++)
	{
		for (unsigned int x = 0; x < mWidth; x++)
		{
			mpPerlinNoise->SetFrequency(mFrequency);
			mpPerlinNoise->SetAmplitude(mAmplitude);

			double X = (double)x / ((double)mWidth);
			double Y = (double)y / ((double)mHeight);
			 
			// Typical Perlin noise
			double n = 0.0; /*mpPerlinNoise->OctavePerlin(X, Y, 0.8, numberOfOctaves, 0.3); */
			//for (unsigned int i = 0; i < numberOfOctaves; i++)
			//{
			//	n += mpPerlinNoise->Perlin(frequency * X, frequency * Y, 0.8);
			//}
			n = mpPerlinNoise->OctavePerlin(X, Y, 0.0, mNumberOfOctaves, mPersistence);

			n *= mGain;

			mpHeightMap[y][x] = n;
		}
	}

	gLogger->WriteLine("Heightmap created.");
	return;
}

void CHeightMap::UpdateMap()
{
	// Allocate row space.
	double** heightMap = new double*[mHeight];

	// Iterate through all the rows.
	for (int x = 0; x < mHeight; x++)
	{
		// Allocate space for the columns.
		heightMap[x] = new double[mWidth];
	}

	int indexY = 0;
	int indexX = 0;

	for (unsigned int y = 0; y < mHeight; y++)
	{
		for (unsigned int x = 0; x < mWidth; x++)
		{
			mpPerlinNoise->SetFrequency(mFrequency);
			mpPerlinNoise->SetAmplitude(mAmplitude);

			double X = (double)x / ((double)mWidth);
			double Y = (double)y / ((double)mHeight);

			// Typical Perlin noise
			double n = 0.0; 
			n = mpPerlinNoise->OctavePerlin(X, Y, 0.0, mNumberOfOctaves, mPersistence);

			n *= mGain;

			heightMap[y][x] = n;
		}
	}
	// Release old height map.
	if (mpHeightMap != nullptr)
	{
		for (int i = 0; i < mOldHeight; i++) {
			delete[] mpHeightMap[i];
			gLogger->MemoryDeallocWriteLine(typeid(mpHeightMap[i]).name());
		}
		delete[] mpHeightMap;
		gLogger->MemoryDeallocWriteLine(typeid(mpHeightMap).name());
	}

	mOldHeightSet = false;
	mpHeightMap = heightMap;

	gLogger->WriteLine("Heightmap created.");
	return;
}

/* Set the persistence used in noise generation.
* @Param persistence - value of persistence used must be between 0.0 and 1.0.
*/
void CHeightMap::SetPersistence(double persistence)
{
	mPersistence = persistence;
}

/* Write height map to a file. Standard should be to use the .map extension. */
void CHeightMap::WriteMapToFile(std::string fileName)
{
	std::ofstream outFile;
	outFile.open(fileName);

	for (int heightCount = 0; heightCount < mHeight; heightCount++)
	{
		for (int widthCount = 0; widthCount < mWidth; widthCount++)
		{
			outFile << mpHeightMap[heightCount][widthCount] << " ";
		}
		outFile << std::endl;
	}

	gLogger->WriteLine("Wrote height map to: " + fileName);
}
