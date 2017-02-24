#ifndef CMODEL_H
#define CMODEL_H

#include "VertexTypeManager.h"
#include "ModelControl.h"
#include <vector>
#include "Logger.h"

class CModel : public CModelControl
{
private:
	CLogger* logger;

private:
	D3DXMATRIX mWorldMatrix;
public:
	CModel();
	~CModel();
	void Shutdown();

	void UpdateMatrices();
	void RenderBuffers(ID3D11DeviceContext* deviceContext, int subMeshIndex, ID3D11Buffer* &vertexBuffer, ID3D11Buffer* &indexBuffer, unsigned int stride);
	D3DXMATRIX GetWorldMatrix() { return mWorldMatrix; };
};

#endif
