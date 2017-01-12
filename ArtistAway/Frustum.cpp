#include "Frustum.h"



CFrustum::CFrustum()
{
}


CFrustum::~CFrustum()
{
}

void CFrustum::ConstructFrustum(float farClip, D3DXMATRIX projMatrix, D3DXMATRIX viewMatrix)
{
	// Calculate the minimum Z in the frustum.
	float minZ = -projMatrix._43 / projMatrix._33;
	float r = farClip / (farClip / minZ);
	projMatrix._33 = r;
	projMatrix._43 = -r * minZ;

	// Create the frustum matrix from view and new proj.
	D3DXMATRIX matrix;
	D3DXMatrixMultiply(&matrix, &viewMatrix, &projMatrix);

	// Calculate near plane of frustum.
	mPlanes[0].a = matrix._14 + matrix._13;
	mPlanes[0].b = matrix._24 + matrix._23;
	mPlanes[0].c = matrix._34 + matrix._33;
	mPlanes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&mPlanes[0], &mPlanes[0]);

	// Calculate far plane of frustum
	mPlanes[1].a = matrix._14 - matrix._13;
	mPlanes[1].b = matrix._24 - matrix._23;
	mPlanes[1].c = matrix._34 - matrix._33;
	mPlanes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&mPlanes[1], &mPlanes[1]);

	// Calculate left plane of frustum
	mPlanes[2].a = matrix._14 + matrix._11;
	mPlanes[2].b = matrix._24 + matrix._21;
	mPlanes[2].c = matrix._34 + matrix._31;
	mPlanes[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&mPlanes[2], &mPlanes[2]);

	// Calculate right plane of frustum
	mPlanes[3].a = matrix._14 - matrix._11;
	mPlanes[3].b = matrix._24 - matrix._21;
	mPlanes[3].c = matrix._34 - matrix._31;
	mPlanes[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&mPlanes[3], &mPlanes[3]);
	
	// Calculate top plane of frustum
	mPlanes[4].a = matrix._14 + matrix._12;
	mPlanes[4].b = matrix._24 + matrix._22;
	mPlanes[4].c = matrix._34 + matrix._32;
	mPlanes[4].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&mPlanes[4], &mPlanes[4]);

	// Calculate bottom plane of frustum
	mPlanes[5].a = matrix._14 - matrix._12;
	mPlanes[5].b = matrix._24 - matrix._22;
	mPlanes[5].c = matrix._34 - matrix._32;
	mPlanes[5].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&mPlanes[5], &mPlanes[5]);
}

bool CFrustum::CheckPoint(float x, float y, float z)
{
	for (int i = 0; i < 6; i++)
	{
		// If coordinates are outside our frustum planes.
		if (D3DXPlaneDotCoord(&mPlanes[i], &D3DXVECTOR3(x, y, z)) < 0.0f)
		{
			// Don't need to render this one.
			return false;
		}
	}
	// Success!
	return true;
}
