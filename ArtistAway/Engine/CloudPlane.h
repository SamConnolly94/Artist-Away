#ifndef CLOUDPLANE_H
#define CLOUDPLANE_H

#include <d3d11.h>
#include <d3dx10math.h>
#include <string>
#include "Texture.h"
#include "PrioEngineVars.h"

class CCloudPlane
{
private:
	CLogger* logger;
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 uv;
	};
public:
	CCloudPlane();
	~CCloudPlane();
public:
	bool Initialise(ID3D11Device* device, std::string textureFilename1, std::string textureFilename2);
	bool Render(ID3D11DeviceContext* deviceContext);
	void Shutdown();
	void Update(float updateTime);
public:
	int GetIndexCount();
	ID3D11ShaderResourceView* GetCloudTexture1();
	ID3D11ShaderResourceView* GetCloudTexture2();
	float GetBrightness();
	D3DXVECTOR2 GetMovement(int cloudIndex);
private:
	bool InitialisePlane(int planeResolution, float planeWidth, float planeTop, float planeBottom, int textureRepeat);
	void ShutdownPlane();

	bool InitialiseBuffers(ID3D11Device* device, int planeResolution);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadCloudTextures(ID3D11Device* device, std::string filename1, std::string filename2);
	void ReleaseCloudTextures();
private:
	VertexType* mpCloudPlane;
	int mVertexCount;
	int mIndexCount;
	ID3D11Buffer* mpVertexBuffer;
	ID3D11Buffer* mpIndexBuffer;
	CTexture* mpCloudTexture1;
	CTexture* mpCloudTexture2;
	float mBrightness;
	//float mMovementSpeed[4];
	D3DXVECTOR2 mMovementSpeed[2];
	//float mTextureMovement[4];
	D3DXVECTOR2 mTextureMovement[2];
};

#endif