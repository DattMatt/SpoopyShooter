#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields	
	vertexShader = 0;
	pixelShader = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class	

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
	delete triangle;
	delete square;
	delete pentagon;
	delete cone;
	delete cube;
	delete ghost;
	delete fencePillar;
	delete camera;
	delete player;
	delete mat;
	delete mat2;
	delete mat3;
	leavesView->Release();
	brickView->Release();
	stoneFence->Release();
	sampler->Release();

	for (int i = 0; i < entities.size(); i++)
	{
		delete entities[i];
	}

	for (int i = 0; i < nodes.size(); i++)
	{
		delete nodes[i];
	}
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	description = {};
	memset(&description, 0, sizeof(D3D11_SAMPLER_DESC));
	description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	description.MaxLOD = D3D11_FLOAT32_MAX;

	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();
	camera = new Camera();	
	camera->UpdateProjectionMatrix(width, height);	
	player = new Player(XMFLOAT3(0.0f, 0.0f, -5.0f), camera);
	printf("Player Position: (%f, %f, %f)\n", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z);
	printf("Camera Position: (%f, %f, %f)\n", player->GetCamera()->GetPosition().x, player->GetCamera()->GetPosition().y, player->GetCamera()->GetPosition().z);
	prevMousePos.x = 0;
	prevMousePos.y = 0;
	isDown = false;
	dirLight = {
		XMFLOAT4(0.1,0.1,0.1,1.0),
		XMFLOAT4(0,0,0,1),
		XMFLOAT3(1,-1,0)
	};
	dirLight2 = {
		XMFLOAT4(0.1,0.1,0.1,1.0),
		XMFLOAT4(0,0,0,1),
		XMFLOAT3(-1,-1,0)
	};	
	pLight = {
		XMFLOAT4(1, 0, 0, 1),
		XMFLOAT3(-1.8f, 1.0f, 0)
	};

	nodes.push_back(new Node(XMFLOAT3(0.0f, 0.0f, 0.0f)));
	nodes.push_back(new Node(XMFLOAT3(0.0f, 0.0f, 2.0f)));
	nodes.push_back(new Node(XMFLOAT3(1.0f, 0.0f, 3.0f)));
	nodes.push_back(new Node(XMFLOAT3(1.0f, 0.0f, 5.0f)));
	nodes.push_back(new Node(XMFLOAT3(3.0f, 0.0f, 2.0f)));

	for (int i = 0; i < nodes.size(); i++)
	{
		if (i != nodes.size() - 1)
			nodes[i]->SetNext(nodes[i + 1]);
		else
			nodes[i]->SetNext(nodes[0]);

		printf("Location of next node: (%f, %f, %f)\n", nodes[i]->GetNext()->GetPosition().x, nodes[i]->GetNext()->GetPosition().y, nodes[i]->GetNext()->GetPosition().z);
	}

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	state = start;
	uDown = false;
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");		

	pixelShader = new SimplePixelShader(device, context);
	if(!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))	
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	HRESULT texResult = CreateWICTextureFromFile(device, context, L"Assets/Textures/leaves.png", 0, &leavesView);
	HRESULT texResult2 = CreateWICTextureFromFile(device, context, L"Assets/Textures/brick.jpg", 0, &brickView);
	HRESULT texResult3 = CreateWICTextureFromFile(device, context, L"Assets/Textures/StoneFence.png", 0, &stoneFence);
	HRESULT sampResult = device->CreateSamplerState(&description, &sampler);

	mat = new Material(vertexShader, pixelShader, leavesView, sampler);
	mat2 = new Material(vertexShader, pixelShader, brickView, sampler);
	mat3 = new Material(vertexShader, pixelShader, stoneFence, sampler);

	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//   update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//   an identity matrix.  This is just to show that HLSL expects a different
	//   matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up  = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V   = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 magenta = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 cyan = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT3 normTemp = XMFLOAT3(0, 0, -1);
	XMFLOAT2 uvTemp = XMFLOAT2(0, 0);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex tVertices[] = 
	{
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f), normTemp, uvTemp},
		{ XMFLOAT3(+0.75f, -0.5f, +0.0f), normTemp, uvTemp},
		{ XMFLOAT3(-0.75f, -0.5f, +0.0f), normTemp, uvTemp},
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	UINT tIndices[] = { 0, 1, 2 };	

	triangle = new Mesh(tVertices, 3, tIndices, 3, device);

	Vertex sVertices[] =
	{
		{ XMFLOAT3(-0.5f, +0.5f, +0.0f), normTemp, uvTemp},
		{ XMFLOAT3(+0.5f, +0.5f, +0.0f), normTemp, uvTemp},
		{ XMFLOAT3(+0.5f, -0.5f, +0.0f), normTemp, uvTemp},
		{ XMFLOAT3(-0.5f, -0.5f, +0.0f), normTemp, uvTemp},
	};

	UINT sIndices[] = { 0, 1, 2, 0, 2, 3 };

	square = new Mesh(sVertices, 4, sIndices, 6, device);

	Vertex pVertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f), normTemp, uvTemp},
		{ XMFLOAT3(+0.5f, +0.25f, +0.0f), normTemp, uvTemp},
		{ XMFLOAT3(+0.25f, -0.5f, +0.0f), normTemp, uvTemp},
		{ XMFLOAT3(-0.25f, -0.5f, +0.0f), normTemp, uvTemp},
		{ XMFLOAT3(-0.5f, +0.25f, +0.0f), normTemp, uvTemp},
	};

	UINT pIndices[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4 };

	pentagon = new Mesh(pVertices, 5, pIndices, 9, device);

	cone = new Mesh("Assets/Models/cone.obj", device);	
	cube = new Mesh("Assets/Models/cube.obj", device);
	ghost = new Mesh("Assets/Models/SpoopyGhost.obj", device);
	fencePillar = new Mesh("Assets/Models/FencePillar.obj", device);

	entities.push_back(new Entity(cone, mat));
	entities.push_back(new Entity(cube, mat2));
	entities.push_back(new Entity(ghost, mat));
	entities.push_back(new Entity(fencePillar, mat3));

	entities[0]->SetPositionVector(XMFLOAT3(-2.0f, 0.0f, 0.0f));
	entities[1]->SetPositionVector(XMFLOAT3(2.0f, 0.0f, 0.0f));
	entities[2]->SetPositionVector(XMFLOAT3(0.0f, 1.0f, -1.0f));
	entities[3]->SetPositionVector(XMFLOAT3(-5.0f, 0.0f, 0.0f));
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();
	camera->UpdateProjectionMatrix(width, height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();		
	if (GetAsyncKeyState('U') & 0x8000)
		ChangeState();

	entities[1]->Move(XMFLOAT3(sin(totalTime) * 3, 0.0f, 0.0f), deltaTime);
	
	player->MoveToward(XMFLOAT3(0.0f, 0.0f, 60.0f), 1.0f, deltaTime);
	player->UpdateCameraPos();

	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->ReconstructWorldMatrix();
	}

	camera->Update(deltaTime);
}

void Game::ChangeState()
{
	switch (state)
	{
		case start : 
		{
			state = game;
			printf("Game");
			break;
		}
		case game:	
		{
			state = end;
			printf("End");
			break;
		}
		case end:	
		{
			state = start;
			printf("Start");
			break;
		}
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Lights
	pixelShader->SetData(
		"dirLight",
		&dirLight,
		sizeof(DirectionalLight));
	pixelShader->SetData(
		"dirLight2",
		&dirLight2,
		sizeof(DirectionalLight));
	pixelShader->SetData(
		"pLight",
		&pLight,
		sizeof(PointLight));
	pixelShader->SetFloat3("CameraPosition", camera->GetPosition());

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	for (int i = 0; i < entities.size(); i++)
	{		
		entities[i]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix());	
		ID3D11Buffer* temp = entities[i]->GetMesh()->GetVertexBuffer();
		context->IASetVertexBuffers(0, 1, &temp, &stride, &offset);
		context->IASetIndexBuffer(entities[i]->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(
			entities[i]->GetMesh()->GetIndexCount(),
			0,
			0);
	}
	

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...	
	isDown = true;

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	isDown = false;

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...	
	if (isDown) {
		camera->Rotate(x - prevMousePos.x, y - prevMousePos.y);
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion