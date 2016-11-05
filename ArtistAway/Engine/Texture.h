#ifndef TEXTURE_H
#define TEXTURE_H

// Include the necessary directX 10 libraries.
#include <d3d11.h>
#include <D3DX11tex.h>
#include "Logger.h"

class CTexture
{
public:
	CTexture();
	~CTexture();

	bool Initialise(ID3D11Device* device, WCHAR* filename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
private:
	ID3D11ShaderResourceView* mpTexture;
	CLogger* mpLogger;
};

#endif