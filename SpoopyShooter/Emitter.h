#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "Camera.h"
#include "SimpleShader.h"

struct Particle 
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 StartingVel;
	float Size;
	float Age;
};

struct ParticleVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT4 Color;
	float Size;
};

class Emitter
{
public:
	Emitter(
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
	);
	~Emitter();

	void Update(float dt);
	virtual void UpdateSingle(float dt, int index);

	virtual void Spawn();

	void CopyPartToGPU(ID3D11DeviceContext* context);
	void CopyOneParticle(int index);
	void Draw(ID3D11DeviceContext* context, Camera* camera);

private:

	DirectX::XMFLOAT3 emitterAccel;
	DirectX::XMFLOAT3 emitterPosition;
	DirectX::XMFLOAT3 startVelocity;
	DirectX::XMFLOAT4 startColor;
	DirectX::XMFLOAT4 endColor;
	float startSize;
	float endSize;

	int particlesPerSecond;
	float secondsPerParticle;
	float timeSinceEmit;

	int liveParticleCount;
	float lifetime;

	//Particle Array
	Particle* particles;
	int maxParticles;
	int firstDeadIndex;
	int firstAliveIndex;

	// Rendering
	ParticleVertex* localParticleVertices;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	ID3D11ShaderResourceView* texture;
	SimpleVertexShader* vs;
	SimplePixelShader* ps;
	
};

