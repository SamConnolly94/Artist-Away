#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <D3DX10math.h>
#include "PrioEngineVars.h"

class CFrustum
{
private:
	CLogger* logger;
public:
	CFrustum();
	~CFrustum();
public:
	void ConstructFrustum(float farClip, D3DXMATRIX projMatrix, D3DXMATRIX viewMatrix);
	bool CheckPoint(float x, float y, float z);
private:
	D3DXPLANE mPlanes[6];
};

#endif