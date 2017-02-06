#ifndef TERRAIN_H
#define TERRAIN_H

#include <d3d11.h>
#include <d3dx10math.h>
#include "PrioEngineVars.h"
#include "ModelControl.h"
#include "Texture.h"

#include <vector>
#include <sstream>

class CTerrain : public CModelControl
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 UV;
		D3DXVECTOR3 normal;
	};
public:
	CTerrain(ID3D11Device* device);
	~CTerrain();
private:
	void ReleaseHeightMap();
	CTexture** mpTextures;
	const unsigned int kmNumberOfTextures = 5;
	float mLowestPoint;
	float mHighestPoint;
public:
	bool CreateTerrain(ID3D11Device* device);
	void Render(ID3D11DeviceContext* context);
	CTexture** GetTexturesArray();
	CTexture* GetTexture() { return mpTextures[0]; };
	unsigned int GetNumberOfTextures() { return kmNumberOfTextures; };
private:
	bool InitialiseBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* context);
	PrioEngine::Math::VEC3 CalculateNormal(VertexType* vertices, int index);
private:
	int mWidth;
	int mHeight;
	int mMaxWidth;
	int mMaxHeight;
	int mVertexCount;
	int mIndexCount;
	double** mpHeightMap;
	// Buffer to store our vertices.
	ID3D11Buffer* mpVertexBuffer;
	// Buffer to store our indices.
	ID3D11Buffer* mpIndexBuffer;

	// A flag which tracks whether we have loaded in a heightmap or not.
	bool mHeightMapLoaded;
// Getters
public:
	int GetVertexCount() { return mVertexCount; };
	int GetIndexCount() { return mIndexCount; }; 
	int GetWidth() { return mWidth; };
	int GetHeight() { return mHeight; };
	float GetHighestPoint() { return mHighestPoint; };
	float GetLowestPoint() { return mLowestPoint; };
// Setters
public:
	void SetWidth(int value) { mWidth = value; };
	void SetHeight(int value) { mHeight = value; };
// Loading functions.
public:
	void LoadHeightMap(double** heightMap);
	bool LoadHeightMapFromFile(std::string filename);
	bool UpdateBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext, double** heightMap, int newWidth, int newHeight);
// Update functions.
public:
	void UpdateMatrices(D3DXMATRIX& world);
};

#endif