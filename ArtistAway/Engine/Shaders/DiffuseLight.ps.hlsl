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

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColour;
	float3 lightDir;
	float lightIntensity;
	float4 colour;


	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColour = textures[0].Sample(SampleType, input.tex);

	// Set the colour to the ambient colour.
	colour = ambientColour;

	// Invert the light direction for calculations.
	lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		colour += (diffuseColour * lightIntensity);
	}

	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
	colour = saturate(colour);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	colour = colour * textureColour;

	if (useAlphaMap)
	{
		float4 alphaColour = textures[1].Sample(SampleType, input.tex);
		if (alphaColour.r == 0.0f)
		{
			 // Don't want to draw this pixel if the alpha map is black.
			discard;
		}
	}
	return colour;
}
