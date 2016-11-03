#include "Node.h"



Node::Node(XMFLOAT3 pos)
{
	position = pos;
	radius = 0.25f;
}

Node::~Node()
{
}

XMFLOAT3 Node::GetPosition() { return position; }
void Node::SetPosition(XMFLOAT3 p) { position = p; }

Node* Node::GetNext() { return nextNode; }
void Node::SetNext(Node* n) { nextNode = n; }

float Node::GetRadius() { return radius; }
