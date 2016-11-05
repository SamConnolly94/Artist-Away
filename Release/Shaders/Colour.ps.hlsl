//
// PIXEL SHADER FILE
//

// Typedefs

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 colour : COLOR;
};

// Pixel shader
float4 ColourPixelShader(PixelInputType input) : SV_TARGET
{
	return input.colour;
}