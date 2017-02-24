#include "Engine/Engine.h"
#include "PerlinNoise.h"
#include "HeightMap.h"
#include <thread>
#include <process.h>

/// Custom types and structures.

/* The variables that are required to set up our tweak bar. */
struct TweakStruct
{
	HANDLE hUpdateTerrainThread;
	CEngine* enginePtr;
	CTerrain* terrainPtr;
	CHeightMap* heightMapPtr;
	bool readyForJoin;
};

/// Function definitions.

/* This is where the game runs. This part should really only contain the loop itself, but if we want to we can put scene init here as well.
 * PARAM engine - The engine which we'll be running on, must already be initialised and allocated memory before being passed in.
*/
void GameLoop(CEngine* &engine);

/* This is where user input is controlled. This function must be called once every tick of our gameloop without exceptions, because we need to make sure we don't miss something like the quit key! 
 * PARAM engine - The pointer to the engine which is being used in our gameloop.
 * PARAM cam - The main camera being used throughout our game.
 * PARAM grid - This is the terrain grid, required to be passed in as we are controlling the terrain in this loop. It is expected to be initialised outside of the control function, by the engine appropriate functions.
*/
void Control(CEngine* &engine, CCamera* cam, CTerrain* terrain);

/* Sets up the tweakbar on our window, adds variables to the window.
 * PARAM ptr - This is the pointer to your tweakbar. It is of type CTwBar, will be passed in by reference so we'll take care of the rest for you.
 * PARAM tweakVars - These are all the variables that will be required to be used throughout our tweakbar. Please look at the TweakStruct to understand the layout of this bar.
*/
void SetupTweakbar(CTwBar *& ptr, TweakStruct* tweakVars);

/* A callback function which dictates what behaviour should occur when the height setting on the tweakbar is changed. */
void TW_CALL SetHeight(const void *value, void * clientData);
/* A getter function which keeps the height variable in our tweakbar up to date. */
void TW_CALL GetHeight(void *value, void * clientData);

/* A callback function which dictates what behaviour should occur when the width setting on the tweakbar is changed. */
void TW_CALL SetWidth(const void *value, void * clientData);
/* A getter function which keeps the width variable in our tweakbar up to date. */
void TW_CALL GetWidth(void *value, void * clientData);

/* A callback function which dictates what behaviour should occur when the frequency setting on the tweakbar is changed. */
void TW_CALL SetFrequency(const void *value, void *clientData);
/* A getter function which keeps the frequency variable in our tweakbar up to date. */
void TW_CALL GetFrequency(void *value, void * clientData);

/* A callback function which dictates what behaviour should occur when the amplitude setting on the tweakbar is changed. */
void TW_CALL SetAmplitude(const void *value, void *clientData);
/* A getter function which keeps the amplitude variable in our tweakbar up to date. */
void TW_CALL GetAmplitude(void *value, void * clientData);

/* A callback function which dictates what behaviour should occur when the persistence setting on the tweakbar is changed. */
void TW_CALL SetPersistence(const void *value, void *clientData);
/* A getter function which keeps the persistence variable in our tweakbar up to date. */
void TW_CALL GetPersistence(void *value, void * clientData);

/* The callback function for our update button. Should run the thread which updates the terrain.*/
void TW_CALL UpdateTerrain(void* clientData);

void TW_CALL SetOctaves(const void * value, void * clientData);
void TW_CALL GetOctaves(void * value, void * clientData);

/* The function which the thread will execute whiich should update a heightmap to the desired values, update the terrains heightmap, vertex and index buffers, and redraw the terrain. */
unsigned int __stdcall UpdateMapThread(void* pdata);

/// Globals

CLogger* gLogger;

/* Entrypoint. */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CEngine* engine;

	// Enable run time memory check while running in debug.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	gLogger->GetInstance().MemoryAllocWriteLine(typeid(gLogger).name());
	// Start the game engine.
	bool result;

	// Create the engine object.
	engine = new CEngine();
	// If failed to create the engine.
	if (!engine)
	{
		// Write a message to the log to let the user know we couldn't create the engine object.
		gLogger->WriteLine("Could not create the engine object.");
		// Return 0, we're saying we're okay, implement error codes in future versions maybe? 
		return 0;
	}
	gLogger->GetInstance().MemoryAllocWriteLine(typeid(engine).name());

	// Set up the engine.
	result = engine->Initialise();
	// If we successfully initialised the game engine
	if (result)
	{
		//engine->ToggleFullscreen(NULL);
		// Start the game loop.
		GameLoop(engine);
	}


	// Shutdown and release the engine.
	engine->Shutdown();
	delete engine;
	gLogger->GetInstance().MemoryDeallocWriteLine(typeid(engine).name());
	engine = nullptr;

	// Delete our logger, we're done with this now.
	gLogger->GetInstance().MemoryDeallocWriteLine(typeid(gLogger).name());

	// The singleton logger will cause a memory leak. Don't worry about it. Should be no more than 64 bytes taken by it though, more likely will only take 48 bytes.
	bool hasMemoryLeaks = _CrtDumpMemoryLeaks();

	return 0;
}

