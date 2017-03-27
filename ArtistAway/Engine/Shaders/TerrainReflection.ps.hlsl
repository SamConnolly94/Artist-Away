//////////////////////////
// Sample states
//////////////////////////

SamplerState TrilinearWrap : register(s0);
SamplerState BilinearMirror : register(s1);

//////////////////////////
// Textures
//////////////////////////

Texture2D WaterHeightMap : register(t0);
Texture2D shaderTexture[2] : register(t1);
Texture2D grassTextures[2] : register(t3);
Texture2D patchMap : register(t5);
Texture2D rockTextures[2] : register(t6);


//////////////////////////
// Constant buffers
//////////////////////////

cbuffer ViewportBuffer : register(b0)
{
	float2 ViewportSize;
}

cbuffer LightBuffer : register(b1)
{
	float4	AmbientColour;
	float4	DiffuseColour;
	float3	LightDirection;
	float4	SpecularColour;
	float3	LightPosition;
	float	SpecularPower;
	float	lightBufferPadding;
}

cbuffer TerrainAreaBuffer : register(b2)
{
	float snowHeight;
	float grassHeight;
	float dirtHeight;
	float sandHeight;
}

cbuffer PositioningBuffer : register(b3)
{
	float yOffset;
	float WaterPlaneY;
	float2 posPadding;
	float4 posPadding2;
}

//////////////////////////
// Structures
//////////////////////////

struct PixelInputType
{
	float4 ProjectedPosition : SV_POSITION;
	float4 WorldPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL;
};

///////////////////////
// Helper Functions
///////////////////////

float4 GetTriplanarTextureColour(int texIndex, float3 blending, float4 worldPosition, float scale)
{

	scale = 0.1f;
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	float4 xAxis = shaderTexture[texIndex].Sample(TrilinearWrap, worldPosition.yz * scale);
	float4 yAxis = shaderTexture[texIndex].Sample(TrilinearWrap, worldPosition.xz * scale);
	float4 zAxis = shaderTexture[texIndex].Sample(TrilinearWrap, worldPosition.xy * scale);

	float4 result;
	result = xAxis * blending.x + yAxis * blending.y + zAxis * blending.z;
	return result;
};

float4 GetTriplanarGrassTextureColour(int texIndex, float3 blending, float4 worldPosition, float scale)
{
	scale = 0.1f;
	float4 xAxis = grassTextures[texIndex].Sample(TrilinearWrap, worldPosition.yz * scale);
	float4 yAxis = grassTextures[texIndex].Sample(TrilinearWrap, worldPosition.xz * scale);
	float4 zAxis = grassTextures[texIndex].Sample(TrilinearWrap, worldPosition.xy * scale);

	float4 result;
	result = xAxis * blending.x + yAxis * blending.y + zAxis * blending.z;
	return result;

}


float4 GetPatchGrassColour(PixelInputType input, float3 blending)
{
	float4 grass1 = GetTriplanarGrassTextureColour(0, blending, input.WorldPosition, 1.0f);
	float4 grass2 = GetTriplanarGrassTextureColour(1, blending, input.WorldPosition, 1.0f);

	float4 patchColour = patchMap.Sample(TrilinearWrap, input.UV / 32.0f);

	float4 textureColour = lerp(grass2, grass1, patchColour.r);

	return textureColour;
}

float4 GetTriplanarRockTextureColour(int texIndex, float3 blending, float4 worldPosition, float scale)
{
	scale = 0.1f;
	float4 xAxis = rockTextures[texIndex].Sample(TrilinearWrap, worldPosition.yz * scale);
	float4 yAxis = rockTextures[texIndex].Sample(TrilinearWrap, worldPosition.xz * scale);
	float4 zAxis = rockTextures[texIndex].Sample(TrilinearWrap, worldPosition.xy * scale);

	float4 result;
	result = xAxis * blending.x + yAxis * blending.y + zAxis * blending.z;
	return result;

}

float4 GetCombinedRockLerp(PixelInputType input, float3 blending)
{
	float4 rock1 = GetTriplanarRockTextureColour(0, blending, input.WorldPosition, 1.0f);
	float4 rock2 = GetTriplanarRockTextureColour(1, blending, input.WorldPosition, 1.0f);

	float4 patchColour = patchMap.Sample(TrilinearWrap, input.UV / 32.0f);

	float4 textureColour = lerp(rock2, rock1, patchColour.r);

	return textureColour;
}

//////////////////////////
// Terrain pixel shader
/////////////////////////

