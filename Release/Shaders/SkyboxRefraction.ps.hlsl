//////////////////////////
// Sample states
//////////////////////////

SamplerState TrilinearWrap : register(s0);
SamplerState BilinearMirror : register(s1);

//////////////////////////
// Textures
//////////////////////////

Texture2D WaterHeightMap : register(t0);

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
	float	lightBufferPadding;
}

cbuffer GradientBuffer : register(b2)
{
	float4 apexColor;
	float4 centerColor;
};

//////////////////////////
// Structures
//////////////////////////

struct PixelInputType
{
	float4 ProjectedPosition : SV_POSITION;
	float4 WorldPosition : POSITION;
	float2 UV : TEXCOORD0;
	float4 domePosition : TEXCOORD1;
	float3 Normal : NORMAL;
};

//////////////////////////
// Pixel shaders
//////////////////////////

float4 SkyDomePixelShader(PixelInputType input) : SV_TARGET
{
	float height;
	float4 outputColor;


	// Determine the position on the sky dome where this pixel is located.
	height = input.domePosition.y;

	// The value ranges from -1.0f to +1.0f so change it to only positive values.
	if (height < 0.0)
	{
		height = 0.0f;
	}

	// Determine the gradient color by interpolating between the apex and center based on the height of the pixel in the sky dome.
	outputColor = lerp(centerColor, apexColor, height);

	return outputColor;
}

float4 SkyboxRefractionPS(PixelInputType input) : SV_TARGET
{
	float2 screenUV = input.ProjectedPosition.xy / ViewportSize;
	float waterHeight = WaterHeightMap.Sample(BilinearMirror, screenUV);
	float objectDepth = waterHeight - input.WorldPosition.y;

	float3 maxLightDistance = float3(15.0f,75.0f,300.0f);
	float waterBrightness = 0.5f;
	float maxDistortionDistance = 40.0f;

	clip(objectDepth);

	float3 sceneColour = SkyDomePixelShader(input);

	float3 depthDarken = saturate(objectDepth / maxLightDistance);

	float3 refractionColour = lerp(sceneColour, normalize(maxLightDistance) * waterBrightness, depthDarken);

	return float4(refractionColour, saturate(objectDepth / maxDistortionDistance));
}