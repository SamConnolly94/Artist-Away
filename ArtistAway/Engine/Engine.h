#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include "PrioEngineVars.h"
#include "GameTimer.h"
#include "Graphics.h"
#include "Input.h"
#include <windows.h>

class CEngine
{
private:
	const float kKeyPressIntervalTime = 0.5f;
	bool mKeyRecentlyHit[256];
	float mTimeSinceLastKeyPress;
	bool mWireframeEnabled;
public:
	// Default constructor.
	CEngine();
	// Default destructor.
	~CEngine();

	// Attempts to instialse Direct X within the window.
	bool Initialise();
	// Handle what happens when the window is closed and engine is told to stop.
	void Shutdown();
	// Runs the engine.
	bool IsRunning();
	void StartTimer();

	// Control what happens when we recieve different messages from the operating system.
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool Frame();
	void InitialiseWindows(int&, int&);
	void ShutdownWindows();
	float mFrameTime;
private:
	// The name of our application as it will appear in windows.
	LPCWSTR mApplicationName;
	// The instance of our application as reffered to by windows.
	HINSTANCE mHinstance;
	// The handle to the window of our application.
	HWND mHwnd;

	CInput* mpInput;
	CGraphics* mpGraphics;

	void CheckWindowsMessages(MSG &msg);
	bool ProcessWindowsMessages();

	CGameTimer* mTimer;
private:
	MSG mMsg;
	bool result;
	bool mIsRunning;
	bool mStopped;
	
public:
	// Model creation functions.
	CPrimitive* CreatePrimitive(PrioEngine::RGBA colour, PrioEngine::Primitives shape);
	CPrimitive* CreatePrimitive(WCHAR* textureFilename, bool useLighting, PrioEngine::Primitives shape);
	CPrimitive* CreatePrimitive(WCHAR* textureFilename, PrioEngine::Primitives shape);
	
	CLight* CreateLight(D3DXVECTOR4 diffuseColour, D3DXVECTOR4 ambientColour);
	bool RemoveLight(CLight* &light);

	CTerrainGrid* CreateTerrainGrid();

	bool RemovePrimitive(CPrimitive* model);
	bool RemoveMesh(CMesh* mesh);

	CMesh* LoadMesh(char* filename, WCHAR* textureFilename);
	CMesh* LoadMesh(char * filename, WCHAR * textureFilename, PrioEngine::ShaderType shaderType);
	

	float GetFrameTime();
	CCamera* CreateCamera();

	void ToggleWireframe();

	SentenceType* CreateText(std::string text, int posX, int posY, int maxLength);
	bool UpdateText(SentenceType *& sentence, std::string text, int posX, int posY, PrioEngine::RGB colour);
	bool RemoveText(SentenceType *& sentence);

	// Control user input.
public:
	bool KeyHit(const unsigned int key);
	bool KeyHeld(const unsigned int key);
	void Stop();
	CCamera* GetMainCamera() { return mpGraphics->GetMainCamera(); };
	C2DImage* CreateUIImages(WCHAR* filename, int width, int height, int posX, int posY);
	bool RemoveUIImage(C2DImage *& element);
	bool UpdateTerrainBuffers(CTerrainGrid* &grid, double** heightmap, int width, int height);
	bool ToggleFullscreen(unsigned int fullscreenKey);
};

// Define WndProc and the application handle pointer here so that we can re-direct the windows system messaging into our message handler 
// function inside the system class.
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static CEngine* ApplicationHandle = 0;

#endif