#ifndef SKYBOX_H
#define SKYBOX_H

#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include "PrioEngineVars.h"

class CSkyBox
{
private:
	CLogger* logger;
public:
	CSkyBox();
	~CSkyBox();

private:
	struct ModelType
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 tex;
		D3DXVECTOR3 normal;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
	};
public:
	bool Initialise(ID3D11Device* device);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	// Retrieve skybox colour at the top
	D3DXVECTOR4 GetApexColor();
	// Retrieve skybox colour at the centre of the sphere.
	D3DXVECTOR4 GetCenterColour();
private:
	bool LoadSkyBoxModel(char* modelName);
	void ReleaseSkyBoxModel();

	bool InitialiseBuffers(ID3D11Device* device);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
private:
	ModelType* mpModel;
	unsigned int mVertexCount;
	unsigned int mIndexCount;
	ID3D11Buffer* mpVertexBuffer;
	ID3D11Buffer* mpIndexBuffer;
	D3DXVECTOR4 mApexColour;
	D3DXVECTOR4 mCentreColour;
	// Arrays to store data about vertices in.
	std::vector<D3DXVECTOR3> mpVerticesList;
	std::vector<D3DXVECTOR2> mpUVList;
	std::vector<D3DXVECTOR3> mpNormalsList;
	std::vector<unsigned long> mpIndicesList;
private:
	const D3DXVECTOR4 kDayCentreColour = { 0.38f, 0.84f, 1.0f, 1.0f };
	const D3DXVECTOR4 kDayApexColour = { 0.0f, 0.15f, 0.66f, 1.0f };
	const D3DXVECTOR4 kNightCentreColour = { 0.27f, 0.11f, 0.70f, 1.0f };
	const D3DXVECTOR4 kNightApexColour = { 0.07f, 0.0f, 0.25f, 1.0f };
	const D3DXVECTOR4 kEveningCentreColour = { 1.0f, 0.44f, 0.11f, 1.0f };
	//const D3DXVECTOR4 kEveningApexColour = { 0.0f, 0.15f, 0.66f, 1.0f };
	const D3DXVECTOR4 kEveningApexColour = { 0.94f, 0.45f, 0.12f, 1.0f };

	bool mIsDayTime = true;
	bool mIsNightTime = false;
	bool mIsEveningTime = false;
	
	const float kColourChangeModifier = 0.1f;
public:
	bool IsDayTime();
	bool IsEveningTime();
	bool IsNightTime();
	
	bool UpdateTimeOfDay(float updateTime);

	bool UpdateToEvening(float updateTime);
	bool UpdateToNight(float updateTime);
	bool UpdateToDay(float updateTime);
};

#endif