#include "CameraControlHandler.h"

CameraControlHandler::CameraControlHandler(HINSTANCE WinInstance, HWND WindowHandle, FreeLookCamera* camera, float keyBoardSpeed, float mouseSpeed)
    : mCamera(camera), mCameraSpeed(keyBoardSpeed), mMouseSpeed(mouseSpeed)
{
    mMouse = std::make_unique<Mouse>();
    mMouse->Initialize(WinInstance, WindowHandle);
}

void CameraControlHandler::HandleInput()
{
    if (GetAsyncKeyState('W') & 0x8000) {
        mCamera->MoveBackForward += mCameraSpeed;
    }
    if (GetAsyncKeyState('A') & 0x8000) {
        mCamera->MoveLeftRight -= mCameraSpeed;
    }
    if (GetAsyncKeyState('S') & 0x8000) {
        mCamera->MoveBackForward -= mCameraSpeed;
    }
    if (GetAsyncKeyState('D') & 0x8000) {
        mCamera->MoveLeftRight += mCameraSpeed;
    }

    if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
    {
        mMouse->GetState();
        if (mMouse->ChangedPosition())
        {
            mCamera->Yaw += mMouse->GetCurrentStateX() * mMouseSpeed;
            mCamera->Pitch += mMouse->GetCurrentStateY() * mMouseSpeed;
            mMouse->SetLastState(mMouse->GetCurrentState());
        }
    }
}