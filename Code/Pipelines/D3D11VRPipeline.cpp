#include "D3D11VRPipeline.h"

D3D11VRPipeline::D3D11VRPipeline(HINSTANCE hInstance) : D3D11BasePipeline(hInstance)
{
}

D3D11VRPipeline::~D3D11VRPipeline()
{
}

void D3D11VRPipeline::Initialize()
{
    D3D11BasePipeline::Initialize();
    vr::EVRInitError eError = vr::VRInitError_None;

    mVRSystem = vr::VR_Init(&eError, vr::VRApplication_Scene);

    if (eError != vr::VRInitError_None)
    {
        MessageBox(nullptr, L"Failed to initalize OpenVR Runtime", L"OpenVR", 0);
    }

    mRenderModels = (vr::IVRRenderModels *)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);
    if (!mRenderModels)
    {
        MessageBox(nullptr, L"Failed Retreving IVRRenderModels", L"OpenVR", 0);
    }

    if (!vr::VRCompositor())
    {
        MessageBox(nullptr, L"Failed Retreving VRCompositor", L"OpenVR", 0);
    }

    vr::VRCompositor()->WaitGetPoses(nullptr, 0, nullptr, 0);

    CreateTextures();
}

void D3D11VRPipeline::CreateTextures()
{
    uint32_t recommendedWidth, recommendedHeight;
    mVRSystem->GetRecommendedRenderTargetSize(&recommendedWidth, &recommendedHeight);

    D3D11_TEXTURE2D_DESC eyeTextureDesc = {};
    eyeTextureDesc.Width = recommendedWidth;
    eyeTextureDesc.Height = recommendedHeight;
    eyeTextureDesc.MipLevels = 1;
    eyeTextureDesc.ArraySize = 1;
    eyeTextureDesc.SampleDesc.Count = 1;
    eyeTextureDesc.SampleDesc.Quality = 0;
    eyeTextureDesc.Usage = D3D11_USAGE_DEFAULT;
    eyeTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
    eyeTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    ThrowIfFailed(mDevice->CreateTexture2D(&eyeTextureDesc, nullptr, mLeftEyeTexture.GetAddressOf()));
    ThrowIfFailed(mDevice->CreateTexture2D(&eyeTextureDesc, nullptr, mRightEyeTexture.GetAddressOf()));

    D3D11_RENDER_TARGET_VIEW_DESC eyeRTVDesc = {};
    eyeRTVDesc.Format = eyeTextureDesc.Format;
    eyeRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    eyeRTVDesc.Texture2D.MipSlice = 0;
    ThrowIfFailed(mDevice->CreateRenderTargetView(mLeftEyeTexture.Get(), &eyeRTVDesc, mLeftEyeRenderTargetView.GetAddressOf()));
    ThrowIfFailed(mDevice->CreateRenderTargetView(mRightEyeTexture.Get(), &eyeRTVDesc, mRightEyeRenderTargetView.GetAddressOf()));

    //Override Viewport
    mViewport.TopLeftX = 0;
    mViewport.TopLeftY = 0;
    mViewport.Width = static_cast<float>(recommendedWidth);
    mViewport.Height = static_cast<float>(recommendedHeight);
    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;
    mImmediateContext->RSSetViewports(1, &mViewport);

    //Override Depth Stencil
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = recommendedWidth;
    depthStencilDesc.Height = recommendedHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;
    ThrowIfFailed(mDevice->CreateTexture2D(&depthStencilDesc, 0, mDepthStencilBuffer.GetAddressOf()));
    ThrowIfFailed(mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), 0, mDepthStencilView.GetAddressOf()));
}

int D3D11VRPipeline::Run()
{
    MSG msg = { 0 };

    mTimer->Reset();
    mTimer->Start();

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Update();
        Draw();
    }
    mTimer->Stop();
    return (int)msg.wParam;
}

DirectX::XMMATRIX D3D11VRPipeline::GetEyeToHeadTransform(vr::Hmd_Eye eye)
{
    if (!mVRSystem)
        return DirectX::XMMATRIX();

    vr::HmdMatrix34_t matrixEye = mVRSystem->GetEyeToHeadTransform(eye);

    DirectX::XMMATRIX matrix(
        matrixEye.m[0][0], matrixEye.m[1][0], matrixEye.m[2][0], 0.0f,
        matrixEye.m[0][1], matrixEye.m[1][1], matrixEye.m[2][1], 0.0f,
        matrixEye.m[0][2], matrixEye.m[1][2], matrixEye.m[2][2], 0.0f,
        matrixEye.m[0][3], matrixEye.m[1][3], matrixEye.m[2][3], 1.0f
    );

    return DirectX::XMMatrixInverse(nullptr, matrix);
}

