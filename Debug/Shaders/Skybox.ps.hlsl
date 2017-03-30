///////////////////////////
// Buffers
///////////////////////////

cbuffer GradientBuffer
{
	float4 apexColour;
	float4 centerColour;
};


///////////////////////////
// Pixel structures
///////////////////////////


struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 domePosition : TEXCOORD0;
};

///////////////////////////
// Pixel shader
///////////////////////////

float4 SkyDomePixelShader(PixelInputType input) : SV_TARGET
{
	float height;
	float4 outputColour;


	// Determine the position on the sky dome where this pixel is located.
	height = input.domePosition.y;

	// The value ranges from -1.0f to +1.0f so change it to only positive values.
	if (height < 0.0)
	{
		height = 0.0f;
	}

	// Determine the gradient color by interpolating between the apex and center based on the height of the pixel in the sky dome.
	outputColour = lerp(centerColour, apexColour, height);

	return outputColour;
}