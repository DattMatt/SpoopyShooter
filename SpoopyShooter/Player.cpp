#include "Player.h"



Player::Player(XMFLOAT3 pos, Camera* cam)
{
	position = pos;
	camera = cam;
	cameraOffset = 1.0f;
}


Player::~Player()
{
}

XMFLOAT3 Player::GetPosition() { return position; }
void Player::SetPosition(XMFLOAT3 pos) { position = pos; }
Camera* Player::GetCamera() { return camera; }
void Player::SetCamera(Camera* cam) { camera = cam; }
float Player::GetCameraOffset() { return cameraOffset; }
void Player::SetCameraOffset(float off) { cameraOffset = off; }
Node* Player::GetCurrent() { return currentNode; }
void Player::SetCurrent(Node* n) { currentNode = n; }

void Player::MoveToward(XMFLOAT3 targ, float speed, float dt)
{
	XMVECTOR vecTo = XMVector3Normalize(XMLoadFloat3(&targ) - XMLoadFloat3(&position));
	vecTo = vecTo * speed * dt;
	vecTo += XMLoadFloat3(&position);
	XMStoreFloat3(&position, vecTo);
}

void Player::UpdateCameraPos()
{
	XMFLOAT3 camPos = position;
	camPos.y += cameraOffset;
	camera->SetPosition(camPos);
}
