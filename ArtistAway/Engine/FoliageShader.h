#ifndef FOLIAGESHADER_H
#define FOLIAGESHADER_H

#include "Shader.h"
class CFoliageShader : public CShader
{
private:
	struct LightBufferType
	{
		D3DXVECTOR4 AmbientColour;
		D3DXVECTOR4 DiffuseColour;
		D3DXVECTOR3 LightDirection;
		float padding;
	};

	struct FoliageBufferType
	{
		D3DXVECTOR3 WindDirection;
		float FrameTime;
		D3DXVECTOR3 FoliageTranslation;
		float WindStrength;
	};
public:
	CFoliageShader();
	~CFoliageShader();
public:
	bool Initialise(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int vertexCount, int instanceCount);

private:
	bool InitialiseShader(ID3D11Device * device, HWND hwnd, std::string vsFilename, std::string psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, std::string shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext);
	void RenderShader(ID3D11DeviceContext * deviceContext, int vertexCount, int instanceCount);

private:
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11InputLayout* mpLayout;
	ID3D11SamplerState* mpSampleState;
	ID3D11Buffer* mpLightBuffer;
	ID3D11Buffer* mpFoliageBuffer;
public:
	void SetGrassTexture(ID3D11ShaderResourceView * grassTexture);
	void SetGrassAlphaTexture(ID3D11ShaderResourceView * alphaTexture);
	void SetReedTexture(ID3D11ShaderResourceView * reedTexture);
	void SetReedAlphaTexture(ID3D11ShaderResourceView * alphaTexture);
	void SetAmbientColour(D3DXVECTOR4 ambientColour);
	void SetDiffuseColour(D3DXVECTOR4 diffuseColour);
	void SetLightDirection(D3DXVECTOR3 lightDirection);
	void SetWindDirection(D3DXVECTOR3 direction);
	void SetFrameTime(float frameTime);
	void SetWindStrength(float strength);
	void SetTranslation(D3DXVECTOR3 translation);
public:
	ID3D11ShaderResourceView * mpGrassTexture;
	ID3D11ShaderResourceView * mpAlphaTexture;
	ID3D11ShaderResourceView * mpReedTexture;
	ID3D11ShaderResourceView * mpReedAlphaTexture;
	D3DXVECTOR4 mAmbientColour;
	D3DXVECTOR4 mDiffuseColour;
	D3DXVECTOR3 mLightDirection;
	D3DXVECTOR3 mWindDirection;
	float mFrameTime;
	float mStrength;
	D3DXVECTOR3 mTranslation;
};

#endif