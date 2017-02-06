///////////////////////////
// Terrain pixel shader.
// Author: Sam Connolly
// Last update: 25/01/2017
///////////////////////////

//////////////////////////
// 1) Dirt
// 2) Snow
// 3) Yellow Grass
// 4) Sand
// 5) Rock
/////////////////////////
Texture2D shaderTexture[5];
SamplerState SampleType;

///////////////////////////
// Buffers
///////////////////////////

/* The buffer which contains details about our lighting information. */
cbuffer LightBuffer : register(b0)
{
	float4 ambientColour;
	float4 diffuseColour;
	float3 lightDirection;
	float padding;
};

/* The buffer which contains information about our terrain. */
cbuffer TerrainInfoBuffer : register(b1)
{
	float highestPosition;
	float lowestPosition;
	float2 terrBuffPadding;
};

/* Information about the model of the terrain itself. */
// Important note - separate from terrainInfoBuffer due to the way in which padding works.
cbuffer PositioningBuffer : register(b2)
{
	float yOffset;
	float3 posPadding;
}

//////////////////////
// Typedefs
/////////////////////

/* The pixel input type is the information that will be passed in from the vertex shader.
*  This will need to match the structure in the vertex shader. */
struct PixelInputType
{
	float4 screenPosition : SV_POSITION;
	float4 worldPosition : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

///////////////////////
// Helper Functions
///////////////////////

/* Get the percentage of a number. 
*  Param number - The number which you want to find the percentage of.
*  Param desiredPercentage - The percentage of the number which you wish to find. */
float GetPercentage(float number, float desiredPercentage)
{
	float onePerc = number / 100.0f;

	return (onePerc * desiredPercentage);
};

float4 GetTriplanarTextureColour(int texIndex, float3 blending, float4 worldPosition, float scale)
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	float4 xAxis = shaderTexture[texIndex].Sample(SampleType, worldPosition.yz * scale);
	float4 yAxis = shaderTexture[texIndex].Sample(SampleType, worldPosition.xz * scale);
	float4 zAxis = shaderTexture[texIndex].Sample(SampleType, worldPosition.xy * scale);

	float4 xAxisTimes4 = shaderTexture[texIndex].Sample(SampleType, worldPosition.yz * (scale *4));
	float4 yAxisTimes4 = shaderTexture[texIndex].Sample(SampleType, worldPosition.xz * (scale * 4));
	float4 zAxisTimes4 = shaderTexture[texIndex].Sample(SampleType, worldPosition.xy * (scale * 4));

	float4 xAxisTimes16 = shaderTexture[texIndex].Sample(SampleType, worldPosition.yz * (scale * 16));
	float4 yAxisTimes16 = shaderTexture[texIndex].Sample(SampleType, worldPosition.xz * (scale * 16));
	float4 zAxisTimes16 = shaderTexture[texIndex].Sample(SampleType, worldPosition.xy * (scale * 16));

	float4 result = xAxis * blending.x + yAxis * blending.y + zAxis * blending.z;
	result += xAxisTimes4 * blending.x + yAxisTimes4 * blending.y + zAxisTimes4 * blending.z;
	result += xAxisTimes16 * blending.x + yAxisTimes16 * blending.y + zAxisTimes16 * blending.z;
	result /= 3;

	return result;
};

