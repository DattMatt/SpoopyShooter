#include "Mesh.h"
#include <vector>
#include <fstream>
#include <DirectXMath.h>
#pragma once

class Terrain
{
	int size;
	int gridSize;
	int heightScale;
	std::vector<float> finalHeights;
	Vertex vertices[1024];
	UINT indicies[6 * 32 * 32];
	Mesh* terrMesh;

public:
	Terrain(int _size, int _heightScale, ID3D11Device* _device);
	~Terrain();
	void LoadRAW();
	void genVerticies();
	void genIndicies();
	void calculateNormals();
	Mesh* getMesh();
};

