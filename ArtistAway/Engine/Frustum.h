#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <D3DX10math.h>

class CFrustum
{
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