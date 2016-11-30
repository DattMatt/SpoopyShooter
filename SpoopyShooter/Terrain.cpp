#include "Terrain.h"

Terrain::Terrain(int _size, int _heightScale, ID3D11Device* _device)
{
	size = _size;
	gridSize = 32;
	heightScale = _heightScale;
	finalHeights.resize(size * size);
	LoadRAW();
	genVerticies();
	genIndicies();
	terrMesh = new Mesh(vertices, 1024, indicies, 6 * 32 * 32, _device);
}

Terrain::~Terrain()
{
	delete terrMesh;
}

void Terrain::LoadRAW()
{
	std::vector<unsigned char> heights(size * size);
	std::ifstream file;
	file.open(L"Assets/Terrain/terrain.raw", std::ios_base::binary);

	if (file)
	{
		file.read((char*)&heights[0], (std::streamsize)file.tellg());
		file.close();
	}

	for (int i = 0; i < size * size; i++)
	{
		finalHeights[i] = (heights[i] / 255.0f) * heightScale;
	}
}

void Terrain::genVerticies()
{
	int i = 0;
	for (int x = 0; x < gridSize; x++)
	{
		for (int z = 0; z < gridSize; z++)
		{
			vertices[i].Position = DirectX::XMFLOAT3(16 * x, finalHeights[i * 16], 16 * z);
			i++;
		}
	}
}

void Terrain::genIndicies()
{
	int ti = 0;
	int vi = 0;
	for (int z = 0; z < gridSize; z++)
	{
		for (int x = 0; x < gridSize; x++)
		{
			indicies[ti] = vi;
			indicies[ti + 1] = indicies[ti + 4] = vi + gridSize;
			indicies[ti + 2] = indicies[ti + 3] = vi + 1;
			indicies[ti + 5] = vi + gridSize + 1;
			ti++;
		}
		vi++;
	}
}

void Terrain::calculateNormals()
{
	DirectX::XMFLOAT3 normal;
	int i = 0;
	for (int x = 0; x < gridSize; x++)
	{
		for (int z = 0; z < gridSize; z++)
		{
			vertices[i].Normal = normal;
			i++;
		}
	}
}

Mesh * Terrain::getMesh()
{
	return terrMesh;
}