DirectX::XMMATRIX D3D11VRPipeline::GetProjectionMatrix(vr::Hmd_Eye eye, float nearZ, float farZ)
{
    if (!mVRSystem)
        return DirectX::XMMATRIX();

    vr::HmdMatrix44_t mat = mVRSystem->GetProjectionMatrix(eye, nearZ, farZ);

    DirectX::XMMATRIX matrix(
        mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
        mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
        mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
        mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
    );

    return matrix;
}

DirectX::XMMATRIX D3D11VRPipeline::ConvertOpenVRMatrix(const vr::HmdMatrix34_t& openVRMatrix)
{
    DirectX::XMMATRIX matrix(
        openVRMatrix.m[0][0], openVRMatrix.m[1][0], openVRMatrix.m[2][0], 0.0f,
        openVRMatrix.m[0][1], openVRMatrix.m[1][1], openVRMatrix.m[2][1], 0.0f,
        openVRMatrix.m[0][2], openVRMatrix.m[1][2], openVRMatrix.m[2][2], 0.0f,
        openVRMatrix.m[0][3], openVRMatrix.m[1][3], openVRMatrix.m[2][3], 1.0f
    );
    return matrix;
}

void D3D11VRPipeline::UpdateMatrices()
{
    mEyePosLeft = GetEyeToHeadTransform(vr::Eye_Left);
    mEyePosRight = GetEyeToHeadTransform(vr::Eye_Right);

    float nearZ = 0.1f;
    float farZ = 10000.0f;
    mProjectionLeft = GetProjectionMatrix(vr::Eye_Left, nearZ, farZ);
    mProjectionRight = GetProjectionMatrix(vr::Eye_Right, nearZ, farZ);

    mRightDirection = DirectX::XMVectorSet(mHMDPose.r[0].m128_f32[0], mHMDPose.r[1].m128_f32[0], mHMDPose.r[2].m128_f32[0], 0.0f);
    mUpDirection = DirectX::XMVectorSet(mHMDPose.r[0].m128_f32[1], mHMDPose.r[1].m128_f32[1], mHMDPose.r[2].m128_f32[1], 0.0f);
    mFrontDirection = DirectX::XMVectorSet(mHMDPose.r[0].m128_f32[2], mHMDPose.r[1].m128_f32[2], mHMDPose.r[2].m128_f32[2], 0.0f);
    mTranslation = DirectX::XMMatrixTranslationFromVector(mPosition);

    mOrientationMatrix = DirectX::XMMatrixTranspose(DirectX::XMMATRIX(
        mRightDirection,
        mUpDirection,
        mFrontDirection,
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)
    ));
}

DirectX::XMMATRIX D3D11VRPipeline::GetCurrentViewProjectionMatrix(vr::Hmd_Eye eye)
{
    DirectX::XMMATRIX viewProjectionMatrix;

    if (eye == vr::Eye_Left)
    {
        return viewProjectionMatrix = (mEyePosLeft * mHMDPose) * mProjectionLeft ;
    }
    else if (eye == vr::Eye_Right)
    {
        return viewProjectionMatrix = (mEyePosRight * mHMDPose) * mProjectionRight ;
    }
}


void D3D11VRPipeline::VRUpdate()
{
    if (!mVRSystem)
        return;

    vr::VRCompositor()->WaitGetPoses(mTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, nullptr, 0);

    mValidPoseCount = 0;
    mStrPoseClasses = "";

    for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
    {
        if (mTrackedDevicePose[nDevice].bPoseIsValid)
        {
            mValidPoseCount++;
            mDevicePose[nDevice] = ConvertOpenVRMatrix(mTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);

            if (mDevClassChar[nDevice] == 0)
            {
                switch (mVRSystem->GetTrackedDeviceClass(nDevice))
                {
                case vr::TrackedDeviceClass_Controller:        mDevClassChar[nDevice] = 'C'; break;
                case vr::TrackedDeviceClass_HMD:               mDevClassChar[nDevice] = 'H'; break;
                case vr::TrackedDeviceClass_Invalid:           mDevClassChar[nDevice] = 'I'; break;
                case vr::TrackedDeviceClass_GenericTracker:    mDevClassChar[nDevice] = 'O'; break;
                case vr::TrackedDeviceClass_TrackingReference: mDevClassChar[nDevice] = 'T'; break;
                default:                                       mDevClassChar[nDevice] = '?'; break;
                }
            }
            mStrPoseClasses += mDevClassChar[nDevice];
        }
    }

    if (mTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
    {
        mHMDPose = mDevicePose[vr::k_unTrackedDeviceIndex_Hmd];
        mHMDPose = DirectX::XMMatrixInverse(nullptr, mHMDPose);
    }

    UpdateMatrices();
}