#ifndef D3D11_H
#define D3D11_H

// Link the necessary Direct X Libraries.
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <string>
#include "PrioEngineVars.h"

const int STRING_NUMBER_OF_BITS = 128;

class CD3D11
{
private:
	int mScreenWidth;
	int mScreenHeight;
	bool mFullscreen;
	bool mVsyncEnabled;
	int mGraphicsCardMemory;
	std::string mGraphicsCardDescription;
	IDXGISwapChain* mpSwapChain;
	ID3D11Device* mpDevice;
	ID3D11DeviceContext* mpDeviceContext;
	ID3D11RenderTargetView* mpRenderTargetView;
	ID3D11Texture2D* mpDepthStencilBuffer;
	ID3D11DepthStencilState* mpDepthEnabledStencilState;
	ID3D11DepthStencilState* mpDepthDisabledStencilState;
	ID3D11DepthStencilView* mpDepthStencilView;
	ID3D11RasterizerState* mpRasterizerState;
	D3DXMATRIX mProjectionMatrix;
	D3DXMATRIX mWorldMatrix;
	D3DXMATRIX mOrthoMatrix;
	std::string mGraphicsCardName;
	ID3D11BlendState* mpAlphaBlendingStateEnabled;
	ID3D11BlendState* mpAlphaBlendingStateDisabled;
public:
	CD3D11();
	~CD3D11();

	bool Initialise(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(D3DMATRIX& projMatrix);
	void GetWorldMatrix(D3DMATRIX& worldMatrix);
	void GetOrthogonalMatrix(D3DMATRIX& orthogMatrix);
	void EnableWireframeFill();
	void EnableSolidFill();

	void EnableAlphaBlending();
	void DisableAlphaBlending();

	void DisableZBuffer();
	void EnableZBuffer();
	bool ToggleFullscreen(bool fullscreenEnabled);

	//void GetGraphicsCardInfo(char* cardName, int& memory);
/* Setup functions. */
private:
	void CreateSwapChainDesc(HWND hwnd, DXGI_SWAP_CHAIN_DESC& swapChainDesc, int refRateNumerator, int refRateDenominator);
	bool CreateSwapChain(D3D_FEATURE_LEVEL& featureLevel, DXGI_SWAP_CHAIN_DESC& swapChainDesc);
	bool CreateDepthBuffer(D3D11_TEXTURE2D_DESC& depthBufferDesc);
	bool CreateDepthStencilView(D3D11_DEPTH_STENCIL_VIEW_DESC& depthStencilViewDesc);
	bool CreateDepthDisabledStencilState(D3D11_DEPTH_STENCIL_DESC& depthStencilBufferDesc);
	bool CreateDepthStencilBuffer(D3D11_DEPTH_STENCIL_DESC& depthStencilBufferDesc);
	bool InitRasterizer(D3D11_RASTERIZER_DESC& rasterDesc);
	void InitViewport(D3D11_VIEWPORT& viewport);
	void CreateProjMatrix(float screenDepth, float screenNear);
};

#endif