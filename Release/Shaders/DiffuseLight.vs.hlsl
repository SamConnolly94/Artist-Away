// Globals
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projMatrix;
	matrix ViewProjMatrix;
};

// Type defs.
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// Vertex shader.
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;

	// Change the position vector to have a 4th element to allow for maths calcs.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against world, view and proj matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, ViewProjMatrix);
	//output.position = mul(output.position, viewMatrix);
	//output.position = mul(output.position, projMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalise the vector.
	output.normal = normalize(output.normal);

	return output;
}