#ifndef TEXTURE_H
#define TEXTURE_H

// Include the necessary directX 10 libraries.
#include <d3d11.h>
#include <D3DX11tex.h>
#include "PrioEngineVars.h"

class CTexture
{
private:
	CLogger* logger;
public:
	CTexture();
	~CTexture();

	bool Initialise(ID3D11Device * device, std::string filename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
private:
	ID3D11ShaderResourceView* mpTexture;
	std::wstring s2ws(const std::string str);
	std::string mFilename;
};

#endif