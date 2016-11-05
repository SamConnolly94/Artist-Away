#ifndef LIGHTCLASS_H
#define LIGHTCLASS_H

#include <D3DX10math.h>
#include "PrioEngineVars.h"
#include "ModelControl.h"

class CLight : CModelControl
{
public:
	CLight();
	~CLight();

	void SetAmbientColour(D3DXVECTOR4 colour);
	void SetDiffuseColour(D3DXVECTOR4 colour);
	void SetDirection(D3DXVECTOR3 direction);

	D3DXVECTOR4 GetDiffuseColour();
	D3DXVECTOR4 GetAmbientColour();
	D3DXVECTOR3 GetDirection();

private:
	D3DXVECTOR4 mDiffuseColour;
	D3DXVECTOR4 mAmbientColour;
	D3DXVECTOR3 mDirection;
};

#endif