//////////////////////////
// Input Structures
/////////////////////////

cbuffer PerFrameBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjMatrix;
	matrix ViewProjMatrix;
};

//////////////////////////
// Input Structures
/////////////////////////

struct VertexInputType
{
	float4 position : POSITION;
	float2 uv : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};


//////////////////////////
// Vertex Shader
/////////////////////////

PixelInputType FontVertexShader(VertexInputType input)
{
	PixelInputType output;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, WorldMatrix);
	output.position = mul(output.position, ViewProjMatrix);
	//output.position = mul(output.position, ViewMatrix);
	//output.position = mul(output.position, ProjMatrix);

	// Store the texture coordinates for the pixel shader.
	output.uv = input.uv;

	return output;
}
