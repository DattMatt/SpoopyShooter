#pragma once
#include "Entity.h"
#include <DirectXMath.h>

class Target : public Entity
{
private:
	float radius;
	XMFLOAT3 centerPoint;
	XMFLOAT3 mins;
	XMFLOAT3 maxs;
public:
	Target();
	Target(Mesh* _mesh, Material* _mat);
	void Update();
	XMFLOAT3 GetCenterPoint();
	float GetRadius();
	~Target();
};

