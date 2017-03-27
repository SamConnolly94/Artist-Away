#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <D3DX10math.h>
#include "PrioEngineVars.h"

class CFrustum
{
private:
	CLogger* logger;
	const int kNumberOfPlanes = 6;
public:
	CFrustum();
	~CFrustum();
public:
	void ConstructFrustum(float farClip, D3DXMATRIX projMatrix, D3DXMATRIX viewMatrix);
	bool CheckPoint(float x, float y, float z);
	bool CheckSphere(D3DXVECTOR3 position, float radius);
private:
	D3DXPLANE mPlanes[6];
};

#endif