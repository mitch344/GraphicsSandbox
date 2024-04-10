#pragma once
#include "openvr.h"
#include "../Pipelines/D3D11Pipeline.h"
#include "../Cameras/FreeLookCamera.h"

class D3D11VRPipeline : public D3D11BasePipeline
{
public:
	D3D11VRPipeline(HINSTANCE);
	virtual ~D3D11VRPipeline();
	void Initialize();
	virtual void Update() = 0;
	virtual void Draw() = 0;
	int Run();

private:
	void CreateTextures();
	void UpdateMatrices();

protected:

	void VRUpdate();

	//Matricies
	DirectX::XMMATRIX mHMDPose;
	DirectX::XMMATRIX mEyePosLeft;
	DirectX::XMMATRIX mEyePosRight;
	DirectX::XMMATRIX mProjectionCenter;
	DirectX::XMMATRIX mProjectionLeft;
	DirectX::XMMATRIX mProjectionRight;
	DirectX::XMMATRIX mOrientationMatrix;
	DirectX::XMMATRIX mTranslation;

	//Vectors
	DirectX::XMVECTOR mPosition;
	DirectX::XMVECTOR mRightDirection;
	DirectX::XMVECTOR mUpDirection;
	DirectX::XMVECTOR mFrontDirection;

	//OpenVR
	vr::IVRSystem* mVRSystem;
	vr::IVRRenderModels* mRenderModels;
	vr::TrackedDevicePose_t mTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	DirectX::XMMATRIX mDevicePose[vr::k_unMaxTrackedDeviceCount];

	//OpenVR Methods/Helpers
	DirectX::XMMATRIX GetEyeToHeadTransform(vr::Hmd_Eye eye);
	DirectX::XMMATRIX GetProjectionMatrix(vr::Hmd_Eye eye, float nearZ, float farZ);
	DirectX::XMMATRIX GetCurrentViewProjectionMatrix(vr::Hmd_Eye eye);
	DirectX::XMMATRIX ConvertOpenVRMatrix(const vr::HmdMatrix34_t& openVRMatrix);

	//Tracking
	std::string mStrPoseClasses;
	char mDevClassChar[vr::k_unMaxTrackedDeviceCount];
	int mTrackedControllerCount;
	int mTrackedControllerCount_Last;
	int mValidPoseCount;
	int mValidPoseCount_Last;

	//Texture and Render Targets for Both Eyes
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mLeftEyeTexture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mRightEyeTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mLeftEyeRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRightEyeRenderTargetView;
};