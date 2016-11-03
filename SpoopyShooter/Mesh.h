#pragma once
#include <d3d11.h>
#include <fstream>
#include <vector>
#include <DirectXMath.h>
#include "Vertex.h"

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
	int numVerts;
	int numIndices;

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();

	void CreateBuffers(Vertex* vertices, int _numVertices, UINT* indices, int numIndices, ID3D11Device* device);
};

