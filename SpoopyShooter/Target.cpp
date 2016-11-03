#include "Target.h"

Target::Target()
{

}

Target::Target(Mesh* _mesh, Material* _mat) : Entity(_mesh, _mat)
{
	mins = XMFLOAT3(0, 0, 0);
	maxs = XMFLOAT3(0, 0, 0);
	printf("%f \n", _mesh->numVerts);
	// Find the mins and maxs
	for (int i = 0; i < _mesh->numVerts; i++)
	{
		// X
		if (_mesh->gVerts[i].Position.x < mins.x) {
			mins.x = _mesh->gVerts[i].Position.x;
		}
		else if (_mesh->gVerts[i].Position.x > maxs.x) {
			maxs.x = _mesh->gVerts[i].Position.x;
		}

		// Y
		if (_mesh->gVerts[i].Position.y < mins.y) {
			mins.y = _mesh->gVerts[i].Position.y;
		}
		else if (_mesh->gVerts[i].Position.y > maxs.y) {
			maxs.y = _mesh->gVerts[i].Position.y;
		}

		// Z
		if (_mesh->gVerts[i].Position.z < mins.z) {
			mins.z = _mesh->gVerts[i].Position.z;
		}
		else if (_mesh->gVerts[i].Position.z > maxs.z) {
			maxs.z = _mesh->gVerts[i].Position.z;
		}
	}

	// Find center point
	centerPoint.x = maxs.x - mins.x;
	centerPoint.y = maxs.y - mins.y;
	centerPoint.z = maxs.z - mins.z;
	
	// Calculate the distance between centerpoint and max
	radius = sqrtf(powf(maxs.x - centerPoint.x, 2) + powf(maxs.y - centerPoint.y, 2) + powf(maxs.z - centerPoint.z, 2));
}

XMFLOAT3 Target::GetCenterPoint()
{
	return centerPoint;
}

float Target::GetRadius()
{
	return radius;
}

Target::~Target()
{
}
