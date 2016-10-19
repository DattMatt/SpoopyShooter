#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"
#include "SimpleShader.h"

using namespace DirectX;

class Entity
{
private:
	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 positionVector;
	XMFLOAT3 rotationVector;
	XMFLOAT3 scaleVector;
	Mesh* mesh;
	Material* mat;

public:
	Entity(Mesh* _mesh, Material* _mat);
	~Entity();

	XMFLOAT4X4 GetWorldMatrix();	
	XMFLOAT3 GetPositionVector();
	XMFLOAT3 GetRotationVector();
	XMFLOAT3 GetScaleVector();

	Mesh* GetMesh();

	void SetWorldMatrix(XMFLOAT4X4 _worldMatrix);
	void SetPositionVector(XMFLOAT3 _pVector);
	void SetRotationVector(XMFLOAT3 _rVector);
	void SetScaleVector(XMFLOAT3 _sVector);	
	
	void ScaleUniform(float s);

	void ReconstructWorldMatrix();

	void Move(XMFLOAT3 mVector, float dt);

	void PrepareMaterial(XMFLOAT4X4 _viewMatrix, XMFLOAT4X4 _projectionMatrix);
};

