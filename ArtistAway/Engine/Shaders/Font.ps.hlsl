//////////////////////////
// Sampler states
/////////////////////////

SamplerState SampleType;

//////////////////////////
// Textures
/////////////////////////

Texture2D shaderTexture;

//////////////////////////
// Pixel buffers
/////////////////////////

cbuffer PixelBuffer
{
	float4 pixelColour;
};

//////////////////////////
// Input Structures
/////////////////////////

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

//////////////////////////
// Pixel Shader
/////////////////////////

float4 FontPixelShader(PixelInputType input): SV_TARGET
{
	float4 colour;


	// Sample the texture pixel at this location.
colour = shaderTexture.Sample(SampleType, input.uv);

	// If the colour is black on the texture then treat this pixel as transparent.
	colour.a = (colour.r + colour.g + colour.b) * 0.333f;

	// If the color is other than black on the texture then this is a pixel in the font so draw it using the font pixel color.
	colour = colour * pixelColour;

	return colour;
}

