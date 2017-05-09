#ifndef TERRAINSHADER_H
#define TERRAINSHADER_H

#include "Shader.h"

class CTerrainShader : public CShader
{
private:
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
		D3DXVECTOR4 terrainInfoPadding;
	};

	struct PositioningBufferType
	{
		float yOffset;
		D3DXVECTOR3 posPadding;
		D3DXVECTOR4 posPadding2;
	};
	struct TerrainAreaBufferType
	{
		float snowHeight;
		float grassHeight;
		float dirtHeight;
		float sandHeight;
		D3DXVECTOR4 terrainAreaPadding;
	};

	struct SlopeBufferType
	{
		float slopeGrassCutoff;
		float slopeRockCuttoff;
		D3DXVECTOR2 slopePadding;
	};
public:
	CTerrainShader();
	~CTerrainShader();

	bool Initialise(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, CTexture** texturesArray, unsigned int numberOfTextures, CTexture** grassTexturesArray, unsigned int numberOfGrassTextures,
		CTexture** rockTexturesArray, unsigned int numberOfRockTextures,
		D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColour, 	D3DXVECTOR4 ambientColour, float highestPos, float lowestPos, D3DXVECTOR3 worldPosition,
		float snowHeight, float grassHeight, float dirtHeight, float sandHeight, float slopeGrassCutoff, float slopeRockCutoff);

private:
	bool InitialiseShader(ID3D11Device* device, HWND hwnd, std::string vsFilename, std::string psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, std::string shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, 
		CTexture** textureArray, unsigned int numberOfTextures, CTexture** grassTexturesArray, unsigned int numberOfGrassTextures,
		CTexture** rockTexturesArray, unsigned int numberOfRockTextures,
		D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColour, D3DXVECTOR4 ambientColour, 
		float highestPos, float lowestPos, D3DXVECTOR3 worldPosition, float snowHeight, float grassHeight, float dirtHeight, float sandHeight,
		float slopeGrassCutoff, float slopeRockCutoff);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11InputLayout* mpLayout;
	ID3D11SamplerState* mpSampleState;
	ID3D11Buffer* mpLightBuffer;
	ID3D11Buffer* mpPositioningBuffer;
	ID3D11Buffer* mpTerrainAreaBuffer;
	ID3D11Buffer* mpSlopeBuffer;
	CTexture* mpPatchMap;
};

#endif