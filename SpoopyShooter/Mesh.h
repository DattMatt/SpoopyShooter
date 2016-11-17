#pragma once
#include <d3d11.h>
#include <fstream>
#include <vector>
#include <DirectXMath.h>
#include "Vertex.h"

using namespace DirectX;

class Mesh
{
public:
	Mesh(
		Vertex* vertices,
		int _numVertices,
		UINT* indices,
		int _numIndices,
		ID3D11Device* device);
	Mesh(char* model, ID3D11Device* device);
	~Mesh();

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	Vertex* verts;
	std::vector<Vertex> gVerts;
	int numVerts;
	int numIndices;

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();

	void CreateBuffers(Vertex* vertices, int _numVertices, UINT* indices, int numIndices, ID3D11Device* device);
	void CalculateTangents(Vertex* verts, int numVerts, UINT* indices, int numIndices);
};

