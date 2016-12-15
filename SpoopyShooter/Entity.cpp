#include "Entity.h"

Entity::Entity()
{
}

Entity::Entity(Mesh* _mesh, Material* _mat)
{
	mesh = _mesh;
	mat = _mat;

	positionVector = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotationVector = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scaleVector = XMFLOAT3(1.0f, 1.0f, 1.0f);

	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
}


Entity::~Entity()
{
}

XMFLOAT4X4 Entity::GetWorldMatrix() { return worldMatrix; }
XMFLOAT3 Entity::GetPositionVector() { return positionVector; }
XMFLOAT3 Entity::GetRotationVector() { return rotationVector; }
XMFLOAT3 Entity::GetScaleVector() { return scaleVector; }
Mesh* Entity::GetMesh() { return mesh; }
bool Entity::GetVisible() { return visible; }

void Entity::SetWorldMatrix(XMFLOAT4X4 _worldMatrix) { worldMatrix = _worldMatrix; }
void Entity::SetPositionVector(XMFLOAT3 _pVector){ 	positionVector = _pVector; }
void Entity::SetRotationVector(XMFLOAT3 _rVector) { rotationVector = _rVector; }
void Entity::SetScaleVector(XMFLOAT3 _sVector) { scaleVector = _sVector; }
void Entity::SetVisible(bool v) { visible = v; }

void Entity::ScaleUniform(float s)
{
	scaleVector.x = s;
	scaleVector.y = s;
	scaleVector.z = s;
}

void Entity::ReconstructWorldMatrix() 
{
	XMMATRIX trans = XMMatrixTranslation(positionVector.x, positionVector.y, positionVector.z);
	XMMATRIX rotX = XMMatrixRotationX(rotationVector.x);
	XMMATRIX rotY = XMMatrixRotationY(rotationVector.y);
	XMMATRIX rotZ = XMMatrixRotationZ(rotationVector.z);
	XMMATRIX scale = XMMatrixScaling(scaleVector.x, scaleVector.y, scaleVector.z);
	
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(trans * rotX * rotY * rotZ * scale));
}

void Entity::Move(XMFLOAT3 mVector, float dt) 
{	
	mVector.x *= dt;
	mVector.y *= dt;
	mVector.z *= dt;
	XMVECTOR sum = XMLoadFloat3(&positionVector) + XMLoadFloat3(&mVector);
	XMStoreFloat3(&positionVector, sum);
}

void Entity::PrepareMaterial(XMFLOAT4X4 _viewMatrix, XMFLOAT4X4 _projectionMatrix) {
	
	mat->GetPixelShader()->SetSamplerState("basicSampler", mat->GetSamplerState());
	mat->GetPixelShader()->SetShaderResourceView("diffuseTexture", mat->GetSRV());
	if (mat->GetIfNorm() == true) {
		mat->GetPixelShader()->SetShaderResourceView("NormalMap", mat->GetNMSRV());
	}
	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.	
	mat->GetVertexShader()->SetMatrix4x4("world", worldMatrix);
	mat->GetVertexShader()->SetMatrix4x4("view", _viewMatrix);
	mat->GetVertexShader()->SetMatrix4x4("projection", _projectionMatrix);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	mat->GetVertexShader()->CopyAllBufferData();
	mat->GetPixelShader()->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	mat->GetVertexShader()->SetShader();
	mat->GetPixelShader()->SetShader();
}