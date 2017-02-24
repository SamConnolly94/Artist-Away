#ifndef MESH_H
#define MESH_H

// Windows library includes.
#include <string>
#include <list>

#include "Model.h"
#include "Texture.h"
#include "DiffuseLightShader.h"
#include "ColourShader.h"
#include "TextureShader.h"
#include "SpecularLightingShader.h"
#include "Light.h"
#include <vector>
#include "Dependencies/assimp-3.3.1/include/assimp/Importer.hpp"
#include "Dependencies/assimp-3.3.1/include\assimp/scene.h"
#include "Dependencies/assimp-3.3.1/include/assimp/postprocess.h"
#include "PrioEngineVars.h"

const int mNumberOfTextures = 3;

class CMesh
{
private:
	CLogger* logger;
private:
	// File strings
	std::string mFilename;

	// A list of the instance of models belonging to this mesh.
	std::list<CModel*> mpModels;

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 uv;
		D3DXVECTOR3 normal;
	};

	struct MaterialType
	{
		ID3D11ShaderResourceView* mTextures[mNumberOfTextures];
	};

	// Arrays to store data about vertices in.
	struct SubMesh
	{
		int numberOfVertices;
		int numberOfIndices;
		int materialIndex;
		aiVector3D* vertices;
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		aiFace* faces;
	};

	ID3D11Device* mpDevice;
	SubMesh* mpSubMeshes;
	unsigned int mNumberOfSubMeshes;
	bool CreateSubmesh(const aiMesh& mesh, SubMesh* subMesh);
public:
	CMesh(ID3D11Device* device);
	~CMesh();

	// Loads data from file into our mesh object.
	CModel* CreateModel();
	bool LoadMesh(std::string filename);

	void Render(ID3D11DeviceContext* context, CDiffuseLightShader* shader, D3DXMATRIX &view, D3DXMATRIX &proj, std::list<CLight*>lights);
	void Shutdown();
private:
	bool LoadAssimpModel(std::string filename);
	unsigned int mVertexCount;
	unsigned int mIndexCount;
	MaterialType* mSubMeshMaterials;
	int numberOfSubMaterials;
};
#endif