// Globals.
Texture2D textures[3];
SamplerState SampleType;

cbuffer LightBuffer : register(b0)
{
	float4 ambientColour;
	float4 diffuseColour;
	float3 lightDirection;
	float specularPower;
	float4 specularColor;
};

cbuffer MapBuffer : register(b1)
{
	bool useAlphaMap;
	bool useSpecularMap;
	float3 padding2;
	float3 viewDirection : TEXCOORD1;
};

// Typedefs
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 TransparentPS(PixelInputType input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	float4 textureColour = textures[0].Sample(SampleType, input.tex);

	//textureColour.rgb += ambientColour;

	if (textureColour.r < 0.9f)
	{
		textureColour.a = 0.0f;
	}

	return textureColour;
}
