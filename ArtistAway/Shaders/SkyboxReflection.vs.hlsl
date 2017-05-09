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
};

struct PixelInputType
{
	float4 ProjectedPosition : SV_POSITION;
	float4 WorldPosition : POSITION;
};

//////////////////////////
// Vertex shader
//////////////////////////

PixelInputType SkyboxReflectionVS(VertexInputType input)
{
	PixelInputType output;

	output.WorldPosition = mul(input.WorldPosition, WorldMatrix);
	output.ProjectedPosition = mul(output.WorldPosition, ViewMatrix);
	output.ProjectedPosition = mul(output.ProjectedPosition, ProjectionMatrix);

	return output;
}