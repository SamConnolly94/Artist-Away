#ifndef FOLIAGE_H
#define FOLIAGE_H

#include <d3d11.h>
#include <d3dx10math.h>
#include "Texture.h"
#include <sstream>
#include <vector>
#include "FoliageQuad.h"
#include "TerrainTile.h"

class CFoliage
{
private:
	CLogger* logger;
private:
	struct InstanceType
	{
		D3DXVECTOR3 TileLLVertexPos;
		D3DXVECTOR3 TileLRVertexPos;
		D3DXVECTOR3 TileULVertexPos;
		D3DXVECTOR3 TileURVertexPos;
		D3DXVECTOR3 TileCentrePos;
	};

public:
	CFoliage();
	~CFoliage();
public:
	bool Initialise(ID3D11Device * device, CTerrainTile** terrainTiles, int terrainWidth, int terrainHeight);
	void Shutdown();
	void Update(float updateTime);
private:
	bool InitialiseBuffers(ID3D11Device * device, CTerrainTile** terrainTiles, int terrainWidth, int terrainHeight);
	void ShutdownQuads();
	void ShutdownHeightMap();
public:
	ID3D11ShaderResourceView* GetFoliageTexture() { return mpFoliageTex->GetTexture(); };
	ID3D11ShaderResourceView* GetFoliageAlphaTexture() { return mpFoliageAlphaTex->GetTexture(); };
	ID3D11ShaderResourceView* GetReedsTexture() { return mpReedsTexture->GetTexture(); };
	ID3D11ShaderResourceView* GetReedsAlphaTexture() { return mpReedsAlphaTexture->GetTexture(); };
	D3DXVECTOR3 GetTranslation() { return mFoliageTranslation; };
private:
	CTexture* mpFoliageAlphaTex;
	CTexture* mpFoliageTex;
	CTexture* mpReedsAlphaTexture;
	CTexture* mpReedsTexture;
	D3DXVECTOR3 mFoliageTranslation;
	D3DXVECTOR3 mWindDirection = { 0.0f, 0.0f, 0.2f };
	ID3D11Buffer* mpInstanceBuffer;
	CFoliageQuad* mpQuadMesh;
	std::vector<InstanceType> mIstanceInfoList;
	int mInstanceCount;
	float mFoliageMinCuttoff;
	float mFoliageMaxCutoff;
	bool mIsUpdating = false;
// Height map functions
private:
	double** mpHeightMap;
	int mWidth;
	int mHeight;
public:
	void SetWidth(int width);
	void SetHeight(int height);
	void LoadHeightMap(double** heightMap);
	bool LoadHeightMap(std::string filename);
	int GetInstanceCount();
	int GetQuadVertexCount() { return mpQuadMesh->GetVertexCount(); };
	void RenderBuffers(ID3D11DeviceContext* deviceContext, int quadIndex, int triangleIndex);
	void SetFoliageMinimumFreq(float value) { mFoliageMinCuttoff = value; };
	void SetFoliageMaximumFreq(float value) { mFoliageMaxCutoff = value; };
	bool UpdateBuffers(ID3D11Device* device, double** heightMap, int mapWidth, int mapHeight, CTerrainTile** terrainTiles, int terrainWidth, int terrainHeight);
	float GetFoliageMinimumFreq() { return mFoliageMinCuttoff; };
	float GetFoliageMaximumFreq() { return mFoliageMaxCutoff; };
	bool IsUpdating() { return mIsUpdating; };
	void SetUpdating(bool value) { mIsUpdating = value; };
};

#endif