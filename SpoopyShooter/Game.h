#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "Entity.h"
#include "Camera.h"
#include "Lights.h"
#include "Target.h"
#include "Node.h"
#include "Player.h"
#include "WICTextureLoader.h"
#include "SmokeEmitter.h"
#include "Terrain.h"
#include <DirectXMath.h>

class Game 
	: public DXCore
{

	enum GameState : int
	{
		start,
		game,
		end
	};

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);

	GameState state;

	void ChangeState();

private:

	Material* mat;
	Material* mat2;
	Material* mat3;

	std::vector<Entity*> entities;
	std::vector<Mesh*> meshes;
	std::vector<Target*> targets;
	std::vector<Node*> nodes;

	Camera* camera;
	Player* player;

	Terrain* terr;

	ID3D11ShaderResourceView* leavesView;
	ID3D11ShaderResourceView* brickView;
	ID3D11ShaderResourceView* stoneFence;
	ID3D11ShaderResourceView* skySRV;
	ID3D11SamplerState* sampler;
	D3D11_SAMPLER_DESC description;

	ID3D11ShaderResourceView* partTex;
	SimpleVertexShader* particleVS;
	SimplePixelShader* particlePS;
	ID3D11DepthStencilState* particleDepthState;
	ID3D11BlendState* particleBlendState;
	SmokeEmitter* emitter;
	ID3D11RasterizerState* skyRastState;
	ID3D11DepthStencilState* skyDepthState;

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimpleVertexShader* skyVS;
	SimplePixelShader* skyPS;

	DirectionalLight dirLight;
	DirectionalLight dirLight2;

	PointLight pLight;

	Mesh* triangle;
	Mesh* square;
	Mesh* pentagon;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
	bool isDown;
	bool uDown;
};

