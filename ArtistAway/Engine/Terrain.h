#ifndef TERRAIN_H
#define TERRAIN_H

#include <d3d11.h>
#include <d3dx10math.h>
#include "PrioEngineVars.h"
#include "ModelControl.h"
#include "Texture.h"

#include <sstream>

class CTerrainGrid : public CModelControl
{
public:
	CTerrainGrid(ID3D11Device* device);
	~CTerrainGrid();
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 UV;
		D3DXVECTOR3 normal;
	};
	void ReleaseHeightMap();
	CTexture* mpTexture;
	float mLowestPoint;
	float mHighestPoint;
public:
	bool CreateGrid();
	void Render(ID3D11DeviceContext* context);
	CTexture* GetTexture() { return mpTexture; };
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

	ID3D11Device* mpDevice;
// Getters and setters.
public:
	int GetVertexCount() { return mVertexCount; };
	int GetIndexCount() { return mIndexCount; }; 
	int GetWidth() { return mWidth; };
	int GetHeight() { return mHeight; };
	void SetWidth(int value) { mWidth = value; };
	void SetHeight(int value) { mHeight = value; };
	float GetHighestPoint() { return mHighestPoint; };
	float GetLowestPoint() { return mLowestPoint; };
	void LoadHeightMap(double** heightMap);
	void LoadHeightMapFromFile(std::string filename);
	bool UpdateBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext, double** heightMap, int newWidth, int newHeight);
};

#endif