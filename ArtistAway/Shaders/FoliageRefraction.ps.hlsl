//////////////////////////
// Sample states
//////////////////////////

SamplerState TrilinearWrap : register(s0);
SamplerState BilinearMirror : register(s1);
SamplerState PointClamp : register(s2);

//////////////////////////
// Textures
//////////////////////////

Texture2D WaterHeightMap : register(t0);
Texture2D GrassTexture : register(t1);
Texture2D AlphaMask : register(t2);
Texture2D ReedTexture : register(t3);
Texture2D ReedAlphaTexture : register(t4);

//////////////////////////
// Constant buffers
//////////////////////////

cbuffer ViewportBuffer : register(b0)
{
	float2 ViewportSize;
	float4 viewportPadding1;
	float2 viewportPadding2;
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

//////////////////////////
// Structures
//////////////////////////

struct PixelInputType
{
	float4 ProjectedPosition : SV_POSITION;
	float4 WorldPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL;
	uint Type : TEXCOORD1;
};

///////////////////////
// Helper Functions
///////////////////////
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

//////////////////////////
// Pixel shader
//////////////////////////
float4 FoliageRefractionPS(PixelInputType input) : SV_TARGET
{
	float2 screenUV = input.ProjectedPosition.xy / ViewportSize;
	float waterHeight = WaterHeightMap.Sample(BilinearMirror, screenUV);
	float objectDepth = waterHeight - input.WorldPosition.y;

	float3 maxLightDistance = float3(15.0f,75.0f,300.0f);
	float waterBrightness = 0.5f;
	float maxDistortionDistance = 40.0f;

	//Remove pixels above water
	clip(objectDepth);

	float3 sceneColour = FoliagePS(input).rgb;

	//Gets the amount to darken the pixel based on depth in the water
	float3 depthDarken = saturate(objectDepth / maxLightDistance);

	//Darked colour using lerp between normal surface colour and the maximum distance the light travels in water
	float3 refractionColour = lerp(sceneColour, normalize(maxLightDistance) * waterBrightness, depthDarken);

	//Store the refraction colour as the rgb in refraction texture, and the depth of the pixel in the alpha channel
	return float4(refractionColour, saturate(objectDepth / maxDistortionDistance)); // Alpha ranges 0->1 for depths of 0 to MaxDistortionDistance
}