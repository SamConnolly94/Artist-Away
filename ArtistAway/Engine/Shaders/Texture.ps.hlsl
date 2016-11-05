// Sample texture pixel shader.

// Globals
Texture2D shaderTexture;
SamplerState SampleType;

// Typedefs
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// Pixel Shader
float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColour;

	// Sample the pixel colour from the texture using a sampler at each texture coordinate location.
	textureColour = shaderTexture.Sample(SampleType, input.tex);

	return textureColour;
}