#ifndef GAMEFONT_H
#define GAMEFONT_H

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>

#include "Texture.h"
#include "PrioEngineVars.h"

class CGameFont
{
private:
	CLogger* logger;
private:
	struct FontType
	{
		float left;
		float right;
		int size;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	CGameFont();
	~CGameFont();

	bool Initialise(ID3D11Device * device, char * fontDataFile, std::string fontTexture);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
	void BuildVertexArray(void* vertices, std::string text, float xPos, float yPos);
private:
	bool LoadFontData(char* dataFile);
	void ReleaseFontData();
	bool LoadTextureFile(ID3D11Device * device, std::string fontTexture);
	void ReleaseTexture();
private:
	FontType* mpFont;
	CTexture* mpTexture;
};

#endif