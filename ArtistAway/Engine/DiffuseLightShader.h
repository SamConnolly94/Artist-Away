#ifndef DIFFUSELIGHTSHADER_H
#define DIFFUSELIGHTSHADER_H

#include "Shader.h"

class CDiffuseLightShader : public CShader
{
private:

	struct LightBufferType
	{
		D3DXVECTOR4 ambientColour;
		D3DXVECTOR4 diffuseColour;
		D3DXVECTOR3 lightDirection;
		float padding;
	};

	struct MapBufferType
	{
		bool useAlphaMap;
		bool useSpecularMap;
		D3DXVECTOR3 padding2;
		D3DXVECTOR4 padding3;
	};

public:
	CDiffuseLightShader();
	~CDiffuseLightShader();

	bool Initialise(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projMatrix, ID3D11ShaderResourceView** textures, int numberOfTextures, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColour, D3DXVECTOR4 ambientColour);
	bool UpdateMapBuffer(ID3D11DeviceContext* deviceContext, bool useAlphaMap, bool useSpecularMap);

private:
	bool InitialiseShader(ID3D11Device * device, HWND hwnd, std::string vsFilename, std::string psFilename, std::string transparentPSFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob *errorMessage, HWND hwnd, std::string shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, ID3D11ShaderResourceView** textures, int numberOfTextures, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColour, D3DXVECTOR4 ambientColour);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11PixelShader* mpTransparentPixelShader;
	ID3D11InputLayout* mpLayout;
	ID3D11Buffer* mpMatrixBuffer;
	ID3D11SamplerState* mpSampleState;
	ID3D11Buffer* mpLightBuffer;
	ID3D11Buffer* mpMapBuffer;
};

#endif