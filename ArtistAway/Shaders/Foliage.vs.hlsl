///////////////////////////
// Buffers
///////////////////////////

cbuffer MatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
	matrix ViewProjMatrix;
};

cbuffer FoliageBuffer : register(b1)
{
	float3 WindDirection;
	float FrameTime;
	float3 FoliageTranslation;
	float WindStrength;
};

///////////////////////////
// Input Structures
///////////////////////////

struct VertexInputType
{
	float4 WorldPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL;
	uint IsTopVertex : TEXCOORD1;
	uint Type : TEXCOORD2;
	uint VertexIndex : TEXCOORD3;
	float3 instanceTileCentrePos : TEXCOORD4;
	float3 instanceTileLLVertexPos : TEXCOORD5;
	float3 instanceTileLRVertexPos : TEXCOORD6;
	float3 instanceTileULVertexPos : TEXCOORD7;
	float3 instanceTileURVertexPos : TEXCOORD8;
};

struct PixelInputType
{
	float4 ScreenPosition : SV_POSITION;
	float4 WorldPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL;
	uint Type : TEXCOORD1;
};

///////////////////////////
// Vertex shader
///////////////////////////
#define GrassType 0

float3 GetPosition(VertexInputType input)
{
	float3 offset = float3(0.0f, 0.0f, 0.0f);
	float3 leftPos = float3(0.0f, 0.0f, 0.0f);
	float3 rightPos = float3(0.0f, 0.0f, 0.0f);

	switch (input.VertexIndex)
	{
	// Lower left vertex on centre quad.
	case 0:
		leftPos = (input.instanceTileULVertexPos + input.instanceTileLLVertexPos) / 2;
		offset.y = leftPos.y;
		break;

	// Lower right vertex on centre quad.
	case 1:
		rightPos = (input.instanceTileURVertexPos + input.instanceTileLRVertexPos) / 2;
		offset.y = rightPos.y;
		break;

	// Upper left vertex on centre quad.
	case 2:
		leftPos = (input.instanceTileULVertexPos + input.instanceTileLLVertexPos) / 2;
		offset.y = leftPos.y;
		break;

	// Upper right vertex on centre quad.
	case 3:
		rightPos = (input.instanceTileURVertexPos + input.instanceTileLRVertexPos) / 2;
		offset.y = rightPos.y;
		break;

	// Lower left vertex on diag down and to the right quad.
	case 4:
		offset.y = input.instanceTileULVertexPos.y;
		break;

	// Lower right vertex on diag down and to the right quad.
	case 5:
		offset.y = input.instanceTileLRVertexPos.y;
		break;

	// Upper left vertex on diag down and to the right quad.
	case 6:
		offset.y = input.instanceTileULVertexPos.y;
		break;

	// Upper right vertex on diag down and to the right quad.
	case 7:
		offset.y = input.instanceTileLRVertexPos.y;
		break;

	// Lower left vertex on diag up and to the right quad.
	case 8:
		offset.y = input.instanceTileLLVertexPos.y;
		break;

	// Lower right vertex on diag up and to the right.
	case 9:
		offset.y = input.instanceTileURVertexPos.y;
		break;

	// Upper left vertex on diag up and to the right.
	case 10:
		offset.y = input.instanceTileLLVertexPos.y;
		break;

	case 11:
		offset.y = input.instanceTileURVertexPos.y;
		break;
	}

	offset.y -= input.instanceTileLLVertexPos.y;

	return offset;
}

PixelInputType FoliageVS(VertexInputType input)
{
	PixelInputType output;

	input.WorldPosition.xyz += input.instanceTileLLVertexPos.xyz;

	float offset = GetPosition(input).y;

	if (offset < 0.0f)
	{
		input.WorldPosition.y += offset;
	}
	else
	{
		input.WorldPosition.y -= offset;
	}

	// Give a 4th element to our matrix so it's the correct size;
	input.WorldPosition.w = 1.0f;
	
	if (input.IsTopVertex == 1 && input.Type == GrassType)
	{
		input.WorldPosition.xyz += FoliageTranslation;
	}

	output.WorldPosition = input.WorldPosition;

	// Calculate the position of the vertex against the world, view and projection matrices.
	output.ScreenPosition = mul(input.WorldPosition, WorldMatrix);
	output.ScreenPosition = mul(output.ScreenPosition, ViewProjMatrix);

	// Store the texture coordinates for the pixel shader.
	output.UV = input.UV;

	// Calculate the normal vector against the world matrix only.
	output.Normal = mul(input.Normal, (float3x3)WorldMatrix);

	// Normalise the vector.
	output.Normal = normalize(output.Normal);

	output.Type = input.Type;

	return output;
}

