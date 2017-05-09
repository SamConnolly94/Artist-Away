#ifndef REFRACTIONSHADER_H
#define REFRACTIONSHADER_H

#include "Shader.h"
#include "Light.h"
class CReflectRefractShader :
	public CShader
{
private:
	struct ViewportBufferType
	{
		D3DXVECTOR2 ViewportSize;
		D3DXVECTOR4 viewportPadding1;
		D3DXVECTOR2 viewportPadding2;
	};

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

	struct TerrainAreaBufferType
	{
		float snowHeight;
		float grassHeight;
		float dirtHeight;
		float sandHeight;
	};

	struct PositioningBufferType
	{
		float yOffset;
		float WaterPlaneY;
		D3DXVECTOR2 posPadding;
		D3DXVECTOR4 posPadding2;
	};

	struct FoliageBufferType
	{
		D3DXVECTOR3 WindDirection;
		float FrameTime;
		D3DXVECTOR3 FoliageTranslation;
		float WindStrength;
	};

	struct CloudBufferType
	{
		D3DXVECTOR2 Cloud1Movement;
		D3DXVECTOR2 Cloud2Movement;
		float Brightness;
		D3DXVECTOR2 ViewportSize;
		float WaterPlaneY;
	};

	struct MapBufferType
	{
		bool useAlphaMap;
		bool useSpecularMap;
		D3DXVECTOR3 padding2;
	};

	struct SkyboxBufferType
	{
		D3DXVECTOR4 apexColour;
		D3DXVECTOR4 centreColour;
		D3DXVECTOR2 ViewportSize;
		float WaterPlaneY;
		float gradientPadding;
	};
public:
	CReflectRefractShader();
	~CReflectRefractShader();
public:
	bool Initialise(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool RefractionRender(ID3D11DeviceContext* deviceContext, int indexCount);
	bool ReflectionRender(ID3D11DeviceContext* deviceContext, int indexCount);
	bool RenderCloudReflection(ID3D11DeviceContext* deviceContext, int indexCount);
	bool RenderFoliageRefraction(ID3D11DeviceContext* deviceContext, int vertexCount, int instanceCount);
	bool RenderModelRefraction(ID3D11DeviceContext * deviceContext, int indexCount);
	bool RenderModelReflection(ID3D11DeviceContext * deviceContext, int indexCount);
	bool RenderSkyboxReflection(ID3D11DeviceContext * deviceContext, int indexCount);
private:
	D3DXMATRIX mWorldMatrix; 
	D3DXMATRIX mViewMatrix;
	D3DXMATRIX mProjMatrix; 
	D3DXVECTOR4 mAmbientColour;
	D3DXVECTOR4 mDiffuseColour;
	D3DXVECTOR3 mLightDirection;
	float mSpecularPower;
	D3DXVECTOR4 mSpecularColour;
	D3DXVECTOR3 mLightPosition;
	D3DXVECTOR2 mViewportSize; 
	float mSnowHeight; 
	float mGrassHeight; 
	float mDirtHeight; 
	float mSandHeight; 
	float mTerrainYOffset;
	float mWaterPlaneYOffset;
	// Don't manage memory for these, we should never allocate it in the class to the texture resoureces, so deallocate / release it outside too.
	ID3D11ShaderResourceView* mpWaterHeightMap; 
	ID3D11ShaderResourceView* mpDirtTexArray[2];
	ID3D11ShaderResourceView* mpGrassTextures[2];
	ID3D11ShaderResourceView* mpPatchMap;
	ID3D11ShaderResourceView* mpRockTextures[2];

	/////////////////////////////
	// Cloud properties
	/////////////////////////////

	D3DXVECTOR2 mCloud1Movement;
	D3DXVECTOR2 mCloud2Movement;
	float mCloudBrightness;
	ID3D11ShaderResourceView* mpCloudTex1;
	ID3D11ShaderResourceView* mpCloudTex2;

	ID3D11ShaderResourceView** mpModelTextures;
	int mModelTextureCount = 0;

	D3DXVECTOR3 mViewDir;
	bool mUseSpecular = false;
	bool mUseAlpha = false;

	/////////////////////////////////////
	// Skybox properties.
	////////////////////////////////////
	D3DXVECTOR4 mApexColour;
	D3DXVECTOR4 mCentreColour;

public:
	void SetLightProperties(CLight* light);
	void SetViewportProperties(int screenWidth, int screenHeight);
	void SetTerrainAreaProperties(float snowHeight, float grassHeight, float dirtHeight, float sandHeight);
	void SetPositioningProperties(float terrainPositionY, float waterPlanePositionY);
	void SetWaterHeightmap(ID3D11ShaderResourceView* waterHeightMap);
	void SetDirtTextureArray(CTexture** dirtTexArray);
	void SetGrassTextureArray(CTexture** grassTexArray);
	void SetPatchMap(CTexture* patchMap);
	void SetRockTexture(CTexture** rockTexArray);
	void SetCloudTextures(ID3D11ShaderResourceView* tex1, ID3D11ShaderResourceView* tex2);
	void SetCloudMovement(D3DXVECTOR2 cloud1Movement, D3DXVECTOR2 cloud2Movement);
	void SetCloudBrightness(float value);
	void SetModelTex(ID3D11ShaderResourceView** resources);
	void SetViewDirection(D3DXVECTOR3 viewDir);
	void SetUseSpecular(bool value);
	void SetUseAlpha(bool value);
	void SetModelTexCount(int value);
	void SetSkyboxColours(D3DXVECTOR4 apexColour, D3DXVECTOR4 centreColour);
private:
	bool InitialiseShader(ID3D11Device * device, HWND hwnd, std::string vsFilename, std::string psFilename, std::string reflectionPSFilename, std::string modelRefractionPSName, std::string modelReflectionPSName);
	bool InitialiseFoliageShader(ID3D11Device * device, HWND hwnd, std::string foliageRefractionVSName, std::string foliageRefractionPSName);
	bool InitialiseCloudShader(ID3D11Device * device, HWND hwnd, std::string vsFilename, std::string psFilename);
	bool InitialiseSkyboxShader(ID3D11Device * device, HWND hwnd, std::string vsFilename, std::string psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob *errorMessage, HWND hwnd, std::string shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext);
	bool SetFoliageShaderParameters(ID3D11DeviceContext* deviceContext);
	bool SetCloudShaderParameters(ID3D11DeviceContext* deviceContext);
	bool SetSkyboxShaderParameters(ID3D11DeviceContext* deviceContext);
	bool SetModelRefractionShaderParameters(ID3D11DeviceContext* deviceContext);
	bool SetModelReflectionShaderParameters(ID3D11DeviceContext* deviceContext);

	void RenderRefractionShader(ID3D11DeviceContext * deviceContext, int indexCount);
	void RenderReflectionShader(ID3D11DeviceContext * deviceContext, int indexCount);
	void RenderFoliageRefractionShader(ID3D11DeviceContext * deviceContext, int vertexCount, int instanceCount);
	void RenderCloudReflectionShader(ID3D11DeviceContext * deviceContext, int indexCount);
	void RenderSkyboxReflectionShader(ID3D11DeviceContext * deviceContext, int indexCount);
	void RenderModelRefractionShader(ID3D11DeviceContext * deviceContext, int indexCount);
	void RenderModelReflectionShader(ID3D11DeviceContext * deviceContext, int indexCount);
private:
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpRefractionPixelShader;

	ID3D11PixelShader* mpReflectionPixelShader;
	ID3D11PixelShader* mpModelReflectionPixelShader;
	ID3D11PixelShader* mpModelRefractionPixelShader;

	ID3D11VertexShader* mpFoliageRefractionVertexShader;
	ID3D11PixelShader* mpFoliageRefractionPixelShader;

	ID3D11VertexShader* mpCloudVertexShader;
	ID3D11PixelShader* mpCloudPixelShader;

	ID3D11VertexShader* mpSkyboxVertexShader;
	ID3D11PixelShader* mpSkyboxPixelShader;

	ID3D11InputLayout* mpModelLayout;
	ID3D11InputLayout* mpSkyboxLayout;
	ID3D11InputLayout* mpCloudLayout;
	ID3D11InputLayout* mpFoliageLayout;
	ID3D11SamplerState* mpTrilinearWrap;
	ID3D11SamplerState* mpPointClamp;
	ID3D11SamplerState* mpBilinearMirror;
	ID3D11Buffer* mpLightBuffer;
	ID3D11Buffer* mpViewportBuffer;
	ID3D11Buffer* mpTerrainAreaBuffer;
	ID3D11Buffer* mpPositioningBuffer;
	ID3D11Buffer* mpFoliageBuffer;
	ID3D11Buffer* mpCloudBuffer;
	ID3D11Buffer* mpSkyboxBuffer;
	ID3D11Buffer* mpMapBuffer;
public:
	void SetGrassTexture(ID3D11ShaderResourceView * grassTexture);
	void SetGrassAlphaTexture(ID3D11ShaderResourceView * alphaTexture);
	void SetWindDirection(D3DXVECTOR3 direction);
	void SetFrameTime(float frameTime);
	void SetWindStrength(float strength);
	void SetTranslation(D3DXVECTOR3 translation);
private:
	ID3D11ShaderResourceView * mpGrassTexture;
	ID3D11ShaderResourceView * mpGrassAlphaTexture;
	D3DXVECTOR3 mWindDirection;
	float mFrameTime;
	float mStrength;
	D3DXVECTOR3 mTranslation;
};

#endif