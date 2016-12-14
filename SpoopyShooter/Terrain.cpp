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
	//calculateNormals();
	terrMesh = new Mesh(vertices, numVerts, indices, numInd, _device);
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
		file.seekg(0, file.end);
		int length = file.tellg();
		file.seekg(0, file.beg);
		file.read((char*)&heights[0], length);
		file.close();
	}

	for (int i = 0; i < 512 * 512; i++)
	{
		finalHeights.push_back((heights[i] / 255.0f) * heightScale);
	}
}

void Terrain::genVertices()
{
	int ti = 0;
	int vi = 0;
	int i = 0;
	for (int x = 0; x < xSize; x++)
	{
		for (int z = 0; z < zSize; z++)
		{
			vertices[i].Position = XMFLOAT3(x - (xSize / 2), finalHeights[i], z - (zSize / 2));
			genIndices(ti, vi);
			vertices[i].Normal = XMFLOAT3(0, 1, 0);
			calculateUV(i);
			i++;
			vi++;
			ti += 6;
		}
		vi++;
	}
	numVerts = i + 1;
	numInd = ti + 1;
}

void Terrain::genIndices(int ti, int vi)
{
	indices[ti] = vi;
	indices[ti + 3] = indices[ti + 2] = vi + 1;
	indices[ti + 4] = indices[ti + 1] = vi + xSize + 1;
	indices[ti + 5] = vi + xSize + 2;
}

void Terrain::calculateNormals()
{
	/*
	for (int i = 0; i < numVerts; i+=3)
	{
		XMVECTOR p0 = XMLoadFloat3(&vertices[i].Position);
		XMVECTOR p1 = XMLoadFloat3(&vertices[i+1].Position);;
		XMVECTOR p2 = XMLoadFloat3(&vertices[i+2].Position);;

		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		XMVECTOR norm = XMVector3Cross(v1, v2);
		XMVector3Normalize(norm);

		XMStoreFloat3(&vertices[i].Position, norm);
		XMStoreFloat3(&vertices[i+1].Position, norm);
		XMStoreFloat3(&vertices[i+2].Position, norm);
	}
	*/
}

void Terrain::calculateUV(int i)
{
	vertices[i].UV = XMFLOAT2(vertices[i].Position.x, vertices[i].Position.z);
}

Mesh * Terrain::getMesh()
{
	return terrMesh;
}

