//////////////////////////
// Constant Buffers
/////////////////////////

cbuffer MatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjMatrix;
	matrix ViewProjMatrix;
}

cbuffer FrameBuffer : register(b1)
{
	float3 CameraPos;
	float GameTime;
	float3 EmitPos;
	float FrameTime;
	float Gravity;
	float WindX;
	float WindZ;
	float frameBufferPadding;
}

//////////////////////////
// Input Structures
/////////////////////////

struct VertexInputType
{
	float3 initialPosition : POSITION;
	float3 initialVelocity : VELOCITY;
	float2 Scale : SIZE;
	float Age : AGE;
	unsigned int Type : TYPE;
};

struct GeometryInputType
{
	float3 position : POSITION;
	unsigned int Type : TYPE;
};

//////////////////////////
// Geometry shader
/////////////////////////

GeometryInputType RainVS(VertexInputType input)
{
	GeometryInputType output;
	float3 Acceleration = float3(WindX, Gravity, WindZ);
	
	output.position = 0.5f * input.Age * input.Age * Acceleration + input.Age + input.initialVelocity + input.initialPosition;
	output.Type = input.Type;
	return output;
}