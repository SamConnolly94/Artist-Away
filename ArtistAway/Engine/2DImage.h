#ifndef SPRITE_H
#define SPRITE_H

#include <d3d11.h>
#include <d3dx10math.h>
#include "Texture.h"
#include "PrioEngineVars.h"

class C2DImage
{
private:
	CLogger* logger;
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	C2DImage();
	~C2DImage();

	// Graphics functions.
	bool Initialise(ID3D11Device* device, int screenWidth, int screenHeight, std::string textureFilename, int width, int height);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int posX, int posY);

	// Property functions.
	int GetNumberOfIndices();
	ID3D11ShaderResourceView* GetTexture();
private:
	// Interal setup functions.
	bool InitialiseBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext* deviceContext, int posX, int posY);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, std::string textureFilename);
	void ReleaseTexture();
private:
	// Member variables.
	ID3D11Buffer* mpVertexBuffer;
	ID3D11Buffer* mpIndexBuffer;
	int mVertexCount;
	int mIndexCount;
	CTexture* mpTexture;

	int mScreenWidth;
	int mScreenHeight;

	int mImgWidth;
	int mImgHeight;
	
	int mPreviousPosX;
	int mPreviousPoxY;
private:
	int mPosX;
	int mPosY;
public:
	int GetX() { return mPosX; };
	int GetY() { return mPosY; };
	void SetX(int x) { mPosX = x; };
	void SetY(int y) { mPosY = y; };
};

#endif