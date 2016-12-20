#ifndef MESH_H
#define MESH_H

// Windows library includes.
#include <string>
#include <list>

#include "Model.h"
#include "Texture.h"
#include "DirectionalLightShader.h"
#include "ColourShader.h"
#include "TextureShader.h"
#include "SpecularLightingShader.h"
#include "Light.h"
#include <vector>
#include "Dependencies/assimp-3.3.1/include/assimp/Importer.hpp"
#include "Dependencies/assimp-3.3.1/include\assimp/scene.h"
#include "Dependencies/assimp-3.3.1/include/assimp/postprocess.h"

class CMesh
{
private:
	// Handle of our main window.
	HWND mHwnd;

	// Pointer to the device object.
	ID3D11Device* mpDevice;

	// Shader objects.
	CDirectionalLightShader* mpDirectionalLightShader;
	CTextureShader* mpTextureShader;
	CColourShader* mpColourShader;
	CSpecularLightingShader* mpSpecularShader;

	// File strings
	std::string mFilename;
	std::string mFileExtension;

	// A list of the instance of models belonging to this mesh.
	std::list<CModel*> mpModels;

	// The object reffering to the texture for this mesh.
	CTexture* mpTexture;

	// Arrays to store data about vertices in.
	std::vector<D3DXVECTOR3> mpVerticesList;
	std::vector<D3DXVECTOR2> mpUVList;
	std::vector<D3DXVECTOR3> mpNormalsList;
	std::vector<D3DXVECTOR4> mpVertexColourList;
	std::vector<unsigned long> mpIndicesList;

	// This will need to be updated every frame.
	D3DXVECTOR3 mCameraPosition;
public:
	CMesh(ID3D11Device* device, HWND hwnd, PrioEngine::ShaderType shaderType);
	~CMesh();

	// Loads data from file into our mesh object.
	CModel* CreateModel();
	bool LoadMesh(char* filename, WCHAR* textureName);

	void Render(ID3D11DeviceContext* context, D3DXMATRIX &view, D3DXMATRIX &proj, std::list<CLight*>lights);

	void SetCameraPos(D3DXVECTOR3 cameraPos) { mCameraPosition = cameraPos; };
private:
	bool LoadAssimpModel(char* filename);
	bool LoadSam();
	bool GetSizes();
	bool InitialiseArrays();
	unsigned int mVertexCount;
	unsigned int mIndexCount;
	
	struct FaceStruct
	{
		unsigned int x;
		unsigned int y;
		unsigned int z;
	};


	PrioEngine::ShaderType mShaderType;
};
#endif