#ifndef TERRAINAREA_H
#define TERRAINAREA_H

#include "PrioEngineVars.h"
#include "TerrainTile.h"
#include <vector>
#include <d3d11.h>
#include <d3dx10math.h>
#include "Texture.h"

class CTerrainArea
{
public:
	CTerrainArea();
	~CTerrainArea();
private:
	std::vector<CTerrainTile> mpTiles;
	int mNumberOfVertices;
	int mNumberOfIndices;
	D3DXVECTOR4 mColour;
	ID3D11Buffer* mpVertexBuffer;
	ID3D11Buffer* mpIndexBuffer;
	CTexture* mpTexture;
public:
	bool LoadTexture(ID3D11Device* device, WCHAR * filename);
	void AddTile(CTerrainTile tile);
	void SetColour(D3DXVECTOR4 colour);
	void ClearAllTiles();
	bool EraseTile(CTerrainTile tile);
	bool SetBuffers(ID3D11Device* device);
	void Render(ID3D11DeviceContext * context);
	void Shutdown();
	int GetNumberOfIndices() { return mNumberOfIndices; };
	CTexture* GetTexture() { return mpTexture; };
private:
	void RenderBuffers(ID3D11DeviceContext * context);
};

#endif