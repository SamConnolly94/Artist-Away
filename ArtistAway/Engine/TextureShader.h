#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include "PrioEngineVars.h"


class CTextureShader
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
public:
	CTextureShader();
	~CTextureShader();
public:
	bool Initialise(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, ID3D11ShaderResourceView* texture);

private:
	bool InitialiseShader(ID3D11Device * device, HWND hwnd, std::string vsFilename, std::string psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, std::string shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11InputLayout* mpLayout;
	ID3D11Buffer* mpMatrixBuffer;
	ID3D11SamplerState* mpSampleState;

	;
};

#endif