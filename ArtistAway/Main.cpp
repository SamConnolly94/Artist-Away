#include "Engine/Engine.h"
#include "PerlinNoise.h"
#include "HeightMap.h"

//#include "Engine\PrioEngineVars.h"

// Declaration of functions used to run game itself.
void GameLoop(CEngine* &engine);
void Control(CEngine* &engine, CCamera* cam);

// Globals
CLogger* gLogger;

// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Enable run time memory check while running in debug.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	gLogger = new CLogger();
	// Start the game engine.
	CEngine* PrioEngine;
	bool result;

	// Create the engine object.
	PrioEngine = new CEngine();
	// If failed to create the engine.
	if (!PrioEngine)
	{
		// Write a message to the log to let the user know we couldn't create the engine object.
		gLogger->WriteLine("Could not create the engine object.");
		// Return 0, we're saying we're okay, implement error codes in future versions maybe? 
		return 0;
	}
	gLogger->MemoryAllocWriteLine(typeid(PrioEngine).name());

	// Set up the engine.
	result = PrioEngine->Initialise();
	// If we successfully initialised the game engine
	if (result)
	{
		// Start the game loop.
		GameLoop(PrioEngine);
	}

	// Shutdown and release the engine.
	PrioEngine->Shutdown();
	delete PrioEngine;
	gLogger->MemoryDeallocWriteLine(typeid(PrioEngine).name());
	PrioEngine = nullptr;
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
	CCamera* myCam;

	CLight* ambientLight;
	CTerrainGrid* grid = engine->CreateTerrainGrid();
	CHeightMap* heightMap = new CHeightMap();
	heightMap->SetHeight(200);
	heightMap->SetWidth(200);
	heightMap->InitialiseMap();
	grid->SetHeight(heightMap->GetHeight());
	grid->SetWidth(heightMap->GetWidth());
	grid->LoadHeightMap(heightMap->GetMap());
	grid->CreateGrid();
	
	// Camera init.
	myCam = engine->CreateCamera();
	myCam->SetPosizionY(50.0f);
	myCam->RotateX(45.0f);

	// Light init
	ambientLight = engine->CreateLight(D3DXVECTOR4{ 1.0f, 1.0f, 1.0f, 1.0f }, D3DXVECTOR4{ 0.15f, 0.15f, 0.15f, 1.0f });
	ambientLight->SetDirection(D3DXVECTOR3{ 0.0f, 0.0f, 1.0f });
	ambientLight->SetSpecularColour(D3DXVECTOR4{ 1.0f, 1.0f, 1.0f, 1.0f });
	ambientLight->SetSpecularPower(32.0f);

	// Start the game timer running.
	engine->StartTimer();

	// Process anything which should happen in the game here.
	while (engine->IsRunning())
	{
		// Get hold of the time it took to draw the last frame.
		frameTime = engine->GetFrameTime();

		// Process any keys pressed this frame.
		Control(engine, myCam);

	}

	delete heightMap;
}

/* Control any user input here, must be called in every tick of the game loop. */
void Control(CEngine* &engine, CCamera* cam)
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


}