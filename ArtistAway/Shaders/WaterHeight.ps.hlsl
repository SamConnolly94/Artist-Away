//////////////////////////
// Sample states
//////////////////////////

SamplerState TrilinearWrap : register(s0);

//////////////////////////
// Structures
//////////////////////////

struct PixelInputType
{
	float4 ProjectedPosition	: SV_POSITION;
	float2 UV					: TEXCOORD0;
	float4 WorldPosition		: POSITION;
};

//////////////////////////
// Pixel shader
//////////////////////////

float4 WaterHeightPS(PixelInputType input) : SV_TARGET
{
	return input.WorldPosition.y;
}