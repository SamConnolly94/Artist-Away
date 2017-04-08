///////////////////////////
// Buffers
///////////////////////////

cbuffer MatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjMatrix;
	matrix ViewProjMatrix;
};

///////////////////////////
// Structures
///////////////////////////

struct VertexInputType
{
	float4 WorldPosition : POSITION;
	float2 UV			 : TEXCOORD0;
};

struct PixelInputType
{
	float4 ProjPosition	 : SV_POSITION;
	float2 UV			 : TEXCOORD0;
};

///////////////////////////
// Vertex shader
///////////////////////////

PixelInputType CloudVS(VertexInputType input)
{
	PixelInputType output;

	input.WorldPosition.w = 1.0f;

	output.ProjPosition = mul(input.WorldPosition, WorldMatrix);
	output.ProjPosition = mul(output.ProjPosition, ViewProjMatrix);
	//output.ProjPosition = mul(output.ProjPosition, ViewMatrix);
	//output.ProjPosition = mul(output.ProjPosition, ProjMatrix);

	output.UV = input.UV;

	return output;
}