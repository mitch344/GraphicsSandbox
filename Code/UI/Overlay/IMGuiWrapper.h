#pragma once
#include "d3d11.h"
#include "Windows.h"

#include "../../imgui-1.90.1/imgui_impl_dx11.h"
#include "../../imgui-1.90.1/imgui_impl_win32.h"
#include "../../imgui-1.90.1/imgui.h"

class IMGuiWrapper
{
public:
	void Initalize(HWND windowHandle, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Update();
	void Draw();
};