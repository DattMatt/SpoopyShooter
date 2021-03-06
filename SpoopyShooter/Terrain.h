#include "Mesh.h"
#include <vector>
#include <fstream>
#include <DirectXMath.h>
#pragma once

using namespace DirectX;

class Terrain
{
	int xSize;
	int zSize;
	int heightScale;
	int numVerts;
	int numInd;
	std::vector<float> finalHeights;
	std::vector<XMFLOAT3> faceNorms;
	Vertex vertices[512 * 512];
	UINT indices[512 * 512 * 6];
	unsigned char heights[512 * 512];
	Mesh* terrMesh;

public:
	Terrain(int _xSize, int _zSize, int _heightScale, ID3D11Device* _device);
	~Terrain();
	void LoadRAW();
	void genVertices();
	void genIndices(int ti, int vi);
	void calculateFaceNormals();
	void calculateUV(int i);
	Mesh* getMesh();
};

