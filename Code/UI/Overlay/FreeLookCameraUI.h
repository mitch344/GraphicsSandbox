#pragma once
#include "./Code/Cameras/FreeLookCamera.h"

#include "./Dependencies/imgui-1.90.1/imgui_impl_dx11.h"
#include "./Dependencies/imgui-1.90.1/imgui_impl_win32.h"
#include "./Dependencies/imgui-1.90.1/imgui.h"

class FreeLookCameraUI
{
public:
    static void DrawFreeLookCameraUI(FreeLookCamera* freeLookCamera)
    {
        ImGui::Begin("Free Look Camera");

        // Position
        ImGui::PushID("Position");
        ImGui::Text("X");
        ImGui::SameLine();
        ImGui::SliderFloat("##X", &freeLookCamera->Position.m128_f32[0], -100.0f, 100.0f);
        ImGui::Text("Y");
        ImGui::SameLine();
        ImGui::SliderFloat("##Y", &freeLookCamera->Position.m128_f32[1], -100.0f, 100.0f);
        ImGui::Text("Z");
        ImGui::SameLine();
        ImGui::SliderFloat("##Z", &freeLookCamera->Position.m128_f32[2], -100.0f, 100.0f);
        ImGui::PopID();

        // Rotation
        ImGui::SliderAngle("Yaw", &freeLookCamera->Yaw, -180.0f, 180.0f);
        ImGui::SliderAngle("Pitch", &freeLookCamera->Pitch, -90.0f, 90.0f);

        // Movement
        ImGui::SliderFloat("Move Left/Right", &freeLookCamera->MoveLeftRight, -10.0f, 10.0f);
        ImGui::SliderFloat("Move Back/Forward", &freeLookCamera->MoveBackForward, -10.0f, 10.0f);

        // View Frustum
        ImGui::SliderAngle("Field of View (Y)", &freeLookCamera->FovY, 1.0f, 179.0f);
        ImGui::SliderFloat("Aspect Ratio", &freeLookCamera->Aspect, 0.1f, 10.0f);
        ImGui::SliderFloat("Near Plane", &freeLookCamera->NearZ, 0.01f, 10.0f);
        ImGui::SliderFloat("Far Plane", &freeLookCamera->FarZ, 10.0f, 1000.0f);
        freeLookCamera->UpdateViewFrustrum();

        ImGui::End();
    }
};