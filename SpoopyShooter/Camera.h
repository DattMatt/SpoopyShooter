#pragma once
#include <d3d11.h>
#include "Target.h"
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
private:
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;
	XMFLOAT3 position;
	XMFLOAT3 forward;
	XMFLOAT3 up;
	XMFLOAT3 right;
	XMFLOAT3 unitZ;
	XMFLOAT3 unitY;
	XMFLOAT3 unitX;
	float xRot;
	float yRot;

public:
	Camera();
	~Camera();

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetForward();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();
	float GetRotationX();
	float GetRotationY();

	void SetViewMatrix(XMFLOAT4X4 _vm);
	void SetProjectionMatrix(XMFLOAT4X4 _pm);
	void SetPosition(XMFLOAT3 _position);
	void SetForward(XMFLOAT3 __forward);
	void SetUp(XMFLOAT3 __up);
	void SetRight(XMFLOAT3 __right);
	void SetRotationX(float _xRot);
	void SetRotationY(float _yRot);

	void Update(float dt);
	void UpdateProjectionMatrix(unsigned int width, unsigned int height);

	void MoveFB(float amount, float dt);
	void StrafeLR(float amount, float dt);
	void MoveUD(float amount, float dt);
	void Rotate(float x, float y);

	void Raycast(float mouseX, float mouseY, std::vector<Target*>& targets);
};

