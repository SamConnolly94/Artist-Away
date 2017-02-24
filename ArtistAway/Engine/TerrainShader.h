#ifndef TERRAINSHADER_H
#define TERRAINSHADER_H

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include "PrioEngineVars.h"
#include "Texture.h"


class CTerrainShader
{
private:
	CLogger* logger;
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct LightBufferType
	{
		D3DXVECTOR4 ambientColour;
		D3DXVECTOR4 diffuseColour;
		D3DXVECTOR3 lightDirection;
		float padding;
	};

	struct TerrainInfoBufferType
	{
		float highestPosition;
		float lowestPosition;
		D3DXVECTOR2 padding2;
	};

	struct PositioningBufferType
	{
		float yOffset;
		D3DXVECTOR3 posPadding;
	};
	struct TerrainAreaBufferType
	{
		float snowHeight;
		float grassHeight;
		float dirtHeight;
		float sandHeight;
	};
public:
	CTerrainShader();
	~CTerrainShader();

	bool Initialise(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projMatrix, CTexture** texturesArray, unsigned int numberOfTextures, CTexture** grassTexturesArray, unsigned int numberOfGrassTextures,
		CTexture** rockTexturesArray, unsigned int numberOfRockTextures,
		D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColour, 	D3DXVECTOR4 ambientColour, float highestPos, float lowestPos, D3DXVECTOR3 worldPosition,
		float snowHeight, float grassHeight, float dirtHeight, float sandHeight);

private:
	bool InitialiseShader(ID3D11Device* device, HWND hwnd, std::string vsFilename, std::string psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, std::string shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
		D3DXMATRIX projMatrix, CTexture** textureArray, unsigned int numberOfTextures, CTexture** grassTexturesArray, unsigned int numberOfGrassTextures,
		CTexture** rockTexturesArray, unsigned int numberOfRockTextures,
		D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColour, D3DXVECTOR4 ambientColour, 
		float highestPos, float lowestPos, D3DXVECTOR3 worldPosition, float snowHeight, float grassHeight, float dirtHeight, float sandHeight);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11InputLayout* mpLayout;
	ID3D11Buffer* mpMatrixBuffer;
	ID3D11SamplerState* mpSampleState;
	ID3D11Buffer* mpLightBuffer;
	ID3D11Buffer* mpPositioningBuffer;
	ID3D11Buffer* mpTerrainAreaBuffer;
	CTexture* mpPatchMap;
};

#endif