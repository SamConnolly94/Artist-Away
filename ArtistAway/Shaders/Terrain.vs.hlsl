//
// VERTEX SHADER FILE.
//

// Globals

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// Typedefs

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 screenPosition : SV_POSITION;
	float4 worldPosition : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// Vertex shader
PixelInputType TerrainVertex(VertexInputType input)
{	
	PixelInputType output;

	// Give a 4th element to our matrix so it's the correct size;
	input.position.w = 1.0f;
	
	output.worldPosition = input.position;

	// Calculate the position of the vertex against the world, view and projection matrices.
	output.screenPosition = mul(input.position, worldMatrix);
	output.screenPosition = mul(output.screenPosition, viewMatrix);
	output.screenPosition = mul(output.screenPosition, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalise the vector.
	output.normal = normalize(output.normal);

	return output;
}