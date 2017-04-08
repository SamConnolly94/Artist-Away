//////////////////////////
// Sample states
//////////////////////////

SamplerState TrilinearWrap : register(s0);
SamplerState BilinearMirror : register(s1);

//////////////////////////
// Textures
//////////////////////////

Texture2D NormalHeightMap : register(t0);
Texture2D RefractionMap : register(t1);
Texture2D ReflectionMap : register(t2);

//////////////////////////
// Constant buffers
//////////////////////////

cbuffer MatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
	matrix ViewProjMatrix;
}

cbuffer WaterBuffer : register(b1)
{
	float2 WaterMovement;
	float WaveHeight;
	float WaveScale;
	float RefractionDistortion;
	float ReflectionDistortion;
	float MaxDistortionDistance;
	float RefractionStrength;
	float ReflectionStrength;
	float3 waterBufferPadding;
}

cbuffer CameraBuffer : register(b2)
{
	matrix	CameraMatrix;
	float3	CameraPosition;
	float	cameraPadding;
}

cbuffer ViewportBuffer : register(b3)
{
	float2 ViewportSize;
}

cbuffer LightBuffer : register(b4)
{
	float4	AmbientColour;
	float4	DiffuseColour;
	float3	LightDirection;
	float4	SpecularColour;
	float3	LightPosition;
	float	SpecularPower;
	float	lightBufferPadding;
}

//////////////////////////
// Structures
//////////////////////////

struct PixelInputType
{
	float4 ProjectedPosition : SV_POSITION;
	float2 UV : TEXCOORD0;
	float4 WorldPosition : POSITION;
};

//////////////////////////
// Pixel shader
//////////////////////////

static const float WaterSize1  = 0.5f;
static const float WaterSize2 = 1.0f;
static const float WaterSize3 = 2.0f;
static const float WaterSize4 = 4.0f;

// Each wave layer moves at different speeds so it animates in a varying manner
static const float WaterSpeed1 = 0.5f;
static const float WaterSpeed2 = 1.0f;
static const float WaterSpeed3 = 1.7f;
static const float WaterSpeed4 = 2.6f;

float4 WaterSurfacePS(PixelInputType input) : SV_TARGET
{
	float2 waterUV = input.UV;
	//waterUV /= 8;
	float3 normal1 = NormalHeightMap.Sample(TrilinearWrap, WaterSize1 * (waterUV + WaterMovement * WaterSpeed1)).rgb * 2.0f - 1.0f;
	float3 normal2 = NormalHeightMap.Sample(TrilinearWrap, WaterSize2 * (waterUV + WaterMovement * WaterSpeed2)).rgb * 2.0f - 1.0f;
	float3 normal3 = NormalHeightMap.Sample(TrilinearWrap, WaterSize3 * (waterUV + WaterMovement * WaterSpeed3)).rgb * 2.0f - 1.0f;
	float3 normal4 = NormalHeightMap.Sample(TrilinearWrap, WaterSize4 * (waterUV + WaterMovement * WaterSpeed4)).rgb * 2.0f - 1.0f;

	normal1.y *= WaterSize1;
	normal2.y *= WaterSize2;
	normal3.y *= WaterSize3;
	normal4.y *= WaterSize4;

	normal1 = normalize(normal1);
	normal2 = normalize(normal2);
	normal3 = normalize(normal3);
	normal4 = normalize(normal4);

	float3 waterNormal = (normal1 + normal2 + normal3 + normal4) / 4.0f;

	waterNormal.yz = waterNormal.zy;

	waterNormal.y /= (WaveScale + 0.001f);
	waterNormal = normalize(waterNormal);

	float2 waterNormal2D = waterNormal.xz;
	float2 offsetDir = waterNormal2D * 0.3f;

	float2 screenUV = input.ProjectedPosition.xy / ViewportSize;
	float4 refractionDepth = RefractionMap.Sample(BilinearMirror, screenUV).a;
	float4 reflectionHeight = ReflectionMap.Sample(BilinearMirror, screenUV).a;

	float2 refractionUV = screenUV + RefractionDistortion * refractionDepth * offsetDir / input.ProjectedPosition.w;
	float2 reflectionUV = screenUV + ReflectionDistortion * reflectionHeight * offsetDir / input.ProjectedPosition.w;
	float4 refractColour = RefractionMap.Sample(BilinearMirror, refractionUV) * RefractionStrength;
	float4 reflectColour = ReflectionMap.Sample(BilinearMirror, reflectionUV) * ReflectionStrength;

	reflectColour = lerp(refractColour, reflectColour, saturate(refractionDepth * MaxDistortionDistance / (0.5f * WaveHeight * WaveScale)));

	// Water Lighting
	float3 normalToCamera = normalize(CameraPosition - input.WorldPosition);

	//// LIGHT 1
	//float3 light1Dir = normalize(LightPosition - input.WorldPosition.xyz);
	//float3 light1Dist = length(LightPosition - input.WorldPosition.xyz);
	//float3 diffuseLight1 = LightColour * max(dot(waterNormal.xyz, light1Dir), 0) / light1Dist;
	//float3 halfway = normalize(light1Dir + normalToCamera);
	//float3 specularLight1 = diffuseLight1 * pow(max(dot(waterNormal.xyz, halfway), 0), SpecularPower);

	float3 halfWayVector = normalize(normalize(-LightDirection) + normalToCamera);
	float3 specLight = AmbientColour * pow(max(dot(waterNormal, halfWayVector), 0), SpecularPower);
	
	reflectColour.rgb += specLight * 0.5f;

	float n1 = 1.0; // Refractive index of air
	float n2 = 1.5f;
	float r0 = ((n1 - n2) / (n1 + n2));
	r0 *= r0;
	float exp = pow(1 - saturate(dot(waterNormal, normalToCamera)), 5);
	float fresnel = saturate(lerp(r0, 1, exp));
	fresnel = 0.25f;
	return lerp(refractColour, reflectColour, fresnel);

}