/* This is where the game runs. This part should really only contain the loop itself, but if we want to we can put scene init here as well.
* PARAM engine - The engine which we'll be running on, must already be initialised and allocated memory before being passed in.
*/
void GameLoop(CEngine* &engine)
{
	const int frameTimePosX = 10;
	const int frameTimePosY = 10;
	const int FPSPosX = 10;
	const int FPSPosY = 50;

	// Constants.
	const float kRotationSpeed = 100.0f;
	const float kMovementSpeed = 1.0f;

	// Variables
	float frameTime;
	CLight* ambientLight;
	CHeightMap* heightMap;
	CTerrain* terrain;
	terrain = engine->CreateTerrain("");
	heightMap = new CHeightMap();
	gLogger->GetInstance().MemoryAllocWriteLine(typeid(heightMap).name());
	CTwBar* tweakBar;

	heightMap->SetHeight(200);
	heightMap->SetWidth(200);
	heightMap->InitialiseMap();
	heightMap->WriteMapToFile("Default.map");
	terrain = engine->CreateTerrain(heightMap->GetMap(), 200, 200);

	TweakStruct* tweakVars = new TweakStruct();
	gLogger->GetInstance().MemoryAllocWriteLine(typeid(tweakVars).name());
	tweakVars->enginePtr = engine;
	tweakVars->heightMapPtr = heightMap;
	tweakVars->terrainPtr = terrain;
	tweakVars->readyForJoin = false;
	SetupTweakbar(tweakBar, tweakVars);

	// Camera init.
	CCamera* myCam = engine->GetMainCamera();
	myCam->SetPosizionY(50.0f);
	myCam->RotateX(45.0f);

	// Start the game timer running.
	engine->StartTimer();

	SentenceType* frametimeText = engine->CreateText("Frametime: ", frameTimePosX, frameTimePosY, 32);
	SentenceType* FPSText = engine->CreateText("FPS: ", FPSPosX, FPSPosY, 32);
	const float kTextUpdateInterval = 0.2f;
	float timeSinceTextUpdate = kTextUpdateInterval;

	// Process anything which should happen in the game here.
	while (engine->IsRunning())
	{
		// Get hold of the time it took to draw the last frame.
		frameTime = engine->GetFrameTime();

		// Process any keys pressed this frame.
		Control(engine, myCam, terrain);

		if (tweakVars->readyForJoin)
		{
			if (!CloseHandle(tweakVars->hUpdateTerrainThread))
			{
				gLogger->GetInstance().WriteLine("Failed to close handle of thread.");
			}

			tweakVars->readyForJoin = false;
		}

		// Update the text on our game.
		if (timeSinceTextUpdate >= kTextUpdateInterval)
		{
			engine->UpdateText(frametimeText, "FrameTime: " + std::to_string(frameTime), frameTimePosX, frameTimePosY, { 1.0f, 1.0f, 0.0f });
			engine->UpdateText(FPSText, "FPS: " + std::to_string(1.0f / frameTime), static_cast<int>(FPSPosX), static_cast<int>(FPSPosY), { 1.0f, 1.0f, 0.0f });
			timeSinceTextUpdate = 0.0f;
		}
		timeSinceTextUpdate += frameTime;
	}
	// Close the thread handle
	WaitForSingleObject(tweakVars->hUpdateTerrainThread, INFINITE);

	TwDeleteBar(tweakBar);
	gLogger->GetInstance().MemoryDeallocWriteLine(typeid(tweakBar).name());
	

	heightMap->WriteMapToFile("Final.map");

	delete tweakVars;
	gLogger->GetInstance().MemoryDeallocWriteLine(typeid(tweakVars).name());
	delete heightMap;
	gLogger->GetInstance().MemoryDeallocWriteLine(typeid(heightMap).name());
}

