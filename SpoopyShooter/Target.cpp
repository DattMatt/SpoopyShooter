#include "Target.h"

Target::Target()
{

}

Target::Target(Mesh* _mesh, Material* _mat) : Entity(_mesh, _mat)
{
	float xTotal = 0;
	float yTotal = 0;
	float zTotal = 0;

	for (int i = 0; i < _mesh->numIndices; i++)
	{
		//_mesh->vertexBuffer->
	}
}

Target::~Target()
{
}
