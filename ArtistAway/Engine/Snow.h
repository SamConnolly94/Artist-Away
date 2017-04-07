#ifndef SNOW_H
#define SNOW_H

#include <d3d11.h>
#include <D3DX10math.h>
#include "PrioEngineVars.h"
#include "Texture.h"
#include "D3D11.h"
#include "RainShader.h"

class CSnow
{
private:
	CLogger* logger;
private:
	struct VertexType
	{
		D3DXVECTOR3 InitialPos;
		D3DXVECTOR3 InitialVelocity;
		D3DXVECTOR2 Size;
		float Age;
		unsigned int Type;
	};
public:
	CSnow();
	~CSnow();
public:
	bool Initialise(ID3D11Device* device, std::string rainTexture, unsigned int numberOfParticles);
	bool InitialiseBuffers(ID3D11Device* device);
	void Shutdown();
	void Update(float updateTime);
	void UpdateRender(ID3D11DeviceContext* deviceContext);
	void Render(ID3D11DeviceContext * deviceContext);
private:
	void ShutdownBuffers();
	ID3D11ShaderResourceView* CreateRandomTexture(ID3D11Device* device);
private:
	ID3D11Buffer* mpInitialBuffer;
	ID3D11Buffer* mpStreamBuffer;
	ID3D11Buffer* mpDrawBuffer;
private:
	unsigned int mNumberOfParticles;
	CTexture* mpRainDropTexture;
	ID3D11ShaderResourceView* mpRandomResourceView;
	bool mFirstRun = false;
	float mAge = 0.0f;
	float mGameTime;
	D3DXVECTOR3 mEmiterPosition;
	D3DXVECTOR3 mEmitterDirection;
	bool mEnabled = false;
public:
	void SetNumberOfParticles(unsigned int numParticles);
	void SetFirstRun(bool firstRun);
	void SetAge(float age);
	void SetGameTime(float gameTime);
	void SetEmitterPos(D3DXVECTOR3 pos);
	void SetEmitterDir(D3DXVECTOR3 dir);


	unsigned int GetNumberOfParticles();
	bool GetIsFirstRun();
	float GetAge();
	float GetGameTime();
	D3DXVECTOR3 GetEmitterPos();
	D3DXVECTOR3 GetEmitterDir();

	ID3D11ShaderResourceView* GetRainTexture();
	ID3D11ShaderResourceView* GetRandomTexture();

	void SetEnabled(bool value) { mEnabled = value; };
	bool IsEnabled() { return mEnabled; };

};

#endif