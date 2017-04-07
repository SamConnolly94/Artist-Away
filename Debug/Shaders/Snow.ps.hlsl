//////////////////////////
// Sampler States
/////////////////////////

SamplerState TrilinearWrap : register(s0);

//////////////////////////
// Textures and Resources
/////////////////////////

Texture2D RainTexture : register(t0);


//////////////////////////
// Input Structures
/////////////////////////

struct PixelInputType
{
	float4 ViewportPosition : SV_Position;
	float2 UV				: TEXCOORD0;
};

//////////////////////////
// Pixel shader
/////////////////////////

float4 SnowPS(PixelInputType input) : SV_TARGET
{
	return RainTexture.Sample(TrilinearWrap, float3(input.UV, 0)) * float4(1.0f, 1.0f, 1.0f, 1.0f);
}