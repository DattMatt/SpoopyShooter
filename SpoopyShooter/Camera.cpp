#include "Camera.h"



Camera::Camera()
{
	unitZ = XMFLOAT3(0.0f, 0.0f, 1.0f);
	unitY = XMFLOAT3(0.0f, 1.0f, 0.0f);
	unitX = XMFLOAT3(1.0f, 0.0f, 0.0f);
	position = XMFLOAT3(0.0f, 0.0f, -5.0f);
	forward = unitZ;
	up = unitY;
	right = unitX;
	xRot = 0.0f;
	yRot = 0.0f;
	XMMATRIX view = XMMatrixLookToLH(
		XMLoadFloat3(&position),			//Camera position
		XMLoadFloat3(&forward),				//Forward vector
		XMLoadFloat3(&up)					//Up Vector
	);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(view));
	XMStoreFloat4x4(&projectionMatrix, XMMatrixIdentity());
}


Camera::~Camera()
{
}

XMFLOAT4X4 Camera::GetViewMatrix() { return viewMatrix; }
XMFLOAT4X4 Camera::GetProjectionMatrix() { return projectionMatrix; }
XMFLOAT3 Camera::GetPosition() { return position; }
XMFLOAT3 Camera::GetForward() { return forward; }
XMFLOAT3 Camera::GetUp() { return up; }
XMFLOAT3 Camera::GetRight() { return right; }
float Camera::GetRotationX() { return xRot; }
float Camera::GetRotationY() { return yRot; }

void Camera::SetViewMatrix(XMFLOAT4X4 _vm) { viewMatrix = _vm; }
void Camera::SetProjectionMatrix(XMFLOAT4X4 _pm) { projectionMatrix = _pm; }
void Camera::SetPosition(XMFLOAT3 _position) { position = _position; }
void Camera::SetForward(XMFLOAT3 __forward) { forward = __forward; }
void Camera::SetUp(XMFLOAT3 __up) { up = __up; }
void Camera::SetRight(XMFLOAT3 __right) { right = __right; }
void Camera::SetRotationX(float _xRot) { xRot = _xRot; }
void Camera::SetRotationY(float _yRot) { yRot = _yRot; }

void Camera::Update(float dt) {
	if (GetAsyncKeyState('W') & 0x8000) { MoveFB(2.0f, dt); }			//Move Forward
	if (GetAsyncKeyState('S') & 0x8000) { MoveFB(-2.0f, dt); }			//Move Backward
	if (GetAsyncKeyState('A') & 0x8000) { StrafeLR(-2.0f, dt); }		//Move Left
	if (GetAsyncKeyState('D') & 0x8000) { StrafeLR(2.0f, dt); }			//Move Right
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) { MoveUD(2.0f, dt); }		//Move Up
	if (GetAsyncKeyState('X') & 0x8000) { MoveUD(-2.0f, dt); }			//Move Down

	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(yRot, xRot, 0.0f);
	XMVECTOR fVector = XMVector3Rotate(XMLoadFloat3(&unitZ), rotation);
	XMVECTOR uVector = XMVector3Rotate(XMLoadFloat3(&unitY), rotation);
	XMVECTOR rVector = XMVector3Rotate(XMLoadFloat3(&unitX), rotation);
	XMStoreFloat3(&forward, fVector);
	XMStoreFloat3(&up, uVector);
	XMStoreFloat3(&right, rVector);
	XMMATRIX view = XMMatrixLookToLH(
		XMLoadFloat3(&position),			//Camera position
		fVector,							//Forward vector
		XMLoadFloat3(&unitY)				//Up Vector
	);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(view));
}

void Camera::MoveFB(float amount, float dt) {
	XMVECTOR move = XMLoadFloat3(&forward);
	move *= (amount * dt);
	XMVECTOR pos = XMLoadFloat3(&position);
	pos += move;
	XMStoreFloat3(&position, pos);
}

void Camera::StrafeLR(float amount, float dt) {
	XMVECTOR move = XMLoadFloat3(&right);
	move *= (amount * dt);
	XMVECTOR pos = XMLoadFloat3(&position);
	pos += move;
	XMStoreFloat3(&position, pos);
}

void Camera::MoveUD(float amount, float dt) {
	XMVECTOR move = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));
	move *= (amount * dt);
	XMVECTOR pos = XMLoadFloat3(&position);
	pos += move;
	XMStoreFloat3(&position, pos);
}

void Camera::Rotate(float x, float y) {
	xRot += (x * 0.001);
	yRot += (y * 0.001);
}

void Camera::UpdateProjectionMatrix(unsigned int width, unsigned int height) {
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}