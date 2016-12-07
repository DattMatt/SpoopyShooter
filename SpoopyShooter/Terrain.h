#include "Mesh.h"
#include <vector>
#include <fstream>
#include <DirectXMath.h>
#pragma once

class Terrain
{
	int xSize;
	int zSize;
	int heightScale;
	int numVerts;
	int numInd;
	std::vector<float> finalHeights;
	Vertex vertices[512 * 512];
	UINT indices[512 * 512 * 6];
	Mesh* terrMesh;

public:
	Terrain(int _xSize, int _zSize, int _heightScale, ID3D11Device* _device);
	~Terrain();
	void LoadRAW();
	void genVertices();
	void genIndices();
	void calculateNormals();
	Mesh* getMesh();
};