/* This is where user input is controlled. This function must be called once every tick of our gameloop without exceptions, because we need to make sure we don't miss something like the quit key!
* PARAM engine - The pointer to the engine which is being used in our gameloop.
* PARAM cam - The main camera being used throughout our game.
* PARAM grid - This is the terrain grid, required to be passed in as we are controlling the terrain in this loop. It is expected to be initialised outside of the control function, by the engine appropriate functions.
*/
void Control(CEngine* &engine, CCamera* cam, CTerrain* terrain)
{
	const float kMoveSpeed = 10.0f;
	const float kRotationSpeed = 10.0f;
	const float kCamRotationSpeed = 2.5f;
	float frameTime = engine->GetFrameTime();

	/// Camera control.
	// Move backwards
	if (engine->KeyHeld(PrioEngine::Key::kS))
	{
		cam->MoveLocalZ(-kMoveSpeed * frameTime);
	}
	// Move Forwards
	else if (engine->KeyHeld(PrioEngine::Key::kW))
	{
		cam->MoveLocalZ(kMoveSpeed * frameTime);
	}
	// Move Left
	if (engine->KeyHeld(PrioEngine::Key::kA))
	{
		cam->MoveLocalX(-kMoveSpeed * frameTime);
	}
	// Move Right
	else if (engine->KeyHeld(PrioEngine::Key::kD))
	{
		cam->MoveLocalX(kMoveSpeed * frameTime);
	}

	// Rotate left
	if (engine->KeyHeld(PrioEngine::Key::kLeft))
	{
		cam->RotateY(-kCamRotationSpeed * frameTime);
	}
	// Rotate right.
	else if (engine->KeyHeld(PrioEngine::Key::kRight))
	{
		cam->RotateY(kCamRotationSpeed * frameTime);
	}
	// Rotate upwards.
	if (engine->KeyHeld(PrioEngine::Key::kUp))
	{
		cam->RotateX(-kCamRotationSpeed * frameTime);
	}
	// Rotate downwards.
	else if (engine->KeyHeld(PrioEngine::Key::kDown))
	{
		cam->RotateX(kCamRotationSpeed * frameTime);
	}

	/// User controls.

	// If the user hits escape.
	if (engine->KeyHit(PrioEngine::Key::kEscape))
	{
		engine->Stop();
	}

	// If the user hits F1.
	if (engine->KeyHit(PrioEngine::Key::kF1))
	{
		engine->ToggleWireframe();
	}

}

/* Sets up the tweakbar on our window, adds variables to the window.
* PARAM ptr - This is the pointer to your tweakbar. It is of type CTwBar, will be passed in by reference so we'll take care of the rest for you.
* PARAM tweakVars - These are all the variables that will be required to be used throughout our tweakbar. Please look at the TweakStruct to understand the layout of this bar.
*/
void SetupTweakbar(CTwBar *& ptr, TweakStruct* tweakVars)
{
	void* tweakVarsPtr = reinterpret_cast<void*>(tweakVars);
	ptr = TwNewBar("Perlin Noise");
	gLogger->GetInstance().MemoryAllocWriteLine(typeid(ptr).name());
	TwDefine(" GLOBAL help='Control the perlin noise generation through these tabs. ' ");
	TwAddVarCB(ptr, "Height", TW_TYPE_INT32, SetHeight, GetHeight, tweakVarsPtr, "min=10 max=1000 step=1 group=Terrain label='Height' ");
	TwAddVarCB(ptr, "Width", TW_TYPE_INT32, SetWidth, GetWidth, tweakVarsPtr, "min=10 max=1000 step=1 group=Terrain label='Width' ");
	TwAddVarCB(ptr, "Persistence", TW_TYPE_DOUBLE, SetPersistence, GetPersistence, tweakVarsPtr, "min=1 max=1000 step=0.1 group=Terrain label='Persistence' ");
	TwAddVarCB(ptr, "Frequency", TW_TYPE_FLOAT, SetFrequency, GetFrequency, tweakVarsPtr, "min=0 max=1000 step=0.1 group=Terrain label='Frequency' ");
	TwAddVarCB(ptr, "Octaves", TW_TYPE_UINT32, SetOctaves, GetOctaves, tweakVarsPtr, "min=1 max=100 step=1 group=Terrain label='Octaves'");
	TwAddVarCB(ptr, "Amplitude", TW_TYPE_FLOAT, SetAmplitude, GetAmplitude, tweakVarsPtr, "min=0 max=100000 step=10000 group=Terrain label='Amplitude' ");
	TwAddButton(ptr, "Update", UpdateTerrain, tweakVarsPtr, "group=Terrain label='Update'");
}

