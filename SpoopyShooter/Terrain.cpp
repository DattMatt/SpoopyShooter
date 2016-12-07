#include "Terrain.h"

Terrain::Terrain(int _xSize, int _zSize, int _heightScale, ID3D11Device* _device)
{
	xSize = _xSize;
	zSize = _zSize;
	numVerts = 0;
	numInd = 0;
	heightScale = _heightScale;
	LoadRAW();
	genVertices();
	genIndices();
	terrMesh = new Mesh(vertices, numVerts, indices, numInd, _device);
}

Terrain::~Terrain()
{
	delete terrMesh;
}

void Terrain::LoadRAW()
{
	std::vector<unsigned char> heights(xSize * zSize);
	std::ifstream file;
	file.open(L"Assets/Terrain/terrain.raw", std::ios_base::binary);

	if (file)
	{
		file.read((char*)&heights[0], (std::streamsize)file.tellg());
		file.close();
	}

	for (int i = 0; i < heights.size(); i++)
	{
		finalHeights.push_back((heights[i] / 255.0f) * heightScale);
	}
}

void Terrain::genVertices()
{
	int i = 0;
	for (int x = 0; x < xSize; x++)
	{
		for (int z = 0; z < zSize; z++)
		{
			vertices[i].Position = DirectX::XMFLOAT3(x - (xSize / 2), finalHeights[i], z - (zSize / 2));
			i++;
		}
	}
	numVerts = i + 1;
}

void Terrain::genIndices()
{
	int ti = 0;
	int vi = 0;
	for (int z = 0; z < zSize; z++)
	{
		for (int x = 0; x < xSize; x++)
		{
			indices[ti] = vi;
			indices[ti + 3] = indices[ti + 2] = vi + 1;
			indices[ti + 4] = indices[ti + 1] = vi + xSize + 1;
			indices[ti + 5] = vi + xSize + 2;
			vi++;
			ti += 6;
		}
		vi++;
	}
	numInd = ti + 1;
}

void Terrain::calculateNormals()
{
	DirectX::XMFLOAT3 normal;
	int i = 0;
	for (int x = 0; x < xSize; x++)
	{
		for (int z = 0; z < zSize; z++)
		{
			//vertices[x][z].Normal = normal;
			//i++;
		}
	}
}

Mesh * Terrain::getMesh()
{
	return terrMesh;
}

