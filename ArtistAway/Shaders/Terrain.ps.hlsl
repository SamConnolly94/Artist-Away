///////////////////////////
// Terrain pixel shader.
// Author: Sam Connolly
// Last update: 28/03/2017
///////////////////////////

//////////////////////////
// Sampler states
/////////////////////////
SamplerState SampleType;

//////////////////////////
// Textures
/////////////////////////
Texture2D shaderTexture[2];
Texture2D grassTextures[2];
Texture2D patchMap;
Texture2D rockTextures[2];

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

/* Information about the model of the terrain itself. */
// Important note - separate from terrainInfoBuffer due to the way in which padding works.
cbuffer PositioningBuffer : register(b1)
{
	float yOffset;
	float3 posPadding;
}

cbuffer TerrainAreaBuffer : register(b2)
{
	float snowHeight;
	float grassHeight;
	float dirtHeight;
	float sandHeight;
}

cbuffer SlopeBuffer : register (b3)
{
	float slopeGrassCutoff;
	float slopeRockCuttoff;
	float2 slopePadding;
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

float4 GetTriplanarTextureColour(int texIndex, float3 blending, float4 worldPosition, float scale)
{

	scale = 0.1f;
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	float4 xAxis = shaderTexture[texIndex].Sample(SampleType, worldPosition.yz * scale);
	float4 yAxis = shaderTexture[texIndex].Sample(SampleType, worldPosition.xz * scale);
	float4 zAxis = shaderTexture[texIndex].Sample(SampleType, worldPosition.xy * scale);

	float4 result;
	result = xAxis * blending.x + yAxis * blending.y + zAxis * blending.z;
	return result;
};

float4 GetTriplanarGrassTextureColour(int texIndex, float3 blending, float4 worldPosition, float scale)
{
	scale = 0.1f;
	float4 xAxis = grassTextures[texIndex].Sample(SampleType, worldPosition.yz * scale);
	float4 yAxis = grassTextures[texIndex].Sample(SampleType, worldPosition.xz * scale);
	float4 zAxis = grassTextures[texIndex].Sample(SampleType, worldPosition.xy * scale);

	float4 result;
	result = xAxis * blending.x + yAxis * blending.y + zAxis * blending.z;
	return result;

}


float4 GetPatchGrassColour(PixelInputType input, float3 blending)
{
	float4 grass1 = GetTriplanarGrassTextureColour(0, blending, input.worldPosition, 1.0f);
	float4 grass2 = GetTriplanarGrassTextureColour(1, blending, input.worldPosition, 1.0f);

	float4 patchColour = patchMap.Sample(SampleType, input.tex / 32.0f);

	float4 textureColour = lerp(grass2, grass1, patchColour.r);

	return textureColour;
}

float4 GetTriplanarRockTextureColour(int texIndex, float3 blending, float4 worldPosition, float scale)
{
	scale = 0.1f;
	float4 xAxis = rockTextures[texIndex].Sample(SampleType, worldPosition.yz * scale);
	float4 yAxis = rockTextures[texIndex].Sample(SampleType, worldPosition.xz * scale);
	float4 zAxis = rockTextures[texIndex].Sample(SampleType, worldPosition.xy * scale);

	float4 result;
	result = xAxis * blending.x + yAxis * blending.y + zAxis * blending.z;
	return result;

}

float4 GetCombinedRockLerp(PixelInputType input, float3 blending)
{
	float4 rock1 = GetTriplanarRockTextureColour(0, blending, input.worldPosition, 1.0f);
	float4 rock2 = GetTriplanarRockTextureColour(1, blending, input.worldPosition, 1.0f);

	float4 patchColour = patchMap.Sample(SampleType, input.tex / 32.0f);

	float4 textureColour = lerp(rock2, rock1, patchColour.r);

	return textureColour;
}


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

	float4 patchColour = patchMap.Sample(SampleType, input.tex);

	//float slopeGrassCutoff = 0.6f;
	//float slopeRockCuttoff = 0.8f;

	/////////// SNOW //////////////////
	if (worldPos > snowHeight)
	{
		//textureColour = GetTriplanarTextureColour(1, blending, input.worldPosition, 1.0f);
		textureColour = GetCombinedRockLerp(input, blending);
	}
	///////////// SNOW BLENDED WITH GRASS /////////////
	else if (worldPos > snowHeight - 5.0f)
	{
		//float4 snowTex = GetTriplanarTextureColour(1, blending, input.worldPosition, 1.0f);
		float4 rockTex = GetCombinedRockLerp(input, blending);
		float4 grassTex = GetPatchGrassColour(input, blending);
		float heightDiff = snowHeight - worldPos;
		float blendFactor = heightDiff / 5.0f;
		float4 grassColour = lerp(rockTex, grassTex, blendFactor);

		float4 slopeColour = GetCombinedRockLerp(input, blending); 
		float4 rockColour = GetCombinedRockLerp(input, blending);

		float slope = 1.0f - input.normal.y;

		if (slope < slopeGrassCutoff)
		{
			float blendFactor = slope / slopeGrassCutoff;
			textureColour = lerp(grassColour, slopeColour, blendFactor);
		}
		else if (slope < slopeRockCuttoff && slope >= slopeGrassCutoff)
		{
			float blendFactor = (slope - slopeGrassCutoff) * (1.0f / (slopeRockCuttoff - slopeGrassCutoff));
			textureColour = lerp(slopeColour, rockColour, blendFactor);
		}
		else
		{
			textureColour = rockColour;
		}
	}
	////////////// GRASS ///////////////////
	else if (worldPos > grassHeight)
	{
		float4 slopeColour = GetCombinedRockLerp(input, blending);
		float4 grassColour = GetPatchGrassColour(input, blending);
		float4 rockColour = GetCombinedRockLerp(input, blending);

		float slope = 1.0f - input.normal.y;

		if (slope < slopeGrassCutoff)
		{
			float blendFactor = slope / slopeGrassCutoff;
			textureColour = lerp(grassColour, slopeColour, blendFactor);
		}
		else if (slope < slopeRockCuttoff && slope >= slopeGrassCutoff)
		{
			float blendFactor = (slope - slopeGrassCutoff) * (1.0f / (slopeRockCuttoff - slopeGrassCutoff));
			textureColour = lerp(slopeColour, rockColour, blendFactor);
		}
		else
		{
			textureColour = rockColour;
		}
	}
	////////////////// GRASS BLENDED WITH DIRT ////////////////
	else if (worldPos > grassHeight - 2.0f)
	{
		float4 grassTex = GetPatchGrassColour(input, blending);
		float4 dirtTex = GetTriplanarTextureColour(0, blending, input.worldPosition, 1.0f);
		float heightDiff = grassHeight - worldPos;
		float grassBlendFactor = heightDiff / 2.0f;
		float4 grassColour = lerp(grassTex, dirtTex, grassBlendFactor);

		float4 slopeColour = GetCombinedRockLerp(input, blending);
		float4 rockColour = GetCombinedRockLerp(input, blending);

		float slope = 1.0f - input.normal.y;

		if (slope < slopeGrassCutoff)
		{
			float blendFactor = slope / slopeGrassCutoff;
			textureColour = lerp(grassColour, slopeColour, blendFactor);
		}
		else if (slope < slopeRockCuttoff && slope >= slopeGrassCutoff)
		{
			float blendFactor = (slope - slopeGrassCutoff) * (1.0f / (slopeRockCuttoff - slopeGrassCutoff));
			textureColour = lerp(slopeColour, rockColour, blendFactor);
		}
		else
		{
			textureColour = rockColour;
		}
	}
	/////////////////////// DIRT //////////////////////
	else if (worldPos > dirtHeight)
	{
		float4 grassColour = GetTriplanarTextureColour(0, blending, input.worldPosition, 1.0f);

		float4 slopeColour = GetCombinedRockLerp(input, blending);
		float4 rockColour = GetCombinedRockLerp(input, blending);

		float slope = 1.0f - input.normal.y;

		if (slope < slopeGrassCutoff)
		{
			float blendFactor = slope / slopeGrassCutoff;
			textureColour = lerp(grassColour, slopeColour, blendFactor);
		}
		else if (slope < slopeRockCuttoff && slope >= slopeGrassCutoff)
		{
			float blendFactor = (slope - slopeGrassCutoff) * (1.0f / (slopeRockCuttoff - slopeGrassCutoff));
			textureColour = lerp(slopeColour, rockColour, blendFactor);
		}
		else
		{
			textureColour = rockColour;
		}

	}
	////////////////////// DIRT BLENDED WITH SAND ///////////////
	else if (worldPos > dirtHeight - 2.0f)
	{
		float4 dirtTex = GetTriplanarTextureColour(0, blending, input.worldPosition, 1.0f);
		float4 sandTex = GetTriplanarTextureColour(1, blending, input.worldPosition, 1.0f);
		float heightDiff = dirtHeight - worldPos;
		float blendFactor = heightDiff / 2.0f;
		float4 grassColour = lerp(dirtTex, sandTex, blendFactor);

		float4 slopeColour = GetCombinedRockLerp(input, blending);
		float4 rockColour = GetCombinedRockLerp(input, blending);

		float slope = 1.0f - input.normal.y;

		if (slope < slopeGrassCutoff)
		{
			float blendFactor = slope / slopeGrassCutoff;
			textureColour = lerp(grassColour, slopeColour, blendFactor);
		}
		else if (slope < slopeRockCuttoff && slope >= slopeGrassCutoff)
		{
			float blendFactor = (slope - slopeGrassCutoff) * (1.0f / (slopeRockCuttoff - slopeGrassCutoff));
			textureColour = lerp(slopeColour, rockColour, blendFactor);
		}
		else
		{
			textureColour = rockColour;
		}

	}
	//////////////////// SAND //////////////////
	else if (worldPos > sandHeight)
	{
		float4 grassColour = GetTriplanarTextureColour(1, blending, input.worldPosition, 1.0f);


		float4 slopeColour = GetCombinedRockLerp(input, blending);
		float4 rockColour = GetCombinedRockLerp(input, blending);

		float slope = 1.0f - input.normal.y;

		if (slope < slopeGrassCutoff)
		{
			float blendFactor = slope / slopeGrassCutoff;
			textureColour = lerp(grassColour, slopeColour, blendFactor);
		}
		else if (slope < slopeRockCuttoff && slope >= slopeGrassCutoff)
		{
			float blendFactor = (slope - slopeGrassCutoff) * (1.0f / (slopeRockCuttoff - slopeGrassCutoff));
			textureColour = lerp(slopeColour, rockColour, blendFactor);
		}
		else
		{
			textureColour = rockColour;
		}
	}
	// Below all height, assume sand.
	else
	{
		float4 grassColour = GetTriplanarTextureColour(1, blending, input.worldPosition, 1.0f);


		float4 slopeColour = GetCombinedRockLerp(input, blending);
		float4 rockColour = GetCombinedRockLerp(input, blending);

		float slope = 1.0f - input.normal.y;

		if (slope < slopeGrassCutoff)
		{
			float blendFactor = slope / slopeGrassCutoff;
			textureColour = lerp(grassColour, slopeColour, blendFactor);
		}
		else if (slope < slopeRockCuttoff && slope >= slopeGrassCutoff)
		{
			float blendFactor = (slope - slopeGrassCutoff) * (1.0f / (slopeRockCuttoff - slopeGrassCutoff));
			textureColour = lerp(slopeColour, rockColour, blendFactor);
		}
		else
		{
			textureColour = rockColour;
		}
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