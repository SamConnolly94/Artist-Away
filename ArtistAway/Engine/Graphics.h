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
#include "TerrainShader.h"
#include "Light.h"
#include "Terrain.h"
#include "GameText.h"
#include "2DImage.h"
#include <AntTweakBar.h>
#include "Frustum.h"
#include <thread>
#include <functional>
#include "SkyBox.h"
#include "SkyboxShader.h"

// Global variables.
// Will the window run in full screen?

// Will VSYNC be enabled? (Caps at your monitor refresh rate)
const bool VSYNC_ENABLED = true;
// Far clip
const float SCREEN_DEPTH = 1000.0f;
// Near clip
const float SCREEN_NEAR = 0.01f;

class CGraphics
{
private:
	CLogger* logger;
private:
	int mScreenWidth;
	int mScreenHeight;
	float mFieldOfView;
	bool mWireframeEnabled;
	CFrustum* mpFrustum;
	bool mFullScreen = false;
	CSkyboxShader* mpSkyboxShader;
public:
	CGraphics();
	~CGraphics();

	bool Initialise(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();
private:
	bool Render();
private:
	bool RenderModels(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);
	bool RenderPrimitives(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);
	bool RenderMeshes(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);
	bool RenderTerrains(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);
	bool RenderSkybox(D3DXMATRIX &world, D3DXMATRIX &view, D3DXMATRIX &proj);
private:
	CD3D11* mpD3D;

	CCamera* mpCamera;
	CPrimitive* mpTriangle;
	CColourShader* mpColourShader;
	CTextureShader* mpTextureShader;
	CDiffuseLightShader* mpDiffuseLightShader;
	CTerrainShader* mpTerrainShader;
	CGameText* mpText;
	D3DXMATRIX mBaseView;
	
	bool RenderPrimitiveWithTexture(CPrimitive* model, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);
	bool RenderPrimitiveWithColour(CPrimitive* model, D3DMATRIX worldMatrix, D3DMATRIX viewMatrix, D3DMATRIX projMatrix);
	//bool RenderPrimitiveWithTextureAndDiffuseLight(CPrimitive* model, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);

	std::list<CPrimitive*> mpPrimitives;
	std::list<CMesh*> mpMeshes;
	std::list<CLight*> mpLights;
	std::list<CTerrain*> mpTerrainGrids;
	std::list<C2DImage*> mpUIImages;
	std::vector<CSkyBox*> mpSkyboxList;

	bool CreateTextureShaderForModel(HWND hwnd);
	bool CreateColourShader(HWND hwnd);
	bool CreateTextureAndDiffuseLightShaderFromModel(HWND hwnd);
	bool CreateTerrainShader(HWND hwnd);
	bool RenderText(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX ortho);
	bool RenderBitmaps(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX ortho);

	float mRotation;

	HWND mHwnd;
public:
	/* Model control from the engine. */
	// Primitive creation / deletion.
	CPrimitive* CreatePrimitive(std::string TextureFilename, PrioEngine::Primitives shape);
	CPrimitive* CreatePrimitive(PrioEngine::RGBA colour, PrioEngine::Primitives shape);
	CPrimitive* CreatePrimitive(std::string TextureFilename, bool useLighting, PrioEngine::Primitives shape);
	bool RemovePrimitive(CPrimitive* &model);

	// Model creation / deletion.
	CMesh* LoadMesh(std::string filename);
	bool RemoveMesh(CMesh* &mesh);

	CTerrain* CreateTerrain(std::string mapFile);
	CTerrain* CreateTerrain(double ** heightMap, int mapWidth, int mapHeight);

	CLight* CreateLight(D3DXVECTOR4 diffuseColour, D3DXVECTOR4 ambientColour);
	bool RemoveLight(CLight* &light);
	/* Camera control, required by the engine. */
	CCamera* CreateCamera();
	void SetCameraPos(float x, float y, float z);
	void ToggleWireframe();
	CCamera* GetMainCamera() {return mpCamera;};
	SentenceType* CreateSentence(std::string text, int posX, int posY, int maxLength);
	bool UpdateSentence(SentenceType* &sentence, std::string text, int posX, int posY, PrioEngine::RGB colour);
	bool RemoveSentence(SentenceType* &sentence);

	C2DImage* CreateUIImages(std::string filename, int width, int height, int posX, int posY );
	bool RemoveUIImage(C2DImage* &element);
	bool UpdateTerrainBuffers(CTerrain* &terrain, double** heightmap, int width, int height);
	bool IsFullscreen();
	bool SetFullscreen(bool enabled);
	CSkyBox* CreateSkybox(D3DXVECTOR4 ambientColour);
};

#endif