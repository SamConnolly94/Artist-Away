#ifndef VERTEXTYPEMANAGER_H
#define VERTEXTYPEMANAGER_H


// Include DirectX libraries.
#include <d3d11.h>
#include <D3DX10math.h>
#include <vector>
#include "PrioEngineVars.h"

class CVertexManager
{
private:
	// Structure types, should reflect the vertex / pixel shader.
	struct VertexColourType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 colour;
	};
	struct VertexTextureType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
	struct VertexDiffuseLightingType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};
	struct VertexSpecularLightingType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	// Definitions of each of the vertex shaders input buffers so that we can use it in C++.
	VertexTextureType* mpVerticesTexture;
	VertexColourType* mpVerticesColour;
	VertexDiffuseLightingType* mpVerticesDiffuse;
	VertexSpecularLightingType* mpVerticesSpecular;

	
	ID3D11Device* mpDevice;
	ID3D11Buffer* mpVertexBuffer;

	PrioEngine::Primitives mShape;
	PrioEngine::RGBA mColour;
	
public:
	CVertexManager(PrioEngine::ShaderType vertexType);
	CVertexManager(PrioEngine::ShaderType vertexType, PrioEngine::Primitives shape);
	~CVertexManager();
	void SetDevicePtr(ID3D11Device* device);
	void CleanArrays();
public:
	void CreateVertexArray();
	void SetNumberOfVertices(int amount);

	void SetVertexArray(float x, float y, float z);
	void SetVertexArray(float x, float y, float z, std::vector<D3DXVECTOR3> vertices, std::vector<D3DXVECTOR2> UV, std::vector<D3DXVECTOR3> normals);
	void SetVertexArray(float x, float y, float z, std::vector<D3DXVECTOR3> vertices, std::vector<D3DXVECTOR2> UV);
	void SetVertexArray(float x, float y, float z, std::vector<D3DXVECTOR3> vertices, std::vector<D3DXVECTOR4> colours);
	bool CreateVertexBuffer();

	void RenderBuffers(ID3D11DeviceContext* deviceContext, ID3D11Buffer* indexBuffer);

	// Set the colour used on this shape.
	void SetColour(PrioEngine::RGBA colour);
private:
	int mNumOfVertices;

	PrioEngine::ShaderType mShaderType;

	void CreateColourVerticesArray();
	void CreateTextureVerticesArray();
	void CreateDiffuseVerticesArray();
	void CreateSpecularVerticesArray();

	// Sets vertice points for cubes.
	void SetColourCube(float x, float y, float z);
	void SetTextureCube(float x, float y, float z);
	void SetDiffuseCube(float x, float y, float z);
	void SetSpecularCube(float x, float y, float z);

	// Sets vertice points for triangles.
	void SetColourTriangle(float x, float y, float z);
	void SetTextureTriangle(float x, float y, float z);
	void SetDiffuseTriangle(float x, float y, float z);
	void SetSpecularTriangle(float x, float y, float z);
};

#endif