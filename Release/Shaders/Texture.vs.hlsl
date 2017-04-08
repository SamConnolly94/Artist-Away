// Sample texture vertex shader.

// Globals
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projMatrix;
	matrix ViewProjMatrix;
};

// Typedefs
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// Vertex shader
PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;

	// Change the position vector to be 4 units so it can be placed in a matrix of length 4.
	input.position.w = 1.0f;

	// Calculate the position of the vertex within the world, proj and view matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, ViewProjMatrix);
	//output.position = mul(output.position, viewMatrix);
	//output.position = mul(output.position, projMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	return output;
}