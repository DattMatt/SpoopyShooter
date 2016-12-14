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
	calculateFaceNormals();
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

void Terrain::calculateFaceNormals()
{
	for (int i = 0; i < numVerts - 512; i++)
	{
		// Get points of quad
		XMVECTOR p0 = XMLoadFloat3(&vertices[i].Position);
		XMVECTOR p1 = XMLoadFloat3(&vertices[i+1].Position);
		XMVECTOR p2 = XMLoadFloat3(&vertices[i+1+xSize].Position);
		XMVECTOR p3 = XMLoadFloat3(&vertices[i+2+xSize].Position);

		// Get old norms to add too
		XMVECTOR oldNormal0 = XMLoadFloat3(&vertices[i].Normal);
		XMVECTOR oldNormal1 = XMLoadFloat3(&vertices[i+1].Normal);
		XMVECTOR oldNormal2 = XMLoadFloat3(&vertices[i+1+xSize].Normal);
		XMVECTOR oldNormal3 = XMLoadFloat3(&vertices[i+2+xSize].Normal);

		// Triangle 1
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		XMVECTOR norm = XMVector3Cross(v2, v1);
		XMVector3Normalize(norm);

		XMFLOAT3 normFace1;
		XMStoreFloat3(&normFace1, norm);

		// Triangle 2
		v1 = XMVectorSubtract(p2, p3);
		v2 = XMVectorSubtract(p1, p3);

		XMVECTOR norm2 = XMVector3Cross(v2, v1);
		XMVector3Normalize(norm2);

		// Add normals to each vertex that was used to calculate it, then store it
		oldNormal0 += norm;
		oldNormal1 += norm + norm2;
		oldNormal2 += norm + norm2;
		oldNormal3 += norm2;

		XMStoreFloat3(&vertices[i].Normal, oldNormal0);
		XMStoreFloat3(&vertices[i+1].Normal, oldNormal1);
		XMStoreFloat3(&vertices[i+1+xSize].Normal, oldNormal2);
		XMStoreFloat3(&vertices[i+2+xSize].Normal, oldNormal3);
	}
}

void Terrain::calculateUV(int i)
{
	vertices[i].UV = XMFLOAT2(vertices[i].Position.x, vertices[i].Position.z);
}

Mesh * Terrain::getMesh()
{
	return terrMesh;
}

