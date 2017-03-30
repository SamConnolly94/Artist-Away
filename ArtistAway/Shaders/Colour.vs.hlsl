//
// VERTEX SHADER FILE.
//

// Globals

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix ViewProjMatrix;
};

// Typedefs

struct VertexInputType
{
	float4 position : POSITION;
	float4 colour : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 colour : COLOR;
};

// Vertex shader
PixelInputType ColourVertexShader(VertexInputType input)
{
	PixelInputType output;

	// Give a 4th element to our matrix so it's the correct size;
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, ViewProjMatrix);
	//output.position = mul(output.position, viewMatrix);
	//output.position = mul(output.position, projectionMatrix);

	output.colour = input.colour;

	return output;
}