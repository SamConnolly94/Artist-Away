#ifndef SKYBOXSHADER_H
#define SKYBOXSHADER_H

#include "Shader.h"

class CSkyboxShader : public CShader
{
public:
	CSkyboxShader();
	~CSkyboxShader();
private:
	struct GradientBufferType
	{
		D3DXVECTOR4 apexColour;
		D3DXVECTOR4 centreColour;
	};
public:
	bool Initialise(ID3D11Device* device, HWND hWnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXVECTOR4 apexColour, D3DXVECTOR4 centreColour);
private:
	bool InitialiseShader(ID3D11Device* device, HWND hWnd, std::string vertexShader, std::string pixelShader);
	void ShutdownShader();

	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, std::string shaderFilename);
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXVECTOR4 apexColour, D3DXVECTOR4 centreColour);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11InputLayout* mpLayout;
	ID3D11Buffer* mpMatrixBuffer;
	ID3D11Buffer* mpGradientBuffer;
};

#endif