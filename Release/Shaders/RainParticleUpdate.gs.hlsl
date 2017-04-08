//////////////////////////
// Compiler definitions
/////////////////////////

#define PT_EMITTER 0
#define PT_FLARE 1

//////////////////////////
// Sampler states
/////////////////////////

SamplerState basicSample : register(s0);

//////////////////////////
// Textures and Resources
/////////////////////////

Texture1D randomTexture : register(t0);

//////////////////////////
// Constant Buffer
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

struct GeometryInputType
{
	float3  InitialPosition	: POSITION;
	float3  InitialVelocity	: VELOCITY;
	float2	Size			: SIZE;
	float   Age				: AGE;
	uint    Type			: TYPE;
};

//////////////////////////
// Helper Functions
/////////////////////////

float3 RandVec3(float offset)
{
	float u = (GameTime + offset);
	float3 v = randomTexture.SampleLevel(basicSample, u, 0).xyz;
	return v;
}

//////////////////////////
// Geometry shader
/////////////////////////

[maxvertexcount(6)]
void RainUpdateGS(point GeometryInputType input[1], inout PointStream<GeometryInputType> pOutStream)
{
	input[0].Age += FrameTime;

	if (input[0].Type == PT_EMITTER)
	{
		if (input[0].Age > 0.001f)
		{
			for (int i = 0; i < 3; ++i)
			{
				float3 vRandom = 200.0f * RandVec3((float)i / 5.0f);
				vRandom.y = 40.0f;

				GeometryInputType p;
				p.InitialPosition = EmitPos.xyz + vRandom;
				p.InitialVelocity = float3(0.0f, 0.0f, 0.0f);
				p.Size = float2(1.0f, 1.0f);
				p.Age = 0.0f;
				p.Type = PT_FLARE;

				pOutStream.Append(p);
			}

			input[0].Age = 0.0f;
		}

		pOutStream.Append(input[0]);
	}
	else
	{
		if (input[0].Age <= 4.0f)
		{
			pOutStream.Append(input[0]);
		}
		else
		{
			float3 vRandom = 200.0f * RandVec3((float)5.0f);
			vRandom.y = 40.0f;
			input[0].InitialPosition = EmitPos.xyz + vRandom;
			input[0].Age = 0.0f;
			pOutStream.Append(input[0]);
		}

	}
}