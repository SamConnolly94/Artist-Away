// Globals
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projMatrix;
	matrix ViewProjMatrix;
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
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
	float3 viewDirection : TEXCOORD1;
};

// Vertex shader.
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;

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

	// Calculate the position of the vertex in the world.
	worldPosition = mul(input.position, worldMatrix);
	
	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	// Normalise the viewing direction vector.
	output.viewDirection = normalize(output.viewDirection);

	return output;
}