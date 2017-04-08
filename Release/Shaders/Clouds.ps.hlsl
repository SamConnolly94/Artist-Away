///////////////////////////
// Sampler states
///////////////////////////

SamplerState TrilinearWrap : register(s0);

///////////////////////////
// Textures
///////////////////////////

Texture2D CloudTexture1 : register(t0);
Texture2D CloudTexture2 : register(t1);

///////////////////////////
// Buffers
///////////////////////////

cbuffer CloudBuffer : register(b0)
{
	float2 Cloud1Movement;
	float2 Cloud2Movement;
	float Brightness;
	float3 cloudBufferPadding;
}

///////////////////////////
// Structures
///////////////////////////

struct PixelInputType
{
	float4 ProjPosition	 : SV_POSITION;
	float2 UV			 : TEXCOORD0;
};

///////////////////////////
// Vertex shader
///////////////////////////

float4 CloudPS(PixelInputType input) : SV_TARGET
{
	float2 texCoords;
	
	// Texture coordinates at the x pos of the first cloud, accounts for movement.
	texCoords.x = input.UV.x + Cloud1Movement.x;
	texCoords.y = input.UV.y + Cloud1Movement.y;

	// Sample the first cloud at the calculated point.
	float4 texture1Colour = CloudTexture1.Sample(TrilinearWrap, texCoords);

	// Texture coordinates at the x pos of the first cloud, accounts for movement.
	texCoords.x = input.UV.x + Cloud2Movement.x;
	texCoords.y = input.UV.y + Cloud2Movement.y;

	float4 texture2Colour = CloudTexture2.Sample(TrilinearWrap, texCoords);

	// Combine the two colours, 0.5 for an even 50/50 split.
	float4 colour = lerp(texture1Colour, texture2Colour, 0.5f);

	colour.rgb *= Brightness;

	// Return the final colour.
	return colour;
}