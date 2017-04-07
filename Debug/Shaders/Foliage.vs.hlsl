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
	float3 pos = float3(0.0f, 0.0f, 0.0f);

	//switch (input.VertexIndex)
	//{
	//	// Lower left vertex on centre quad.
	//case 0:
	//	pos.y = input.instanceTileULVertexPos.y - input.instanceTileLLVertexPos.y;
	//	break;
	//	// Lower right vertex on centre quad.
	//case 1:
	//	pos.y = input.instanceTileURVertexPos.y - input.instanceTileLRVertexPos.y;
	//	break;
	//	// Upper left vertex on centre quad.
	//case 2:
	//	pos.y = input.instanceTileULVertexPos.y - input.instanceTileLLVertexPos.y;
	//	break;
	//	// Upper right vertex on centre quad.
	//case 3:
	//	pos.y = input.instanceTileURVertexPos.y - input.instanceTileLRVertexPos.y;
	//	break;
	//	// Lower left vertex on diag down and to the right quad.
	//case 4:
	//	if (input.instanceTileULVertexPos.y > input.instanceTileLLVertexPos.y)
	//	{
	//		pos.y = input.instanceTileULVertexPos.y - input.instanceTileLLVertexPos.y;
	//	}
	//	else
	//	{
	//		pos.y = input.instanceTileLLVertexPos.y - input.instanceTileULVertexPos.y;
	//	}
	//	break;
	//	// Lower right vertex on diag down and to the right quad.
	//case 5:
	//	if (input.instanceTileLLVertexPos.y > input.instanceTileLLVertexPos.y)
	//	{
	//		pos.y = input.instanceTileLRVertexPos.y - input.instanceTileLLVertexPos.y;
	//	}
	//	else
	//	{
	//		pos.y = input.instanceTileLLVertexPos.y - input.instanceTileLRVertexPos.y;
	//	}
	//	break;
	//	// Upper left vertex on diag down and to the right quad.
	//case 6:
	//	if (input.instanceTileULVertexPos.y > input.instanceTileLLVertexPos.y)
	//	{
	//		pos.y = input.instanceTileULVertexPos.y - input.instanceTileLLVertexPos.y;
	//	}
	//	else
	//	{
	//		pos.y = input.instanceTileLLVertexPos.y - input.instanceTileULVertexPos.y;
	//	}
	//	break;
	//	// Upper right vertex on diag down and to the right quad.
	//case 7:
	//	if (input.instanceTileLRVertexPos.y > input.instanceTileLLVertexPos.y)
	//	{
	//		pos.y = input.instanceTileLRVertexPos.y - input.instanceTileLLVertexPos.y;
	//	}
	//	else
	//	{
	//		pos.y = input.instanceTileLLVertexPos.y - input.instanceTileLRVertexPos.y;
	//	}
	//	break;
	//	// Lower left vertex on diag up and to the right quad.
	//case 8:
	//	pos.y = 0.0f;
	//	break;
	//	// Lower right vertex on diag up and to the right.
	//case 9:
	//	if (input.instanceTileURVertexPos.y > input.instanceTileLLVertexPos.y)
	//	{
	//		pos.y = input.instanceTileURVertexPos.y - input.instanceTileLLVertexPos.y;
	//	}
	//	else
	//	{
	//		pos.y = input.instanceTileLLVertexPos.y - input.instanceTileURVertexPos.y;
	//	}
	//	break;
	//case 10:
	//	pos.y = 0.0f;
	//	break;
	//case 11:
	//	if (input.instanceTileURVertexPos.y > input.instanceTileLLVertexPos.y)
	//	{
	//		pos.y = input.instanceTileURVertexPos.y - input.instanceTileLLVertexPos.y;
	//	}
	//	else
	//	{
	//		pos.y = input.instanceTileLLVertexPos.y - input.instanceTileURVertexPos.y;
	//	}
	//	break;
	//}

	//pos += input.instanceTileLLVertexPos;

	return input.instanceTileLLVertexPos + pos;
}

PixelInputType FoliageVS(VertexInputType input)
{
	PixelInputType output;

	input.WorldPosition.xyz += GetPosition(input);
	//input.WorldPosition.x += input.instanceTileCentrePos.x;
	//input.WorldPosition.y += input.instanceTileCentrePos.y;
	//input.WorldPosition.z += input.instanceTileCentrePos.z;

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