/* This is the main body of our pixel shader and our entry point.
*  The purpose of this is to shade the pictures using a different texture depending on the height of the current vertex.
*  So far we have implemented Snow, Grass, Dirt, Sand, Rocks and at the bottom below the rocks there are some pools of water. */
float4 TerrainPixel(PixelInputType input) : SV_TARGET
{
	float4 textureColour;
float3 lightDir;
float lightIntensity;
float4 colour;

// Get normals on different planes
float3 blending = abs(input.Normal);
// Make sure the blending weight is of length 1.
blending = normalize(max(blending, 0.00001));
float blendLen = (blending.x + blending.y + blending.z);
blending /= (blendLen, blendLen, blendLen);

// Find the world position by moving the vertex by whatever our current Y position of the terrain is.
// This only needs to be done as we support movement of terrain in Prio Engine.
float worldPos = input.WorldPosition.y + yOffset;

float4 patchColour = patchMap.Sample(TrilinearWrap, input.UV);

/////////// SNOW //////////////////
if (worldPos > snowHeight)
{
	//textureColour = GetTriplanarTextureColour(1, blending, input.WorldPosition, 1.0f);
	textureColour = GetCombinedRockLerp(input, blending);
}
///////////// SNOW BLENDED WITH GRASS /////////////
else if (worldPos > snowHeight - 5.0f)
{
	//float4 snowTex = GetTriplanarTextureColour(1, blending, input.WorldPosition, 1.0f);
	float4 rockTex = GetCombinedRockLerp(input, blending);
	float4 grassTex = GetPatchGrassColour(input, blending);
	float heightDiff = snowHeight - worldPos;
	float blendFactor = heightDiff / 5.0f;
	textureColour = lerp(rockTex, grassTex, blendFactor);
}
////////////// GRASS ///////////////////
else if (worldPos > grassHeight)
{
	textureColour = GetPatchGrassColour(input, blending);
}
////////////////// GRASS BLENDED WITH DIRT ////////////////
else if (worldPos > grassHeight - 2.0f)
{
	float4 grassTex = GetPatchGrassColour(input, blending);
	float4 dirtTex = GetTriplanarTextureColour(0, blending, input.WorldPosition, 1.0f);
	float heightDiff = grassHeight - worldPos;
	float blendFactor = heightDiff / 2.0f;
	textureColour = lerp(grassTex, dirtTex, blendFactor);
}
/////////////////////// DIRT //////////////////////
else if (worldPos > dirtHeight)
{
	textureColour = GetTriplanarTextureColour(0, blending, input.WorldPosition, 1.0f);
}
////////////////////// DIRT BLENDED WITH SAND ///////////////
else if (worldPos > dirtHeight - 2.0f)
{
	float4 dirtTex = GetTriplanarTextureColour(0, blending, input.WorldPosition, 1.0f);
	float4 sandTex = GetTriplanarTextureColour(1, blending, input.WorldPosition, 1.0f);
	float heightDiff = dirtHeight - worldPos;
	float blendFactor = heightDiff / 2.0f;
	textureColour = lerp(dirtTex, sandTex, blendFactor);
}
//////////////////// SAND //////////////////
else if (worldPos > sandHeight)
{
	textureColour = GetTriplanarTextureColour(1, blending, input.WorldPosition, 1.0f);
}
// Below all height, assume sand.
else
{
	textureColour = GetTriplanarTextureColour(1, blending, input.WorldPosition, 1.0f);
}

// Set the colour to the ambient colour.
colour = AmbientColour;

// Invert the light direction for calculations.
lightDir = -LightDirection;

// Calculate the amount of light on this pixel.
lightIntensity = saturate(dot(input.Normal, lightDir));

if (lightIntensity > 0.0f)
{
	colour += (DiffuseColour * lightIntensity);
}

// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
colour = saturate(colour);

// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
colour = colour * textureColour;

// Return the colour of the current pixel.
return colour;
}


//////////////////////////
// Pixel shader
//////////////////////////

float4 TerrainReflectionPS(PixelInputType input) : SV_TARGET
{
	float2 screenUV = input.ProjectedPosition.xy / ViewportSize;
	float waterHeight = WaterHeightMap.Sample(BilinearMirror, screenUV);
	float objectHeight = input.WorldPosition.y - (WaterPlaneY - waterHeight);

	clip(objectHeight);
	return float4(TerrainPixel(input).rgb, saturate(objectHeight / 40.0f));
}