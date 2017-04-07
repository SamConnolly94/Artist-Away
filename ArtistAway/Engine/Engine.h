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
public:
	CEngine();
	~CEngine();

	// Attempts to instialse Direct X within the window.
	bool Initialise(std::string windowName);
	// Handle what happens when the window is closed and engine is told to stop.
	void Shutdown();
	// Runs the engine.
	bool IsRunning();
	// Start the timer object which keeps track of timing within our game.
	void StartTimer();
	// Stop the engine from running.
	void Stop();
	// Control what happens when we recieve different messages from the operating system.
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	// Process and render the graphics for current frame.
	bool Frame();
	// Create the window of this size.
	void InitialiseWindows(std::string windowName, int& screenWidth, int& screenHeight);
	// Close the current window in which the engine is running.
	void ShutdownWindows();
	// Check for any incoming messages from the operating system.
	void CheckWindowsMessages(MSG &msg);
	// Handle any incoming messages from the operating system.
	bool ProcessWindowsMessages();
private:
	// The name of our application as it will appear in windows.
	std::string mApplicationName;
	// The instance of our application as reffered to by windows.
	HINSTANCE mHinstance;
	// The handle to the window of our application.
	HWND mHwnd;
	// The object which handles user input.
	CInput* mpInput;
	// The object which handles all rendering and possibly updates of some elements too.
	CGraphics* mpGraphics;
	// The timing object which will control all timing throughout our engine.
	CGameTimer* mTimer;
	// The time it took to render the last frame. User should not need to calculate this, just call a get on it from the engine.
	float mFrameTime;
private:
	// The logger object which writes any errors / memory allocations to logs in a text file.
	CLogger* logger;
	// The message which we will recieve from windows OS.
	MSG mMsg;
	// A boolean toggle which defines whether the engine is running or not.
	bool mIsRunning;
	// A boolean toggle which allows the user to stop running the engine.
	bool mStopped;
public:
	/////////////////////////
	// Predefined shape control
	////////////////////////

	// Define a shape such as cube or triangle with a solid colour. Useful for quick and easy model creation.
	CPrimitive* CreatePrimitive(PrioEngine::RGBA colour, PrioEngine::Primitives shape);
	// Define a shape such as cube or triangle with a texture, and with the option to use lighting such as diffuse or specular.
	CPrimitive* CreatePrimitive(std::string textureFilename, bool useLighting, PrioEngine::Primitives shape);
	// Define a shape such as cube or triangle with a texture, but no lighting.
	CPrimitive* CreatePrimitive(std::string textureFilename, PrioEngine::Primitives shape);
	// Remove a shape from the scene, pass in a pointer to the engine, it will search for the shape in it's lists and remove it.
	bool RemovePrimitive(CPrimitive* model);
	
	/////////////////////////
	// Terrain control
	////////////////////////

	// Create a terrain from a height map text file. This can be exported from artist away.
	CTerrain* CreateTerrain(std::string mapFile);
	// Create a terrain from a height map in the form of a 2D array, width and height should be the size of the 2D array.
	CTerrain* CreateTerrain(double** heightMap, int mapWidth, int mapHeight);
	// Update the existing terrain to a new terrain. Only possible through a 2D array, must destroy and recreate for map files.
	bool UpdateTerrainBuffers(CTerrain *& terrain, double** heightmap, int width, int height);
	// Remove all scenery added by the terrain.
	void RemoveScenery();
	// Adds entities around the terrain to make it more realistic. This should be called after terrain has been initialised.
	bool AddSceneryToTerrain(CTerrain* terrainPtr);

	/////////////////////////
	// Foliage control
	////////////////////////

	// Create foliage quads from a height map text file with a higher frequency than the standard terrain.
	CFoliage* CreateFoliage(std::string mapFile);
	// Create foliage from a dynamic array height map passed in which should have a higher frequency than the standard terrain height map.
	CFoliage* CreateFoliage(double** heightMap, int mapWidth, int mapHeight);
	// Update the foliage map being used. May prove to be useful when generating new terrains.
	bool UpdateFoliage(double** heightMap, int width, int height);
	// Get a pointer to the foliage object.
	CFoliage* GetFoliage();

	/////////////////////////
	// Mesh control
	////////////////////////
	
	// Load a mesh into the engine from filename X, textures will need to be located in 'Resources/Textures/'
	CMesh* LoadMesh(std::string filename, float radius = 1.0f);
	// Remove a mesh from the engine.
	bool RemoveMesh(CMesh* mesh);

	/////////////////////////
	// 2D UI Image Control
	////////////////////////

	// Create a UI image which can be displayed on screen. 
	C2DImage* CreateUIImages(std::string filename, int width, int height, int posX, int posY);
	// Remove a UI image.
	bool RemoveUIImage(C2DImage *& element);

	/////////////////////////
	// Text control
	////////////////////////

	// Create some on screen text.
	SentenceType* CreateText(std::string text, int posX, int posY, int maxLength);
	// Update the on scene text, requires a pointer to the existing text.
	bool UpdateText(SentenceType *& sentence, std::string text, int posX, int posY, PrioEngine::RGB colour);
	// Remove the text from the scene.
	bool RemoveText(SentenceType *& sentence);

	/////////////////////////
	// Skybox control
	////////////////////////

	// Disable the skybox from automatically changing colour through day, evening and night cycles.
	void DisableAutomaticSkyboxChange();
	// Enable the skybox to change colour automatically and go through day evening and night cycles.
	void EnableAutomaticSkyboxChange();
	// Set the amount of seconds before the skybox should change colour.
	void SetSkyboxChangeInterval(float interval);
	// Get the amount of time it takes in seconds before changing the skybox.
	float GetSkyboxChangeInterval();
	// Change the skybox to represent day time.
	void SetDayTime();
	// Change the skybox to represent night time.
	void SetNightTime();
	// Change the skybox to represent evening time.
	void SetEveningTime();
	// Find whether it's day time in the skybox.
	bool IsDayTime();
	// Find whether it's night time in the skybox.
	bool IsNightTime();
	// Find whether it's evening time in the skybox.
	bool IsEveningTime();

	/////////////////////////
	// Engine functions
	////////////////////////

	// Get a pointer to the main camera from our graphics object.
	CCamera* GetMainCamera() { return mpGraphics->GetMainCamera(); };
	// Get the time it took to process the last frame.
	float GetFrameTime();
	// Enable or disable full screen.
	bool ToggleFullscreen(unsigned int fullscreenKey);
	// Toggle a wireframe view of everything in the scene. Useful for debugging when defining vertices.
	void ToggleWireframe();
	// Has key been hit?
	bool KeyHit(const unsigned int key);
	// Has keen been held down?
	bool KeyHeld(const unsigned int key);

	/////////////////////////
	// Weather functions
	////////////////////////

	// Toggle on or off rain which will fall from above the terrain.
	void SetRainEnabled(bool value);
	// Grab the toggle to find if rain is enabled or not.
	bool GetRainEnabled();
	// Toggle on or off snow which will fall from above the terrain.
	void SetSnowEnabled(bool value);
	// Grab the toggle to find if snow is enabled or not.
	bool GetSnowEnabled();
private:
	std::vector<CMesh*> mpListOfTreeMeshes;
};

// Define WndProc and the application handle pointer here so that we can re-direct the windows system messaging into our message handler 
// function inside the system class.
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static CEngine* ApplicationHandle = 0;

#endif