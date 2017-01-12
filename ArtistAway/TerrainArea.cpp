#include "TerrainArea.h"



CTerrainArea::CTerrainArea()
{
	mNumberOfVertices = 0;
	mNumberOfIndices = 0;

	mpVertexBuffer = nullptr;
	mpIndexBuffer = nullptr;

	mpTexture = new CTexture();
	gLogger->MemoryAllocWriteLine(typeid(mpTexture).name());
}


CTerrainArea::~CTerrainArea()
{
	delete mpTexture;
	gLogger->MemoryDeallocWriteLine(typeid(mpTexture).name());
}

bool CTerrainArea::LoadTexture(ID3D11Device * device, WCHAR * filename)
{
	if (!mpTexture->Initialise(device, filename))
	{
		gLogger->WriteLine("Failed to initialise a texture in TerrainArea.cpp.");
		return false;
	}

	return true;
}

void CTerrainArea::AddTile(CTerrainTile tile)
{
	mpTiles.push_back(tile);
}

void CTerrainArea::SetColour(D3DXVECTOR4 colour)
{
	mColour = colour;
}

void CTerrainArea::ClearAllTiles()
{
	mpTiles.clear();
	//Shutdown();
}

bool CTerrainArea::EraseTile(CTerrainTile tile)
{
	std::vector<CTerrainTile>::iterator it = mpTiles.begin();

	while (it != mpTiles.end())
	{
		if (tile.Compare((*it)))
		{
			mpTiles.erase(it);
			return true;
		}
		it++;
	}

	return false;
}

bool CTerrainArea::SetBuffers(ID3D11Device* device)
{
	mNumberOfVertices = mpTiles.size() * 3;
	mNumberOfIndices = mNumberOfVertices;

	CTerrainTile::VertexType* vertices = new CTerrainTile::VertexType[mNumberOfVertices];
	unsigned long* indices = new unsigned long[mNumberOfIndices];

	int count = 0;

	// Init vertices.
	for (int i = 0; i < mpTiles.size(); i++)
	{
		for (int vertex = 0; vertex < 3; vertex++)
		{
			vertices[count] = mpTiles[i].mVertices[vertex];

			count++;
		}
	}

	// Init indices.
	for (int i = 0; i < mNumberOfIndices; i++)
	{
		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;

	// Set up the descriptor of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(CTerrainTile::VertexType) * mNumberOfVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	bool result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mpVertexBuffer);
	if (FAILED(result))
	{
		gLogger->WriteLine("Failed to create the vertex buffer from the buffer description in TerrainTile.");
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mNumberOfIndices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mpIndexBuffer);
	if (FAILED(result))
	{
		gLogger->WriteLine("Failed to create the index buffer from the buffer description in terrain tile..");
		return false;
	}

	delete[] vertices;
	delete[] indices;

	return true;
}

void CTerrainArea::Render(ID3D11DeviceContext * context)
{
	RenderBuffers(context);
}

void CTerrainArea::Shutdown()
{
	if (mpVertexBuffer != nullptr)
	{
		mpVertexBuffer->Release();
	}

	if (mpIndexBuffer != nullptr)
	{
		mpIndexBuffer->Release();
	}
}

void CTerrainArea::RenderBuffers(ID3D11DeviceContext * context)
{
	unsigned int stride;
	unsigned int offset;

	// Set the vertex buffer stride and offset.
	stride = sizeof(CTerrainTile::VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler.
	context->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler.
	context->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Tell directx we've passed it a triangle list in the form of indices.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
