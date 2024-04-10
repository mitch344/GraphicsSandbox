#include "IMGuiWrapper.h"

void IMGuiWrapper::Initalize(HWND windowHandle, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(windowHandle);
	ImGui_ImplDX11_Init(device, deviceContext);
	ImGui::StyleColorsClassic();
}

void IMGuiWrapper::Update()
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDrawCursor = true;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void IMGuiWrapper::Draw()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}