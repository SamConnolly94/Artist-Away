#ifndef WATERSHADER_H
#define WATERSHADER_H

#include "Shader.h"
#include "Light.h"

class CWaterShader :
	public CShader
{
// Constant buffer structs
private:
	// Vertex shader and surface pixel shader buffer reg 1.
	struct WaterBufferType
	{
		D3DXVECTOR2 WaterMovement;
		float WaveHeight;
		float WaveScale;
		float RefractionDistortion;
		float ReflectionDistortion;
		float MaxDistortionDistance;
		float RefractionStrength;
		float ReflectionStrength;
		D3DXVECTOR3 waterBufferPadding;
	};

	// surface pixel shader reg 2.
	struct CameraBufferType
	{
		D3DXMATRIX CameraMatrix;
		D3DXVECTOR3	CameraPosition;
		float cameraPadding;
	};

	// Surface pixel shader reg 3.
	struct ViewportBufferType
	{
		D3DXVECTOR2 ViewportSize;
		D3DXVECTOR4 viewportPadding1;
		D3DXVECTOR2 viewportPadding2;
	};

	// Surface pixel shader reg 4
	struct LightBufferType
	{
		D3DXVECTOR4	AmbientColour;
		D3DXVECTOR4	DiffuseColour;
		D3DXVECTOR3	LightDirection;
		D3DXVECTOR4 mSpecularColour;
		D3DXVECTOR3 mLightPosition;
		float mSpecularPower;
		float lightBufferPadding;
	};
public:
	CWaterShader();
	~CWaterShader();

	bool Initialise(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool RenderSurface(ID3D11DeviceContext* deviceContext, int indexCount);

	bool RenderHeight(ID3D11DeviceContext* deviceContext, int indexCount);
private:
	D3DXVECTOR2 mWaterMovement; 
	float mWaveHeight; 
	float mWaveScale; 
	float mRefractionDistortion; 
	float mReflectionDistortion; 
	float mMaxDistortion;
	float mRefractionStrength; 
	float mReflectionStrength; 
	D3DXMATRIX mCameraMatrix; 
	D3DXVECTOR3 mCameraPosition; 
	D3DXVECTOR2 mViewportSize; 

	D3DXVECTOR4 mAmbientColour;
	D3DXVECTOR4 mDiffuseColour; 
	D3DXVECTOR3 mLightDirection; 
	float mSpecularPower;
	D3DXVECTOR4 mSpecularColour;
	D3DXVECTOR3 mLightPosition;

	ID3D11ShaderResourceView* mpNormalMap; 
	ID3D11ShaderResourceView* mpRefractionMap; 
	ID3D11ShaderResourceView* mpReflectionMap;
public:
	void SetWaterMovement(D3DXVECTOR2 movement);
	void SetWaveHeight(float height);
	void SetWaveScale(float scale);
	void SetDistortion(float refractionDistortion, float reflectionDistortion);
	void SetMaxDistortion(float maxDistortion);
	void SetRefractionStrength(float strength);
	void SetReflectionStrength(float strength);
	void SetCameraMatrix(D3DXMATRIX cameraWorld);
	void SetCameraPosition(D3DXVECTOR3 position);
	void SetViewportSize(int screenWidth, int screenHeight);
	void SetLightProperties(CLight* light);
	void SetNormalMap(CTexture* normalMap);
	void SetRefractionMap(ID3D11ShaderResourceView* refractionMap);
	void SetReflectionMap(ID3D11ShaderResourceView* reflectionMap);

private:
	bool InitialiseShader(ID3D11Device* device, HWND hwnd, std::string vsFilename, std::string surfacePsFilename, std::string heightPsFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, std::string shaderFilename);

	bool SetHeightShaderParameters(ID3D11DeviceContext* deviceContext);
	bool SetSurfaceShaderParameters(ID3D11DeviceContext* deviceContext);
	void RenderSurfaceShader(ID3D11DeviceContext* deviceContext, int indexCount);
	void RenderHeightShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* mpSurfaceVertexShader;
	ID3D11PixelShader* mpSurfacePixelShader;
	ID3D11PixelShader* mpHeightPixelShader;
	ID3D11InputLayout* mpLayout;
	ID3D11InputLayout* mpFoliageLayout;
	ID3D11SamplerState* mpTrilinearWrap;
	ID3D11SamplerState* mpBilinearMirror;
	
	ID3D11Buffer* mpWaterBuffer;
	ID3D11Buffer* mpCameraBuffer;
	ID3D11Buffer* mpViewportBuffer;
	ID3D11Buffer* mpLightBuffer;

};

#endif