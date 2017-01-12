#ifndef TERRAINTILE_H
#define TERRAINTILE_H

#include "PrioEngineVars.h"

#include <d3d11.h>
#include <d3dx10math.h>

class CTerrainTile
{
public:
	enum TerrainType
	{
		Grass,
		Snow,
		Sand
	};
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 UV;
		D3DXVECTOR3 normal;
	};
public:
	CTerrainTile();
	~CTerrainTile();
// Leaving these public makes life so much easier. Bad practice, but so much time / code saved in the long run of things, instead of getters and setters.
public:
	TerrainType terrainType;
	VertexType mVertices[3];
public:
	bool Compare(CTerrainTile tile);
};

#endif