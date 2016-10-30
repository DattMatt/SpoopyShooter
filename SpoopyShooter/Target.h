#pragma once
#include "Entity.h"

class Target : public Entity
{
private:
	float radius;
	XMFLOAT3 centerPoint;
public:
	Target();
	Target(Mesh* _mesh, Material* _mat);
	~Target();
};

