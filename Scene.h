#pragma once
#include <memory>

#include "./Code/Pipelines/D3D11Pipeline.h"

#include "./Code/AssimpLoader/AssimpLoader.h"
#include "./Code/ModelGeometry/Model.h"

#include "./Code/Lights/DirectionalLight.h"

#include "./Code/Layouts/ResourceSlot.h"

#include "./Code/Cameras/FreeLookCamera.h"

#include "./Code/Input/Mouse.h"
#include "CameraControlHandler.h"

#include "./Code/UI/Overlay/IMGuiWrapper.h"
#include "./Code/UI/Overlay/LightUI.h"
#include "./Code/UI/Overlay/FreeLookCameraUI.h"

class Scene :public D3D11Pipeline
{
public:
	Scene(HINSTANCE hInstance);
	~Scene();
	void Initialize();
	void Update();
	void Draw();

	std::unique_ptr<FreeLookCamera> mPrimaryCamera;
	std::unique_ptr<CameraControlHandler> mFreeLookCamKeyHandler;

	std::unique_ptr<Mouse> mMouse;

	std::unique_ptr<Model> mModel;
	std::unique_ptr<AssimpLoader> mAssimpLoader;

	std::unique_ptr<DirectionalLight> mDirectionalLight;

	std::unique_ptr<IMGuiWrapper> guiWrapper;
};