///////////////////////////
// Pixel Shading Functions
///////////////////////////

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
	float3 blending = abs(input.normal);
	// Make sure the blending weight is of length 1.
	blending = normalize(max(blending, 0.00001));
	float blendLen = (blending.x + blending.y + blending.z);
	blending /= (blendLen, blendLen, blendLen);

	// Find the world position by moving the vertex by whatever our current Y position of the terrain is.
	// This only needs to be done as we support movement of terrain in Prio Engine.
	float worldPos = input.worldPosition.y + yOffset;

	const float changeInHeight = highestPosition - lowestPosition;

	// Define the position 
	const float snowHeight = lowestPosition + GetPercentage(changeInHeight, 60);	// 60% and upwards will be snow.
	const float grassHeight = lowestPosition + GetPercentage(changeInHeight, 30);	// 30% and upwards will be grass.
	const float dirtHeight = lowestPosition + GetPercentage(changeInHeight, 15);	// 15% and upwards will be dirt.
	const float sandHeight = lowestPosition + GetPercentage(changeInHeight, 10);	// 10% and upwards will be sand.
	const float rockHeight = lowestPosition + GetPercentage(changeInHeight, 4);		// 4%  and upwards will be rocks.

	const float rockBlendRange = GetPercentage(changeInHeight, 3);	// 2% of the terrain is given to blending the rocks with anything below rocks (water) **** CURRENTLY NOT IN USE *****.
	const float sandBlendRange = GetPercentage(changeInHeight, 3);	// 3% of the terrain is given to blending the sand with rocks.
	const float dirtBlendRange = GetPercentage(changeInHeight, 3);	// 3% of the terrain is given to blending the dirt with sand.
	const float grassBlendRange = GetPercentage(changeInHeight, 3);	// 3% of the terrain is given to blending the grass with dirt.
	const float snowBlendRange = GetPercentage(changeInHeight, 10);	// 10% of the terrain is given to blending the snow with grass.

	/////////// SNOW //////////////////
	if (worldPos > snowHeight)
	{
		textureColour = GetTriplanarTextureColour(1, blending, input.worldPosition, 1.0f);
	}
	///////////// SNOW BLENDED WITH GRASS /////////////
	else if (worldPos > snowHeight - snowBlendRange)
	{
		float4 snowTex = GetTriplanarTextureColour(1, blending, input.worldPosition, 1.0f);
		float4 grassTex = GetTriplanarTextureColour(2, blending, input.worldPosition, 1.0f);
		float heightDiff = snowHeight - worldPos;
		float blendFactor = heightDiff / snowBlendRange;
		textureColour = lerp(snowTex, grassTex, blendFactor);
	}
	////////////// GRASS ///////////////////
	else if (worldPos > grassHeight)
	{
		textureColour = GetTriplanarTextureColour(2, blending, input.worldPosition, 1.0f);
	}
	////////////////// GRASS BLENDED WITH DIRT ////////////////
	else if (worldPos > grassHeight - grassBlendRange)
	{
		float4 grassTex = GetTriplanarTextureColour(2, blending, input.worldPosition, 1.0f);
		float4 dirtTex = GetTriplanarTextureColour(0, blending, input.worldPosition, 1.0f);
		float heightDiff = grassHeight - worldPos;
		float blendFactor = heightDiff / grassBlendRange;
		textureColour = lerp(grassTex, dirtTex, blendFactor);
	}
	/////////////////////// DIRT //////////////////////
	else if (worldPos > dirtHeight)
	{
		textureColour = GetTriplanarTextureColour(0, blending, input.worldPosition, 1.0f);
	}
	////////////////////// DIRT BLENDED WITH SAND ///////////////
	else if (worldPos > dirtHeight - dirtBlendRange)
	{
		float4 dirtTex = GetTriplanarTextureColour(0, blending, input.worldPosition, 1.0f);
		float4 sandTex = GetTriplanarTextureColour(3, blending, input.worldPosition, 1.0f);
		float heightDiff = dirtHeight - worldPos;
		float blendFactor = heightDiff / dirtBlendRange;
		textureColour = lerp(dirtTex, sandTex, blendFactor);
	}
	//////////////////// SAND //////////////////
	else if (worldPos > sandHeight)
	{
		textureColour = GetTriplanarTextureColour(3, blending, input.worldPosition, 1.0f);
	}
	////////////////////// SAND BLENDED WITH ROCKS ///////////////////
	else if (worldPos > sandHeight - sandBlendRange)
	{
		float4 sandTex = GetTriplanarTextureColour(3, blending, input.worldPosition, 1.0f);
		float4 rockTex = GetTriplanarTextureColour(4, blending, input.worldPosition, 1.0f);
		float heightDiff = sandHeight - worldPos;
		float blendFactor = heightDiff / sandBlendRange;
		textureColour = lerp(sandTex, rockTex, blendFactor);
	}
	//////////////////// ROCKS ///////////////////////////
	else if (worldPos > rockHeight)
	{
		textureColour = GetTriplanarTextureColour(4, blending, input.worldPosition, 1.0f);
	}
	////////////////// BELOW THE ROCKS TILE, SET TO WATER. /////////////////////
	else
	{
		// Default pixel to blue to show any obvious errors (if its lower, blue is okay because it can represent water.
		textureColour.r = 0.0f;
		textureColour.g = 0.0f;
		textureColour.b = 1.0f;
		textureColour.a = 1.0f;
	}
	
	// Set the colour to the ambient colour.
	colour = ambientColour;

	// Invert the light direction for calculations.
	lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		colour += (diffuseColour * lightIntensity);
	}

	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
	colour = saturate(colour);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	colour = colour * textureColour;

	// Return the colour of the current pixel.
	return colour;
}