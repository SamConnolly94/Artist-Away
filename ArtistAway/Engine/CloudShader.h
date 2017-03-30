#ifndef CLOUDSHADER_H
#define CLOUDSHADER_H

#include "Shader.h"
class CCloudShader : public CShader
{
private:
	struct CloudBufferType
	{
		D3DXVECTOR2 Cloud1Movement;
		D3DXVECTOR2 Cloud2Movement;
		float Brightness;
		D3DXVECTOR3 cloudBufferPadding;
	};
public:
	CCloudShader();
	~CCloudShader();
public:
	bool Initialise(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount);
private:
	bool InitialiseShader(ID3D11Device* device, HWND hwnd, std::string vsFilename, std::string psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, std::string shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
private:
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11InputLayout* mpLayout;
	ID3D11SamplerState* mpTrilinearWrap;
	ID3D11Buffer* mpCloudBuffer;
private:
	ID3D11ShaderResourceView* mpCloudTexture1;
	ID3D11ShaderResourceView* mpCloudTexture2;
	float mBrightness;
	D3DXVECTOR2 mCloud1Movement;
	D3DXVECTOR2 mCloud2Movement;
public:
	void SetCloudTexture1(ID3D11ShaderResourceView* resource);
	void SetCloudTexture2(ID3D11ShaderResourceView* resource);
	void SetBrightness(float brightness);
	void SetCloud1Movement(float movementX, float movementZ);
	void SetCloud2Movement(float movementX, float movementZ);
};

#endif