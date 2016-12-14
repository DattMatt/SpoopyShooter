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
	if (GetAsyncKeyState('W') & 0x8000) { MoveFB(20.0f, dt); }			//Move Forward
	if (GetAsyncKeyState('S') & 0x8000) { MoveFB(-20.0f, dt); }			//Move Backward
	if (GetAsyncKeyState('A') & 0x8000) { StrafeLR(-20.0f, dt); }		//Move Left
	if (GetAsyncKeyState('D') & 0x8000) { StrafeLR(20.0f, dt); }			//Move Right
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) { MoveUD(20.0f, dt); }		//Move Up
	if (GetAsyncKeyState('X') & 0x8000) { MoveUD(-20.0f, dt); }			//Move Down

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

void Camera::Raycast(float mouseX, float mouseY, std::vector<Target*>& targets)
{
	// Set origin to near plane and end to far plane
	XMFLOAT4 origin = XMFLOAT4(mouseX, mouseY, 0, 1);
	XMFLOAT4 end = XMFLOAT4(mouseX, mouseY, 1, 1);

	// Load into XMMATRIX
	XMMATRIX tempView = XMLoadFloat4x4(&viewMatrix);
	XMMATRIX tempProj = XMLoadFloat4x4(&projectionMatrix);

	// Get the inverse view-proj matrix
	XMMATRIX viewProj = XMMatrixInverse(nullptr, XMMatrixMultiply(tempView, tempProj));

	// Transform the end point to find the near and far 3d points
	XMVECTOR point3DNear =  XMVector4Transform(XMVectorSet(origin.x, origin.y, origin.z, 1), viewProj);
	XMVECTOR point3DFar = XMVector4Transform(XMVectorSet(end.x, end.y, end.z, 1), viewProj);
	//XMVECTOR rayDir = XMVector4Normalize(XMVectorSubtract(XMVectorSet(position.x, position.y, position.z, 1), XMVectorSet(forward.x, forward.y, forward.z, 1)));

	// Loop through targets to check for collisions
	for (int i = 0; i < targets.size(); i++)
	{
		
		XMVECTOR distObj = XMVectorSubtract(XMVectorSet(position.x, position.y, position.z, 1), XMVectorSet(targets[i]->GetCenterPoint().x, targets[i]->GetCenterPoint().y, targets[i]->GetCenterPoint().z, 1) );
		/*
		float B_half;
		float C;
		XMStoreFloat(&B_half, XMVector4Dot(distObj, rayDir));
		XMStoreFloat(&C, XMVector4Dot(distObj, distObj));
		float intersect = (B_half * B_half) - C;
		printf("%f", C);
		

		float a;
		XMStoreFloat(&a, XMVector4Dot(distObj, XMVectorSet(forward.x, forward.y, forward.z, 1)));
		printf("%f", a);
		if (a >= 0)
		{
			float disDot;
			XMStoreFloat(&disDot, XMVector4Dot(distObj, distObj));
			float d = sqrtf(disDot - (a * a));
			if (d >= 0)
			{
				printf("HIT!!! ");
			}
			printf("%f", d);
		}
		*/
		//printf("%f ", forward.x);
		float proj;
		XMStoreFloat(&proj, XMVector4Dot(distObj, XMVectorSet(forward.x, forward.y, forward.z, 1)));
		XMVECTOR projVec = XMVector4Normalize(XMVectorSet(forward.x, forward.y, forward.z, 1));
		projVec *= proj;
		XMFLOAT4 fOTP;
		XMFLOAT4 fDistObj;
		XMFLOAT4 fProjVec;
		XMStoreFloat4(&fDistObj, distObj);
		XMStoreFloat4(&fProjVec, projVec);
		//XMStoreFloat4(&fOTP, XMVectorSubtract(distObj, projVec));
		//float distFromObjToProj = sqrtf(fOTP.x * fOTP.x + fOTP.y * fOTP.y + fOTP.z * fOTP.z);
		float distFromObjToProj = sqrtf(powf(fDistObj.x - fProjVec.x, 2) + powf(fDistObj.y - fProjVec.y, 2) + powf(fDistObj.z - fProjVec.z, 2));
		printf("%f \n", distFromObjToProj);
		printf("%f \n", targets[i]->GetRadius());

		if (distFromObjToProj <= targets[i]->GetRadius() && targets[i]->GetVisible())
		{
			printf("HIT");
			targets[i]->SetVisible(false);
		}
	}
}

void Camera::UpdateProjectionMatrix(unsigned int width, unsigned int height) {
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}