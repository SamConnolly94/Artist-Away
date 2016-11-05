#ifndef CAMERA_H
#define CAMERA_H

#include <d3dx10math.h>

class CCamera
{
public:
	CCamera(int screenWidth, int screenHeight, float fov, float nearClip, float farClip);
	~CCamera();

	// Set the position of the camera in the world.
	void SetPosition(float x, float y, float z);
	void SetPositionX(float x);
	void SetPosizionY(float y);
	void SetPositionZ(float z);

	void MoveX(float x);
	void MoveY(float y);
	void MoveZ(float z);

	void MoveLocalX(float x);
	void MoveLocalY(float y);
	void MoveLocalZ(float z);

	void RotateX(float x);
	void RotateY(float y);
	void RotateZ(float z);

	float GetRotX();
	float GetRotY();
	float GetRotZ();

	float GetX();
	float GetY();
	float GetZ();

	void SetRotation(float x, float y, float z);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX& viewMatrix);

private:
	int mScreenWidth;
	int mScreenHeight;

	float mFov;
	float mNearClip;
	float mFarClip;

	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mRotation;

	D3DXMATRIX mWorldMatrix;
	D3DXMATRIX mViewMatrix;
	D3DXMATRIX mProjMatrix;
	D3DXMATRIX mViewProjMatrix;

	void UpdateMatrices();
};

#endif