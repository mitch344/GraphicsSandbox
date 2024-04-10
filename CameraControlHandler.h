#pragma once
#include <windows.h>
#include <memory>

#include "./Code/Cameras/FreeLookCamera.h"
#include "./Code/Input/Mouse.h"

class CameraControlHandler
{
public:
    CameraControlHandler(HINSTANCE WinInstance, HWND WindowHandle, FreeLookCamera* camera, float keyBoardSpeed, float mouseSpeed);
    void HandleInput();

public:
    FreeLookCamera* mCamera;
    float mCameraSpeed;
    float mMouseSpeed;
    std::unique_ptr<Mouse> mMouse;
};