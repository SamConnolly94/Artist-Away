#ifndef WATER_H
#define WATER_H

#include "Texture.h"
#include "RenderTexture.h"
#include "PrioEngineVars.h"
#include "ModelControl.h"
#include <memory>

class CWater : public CModelControl
{
private:
	CLogger* logger;
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texCoord;
		D3DXVECTOR3 normal;
	};
public:
	CWater();
	~CWater();
	bool Initialise(ID3D11Device* device, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint, unsigned int subDivisionX, unsigned int subDivisionZ, std::string normalMap, int screenWidth, int screenHeight);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);
	void Update(float frameTime);
private:
	unsigned int mNumVertices;
	unsigned int mNumIndices;
	ID3D11Buffer* mpVertexBuffer;
	ID3D11Buffer* mpIndexBuffer;
	CTexture* mpNormalMap;
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
public:
	unsigned int GetNumberOfIndices();
	CTexture* GetNormalMap();
	CRenderTexture* GetRefractionTexture();
	CRenderTexture* GetReflectionTexture();
	CRenderTexture* GetHeightTexture();
public:
	void SetRefractionRenderTarget(ID3D11DeviceContext * deviceContext, ID3D11DepthStencilView * depthStencilView);
	void SetReflectionRenderTarget(ID3D11DeviceContext * deviceContext, ID3D11DepthStencilView * depthStencilView);
	void SetHeightMapRenderTarget(ID3D11DeviceContext * deviceContext, ID3D11DepthStencilView * depthStencilView);
private:
	bool InitialiseBuffers(ID3D11Device* device, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint, unsigned int subDivisionX, unsigned int subDivisionZ, bool uvs = true, bool normals = true);
private:
	CRenderTexture* mpRefractionRenderTexture;
	CRenderTexture* mpReflectionRenderTexture;
	CRenderTexture* mpHeightRenderTexture;

	D3DXVECTOR2 mMovement;
	float mWaveHeight;
	float mWaveScale;
	float mRefractionDistortion;
	float mReflectionDistortion;
	float mMaxDistortionDistance;
	float mRefractionStrength;
	float mReflectionStrength;
	float mWaterDepth;
public:
	D3DXVECTOR2 GetMovement();
	float GetWaterMovementX();
	float GetWaterMovementY();
	float GetWaveHeight();
	float GetWaveScale();
	float GetRefractionDistortion();
	float GetReflectionDistortion();
	float GetMaxDistortionDistance();
	float GetRefractionStrength();
	float GetReflectionStrength();
	float GetDepth();
public:
	void SetMovementX(float movementX);
	void SetMovementY(float movementY);
	void SetWaveHeight(float height);
	void SetWaveScale(float scale);
	void SetRefractionDistortion(float refractionDistortion);
	void SetReflectionDistortion(float reflectionDistortion);
	void SetRefractionStrength(float refractionStrength);
	void SetReflectionStrength(float reflectionStrength);
	void SetDepth(float depth);
};

#endif