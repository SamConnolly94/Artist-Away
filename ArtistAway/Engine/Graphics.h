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
#include "RefractReflectShader.h"
#include "Water.h"
#include "WaterShader.h"
#include "CloudPlane.h"
#include "CloudShader.h"
#include "RainShader.h"
#include "Rain.h"
#include "SnowShader.h"
#include "Snow.h"
#include "FoliageShader.h"
#include "Foliage.h"
#include <SFML/Audio.hpp>

// Global variables.

// Will VSYNC be enabled? (Caps at your monitor refresh rate)
const bool VSYNC_ENABLED = false;
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
public:
	CGraphics();
	~CGraphics();

	bool Initialise(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame(float updateTime);
private:
	bool Render();
private:
	bool RenderModels(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX viewProj, D3DXMATRIX reflectionViewProj);
	bool RenderPrimitives(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX viewProj);
	bool RenderMeshes(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX viewProj);
	bool RenderTerrains(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX viewProj);
	bool RenderSkybox(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX viewProj);
	bool RenderWater(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX viewProj, D3DXMATRIX reflectionViewProj);
	bool RenderRain(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX viewProj);
	bool RenderSnow(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX viewProj);
	bool RenderFoliage(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX viewProj);
private:
	CD3D11* mpD3D;
	CCamera* mpCamera;
	CCamera* mpReflectionCamera;
	CGameText* mpText;
	D3DXMATRIX mBaseView;

	// Shaders
	CColourShader* mpColourShader;
	CTextureShader* mpTextureShader;
	CDiffuseLightShader* mpDiffuseLightShader;
	CTerrainShader* mpTerrainShader;
	CSkyboxShader* mpSkyboxShader;
	CWaterShader* mpWaterShader;
	CReflectRefractShader* mpRefractionShader;
	CRainShader* mpRainShader;
	CSnowShader* mpSnowShader;
	CFoliageShader* mpFoliageShader;
	
	bool RenderPrimitiveWithTexture(CPrimitive* model, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);
	bool RenderPrimitiveWithColour(CPrimitive* model, D3DMATRIX worldMatrix, D3DMATRIX viewMatrix, D3DMATRIX projMatrix);

	std::list<CPrimitive*> mpPrimitives;
	std::list<CMesh*> mpMeshes;
	std::list<C2DImage*> mpUIImages;

	CLight* mpSceneLight;
	CSkyBox* mpSkybox;
	CTerrain* mpTerrain;

	bool CreateTextureShaderForModel(HWND hwnd);
	bool CreateColourShader(HWND hwnd);
	bool CreateTextureAndDiffuseLightShaderFromModel(HWND hwnd);
	bool CreateTerrainShader(HWND hwnd);
	bool RenderText(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX ortho, D3DXMATRIX viewProj);
	bool RenderBitmaps(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX ortho, D3DXMATRIX viewProj);
	float mFrameTime;

	HWND mHwnd;
public:
	/* Model control from the engine. */
	// Primitive creation / deletion.
	CPrimitive* CreatePrimitive(std::string TextureFilename, PrioEngine::Primitives shape);
	CPrimitive* CreatePrimitive(PrioEngine::RGBA colour, PrioEngine::Primitives shape);
	CPrimitive* CreatePrimitive(std::string TextureFilename, bool useLighting, PrioEngine::Primitives shape);
	bool RemovePrimitive(CPrimitive* &model);

	// Model creation / deletion.
	CMesh* LoadMesh(std::string filename, float radius = 1.0f);
	bool RemoveMesh(CMesh* &mesh);

	CTerrain* CreateTerrain(std::string mapFile);
	CTerrain* CreateTerrain(double ** heightMap, int mapWidth, int mapHeight);

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
	CSkyBox* CreateSkybox();
	void UpdateScene(float updateTime);
private:
	CLight* CreateLight(D3DXVECTOR4 diffuseColour, D3DXVECTOR4 specularColour, float specularPower, D3DXVECTOR4 ambientColour, D3DXVECTOR3 direction);
private:
	const float mAmbientMultiplier = 0.7f;
	float mTimeSinceLastSkyboxUpdate = 0.0f;
	float mSkyboxUpdateInterval = 10.0f;
	bool mUseTimeBasedSkybox = true;
	bool mUpdateToDayTime = false;
	bool mUpdateToNightTime = false;
	bool mUpdateToEveningTime = false;
public:
	void EnableTimeBasedSkybox(bool enabled);
	bool GetTimeBasedSkyboxEnabled();
	void SetSkyboxUpdateInterval(float interval);
	float GetSkyboxUpdateInterval();

	void SetDayTime();
	void SetNightTime();
	void SetEveningTime();

	bool IsDayTime();
	bool IsNightTime();
	bool IsEveningTime();
private:
	CCloudPlane* mpCloudPlane;
	CCloudShader* mpCloudShader;
	CRain* mpRain;
	CSnow* mpSnow;
	float mRunTime = 0.0f;
	CFoliage* mpFoliage;
	bool mRenderingMeshes = false;
	bool mRenderingFoliage = false;
	sf::SoundBuffer* mpRainSoundBuffer;
	sf::Sound* mpRainSound;
	sf::SoundBuffer* mpBirdSquawkSoundBuffer;
	sf::Sound* mpBirdSquawkSound;
	sf::SoundBuffer* mpRunningWaterSoundBuffer;
	sf::Sound* mpRunningWaterSound;

	float mTimeSinceLastBirdSquawk = 0.0f;
	const float mBirdSquawkPlayInterval = 20.0f;
public:
	bool CreateFoliage(std::string filename);
	bool CreateFoliage(double** heightMap, int width, int height);
	CFoliage* GetFoliage() { return mpFoliage; };
	bool UpdateFoliage(double** heightMap, int width, int height);
	void SetSnowEnabled(bool value);
	bool GetSnowEnabled();
	void SetRainEnabled(bool value);
	bool GetRainEnabled();
	bool IsRenderingMeshes() { return mRenderingMeshes; };
	bool IsRenderingFoliage() { return mRenderingFoliage; };
};

#endif