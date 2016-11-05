#ifndef TRIANGLE_H
#define TRIANGLE

#include <d3d11.h>
#include <D3DX10math.h>
#include "Primitive.h"

class CTriangle : public CPrimitive
{
	public:
		CTriangle(WCHAR* textureFilename);
		CTriangle(WCHAR* filename, bool useLighting);
		CTriangle(PrioEngine::RGBA colour);

		~CTriangle();

		// Virtual functions redefined.
	public:
		bool Initialise(ID3D11Device* device);

	private:
		bool InitialiseBuffers(ID3D11Device * device);

};

#endif