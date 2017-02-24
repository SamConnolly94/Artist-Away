#ifndef TERRAIN_H
#define TERRAIN_H

#include <d3d11.h>
#include <d3dx10math.h>
#include "PrioEngineVars.h"
#include "ModelControl.h"
#include "Texture.h"
#include "Mesh.h"

#include <vector>
#include <sstream>
#include "PrioEngineVars.h"

class CTerrain : public CModelControl
{
private:
	CLogger* logger;
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 UV;
		D3DXVECTOR3 normal;
	};

	enum VertexAreaType
	{
		Snow,
		Grass,
		Dirt,
		Sand
	};

public:
	CTerrain(ID3D11Device* device);
	~CTerrain();
private:
	void ReleaseHeightMap();
	CTexture** mpTextures;
	const unsigned int kmNumberOfTextures = 2;
	CTexture** mpGrassTextures;
	const unsigned int kNumberOfGrassTextures = 2;
	CTexture** mpRockTextures;
	const unsigned int kNumberOfRockTextures = 2;
	float mLowestPoint;
	float mHighestPoint;
public:
	bool CreateTerrain(ID3D11Device* device);
	void Render(ID3D11DeviceContext* context);
	CTexture** GetTexturesArray();
	CTexture** GetGrassTextureArray();
	CTexture** GetRockTextureArray();
	unsigned int GetNumberOfTextures() { return kmNumberOfTextures; };
	unsigned int GetNumberOfGrassTextures();
	unsigned int GetNumberOfRockTextures();
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
private:
	struct TerrainEntityType
	{
		D3DXVECTOR3 position;
		float rotation;
		float scale;
	};
	std::vector<TerrainEntityType> mTreesInfo;
	std::vector<TerrainEntityType> mPlantsInfo;
	bool PositionTreeHere();
	bool CreateTree(D3DXVECTOR3 position);
	bool PositionPlantHere();
	bool CreatePlant(D3DXVECTOR3 position);
	CTerrain::VertexAreaType FindAreaType(float height);
public:
	std::vector<TerrainEntityType> GetTreeInformation() { return mTreesInfo; };
	std::vector<TerrainEntityType> GetPlantInformation() { return mPlantsInfo; };
private:
	float mSnowHeight;	// 60% and upwards will be snow.
	float mGrassHeight;	// 30% and upwards will be grass.
	float mSandHeight;	// 10% and upwards will be sand.
	float mDirtHeight;	// 15% and upwards will be dirt.
public:
	float GetSnowHeight() { return mSnowHeight; };
	float GetGrassHeight() { return mGrassHeight; };
	float GetSandHeight() { return mSandHeight; };
	float GetDirtHeight() { return mDirtHeight; };
};

#endif