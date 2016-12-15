#include "Emitter.h"

using namespace DirectX;

Emitter::Emitter(
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
{
	this->vs = vs;
	this->ps = ps;
	this->texture = texture;

	this->maxParticles = maxParticles;
	this->lifetime = lifetime;
	this->startColor = startColor;
	this->endColor = endColor;
	this->startVelocity = startVelocity;
	this->startSize = startSize;
	this->endSize = endSize;
	this->particlesPerSecond = particlesPerSecond;
	this->secondsPerParticle = 1.0f / particlesPerSecond;

	this->emitterPosition = emitterPosition;
	this->emitterAccel = emitterAcceleration;

	timeSinceEmit = 0;
	liveParticleCount = 0;
	firstAliveIndex = 0;
	firstDeadIndex = 0;

	// Make the particle array
	particles = new Particle[maxParticles];

	//Create local particle vertices and UVs
	localParticleVertices = new ParticleVertex[4 * maxParticles];
	for (int i = 0; i < maxParticles * 4; i += 4)
	{
		localParticleVertices[i + 0].UV = XMFLOAT2(0, 0);
		localParticleVertices[i + 1].UV = XMFLOAT2(1, 0);
		localParticleVertices[i + 2].UV = XMFLOAT2(1, 1);
		localParticleVertices[i + 3].UV = XMFLOAT2(0, 1);
	}

	// DYNAMIC vertex buffer (no initial data necessary)
	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	vbDesc.ByteWidth = sizeof(ParticleVertex) * 4 * maxParticles;
	device->CreateBuffer(&vbDesc, 0, &vertexBuffer);

	// Index buffer data
	unsigned int* indices = new unsigned int[maxParticles * 6];
	int indexCount = 0;
	for (int i = 0; i < maxParticles * 4; i += 4)
	{
		indices[indexCount++] = i;
		indices[indexCount++] = i + 1;
		indices[indexCount++] = i + 2;
		indices[indexCount++] = i;
		indices[indexCount++] = i + 2;
		indices[indexCount++] = i + 3;
	}
	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices;	

	// Regular index buffer
	D3D11_BUFFER_DESC ibDesc = {};
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.ByteWidth = sizeof(unsigned int) * maxParticles * 6;
	device->CreateBuffer(&ibDesc, &indexData, &indexBuffer);

	delete[] indices;
}


Emitter::~Emitter()
{		
	delete[] particles;
	delete[] localParticleVertices;
	vertexBuffer->Release();
	indexBuffer->Release();
}

void Emitter::Update(float dt, XMFLOAT3 playerPos)
{
	if (firstAliveIndex < firstDeadIndex)
	{
		for (int i = firstAliveIndex; i < firstDeadIndex; i++)
			UpdateSingle(dt, i);
	}
	else
	{
		for (int i = firstAliveIndex; i < maxParticles; i++)
			UpdateSingle(dt, i);
		for (int i = 0; i < firstDeadIndex; i++)
			UpdateSingle(dt, i);
	}

	//Add to time
	timeSinceEmit += dt;

	while (timeSinceEmit > secondsPerParticle)
	{
		Spawn();
		timeSinceEmit -= secondsPerParticle;
	}

	emitterPosition = playerPos;
}

void Emitter::UpdateSingle(float dt, int index)
{
	if (particles[index].Age >= lifetime)
		return;

	particles[index].Age += dt;
	if (particles[index].Age >= lifetime)
	{
		firstAliveIndex++;
		firstAliveIndex %= maxParticles;
		liveParticleCount--;
	}

	//Current percent of lifetime
	float agePercent = particles[index].Age / lifetime;

	//Color
	XMStoreFloat4(&particles[index].Color, XMVectorLerp(XMLoadFloat4(&startColor), XMLoadFloat4(&endColor), agePercent));

	//Size
	particles[index].Size = startSize + agePercent * (endSize - startSize);

	//Move particles
	XMVECTOR startPos = XMLoadFloat3(&emitterPosition);
	XMVECTOR startVel = XMLoadFloat3(&particles[index].StartingVel);
	XMVECTOR accel = XMLoadFloat3(&emitterAccel);
	float t = particles[index].Age;

	XMStoreFloat3(&particles[index].Position, accel * t * t / 2.0f + startVel * t + startPos);
}

void Emitter::Spawn()
{
	if (liveParticleCount == maxParticles)
		return;

	//Reset first dead
	particles[firstDeadIndex].Age = 0;
	particles[firstDeadIndex].Size = startSize;
	particles[firstDeadIndex].Color = startColor;
	particles[firstDeadIndex].Position = emitterPosition;
	particles[firstDeadIndex].StartingVel = startVelocity;
	

	//Increment and wrap
	firstDeadIndex++;
	firstDeadIndex %= maxParticles;

	liveParticleCount++;
}

void Emitter::CopyPartToGPU(ID3D11DeviceContext* context)
{
	if (firstAliveIndex < firstDeadIndex)
	{
		for (int i = firstAliveIndex; i < maxParticles; i++)
			CopyOneParticle(i);
	}
	else
	{
		for (int i = firstAliveIndex; i < maxParticles; i++)
			CopyOneParticle(i);

		for (int i = 0; i < firstDeadIndex; i++)
			CopyOneParticle(i);
	}

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	memcpy(mapped.pData, localParticleVertices, sizeof(ParticleVertex) * 4 * maxParticles);

	context->Unmap(vertexBuffer, 0);
}

void Emitter::CopyOneParticle(int index)
{
	int i = index * 4;

	localParticleVertices[i + 0].Position = particles[index].Position;
	localParticleVertices[i + 1].Position = particles[index].Position;
	localParticleVertices[i + 2].Position = particles[index].Position;
	localParticleVertices[i + 3].Position = particles[index].Position;

	localParticleVertices[i + 0].Size = particles[index].Size;
	localParticleVertices[i + 1].Size = particles[index].Size;
	localParticleVertices[i + 2].Size = particles[index].Size;
	localParticleVertices[i + 3].Size = particles[index].Size;

	localParticleVertices[i + 0].Color = particles[index].Color;
	localParticleVertices[i + 1].Color = particles[index].Color;
	localParticleVertices[i + 2].Color = particles[index].Color;
	localParticleVertices[i + 3].Color = particles[index].Color;
}

void Emitter::Draw(ID3D11DeviceContext* context, Camera* camera)
{
	// Copy to dynamic buffer
	CopyPartToGPU(context);

	//Set up Buffers
	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	vs->SetMatrix4x4("view", camera->GetViewMatrix());
	vs->SetMatrix4x4("projection", camera->GetProjectionMatrix());
	vs->SetShader();
	vs->CopyAllBufferData();

	ps->SetShaderResourceView("particle", texture);
	ps->SetShader();
	ps->CopyAllBufferData();

	// Draw the correct parts of the buffer
	if (firstAliveIndex < firstDeadIndex)
	{
		context->DrawIndexed(liveParticleCount * 6, firstAliveIndex * 6, 0);
	}
	else
	{
		// Draw first half (0 -> dead)
		context->DrawIndexed(firstDeadIndex * 6, 0, 0);

		// Draw second half (alive -> max)
		context->DrawIndexed((maxParticles - firstAliveIndex) * 6, firstAliveIndex * 6, 0);
	}
}