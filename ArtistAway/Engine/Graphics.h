#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <list>
#include "D3D11.h"
#include "Camera.h"
#include "Cube.h"
#include "Triangle.h"
#include "Primitive.h"
#include "Mesh.h"
#include "ColourShader.h"
#include "TextureShader.h"
#include "DiffuseLightShader.h"
#include "Light.h"

// Global variables.
// Will the window run in full screen?
const bool FULL_SCREEN = false;
// Will VSYNC be enabled? (Caps at your monitor refresh rate)
const bool VSYNC_ENABLED = true;
// Far clip
const float SCREEN_DEPTH = 1000.0f;
// Near clip
const float SCREEN_NEAR = 0.1f;

class CGraphics
{
private:
	;
	int mScreenWidth, mScreenHeight;
	float mFieldOfView;
public:
	CGraphics();
	~CGraphics();

	bool Initialise(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();
private:
	bool Render();

	CD3D11* mpD3D;

	CCamera* mpCamera;
	CPrimitive* mpTriangle;
	CColourShader* mpColourShader;
	CTextureShader* mpTextureShader;
	CDirectionalLightShader* mpDiffuseLightShader;
	
	bool RenderPrimitiveWithTexture(CPrimitive* model, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);
	bool RenderPrimitiveWithColour(CPrimitive* model, D3DMATRIX worldMatrix, D3DMATRIX viewMatrix, D3DMATRIX projMatrix);
	bool RenderPrimitiveWithTextureAndDiffuseLight(CPrimitive* model, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);

	std::list<CPrimitive*> mpPrimitives;
	std::list<CMesh*> mpMeshes;
	std::list<CLight*> mpLights;

	bool CreateTextureShaderForModel(HWND hwnd);
	bool CreateColourShaderForModel(HWND hwnd);
	bool CreateTextureAndDiffuseLightShaderFromModel(HWND hwnd);
	bool RenderModels(D3DXMATRIX view, D3DXMATRIX world, D3DXMATRIX proj);

	float mRotation;

	HWND mHwnd;
public:
	/* Model control from the engine. */
	// Primitive creation / deletion.
	CPrimitive* CreatePrimitive(WCHAR* TextureFilename, PrioEngine::Primitives shape);
	CPrimitive* CreatePrimitive(PrioEngine::RGBA colour, PrioEngine::Primitives shape);
	CPrimitive* CreatePrimitive(WCHAR* TextureFilename, bool useLighting, PrioEngine::Primitives shape);
	bool RemovePrimitive(CPrimitive* &model);

	// Model creation / deletion.
	CMesh* LoadMesh(char* filename, WCHAR* textureFilename);
	CMesh* LoadMesh(char * filename, WCHAR* textureFilename, ShaderType shaderType);
	bool RemoveMesh(CMesh* &mesh);

	CLight* CreateLight(D3DXVECTOR4 diffuseColour, D3DXVECTOR4 ambientColour);
	bool RemoveLight(CLight* &light);
	/* Camera control, required by the engine. */
	CCamera* CreateCamera();
	void SetCameraPos(float x, float y, float z);
};

#endif