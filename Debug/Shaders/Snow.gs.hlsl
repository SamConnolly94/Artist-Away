//////////////////////////
// Preprocesser Definitions
/////////////////////////

#define PT_EMITTER 0
#define PT_FLARE 1

//////////////////////////
// Constant Buffer
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

struct VertexInputType
{
	float3 WorldPosition : POSITION;
	uint   Type			 : TYPE;
};

struct PixelInputType
{
	float4 ProjectedPosition : SV_Position;
	float2 UV				 : TEXCOORD0; 
};

//////////////////////////
// Geometry Shader
/////////////////////////

[maxvertexcount(2)]
void SnowGS(point VertexInputType input[1], inout LineStream<PixelInputType> lineStream)
{
	float3 Acceleration = float3(WindX, Gravity, WindZ);

	if (input[0].Type != PT_EMITTER)
	{
		float3 p0 = input[0].WorldPosition.xyz;
		float3 p1 = input[0].WorldPosition.xyz + 0.07f*Acceleration;

		PixelInputType v0;
		v0.ProjectedPosition = mul(float4(p0, 1.0f), ViewProjMatrix);
		v0.UV = float2(0.0f, 0.0f);
		lineStream.Append(v0);

		PixelInputType v1;
		v1.ProjectedPosition = mul(float4(p1, 1.0f), ViewProjMatrix);
		v1.UV = float2(1.0f, 1.0f);
		lineStream.Append(v1);
	}
}