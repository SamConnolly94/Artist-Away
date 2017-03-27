//////////////////////////
// Sample states
//////////////////////////

SamplerState TrilinearWrap : register(s0);

//////////////////////////
// Textures
//////////////////////////

Texture2D NormalHeightMap : register(t0);

//////////////////////////
// Constant buffers
// We probably won't need most of these, but by being consistent it makes things a little bit easier
// to read when flicking between shaders / adding new stuff.
//////////////////////////

cbuffer MatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
}

cbuffer WaterBuffer : register(b1)
{
	float2 WaterMovement;
	float WaveHeight;
	float WaveScale;
	float RefractionDistortion;
	float ReflectionDistortion;
	float MaxDistortionDistance;
	float RefractionStrength;
	float ReflectionStrength;
	float3 waterBufferPadding;
}

//////////////////////////
// Structures
//////////////////////////

struct VertexInputType
{
	float3 WorldPosition	: POSITION;
	float2 UV				: TEXCOORD0;
	float3 Normal			: NORMAL;
};

struct PixelInputType
{
	float4 ProjectedPosition	: SV_POSITION;
	float2 UV					: TEXCOORD0;
	float4 WorldPosition		: POSITION;
};

//////////////////////////
// Vertex shader
//////////////////////////

static const float WaterSize1 = 0.5f;
static const float WaterSize2 = 1.0f;
static const float WaterSize3 = 2.0f;
static const float WaterSize4 = 4.0f;

// Each wave layer moves at different speeds so it animates in a varying manner
static const float WaterSpeed1 = 0.5f;
static const float WaterSpeed2 = 1.0f;
static const float WaterSpeed3 = 1.7f;
static const float WaterSpeed4 = 2.6f;

PixelInputType WaterSurfaceVS(VertexInputType input)
{
	PixelInputType output;

	float2 waterUV = input.UV;
	//waterUV /= 8;
	float normal1 = NormalHeightMap.SampleLevel(TrilinearWrap, WaterSize1 * (waterUV + WaterMovement * WaterSpeed1), 0.0f).a;
	float normal2 = NormalHeightMap.SampleLevel(TrilinearWrap, WaterSize2 * (waterUV + WaterMovement * WaterSpeed2), 0.0f).a;
	float normal3 = NormalHeightMap.SampleLevel(TrilinearWrap, WaterSize3 * (waterUV + WaterMovement * WaterSpeed3), 0.0f).a;
	float normal4 = NormalHeightMap.SampleLevel(TrilinearWrap, WaterSize4 * (waterUV + WaterMovement * WaterSpeed4), 0.0f).a;

	float height = normal1 + normal2 + normal3 + normal4;

	float4 modelPosition = float4(input.WorldPosition.xyz, 1.0f);

	modelPosition.y += (0.25f * height - 0.5f) * WaveHeight * WaveScale; 			// Transform new water vertex position to world space and send to pixel shader
	
	float4 worldPosition = mul(modelPosition, WorldMatrix);
	output.WorldPosition = worldPosition;
	float4 viewPosition = mul(worldPosition, ViewMatrix);
	output.ProjectedPosition = mul(viewPosition, ProjectionMatrix);

	output.UV = input.UV;

	return output;
}