#ifndef MODELCONTROL_H
#define MODELCONTROL_H

#include <D3DX10math.h>
#include "PrioEngineVars.h"

class CModelControl
{
private:
	CLogger* logger;
	float ToRadians(float degrees);
protected:
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mRotation;
	D3DXVECTOR3 mScale;
	CModelControl* mpParent;
	D3DXMATRIX mWorldMatrix;
public:
	/* Rotation. */
	void RotateX(float x);
	void RotateY(float y);
	void RotateZ(float z);

	float GetRotationX();
	float GetRotationY();
	float GetRotationZ();
	D3DXVECTOR3 GetRotation();

	void SetRotationX(float x);
	void SetRotationY(float y);
	void SetRotationZ(float z);
	void SetRotation(float x, float y, float z);

	/* Translation. */
	void MoveX(float x);
	void MoveY(float y);
	void MoveZ(float z);

	float GetPosX();
	float GetPosY();
	float GetPosZ();
	D3DXVECTOR3 GetPos();

	void SetXPos(float x);
	void SetYPos(float y);
	void SetZPos(float z);
	void SetPos(float x, float y, float z);

	/* Scale. */
	void ScaleX(float x);
	void ScaleY(float y);
	void ScaleZ(float z);
	void Scale(float value);

	float GetScaleX();
	float GetScaleY();
	float GetScaleZ();
	D3DXVECTOR3 GetScale();
	float GetScaleRadius(float initialRadius);

	void SetScaleX(float x);
	void SetScaleY(float y);
	void SetScaleZ(float z);
	void SetScale(float x, float y, float z);
	void SetScale(float value);

	void AttatchToParent(CModelControl* parent);
	void SeperateFromParent();

	void UpdateMatrices();

	void GetWorldMatrix(D3DXMATRIX& world) { world = mWorldMatrix; };
public:
	CModelControl();
	~CModelControl();
};

#endif