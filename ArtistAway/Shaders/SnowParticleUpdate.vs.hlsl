//////////////////////////
// Constant buffers
/////////////////////////

cbuffer MatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjMatrix;
	matrix ViewProjMatrix;
}

cbuffer FrameBuffer : register(b1)
{
	float3 CameraPos;
	float GameTime;
	float3 EmitPos;
	float FrameTime;
	float Gravity;
	float WindX;
	float WindZ;
	float frameBufferPadding;
}

//////////////////////////
// Input Structures
/////////////////////////

struct RainType
{
	float3 InitialPosition : POSITION;
	float3 InitialVelocity : VELOCITY;
	float2 Size			   : SIZE;
	float  Age			   : AGE;
	uint   Type			   : TYPE;
};

//////////////////////////
// Vertex Shader
/////////////////////////

RainType SnowUpdateVS(RainType input)
{
	return input;
}