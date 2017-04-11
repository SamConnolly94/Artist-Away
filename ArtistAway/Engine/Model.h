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
	D3DXMATRIX GetWorldMatrix() { return mWorldMatrix; };
};

#endif
