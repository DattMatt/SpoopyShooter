#include "SmokeEmitter.h"



SmokeEmitter::SmokeEmitter(
	int maxParticles,
	int particlesPerSecond,
	float lifetime,
	float startSize,
	float endSize,
	DirectX::XMFLOAT4 startColor,
	DirectX::XMFLOAT4 endColor,
	DirectX::XMFLOAT3 startVelocity,
	DirectX::XMFLOAT3 emitterPosition,
	DirectX::XMFLOAT3 emitterAcceleration,
	ID3D11Device* device,
	SimpleVertexShader* vs,
	SimplePixelShader* ps,
	ID3D11ShaderResourceView* texture
)
	: Emitter(maxParticles, particlesPerSecond, lifetime, startSize, endSize, startColor, endColor, startVelocity, emitterPosition, emitterAcceleration, device, vs, ps, texture)
{
}


SmokeEmitter::~SmokeEmitter()
{
}

void SmokeEmitter::Spawn()
{
	if (liveParticleCount == maxParticles)
		return;

	//Reset first dead
	particles[firstDeadIndex].Age = 0;
	particles[firstDeadIndex].Size = startSize;
	particles[firstDeadIndex].Color = startColor;
	particles[firstDeadIndex].Position = emitterPosition;
	particles[firstDeadIndex].StartingVel = startVelocity;
	RandomizeStartingVelocity();
	particles[firstDeadIndex].StartingVel.x += ((float)rand() / RAND_MAX) * 0.4f - 0.2f;
	particles[firstDeadIndex].StartingVel.y += ((float)rand() / RAND_MAX) * 0.4f - 0.2f;
	particles[firstDeadIndex].StartingVel.z += ((float)rand() / RAND_MAX) * 0.4f - 0.2f;

	//Increment and wrap
	firstDeadIndex++;
	firstDeadIndex %= maxParticles;

	liveParticleCount++;
}

void SmokeEmitter::RandomizeStartingVelocity()
{
	XMFLOAT3 rotVec = XMFLOAT3(rand() % 2, rand() % 2, 0);
	XMVECTOR rotQuat = XMVector3Normalize(XMLoadFloat3(&rotVec));
	XMStoreFloat3(&startVelocity, XMVector3Rotate(XMLoadFloat3(&startVelocity), rotQuat));
}



