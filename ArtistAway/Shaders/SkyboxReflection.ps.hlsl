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

cbuffer GradientBuffer
{
	float4 apexColour;
	float4 centreColour;
	float2 ViewportSize;
	float WaterPlaneY;
	float gradientPadding;
};

//////////////////////////
// Structures
//////////////////////////

struct PixelInputType
{
	float4 ProjectedPosition : SV_POSITION;
	float4 WorldPosition : TEXCOORD0;
};


///////////////////////////
// Pixel shader
///////////////////////////

float4 SkyboxPS(PixelInputType input) : SV_TARGET
{
	float height;
	float4 outputColour;


	// Determine the position on the sky dome where this pixel is located.
	height = input.WorldPosition.y;

	// The value ranges from -1.0f to +1.0f so change it to only positive values.
	if (height > 0.0)
	{
		height = 0.0f;
	}

	// Determine the gradient color by interpolating between the apex and center based on the height of the pixel in the sky dome.
	outputColour = lerp(apexColour, centreColour, height);

	return outputColour;
}

//////////////////////////
// Pixel shader
//////////////////////////

float4 SkyboxReflectionPS(PixelInputType input) : SV_TARGET
{
	float2 screenUV = input.ProjectedPosition.xy / ViewportSize;
	float waterHeight = WaterHeightMap.Sample(BilinearMirror, screenUV);
	float objectHeight = input.WorldPosition.y - (WaterPlaneY - waterHeight);

	clip(objectHeight);
	return float4(SkyboxPS(input).rgb, saturate(objectHeight / 40.0f));
}