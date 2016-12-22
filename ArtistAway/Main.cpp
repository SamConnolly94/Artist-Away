#include "Engine/Engine.h"
#include "PerlinNoise.h"
#include "HeightMap.h"
#include <thread>
#include <process.h>

//#include "Engine\PrioEngineVars.h"

// Declaration of functions used to run game itself.
void GameLoop(CEngine* &engine);
void Control(CEngine* &engine, CCamera* cam, CTerrainGrid* grid);

struct TweakStruct
{
	HANDLE hUpdateTerrainThread;
	CEngine* enginePtr;
	CTerrainGrid* terrainPtr;
	CHeightMap* heightMapPtr;
	bool readyForJoin;
};

// Globals
void SetupTweakbar(CTwBar *& ptr, CHeightMap* &heightMapPtr, TweakStruct* tweakVars);
void TW_CALL SetHeight(const void *value, void * /*Client data. */);
void TW_CALL GetHeight(void *value, void * /*clientData*/);

void TW_CALL SetWidth(const void *value, void * /*Client data. */);
void TW_CALL GetWidth(void *value, void * /*clientData*/);

void TW_CALL SetFrequency(const void *value, void * /*Client data. */);
void TW_CALL GetFrequency(void *value, void * /*clientData*/);

void TW_CALL SetAmplitude(const void *value, void * /*Client data. */);
void TW_CALL GetAmplitude(void *value, void * /*clientData*/);

void TW_CALL SetPersistence(const void *value, void * /*Client data. */);
void TW_CALL GetPersistence(void *value, void * /*clientData*/);

void TW_CALL UpdateTerrain(void* clientData);

unsigned int __stdcall UpdateMapThread(void* pdata);
CLogger* gLogger;

// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CEngine* engine;

	// Enable run time memory check while running in debug.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	gLogger = new CLogger();
	gLogger->MemoryAllocWriteLine(typeid(gLogger).name());
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
	gLogger->MemoryAllocWriteLine(typeid(engine).name());

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
	gLogger->MemoryDeallocWriteLine(typeid(engine).name());
	engine = nullptr;
	gLogger->MemoryDeallocWriteLine(typeid(gLogger).name());
	delete gLogger;

	// The singleton logger will cause a memory leak. Don't worry about it. Should be no more than 64 bytes taken by it though, more likely will only take 48 bytes.
	_CrtDumpMemoryLeaks();

	return 0;
}

/* Controls any gameplay and things that should happen when we play the game. */
void GameLoop(CEngine* &engine)
{
	// Constants.
	const float kRotationSpeed = 100.0f;
	const float kMovementSpeed = 1.0f;

	// Variables
	float frameTime;
	CLight* ambientLight;
	CHeightMap* heightMap;
	CTerrainGrid* terrain;
	terrain = engine->CreateTerrainGrid();
	heightMap = new CHeightMap();
	gLogger->MemoryAllocWriteLine(typeid(heightMap).name());
	CTwBar* tweakBar;

	heightMap->SetHeight(200);
	heightMap->SetWidth(200);
	heightMap->InitialiseMap();
	heightMap->WriteMapToFile("Default.map");
	terrain->SetHeight(200);
	terrain->SetWidth(200);
	terrain->LoadHeightMap(heightMap->GetMap());
	terrain->CreateGrid();

	TweakStruct* tweakVars = new TweakStruct();
	gLogger->MemoryAllocWriteLine(typeid(tweakVars).name());
	tweakVars->enginePtr = engine;
	tweakVars->heightMapPtr = heightMap;
	tweakVars->terrainPtr = terrain;
	tweakVars->readyForJoin = false;
	SetupTweakbar(tweakBar, heightMap, tweakVars);

	// Camera init.
	CCamera* myCam = engine->GetMainCamera();
	myCam->SetPosizionY(50.0f);
	myCam->RotateX(45.0f);

	// Light init
	ambientLight = engine->CreateLight(D3DXVECTOR4{ 1.0f, 1.0f, 1.0f, 1.0f }, D3DXVECTOR4{ 0.15f, 0.15f, 0.15f, 1.0f });
	ambientLight->SetDirection(D3DXVECTOR3{ 0.0f, -0.3f, 1.0f});
	ambientLight->SetSpecularColour(D3DXVECTOR4{ 1.0f, 1.0f, 1.0f, 1.0f });
	ambientLight->SetSpecularPower(30000.0f);

	// Start the game timer running.
	engine->StartTimer();

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
				gLogger->WriteLine("Failed to close handle of thread.");
			}

			
			tweakVars->readyForJoin = false;
		}
	}
	// Close the thread handle
	WaitForSingleObject(tweakVars->hUpdateTerrainThread, INFINITE);

	TwDeleteBar(tweakBar);
	gLogger->MemoryDeallocWriteLine(typeid(tweakBar).name());
	

	delete tweakVars;
	gLogger->MemoryDeallocWriteLine(typeid(tweakVars).name());
	delete heightMap;
	gLogger->MemoryDeallocWriteLine(typeid(heightMap).name());
}

