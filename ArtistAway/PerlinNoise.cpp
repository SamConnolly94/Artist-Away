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
	int X = (int)std::floor(x) & 255;
	int Y = (int)std::floor(y) & 255;
	int Z = (int)std::floor(z) & 255;

	// Calculate the frequencies.
	double xf = x - floor(x);
	double yf = y - floor(y);
	double zf = z - floor(z);

	// Calculate fade curves of u, v and w.
	double u = Fade(x);
	double v = Fade(y);
	double w = Fade(z);

	// Hash the coordinates of the 8 cube coordinates (vertices) which our perlin noise will occur in.
	int aaa = p[p[p[X] + Y] + Z];
	int aba = p[p[p[X] + inc(Y)] + Z];
	int aab = p[p[p[X] + Y] + inc(Z)];
	int abb = p[p[p[X] + inc(Y)] + inc(Z)];
	int baa = p[p[p[inc(X)] + Y] + Z];
	int bba = p[p[p[inc(X)] + inc(Y)] + Z];
	int bab = p[p[p[inc(X)] + Y] + inc(Z)];
	int bbb = p[p[p[inc(X)] + inc(Y)] + inc(Z)];
	
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
	double total = 0;
	double frequency = 1;
	double amplitude = 1;
	// Max value can be used for normalising the result between 0 and 1.
	double maxValue = 0;
	
	for (unsigned int i = 0; i < octaves; i++)
	{
		total += Perlin(x * frequency, y * frequency, z * frequency) * amplitude;

		maxValue += amplitude;

		amplitude *= persistence;

		frequency *= 2;
	}

	return total / maxValue;
}

int CPerlinNoise::inc(int number)
{
	number++;
	if (repeat > 0)
	{
		number %= repeat;
	}
	return number;
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
	int h = hash & 15;                                    // Take the hashed value and take the first 4 bits of it (15 == 0b1111)
	double u = h < 8 /* 0b1000 */ ? x : y;                // If the most significant bit (MSB) of the hash is 0 then set u = x.  Otherwise y.

	double v;                                             // In Ken Perlin's original implementation this was another conditional operator (?:).  I
														  // expanded it for readability.

	if (h < 4 /* 0b0100 */)                                // If the first and second significant bits are 0 set v = y
		v = y;
	else if (h == 12 /* 0b1100 */ || h == 14 /* 0b1110*/)  // If the first and second significant bits are 1 set v = x
		v = x;
	else                                                  // If the first and second significant bits are not equal (0/1, 1/0) set v = z
		v = z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v); // Use the last 2 bits to decide if u and v are positive or negative.  Then return their addition.
}
