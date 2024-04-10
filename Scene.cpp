#include "Scene.h"

Scene::Scene(HINSTANCE hInstance) : D3D11Pipeline(hInstance)
{
}

Scene::~Scene()
{
}

void Scene::Initialize()
{
	//Init the Pipeline
	D3D11Pipeline::Initialize();

	/*
	* WireFrame
	D3D11_RASTERIZER_DESC wireframeRasterizerDesc = {};
	wireframeRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeRasterizerDesc.CullMode = D3D11_CULL_BACK;
	wireframeRasterizerDesc.FrontCounterClockwise = false;
	wireframeRasterizerDesc.DepthClipEnable = true;
	ThrowIfFailed(mDevice->CreateRasterizerState(&wireframeRasterizerDesc, this->mRasterizeState.GetAddressOf()));
	mImmediateContext->RSSetState(mRasterizeState.Get());
	*/

	//Create the Overlay GUI
	guiWrapper = std::make_unique<IMGuiWrapper>();
	guiWrapper->Initalize(WindowHandle, mDevice.Get(), mImmediateContext.Get());

	//Create a FreeLook Camera and Controls
	mPrimaryCamera = std::make_unique<FreeLookCamera>(WindowHeight, WindowWidth);
	mPrimaryCamera->SetPosition(2.58f, 5.51f, 20.02f);
	mFreeLookCamKeyHandler = std::make_unique<CameraControlHandler>(WinInstance, WindowHandle, mPrimaryCamera.get(), 1.0f, 0.0015f);

	//Load the 3D Model
	mAssimpLoader = std::make_unique<AssimpLoader>();
	mModel = std::make_unique<Model>();
	if (!mAssimpLoader->LoadModel(".\\Models\\windmill_obj\\windmill.obj", mModel))
		MessageBox(nullptr, L"Failed to load the 3D model.", L"Error", MB_ICONWARNING);

	//Build the Vertex and Index buffers
	mModel->BuildBuffers(mDevice.Get());
	mModel->BuildTextures(mDevice.Get(), mImmediateContext.Get(), ".\\Models\\windmill_obj\\textures");

	//Create a Directional Light
	mDirectionalLight = std::make_unique<DirectionalLight>();
	mDirectionalLight->CreateConstantBuffer(mDevice.Get());
	mDirectionalLight->mCBDirectionalLightData.position = DirectX::XMFLOAT3(0.0f, 55.0f, 0.0f);
	float ambientLightConstant = 0.1f;
	mDirectionalLight->mCBDirectionalLightData.ambient = DirectX::XMFLOAT3(ambientLightConstant, ambientLightConstant, ambientLightConstant);
	mDirectionalLight->mCBDirectionalLightData.diffuse = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);
	mDirectionalLight->mCBDirectionalLightData.specular = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);
	mDirectionalLight->mCBDirectionalLightData.ambientIntensity = 1.0f;
	mDirectionalLight->UpdateAndSetToShader(mImmediateContext.Get(), DIRECTIONAL_LIGHT_SLOT);
}

void Scene::Update()
{
	//Handle input and movment of FreeLook Camera
	mFreeLookCamKeyHandler->HandleInput();
	mPrimaryCamera->UpdateViewMatrix();
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX WVP = World * mPrimaryCamera->GetViewProjection();
	mCBPerVertexData.WVP = DirectX::XMMatrixTranspose(WVP);
	mImmediateContext->UpdateSubresource(mCBPerVertexBuffer.Get(), 0, nullptr, &mCBPerVertexData, 0, 0);
	mImmediateContext->VSSetConstantBuffers(PER_VERTEX_SLOT, 1, mCBPerVertexBuffer.GetAddressOf());

	//Send the view direction to the pixel shader for Blinn-Phong Reflection
	mCBViewData.ViewDirection = mPrimaryCamera->GetViewDirection();
	mImmediateContext->UpdateSubresource(mCBViewBuffer.Get(), 0, nullptr, &mCBViewData, 0, 0);
	mImmediateContext->PSSetConstantBuffers(VIEW_BUFFER_SLOT, 1, mCBViewBuffer.GetAddressOf());

	//Prepare the UI before Draw
	guiWrapper->Update();

	//Draw Light UI
	LightUI::DrawDirectionalLightUI(mDirectionalLight.get());
	mDirectionalLight->UpdateAndSetToShader(mImmediateContext.Get(), DIRECTIONAL_LIGHT_SLOT);

	//Draw Camera UI
	FreeLookCameraUI::DrawFreeLookCameraUI(mPrimaryCamera.get());
	mPrimaryCamera->UpdateViewMatrix();
}

void Scene::Draw()
{
	mImmediateContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	mImmediateContext->ClearRenderTargetView(mRenderTargetView.Get(), ClearColor);
	mImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	mModel->Draw(mImmediateContext.Get(), mConstantBuffers.get());
	guiWrapper->Draw();
	mSwapChain->Present(1, 0);
}