/* Control any user input here, must be called in every tick of the game loop. */
void Control(CEngine* &engine, CCamera* cam, CTerrainGrid* grid)
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

void SetupTweakbar(CTwBar *& ptr, CHeightMap* &heightMapPtr, TweakStruct* tweakVars)
{
	void* tweakVarsPtr = reinterpret_cast<void*>(tweakVars);
	ptr = TwNewBar("Perlin Noise");
	gLogger->MemoryAllocWriteLine(typeid(ptr).name());
	TwDefine(" GLOBAL help='Control the perlin noise generation through these tabs. ' ");
	TwAddVarCB(ptr, "Height", TW_TYPE_INT32, SetHeight, GetHeight, tweakVarsPtr, "min=10 max=1000 step=1 group=Terrain label='Height' ");
	TwAddVarCB(ptr, "Width", TW_TYPE_INT32, SetWidth, GetWidth, tweakVarsPtr, "min=10 max=1000 step=1 group=Terrain label='Width' ");
	TwAddVarCB(ptr, "Persistence", TW_TYPE_DOUBLE, SetPersistence, GetPersistence, tweakVarsPtr, "min=1 max=1000 step=0.1 group=Terrain label='Persistence' ");
	TwAddVarCB(ptr, "Frequency", TW_TYPE_FLOAT, SetFrequency, GetFrequency, tweakVarsPtr, "min=0 max=1000 step=0.1 group=Terrain label='Frequency' ");
	TwAddVarCB(ptr, "Amplitude", TW_TYPE_FLOAT, SetAmplitude, GetAmplitude, tweakVarsPtr, "min=0 max=100000 step=10000 group=Terrain label='Amplitude' ");
	TwAddButton(ptr, "Update", UpdateTerrain, tweakVarsPtr, "group=Terrain label='Update'");
}

void TW_CALL SetHeight(const void *value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	int height = *static_cast<const int *>(value);
	tweakVars->heightMapPtr->SetRequestedHeight(height);
}

void TW_CALL GetHeight(void *value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	*static_cast<int *>(value) = tweakVars->heightMapPtr->GetRequestedHeight();
}

void TW_CALL SetWidth(const void * value, void *clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	int width = *static_cast<const int *>(value);
	tweakVars->heightMapPtr->SetRequestedWidth(width);
}

void TW_CALL GetWidth(void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	*static_cast<int*>(value) = tweakVars->heightMapPtr->GetRequestedWidth();
}

void TW_CALL SetFrequency(const void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	float frequency = *static_cast<const float *>(value);
	tweakVars->heightMapPtr->SetFrequency(frequency);
}

void TW_CALL GetFrequency(void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	*static_cast<float *>(value) = tweakVars->heightMapPtr->GetFrequency();
}

void TW_CALL SetAmplitude(const void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	float amplitude = *static_cast<const float *>(value);
	tweakVars->heightMapPtr->SetAmplitude(amplitude);
}

void TW_CALL GetAmplitude(void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	*static_cast<float *>(value) = tweakVars->heightMapPtr->GetAmplitude();
}

void TW_CALL SetPersistence(const void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	float persistence = *static_cast<const double *>(value);
	tweakVars->heightMapPtr->SetPersistence(persistence);
}

void TW_CALL GetPersistence(void * value, void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	*static_cast<double *>(value) = tweakVars->heightMapPtr->GetPersistence();
}

void TW_CALL UpdateTerrain(void * clientData)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(clientData);
	tweakVars->hUpdateTerrainThread = (HANDLE)_beginthreadex(NULL, 0, UpdateMapThread, clientData, 0, NULL);
}

unsigned int __stdcall UpdateMapThread(void* pdata)
{
	TweakStruct* tweakVars = reinterpret_cast<TweakStruct*>(pdata);
	SentenceType* sentence = tweakVars->enginePtr->CreateText("Generating terrain on separate thread, please wait...", 500, 100, 128);
	tweakVars->heightMapPtr->UpdateMap();
 // Update the grid in the engine.
	tweakVars->enginePtr->UpdateTerrainBuffers(tweakVars->terrainPtr, tweakVars->heightMapPtr->GetMap(), tweakVars->heightMapPtr->GetWidth(), tweakVars->heightMapPtr->GetHeight());
	tweakVars->enginePtr->RemoveText(sentence);
	gLogger->MemoryDeallocWriteLine(typeid(sentence).name());
	tweakVars->readyForJoin = true;

	return 0;

}