#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include "PrioEngineVars.h"
#include "Texture.h"

class CShader
{
protected:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
public:
	CShader();
	~CShader();
protected:
	CLogger* logger;
public:
	virtual bool Initialise(ID3D11Device* device, HWND hwnd) = 0;
	virtual void Shutdown() = 0;
};

#endif