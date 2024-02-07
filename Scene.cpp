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

	//Create the Overlay GUI
	guiWrapper = std::make_unique<IMGuiWrapper>();
	guiWrapper->Initalize(WindowHandle, mDevice.Get(), mImmediateContext.Get());

	//Create a FreeLook Camera and Controls
	mPrimaryCamera = std::make_unique<FreeLookCamera>(WindowHeight, WindowWidth);
	mPrimaryCamera->SetPosition(2.58f, 1.51f, -1.02f);
	mFreeLookCamKeyHandler = std::make_unique<CameraControlHandler>(WinInstance, WindowHandle, mPrimaryCamera.get(), 0.09f, 0.0015f);

	//Load the 3D Model
	mAssimpLoader = std::make_unique<AssimpLoader>();
	mModel = std::make_unique<Model>();
	if (!mAssimpLoader->LoadModel(".\\Models\\Range Rover\\2016 Custom Range Rover Sport.obj", mModel))
		MessageBox(nullptr, L"Failed to load the 3D model.", L"Error", MB_ICONWARNING);

	//Build the Vertex and Index buffers
	mModel->BuildBuffers(mDevice.Get());
	mModel->BuildTextures(mDevice.Get(), mImmediateContext.Get(), ".\\Models\\model_folder_name\\texture_folder_name");

	//Create a Directional Light
	mDirectionalLight = std::make_unique<DirectionalLight>();
	mDirectionalLight->CreateConstantBuffer(mDevice.Get());
	mDirectionalLight->mCBDirectionalLightData.position = DirectX::XMFLOAT3(14.737f, 12.632f, -4.210f);
	mDirectionalLight->mCBDirectionalLightData.ambient = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	mDirectionalLight->mCBDirectionalLightData.diffuse = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);
	mDirectionalLight->mCBDirectionalLightData.specular = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
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

	//Create controls for the direction light so we adjust in realtime
	LightUI::DrawDirectionalLightUI(mDirectionalLight.get());
	mDirectionalLight->UpdateAndSetToShader(mImmediateContext.Get(), DIRECTIONAL_LIGHT_SLOT);
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