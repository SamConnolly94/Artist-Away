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
		D3DXMATRIX viewProj;
	};
public:
	CShader();
	~CShader();
protected:
	CLogger* logger;
private:
	ID3D11Buffer* mpMatrixBuffer;
public:
	virtual bool Initialise(ID3D11Device* device, HWND hwnd) = 0;
	virtual void Shutdown() = 0;
protected:
	D3DXMATRIX mWorldMatrix;
	D3DXMATRIX mViewMatrix;
	D3DXMATRIX mProjMatrix;
	D3DXMATRIX mViewProjMatrix;
public:
	void SetWorldMatrix(D3DXMATRIX world);
	void SetViewMatrix(D3DXMATRIX view);
	void SetProjMatrix(D3DXMATRIX proj);
	void SetViewProjMatrix(D3DXMATRIX viewProj);
protected:
	enum ShaderType
	{
		Vertex,
		Geometry,
		Pixel
	};
	bool SetupMatrixBuffer(ID3D11Device * device);
	bool SetMatrixBuffer(ID3D11DeviceContext * deviceContext, unsigned int bufferSlot, ShaderType shaderType);
};

#endif