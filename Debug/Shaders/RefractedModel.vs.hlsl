//////////////////////////
// Constant buffers
//////////////////////////

cbuffer MatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

//////////////////////////
// Structures
//////////////////////////

struct VertexInputType
{
	float4 WorldPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL;
};

struct PixelInputType
{
	float4 ProjectedPosition : SV_POSITION;
	float4 WorldPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL;
};

//////////////////////////
// Vertex shader
//////////////////////////

PixelInputType RefractionVS(VertexInputType input)
{
	PixelInputType output;

	output.WorldPosition = mul(input.WorldPosition, WorldMatrix);
	output.ProjectedPosition = mul(output.WorldPosition, ViewMatrix);
	output.ProjectedPosition = mul(output.ProjectedPosition, ProjectionMatrix);

	output.Normal = mul(input.Normal, (float3x3)WorldMatrix);

	output.UV = input.UV;

	output.Normal = normalize(output.Normal);

	return output;
}