#ifndef CUBE_H
#define CUBE_H
#include <d3d11.h>
#include <D3DX10math.h>
#include "Primitive.h"
#include "PrioEngineVars.h"

class CCube : public CPrimitive
{
private:
	CLogger* logger;
public:
	CCube(std::string textureFilename);
	CCube(std::string filename, bool useLighting);
	CCube(PrioEngine::RGBA colour);

	~CCube();

	// Virtual functions redefined.
public:
	bool Initialise(ID3D11Device* device);

private:
	bool InitialiseBuffers(ID3D11Device * device);
};

#endif