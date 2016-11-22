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


