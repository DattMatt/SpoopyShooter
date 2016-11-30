#include "Mesh.h"
#include <vector>
#include <fstream>
#include <DirectXMath.h>
#pragma once

class Terrain
{
	int size;
	int heightScale;
	std::vector<unsigned char> heights;
	std::vector<float> finalHeights;
	Vertex* verticies;
	UINT* indicies;
	Mesh* terrMesh;

public:
	Terrain(int _size, int _heightScale, ID3D11Device* _device);
	~Terrain();
	void LoadRAW();
	void genVerticies();
	void genIndicies();
	Mesh* getMesh();
};

