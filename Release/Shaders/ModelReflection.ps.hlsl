//////////////////////////
// Sample states
//////////////////////////

SamplerState TrilinearWrap : register(s0);
SamplerState BilinearMirror : register(s1);

//////////////////////////
// Textures
//////////////////////////

Texture2D WaterHeightMap : register(t0);
Texture2D shaderTexture[3] : register(t1);

//////////////////////////
// Constant buffers
//////////////////////////

cbuffer ViewportBuffer : register(b0)
{
	float2 ViewportSize;
}

cbuffer LightBuffer : register(b1)
{
	float4	AmbientColour;
	float4	DiffuseColour;
	float3	LightDirection;
	float4	SpecularColour;
	float3	LightPosition;
	float	SpecularPower;
	float	lightBufferPadding;
}

cbuffer MapBuffer : register(b2)
{
	bool useAlphaMap;
	bool useSpecularMap;
	float3 padding2;
	float3 viewDirection : TEXCOORD1;
};

cbuffer PositioningBuffer : register(b3)
{
	float WaterPlaneY;
	float3 posPadding;
	float4 posPadding2;
}

//////////////////////////
// Structures
//////////////////////////

struct PixelInputType
{
	float4 ProjectedPosition : SV_POSITION;
	float4 WorldPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL;
};

//////////////////////////
// Pixel shader
//////////////////////////

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColour;
	float3 lightDir;
	float lightIntensity;
	float4 colour;


	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColour = shaderTexture[0].Sample(TrilinearWrap, input.UV);

	// Set the colour to the ambient colour.
	colour = AmbientColour;

	// Invert the light direction for calculations.
	lightDir = -LightDirection;

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.Normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		colour += (DiffuseColour * lightIntensity);
	}

	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
	colour = saturate(colour);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	colour = colour * textureColour;

	if (useAlphaMap)
	{
		float4 alphaColour = shaderTexture[1].Sample(TrilinearWrap, input.UV);
		if (alphaColour.r == 0.0f)
		{
			// Don't want to draw this pixel if the alpha map is black.
			discard;
		}
	}
	return colour;
}



float4 ModelReflectionPS(PixelInputType input) : SV_TARGET
{
	float2 screenUV = input.ProjectedPosition.xy / ViewportSize;
	float waterHeight = WaterHeightMap.Sample(BilinearMirror, screenUV);
	float objectHeight = input.WorldPosition.y - (WaterPlaneY - waterHeight);

	clip(objectHeight);
	return float4(LightPixelShader(input).rgb, saturate(objectHeight / 40.0f));
}