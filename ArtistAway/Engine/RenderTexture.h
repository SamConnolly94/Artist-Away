#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include <d3d11.h>
#include "Logger.h"

class CRenderTexture
{
private:
	CLogger* logger;
public:
	CRenderTexture();
	~CRenderTexture();
public:
	bool Initialise(ID3D11Device * device, int width, int height, DXGI_FORMAT format);
	void Shutdown();
	void SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView);
	void ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView, float red, float green, float blue, float alpha);
	ID3D11ShaderResourceView* &GetShaderResourceView();
private:
	ID3D11Texture2D* mpRenderTargetTexture;
	ID3D11RenderTargetView* mpRenderTargetView;
	ID3D11ShaderResourceView* mpShaderResourceView;
};

#endif