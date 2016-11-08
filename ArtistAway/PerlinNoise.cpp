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
}


CPerlinNoise::~CPerlinNoise()
{
	gLogger->MemoryDeallocWriteLine(typeid(this).name());
}

double CPerlinNoise::Perlin(double x, double y, double z)
{
	// Discover the unit cube that contains the point.
	int X = (int)std::floor(x) & 255;
	int Y = (int)std::floor(y) & 255;
	int Z = (int)std::floor(z) & 255;

	// Find the relative positions of the point in a cube.
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Calculate fade curves of u, v and w.
	double u = Fade(x);
	double v = Fade(y);
	double w = Fade(z);

	// Hash the coordinates of the 8 cube coordinates (vertices) which our perlin noise will occur in.
	int a = p[X] + Y;
	int b = p[a] + Z;
	int c = p[a + 1] + Z;
	int d = p[X + 1] + Y;
	int e = p[d] + Z;
	int f = p[d + 1] + Z;

	// Add blended results from 8 corners of cube.
	double result = Lerp(w, Lerp(v, Lerp(u, Gradient(p[b], x, y, z), Gradient(p[e], x - 1, y, z)), Lerp(u, Gradient(p[c], x, y - 1, z), Gradient(p[f], x - 1, y - 1, z))), Lerp(v, Lerp(u, Gradient(p[b + 1], x, y, z - 1), Gradient(p[e + 1], x - 1, y, z - 1)), Lerp(u, Gradient(p[c + 1], x, y - 1, z - 1), Gradient(p[f + 1], x - 1, y - 1, z - 1))));

	return (result + 1.0) / 2.0;
}

double CPerlinNoise::Fade(double time)
{
	return time * time * time * (time * (time *  6 - 15) + 10);
}

double CPerlinNoise::Lerp(double time, double a, double b)
{
	return a + time * (b - a);
}

double CPerlinNoise::Gradient(int hash, double x, double y, double z)
{
	int h = hash & 15;

	// Convert lower 4 bits of hash into gradient directions.
	double u;
	double v;

	if (h < 8)
	{
		u = x;
	}
	else
	{
		u = y;
	}

	if (h < 4)
	{
		v = y;
	}
	else if (h == 12 || h == 14)
	{
		v = x;
	}
	else
	{
		v = z;
	}

	if (h & 1 != 0)
	{
		u = -u;
	}

	if (h & 2 != 0)
	{
		v = -v;
	}

	return u + v;
}
