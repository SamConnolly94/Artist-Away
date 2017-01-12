#include "TerrainTile.h"



CTerrainTile::CTerrainTile()
{
}


CTerrainTile::~CTerrainTile()
{
}

bool CTerrainTile::Compare(CTerrainTile tile)
{
	if (tile.mVertices == mVertices && 
		tile.terrainType == terrainType)
	{
		return true;
	}
	return false;
}