//////////////////////////
// Constant buffers
//////////////////////////

cbuffer MatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
	matrix ViewProjMatrix;
};

//////////////////////////
// Structures
//////////////////////////

struct VertexInputType
{
	float4 WorldPosition : POSITION;
	float2 UV : TEXCOORD0;
};

struct PixelInputType
{
	float4 ProjectedPosition : SV_POSITION;
	float4 WorldPosition : POSITION;
	float2 UV : TEXCOORD0;
};

//////////////////////////
// Vertex shader
//////////////////////////

PixelInputType CloudReflectionVS(VertexInputType input)
{
	PixelInputType output;

	output.WorldPosition = mul(input.WorldPosition, WorldMatrix);
	output.ProjectedPosition = mul(output.WorldPosition, ViewMatrix);
	output.ProjectedPosition = mul(output.ProjectedPosition, ProjectionMatrix);

	output.UV = input.UV;

	return output;
}