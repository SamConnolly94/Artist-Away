#ifndef CAMERA_H
#define CAMERA_H

#include <d3dx10math.h>

class CCamera
{
public:
	CCamera(int screenWidth, int screenHeight, float fov, float nearClip, float farClip);
	~CCamera();

	// Set the position of the camera in the world.
	void SetPosition(D3DXVECTOR3 pos);
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
	void GetWorldMatrix(D3DXMATRIX& worldMatrix);
	void GetViewMatrix(D3DXMATRIX& viewMatrix);
	void SetWorldMatrix(D3DXMATRIX world) { mWorldMatrix = world; };
	void GetViewProjMatrix(D3DXMATRIX& ViewProjMatrix, D3DXMATRIX proj);
	void GetReflectionView(D3DXMATRIX& view);
	void RenderReflection(float waterHeight);
	void GetReflectionViewMatrix(D3DXMATRIX& reflectionView);
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
	D3DXMATRIX mReflectionMatrix;

	void UpdateMatrices();
	float ToRadians(float degrees);

};

#endif