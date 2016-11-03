#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Camera.h"
#include "Node.h"

using namespace DirectX;

class Player
{
private:

	XMFLOAT3 position;
	Camera* camera;
	float cameraOffset;
	Node* currentNode;

public:
	Player(XMFLOAT3 pos, Camera* cam);
	~Player();

	XMFLOAT3 GetPosition();
	void SetPosition(XMFLOAT3 pos);
	Camera* GetCamera();
	void SetCamera(Camera* cam);
	float GetCameraOffset();
	void SetCameraOffset(float off);
	Node* GetCurrent(); 
	void SetCurrent(Node* n);

	void MoveToward(XMFLOAT3 targ, float speed, float dt);
	void UpdateCameraPos();
};

