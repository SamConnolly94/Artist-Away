//////////////////////////
// Sampler states
/////////////////////////

SamplerState PointClamp : register(s0);

//////////////////////////
// Textures
/////////////////////////

Texture2D GrassTexture : register(t0);
Texture2D AlphaMask : register(t1);
Texture2D ReedTexture : register(t2);
Texture2D ReedAlphaTexture : register(t3);

//////////////////////////
// Input Structures
/////////////////////////

struct PixelInputType
{
	float4 ScreenPosition : SV_POSITION;
	float4 WorldPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL;
	uint Type : TEXCOORD1;
};

//////////////////////////
// Constant buffers
//////////////////////////

cbuffer LightBuffer : register(b0)
{
	float4 AmbientColour;
	float4 DiffuseColour;
	float3 LightDirection;
	float padding;
};

//////////////////////////
// Pixel Shader
/////////////////////////
#define GrassType 0
#define ReedType 1

float4 FoliagePS(PixelInputType input) : SV_TARGET
{
	float4 textureColour = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 alpha = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	if (input.Type == GrassType)
	{
		textureColour = GrassTexture.Sample(PointClamp, input.UV);
		alpha = AlphaMask.Sample(PointClamp, input.UV);
	}
	else if (input.Type == ReedType)
	{
		textureColour = ReedTexture.Sample(PointClamp, input.UV);
		alpha = ReedAlphaTexture.Sample(PointClamp, input.UV);
	}
	else
	{
		discard;
	}

	if (alpha.g == 0.0f)
	{
		discard;
	}

	textureColour.a = (textureColour.r + textureColour.g + textureColour.b) * 0.333f;

	if (textureColour.a < 0.1f)
	{
		discard;
	}
	else
	{
		textureColour.a = 1.0f;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// Set the colour to the ambient colour.
	float4 colour = AmbientColour;

	// Invert the light direction for calculations.
	float3 lightDir = -LightDirection;

	// Calculate the amount of light on this pixel.
	float lightIntensity = saturate(dot(input.Normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		colour += (DiffuseColour * lightIntensity);
	}

	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
	colour = saturate(colour);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	colour = colour * textureColour;

	return colour;
}

