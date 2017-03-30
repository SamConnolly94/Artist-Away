#ifndef FONTSHADER_H
#define FONTSHADER_H

#include "Shader.h"

class CFontShader : public CShader
{
private:
	struct PixelBufferType
	{
		D3DXVECTOR4 pixelColour;
		D3DXVECTOR4 pixelBufferPadding;
	};
public:
	CFontShader();
	~CFontShader();
public:
	bool Initialise(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, ID3D11ShaderResourceView* texture, D3DXVECTOR4 pixelColour);
private:
	bool InitialiseShader(ID3D11Device* device, HWND hwnd, std::string vsFilename, std::string psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, std::string shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, D3DXVECTOR4 pixelColour);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
private:
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11InputLayout* mpLayout;
	ID3D11SamplerState* mpSampleState;
	ID3D11Buffer* mpPixelBuffer;
};

#endif