/* A callback function which dictates what behaviour should occur when the height setting on the tweakbar is changed. */
void TW_CALL SetHeight(const void *value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	int height = *static_cast<const int *>(value);
	tweakVars->heightMapPtr->SetRequestedHeight(height);
}

/* A getter function which keeps the height variable in our tweakbar up to date. */
void TW_CALL GetHeight(void *value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	*static_cast<int *>(value) = tweakVars->heightMapPtr->GetRequestedHeight();
}

/* A callback function which dictates what behaviour should occur when the width setting on the tweakbar is changed. */
void TW_CALL SetWidth(const void * value, void *clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	int width = *static_cast<const int *>(value);
	tweakVars->heightMapPtr->SetRequestedWidth(width);
}

/* A getter function which keeps the width variable in our tweakbar up to date. */
void TW_CALL GetWidth(void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	*static_cast<int*>(value) = tweakVars->heightMapPtr->GetRequestedWidth();
}

/* A callback function which dictates what behaviour should occur when the frequency setting on the tweakbar is changed. */
void TW_CALL SetFrequency(const void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	float frequency = *static_cast<const float *>(value);
	tweakVars->heightMapPtr->SetFrequency(frequency);
}

/* A getter function which keeps the frequency variable in our tweakbar up to date. */
void TW_CALL GetFrequency(void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	*static_cast<float *>(value) = tweakVars->heightMapPtr->GetFrequency();
}

/* A callback function which dictates what behaviour should occur when the amplitude setting on the tweakbar is changed. */
void TW_CALL SetAmplitude(const void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	float amplitude = *static_cast<const float *>(value);
	tweakVars->heightMapPtr->SetAmplitude(amplitude);
}

/* A getter function which keeps the amplitude variable in our tweakbar up to date. */
void TW_CALL GetAmplitude(void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	*static_cast<float *>(value) = tweakVars->heightMapPtr->GetAmplitude();
}

/* A callback function which dictates what behaviour should occur when the persistence setting on the tweakbar is changed. */
void TW_CALL SetPersistence(const void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	float persistence = *static_cast<const double *>(value);
	tweakVars->heightMapPtr->SetPersistence(persistence);
}

/* A getter function which keeps the persistence variable in our tweakbar up to date. */
void TW_CALL GetPersistence(void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	*static_cast<double *>(value) = tweakVars->heightMapPtr->GetPersistence();
}

void TW_CALL SetOctaves(const void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	int octaves = *static_cast<const int *>(value);
	tweakVars->heightMapPtr->SetNumberOfOctaves(octaves);
}

void TW_CALL GetOctaves(void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	*static_cast<unsigned int *>(value) = tweakVars->heightMapPtr->GetNumberOfOctaves();
}

/* The callback function for our update button. Should run the thread which updates the terrain.*/
void TW_CALL UpdateTerrain(void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	tweakVars->hUpdateTerrainThread = (HANDLE)_beginthreadex(NULL, 0, UpdateMapThread, clientData, 0, NULL);
}

/* The function which the thread will execute whiich should update a heightmap to the desired values, update the terrains heightmap, vertex and index buffers, and redraw the terrain. */
unsigned int __stdcall UpdateMapThread(void* pdata)
{
	// Cast the void data back over to our TweakStructure type.
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(pdata);
	// Define a new sentence and display it on the screen so that the user is aware what's happening since they've clicked update.
	SentenceType* sentence = tweakVars->enginePtr->CreateText("Generating terrain on separate thread, please wait...", 500, 100, 128);
	// Update the heightmap with new values.
	tweakVars->heightMapPtr->UpdateMap();
	// Set the new values in the vertex and index buffers of the terrain, and copy over the values out the new heightmap we just generated.
	tweakVars->enginePtr->UpdateTerrainBuffers(tweakVars->terrainPtr, tweakVars->heightMapPtr->GetMap(), tweakVars->heightMapPtr->GetWidth(), tweakVars->heightMapPtr->GetHeight());
	// Remove thhe sentence because we're done now.
	tweakVars->enginePtr->RemoveText(sentence);
	// Output a deallocation of memory message to the log.
	gLogger->GetInstance().MemoryDeallocWriteLine(typeid(sentence).name());
	// Set flag which indicates that the thread is ready to rejoin the main thread, the mainthread should be checking this in the game loop.
	tweakVars->readyForJoin = true;

	// No errors, return success!
	return 0;

}