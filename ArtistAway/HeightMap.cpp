#include "HeightMap.h"


void CHeightMap::ReleaseHeightmap()
{
	if (mUpdating)
	{
		logger->GetInstance().WriteLine("Thread was being updated. This would cause issues trying to deallocate memory.");
	}
	else if (mpHeightMap != nullptr)
	{
		for (int i = 0; i < mHeight; ++i) {
			delete[] mpHeightMap[i];
			logger->GetInstance().MemoryDeallocWriteLine(typeid(mpHeightMap[i]).name());
		}
		delete[] mpHeightMap;
		logger->GetInstance().MemoryDeallocWriteLine(typeid(mpHeightMap).name());
	}
}

CHeightMap::CHeightMap()
{
	mpPerlinNoise = new CPerlinNoise();
	logger->GetInstance().MemoryAllocWriteLine(typeid(mpPerlinNoise).name());
	
	mpHeightMap = nullptr;

	mWidth = 100;
	mHeight = 100;

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
	logger->GetInstance().MemoryDeallocWriteLine(typeid(mpPerlinNoise).name());

	ReleaseHeightmap();
}

void CHeightMap::InitialiseMap()
{
	ReleaseHeightmap();

	mpHeightMap = new double*[mHeight];

	for (int y = 0; y < mHeight; y++)
	{
		// Allocate space for the columns.
		mpHeightMap[y] = new double[mWidth];
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

			mpHeightMap[y][x] = n;
		}
	}

	logger->GetInstance().WriteLine("Heightmap created.");
	return;
}

void CHeightMap::UpdateMap()
{
	// Release old height map.
	ReleaseHeightmap();
	mUpdating = true;

	// Allocate row space.
	mpHeightMap = new double*[mRequestedHeight];
	logger->GetInstance().MemoryAllocWriteLine(typeid(mpHeightMap).name());

	// Iterate through all the rows.
	for (int x = 0; x < mRequestedHeight; x++)
	{
		// Allocate space for the columns.
		mpHeightMap[x] = new double[mRequestedWidth];
		logger->GetInstance().MemoryAllocWriteLine(typeid(mpHeightMap[x]).name());
	}

	mWidth = mRequestedWidth;
	mHeight = mRequestedHeight;

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

			mpHeightMap[y][x] = n;
		}
	}

	logger->GetInstance().WriteLine("Heightmap created.");
	mUpdating = false;
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

	logger->GetInstance().WriteLine("Wrote height map to: " + fileName);
}
