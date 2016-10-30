#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Node
{

private:
	
	XMFLOAT3 position;
	Node* nextNode;

public:

	Node(XMFLOAT3 pos);
	~Node();

	XMFLOAT3 GetPosition();
	void SetPosition(XMFLOAT3 p);

	Node* GetNext();
	void SetNext(Node* n);

};