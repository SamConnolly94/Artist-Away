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
	bool Initialise(ID3D11Device* device, D3DXVECTOR4 ambientColour);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	// Retrieve skybox colour at the top
	D3DXVECTOR4 GetApexColor();
	// Retrieve skybox colour at the centre of the sphere.
	D3DXVECTOR4 GetCenterColor();
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
};

#endif