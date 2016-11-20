#include "PerlinNoise.h"


/* Default constructor which uses default prenumeration. */
CPerlinNoise::CPerlinNoise()
{
	gLogger->MemoryAllocWriteLine(typeid(this).name());

	// Initialise the prenumeration vector with the reccomended values from the author of the perlin noise algorithm himself.
	p = {	151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
			8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117,
			35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
			134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41,
			55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89,
			18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
			250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
			189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167,
			43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246,
			97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
			107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
			138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180			};

	// Duplicate the prenumeration vector.
	p.insert(p.end(), p.begin(), p.end());

	mFrequency = 1;
	mAmplitude = 0.5;
	repeat = -1;
}

/* Constructor for a custom seed which we want to use, this will be more random. */
CPerlinNoise::CPerlinNoise(unsigned int seed)
{
	// Set the initial size of our prenumeration vector.
	p.resize(256);

	// Fill the prenumeration vector with values from 0 - 255.
	std::iota(p.begin(), p.end(), 0);

	// Initialise a default random number generator engine class with a seed.
	std::default_random_engine engine(seed);

	// Randomly order our prenumeration vector by passing in the vector and the random number generator engine.
	std::shuffle(p.begin(), p.end(), engine);

	// Duplicate the prenumeration vector.
	p.insert(p.end(), p.begin(), p.end());

	mFrequency = 1;
	mAmplitude = 1;
	repeat = -1;
}


CPerlinNoise::~CPerlinNoise()
{
	gLogger->MemoryDeallocWriteLine(typeid(this).name());
}

double CPerlinNoise::Perlin(double x, double y, double z)
{
	if (repeat > 0)
	{
		x = static_cast<int>(x) % repeat;
		y = static_cast<int>(y) % repeat;
		z = static_cast<int>(z) % repeat;
	}

	// Discover the unit cube that contains the point.
	int xi = (int)std::floor(x) & 255;
	int yi = (int)std::floor(y) & 255;
	int zi = (int)std::floor(z) & 255;

	// Calculate the frequencies.
	double xf = x - floor(x);
	double yf = y - floor(y);
	double zf = z - floor(z);

	// Calculate fade curves of u, v and w.
	double u = Fade(xf);
	double v = Fade(yf);
	double w = Fade(zf);

	// Hash the coordinates of the 8 cube coordinates (vertices) which our perlin noise will occur in.
	int aaa, aba, aab, abb, baa, bba, bab, bbb;
	aaa = p[p[p[xi] + yi] + zi];
	aba = p[p[p[xi] + yi + 1] + zi];
	aab = p[p[p[xi] + yi] + zi + 1];
	abb = p[p[p[xi] + yi + 1] + zi + 1];
	baa = p[p[p[xi + 1] + yi] + zi];
	bba = p[p[p[xi + 1] + yi + 1] + zi];
	bab = p[p[p[xi + 1] + yi] + zi + 1];
	bbb = p[p[p[xi + 1] + yi + 1] + zi + 1];
	
	double x1;
	double x2;
	double y1;
	double y2;
	
	x1 = Lerp(u, Gradient(aaa, xf, yf, zf), Gradient(baa, xf - 1, yf, zf));
	x2 = Lerp(u, Gradient(aba, xf, yf - 1, zf), Gradient(bba, xf - 1, yf - 1, zf));
	y1 = Lerp(v, x1, x2);
	x1 = Lerp(u, Gradient(aab, xf, yf, zf - 1), Gradient(bab, xf - 1, yf, zf - 1));
	x2 = Lerp(u, Gradient(abb, xf, yf - 1, zf - 1), Gradient(bbb, xf - 1, yf - 1, zf - 1));
	y2 = Lerp(v, x1, x2);

	return (Lerp(w, y1, y2) + 1.0) / 2.0;
}

/* A function which returns a number of layers of perlin noise. */
double CPerlinNoise::OctavePerlin(double x, double y, double z, unsigned int octaves, double persistence)
{
	//return total / maxValue;
	double total = 0.0;

	double maxVal = 0.0;
	for (unsigned int i = 0; i < octaves; i++)
	{
		total += Perlin(x * mFrequency, y * mFrequency, z * mFrequency) * mAmplitude;

		maxVal += mAmplitude;

		mAmplitude *= persistence;

		mFrequency *= 2.0f;
	}
	return total / maxVal;
}

double CPerlinNoise::Fade(double t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double CPerlinNoise::Lerp(double t, double a, double b)
{
	return a + t * (b - a);
}

double CPerlinNoise::Gradient(int hash, double x, double y, double z)
{
	int h = hash & 15;
	// Convert lower 4 bits of hash into 12 gradient directions
	double u = h < 8 ? x : y,
		v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
