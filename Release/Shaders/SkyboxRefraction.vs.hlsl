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
	float3 WorldPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL;
};

struct PixelInputType
{
	float4 ProjectedPosition : SV_POSITION;
	float4 WorldPosition : POSITION;
	float2 UV : TEXCOORD0;
	float4 domePosition : TEXCOORD1;
	float3 Normal : NORMAL;
};

//////////////////////////
// Vertex shader
//////////////////////////

PixelInputType SkyboxVS(VertexInputType input)
{
	PixelInputType output;

	float4 modelPos = float4(input.WorldPosition, 1.0f);

	output.domePosition = mul(modelPos, WorldMatrix);

	// Going to use this as the dome pos in new shader so don't do anything fancy with it.
	output.WorldPosition = mul(modelPos, WorldMatrix);

	output.ProjectedPosition = mul(modelPos, WorldMatrix);
	output.ProjectedPosition = mul(output.ProjectedPosition, ViewProjMatrix);
	//output.ProjectedPosition = mul(output.ProjectedPosition, ViewMatrix);
	//output.ProjectedPosition = mul(output.ProjectedPosition, ProjectionMatrix);

	output.Normal = mul(input.Normal, (float3x3)WorldMatrix);

	output.UV = input.UV;

	output.Normal = normalize(output.Normal);

	return output;
}