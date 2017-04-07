#ifndef FOLIAGEQUAD_H
#define FOLIAGEQUAD_H

#include <d3d11.h>
#include <d3dx10math.h>
#include "PrioEngineVars.h"

class CFoliageQuad
{
private:
	CLogger* logger;
public:
	struct QuadType
	{
		D3DXVECTOR3 Position[4];
		D3DXVECTOR2 UV[4];
		D3DXVECTOR3 Normal[4];
	};

	struct FoliageVertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 UV;
		D3DXVECTOR3 normal;
		unsigned int IsTopVertex;
		unsigned int Type;
		unsigned int VertexIndex;
		D3DXVECTOR3 instanceTileLLVertexPos;
		D3DXVECTOR3 instanceTileLRVertexPos;
		D3DXVECTOR3 instanceTileULVertexPos;
		D3DXVECTOR3 instanceTileURVertexPos;
		D3DXVECTOR3 instanceTileCentrePos;
	};

public:
	CFoliageQuad();
	~CFoliageQuad();
	bool Initialise(ID3D11Device* device);
	void Shutdown();
private:
	void GeneratePoints();
	bool InitialiseBuffers(ID3D11Device* device);
private:
	QuadType mFoliageRect[3];
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mCentrePos;
	const int mVertexCount = 3;
	ID3D11Buffer* mpVertexBufferRect1Tri1;
	ID3D11Buffer* mpVertexBufferRect1Tri2;
	ID3D11Buffer* mpVertexBufferRect2Tri1;
	ID3D11Buffer* mpVertexBufferRect2Tri2;
	ID3D11Buffer* mpVertexBufferRect3Tri1;
	ID3D11Buffer* mpVertexBufferRect3Tri2;
public:
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetCentrePos();
	int GetVertexCount() { return mVertexCount; };
	ID3D11Buffer* GetVertexBufferRect1Tri1() { return mpVertexBufferRect1Tri1; };
	ID3D11Buffer* GetVertexBufferRect1Tri2() { return mpVertexBufferRect1Tri2; };
	ID3D11Buffer* GetVertexBufferRect2Tri1() { return mpVertexBufferRect2Tri1; };
	ID3D11Buffer* GetVertexBufferRect2Tri2() { return mpVertexBufferRect2Tri2; };
	ID3D11Buffer* GetVertexBufferRect3Tri1() { return mpVertexBufferRect3Tri1; };
	ID3D11Buffer* GetVertexBufferRect3Tri2() { return mpVertexBufferRect3Tri2; };
};

#endif