#include "Terrain.h"

Terrain::Terrain(int _size, int _heightScale, ID3D11Device* _device)
{
	size = _size;
	heightScale = _heightScale;
	heights.reserve(size * size);
	finalHeights.reserve(size * size);
	LoadRAW();
	genVerticies();
	genIndicies();
	terrMesh = new Mesh(verticies, 33 * 33, indicies, 33 * 33, _device);
}

Terrain::~Terrain()
{
	delete terrMesh;
}

void Terrain::LoadRAW()
{
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
	for (int x = 0; x < 33; x++)
	{
		for (int z = 0; z < 33; z++)
		{
			verticies[i].Position = DirectX::XMFLOAT3(16 * x, finalHeights[i], 16 * z);
			i++;
		}
	}
}

void Terrain::genIndicies()
{
	int ti, vi = 0;
	for (int z = 0; z < 33; z++)
	{
		for (int x = 0; x < 33; x++)
		{
			indicies[ti] = vi;
			indicies[ti + 1] = indicies[ti + 4] = vi + 33;
			indicies[ti + 2] = indicies[ti + 3] = vi + 1;
			indicies[ti + 5] = vi + 34;
			ti++;
		}
		vi++;
	}
}

Mesh * Terrain::getMesh()
{
	return terrMesh;
}

