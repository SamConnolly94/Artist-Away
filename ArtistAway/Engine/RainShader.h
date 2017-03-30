#ifndef RAINSHADER_H
#define RAINSHADER_H

#include "Shader.h"

class CRainShader : public CShader
{
private:
	struct FrameBufferType
	{
		D3DXVECTOR3 CameraPos;
		float GameTime;
		D3DXVECTOR3 EmitPos;
		float FrameTime;
		float Gravity;
		float WindX;
		float WindZ;
		float frameBufferPadding;
	};

	struct VertexType
	{
		D3DXVECTOR3 InitialPos;
		D3DXVECTOR3 InitialVelocity;
		D3DXVECTOR2 Size;
		float Age;
		unsigned int Type;
	};

public:
	CRainShader();
	~CRainShader();

	bool Initialise(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext);
	bool UpdateRender(ID3D11DeviceContext* deviceContext);
private:
	bool InitialiseShader(ID3D11Device* device, HWND hwnd, std::string vsFilename, std::string gsFilename, std::string psFilename, std::string vsUpdateFilename, std::string gsUpdateFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, std::string shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext);
	void RenderShader(ID3D11DeviceContext* deviceContext);
	bool UpdateParticles(ID3D11DeviceContext* deviceContext);

private:
	ID3D11VertexShader* mpVertexShader;
	ID3D11GeometryShader* mpGeometryShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11VertexShader* mpUpdateVertexShader;
	ID3D11GeometryShader* mpUpdateGeometryShader;
	ID3D11InputLayout* mpLayout;
	ID3D11SamplerState* mpTrilinearWrap;
	ID3D11Buffer* mpMatrixBuffer;
	ID3D11Buffer* mpFrameBuffer;
private:
	ID3D11ShaderResourceView* mpRainTexture;
	ID3D11ShaderResourceView* mpRandomTexture;
	D3DXVECTOR3 mCameraWorldPosition;
	D3DXVECTOR3 mEmitWorldPosition;
	D3DXVECTOR3 mEmitWorldDirection;
	float mGameTime;
	float mFrameTime;
	float mGravityAcceleration;
	float mWindX;
	float mWindZ;
	bool mFirstRun = false;
public:
	void SetRainTexture(ID3D11ShaderResourceView* resource);
	void SetRandomTexture(ID3D11ShaderResourceView* resource);
	void SetCameraWorldPosition(D3DXVECTOR3 pos);
	void SetEmitterWorldPosition(D3DXVECTOR3 pos);
	void SetEmitterWorldDirection(D3DXVECTOR3 dir);
	void SetGameTime(float time);
	void SetFrameTime(float updateTime);
	void SetGravityAcceleration(float gravityAcceleration);
	void SetWindX(float windX);
	void SetWindZ(float windZ);
	void SetFirstRun(bool firstRun);
	ID3D11InputLayout* GetLayout() { return mpLayout; };
};

#endif