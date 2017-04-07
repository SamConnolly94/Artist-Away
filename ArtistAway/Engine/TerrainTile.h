#ifndef TERRAINTILE_H
#define TERRAINTILE_H

#include <d3dx10math.h>

class CTerrainTile
{
public:
	enum TileType
	{
		Rock,
		Grass,
		Dirt,
		Sand
	};
public:
	CTerrainTile();
	~CTerrainTile();
private:
	D3DXVECTOR3 mCentrePosition;
	D3DXVECTOR3 mLowerLeftVertexPosition;
	D3DXVECTOR3 mLowerRightVertexPosition;
	D3DXVECTOR3 mUpperLeftVertexPosition;
	D3DXVECTOR3 mUpperRightVertexPosition;
	TileType mTileType;

	// Pointers to the surrounding tiles.
	CTerrainTile* mpLeft;
	CTerrainTile* mpRight;
	CTerrainTile* mpUp;
	CTerrainTile* mpDown;
public:
	void SetCentrePosition(D3DXVECTOR3 position);
	void SetLeftTile(CTerrainTile* tile);
	void SetRightTile(CTerrainTile* tile);
	void SetUpTile(CTerrainTile* tile);
	void SetDownTile(CTerrainTile* tile);
	void SetTileType(TileType tileType);
	void SetLowerLeftVertexPosition(D3DXVECTOR3 position);
	void SetLowerRightVertexPosition(D3DXVECTOR3 position);
	void SetUpperLeftVertexPosition(D3DXVECTOR3 position);
	void SetUpperRightVertexPosition(D3DXVECTOR3 position);

public:
	D3DXVECTOR3 GetCentrePosition();
	CTerrainTile* GetLeftTile();
	CTerrainTile* GetRightTile();
	CTerrainTile* GetUpTile();
	CTerrainTile* GetDownTile();
	D3DXVECTOR3 GetLowerLeftVertexPosition();
	D3DXVECTOR3 GetLowerRightVertexPosition();
	D3DXVECTOR3 GetUpperLeftVertexPosition();
	D3DXVECTOR3 GetUpperRightVertexPosition();
};

#endif