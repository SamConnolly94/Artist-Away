#ifndef CUBE_H
#define CUBE_H
#include <d3d11.h>
#include <D3DX10math.h>
#include "Primitive.h"

class CCube : public CPrimitive
{
public:
	CCube(WCHAR* textureFilename);
	CCube(WCHAR* filename, bool useLighting);
	CCube(PrioEngine::RGBA colour);

	~CCube();

	// Virtual functions redefined.
public:
	bool Initialise(ID3D11Device* device);

private:
	bool InitialiseBuffers(ID3D11Device * device);
};

#endif