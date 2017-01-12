// Globals.
Texture2D shaderTextures[3];
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColour;
	float4 diffuseColour;
	float3 lightDirection;
	float padding;
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
textureColour = shaderTextures[0].Sample(SampleType, input.tex);

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

return colour;
}
