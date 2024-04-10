#include "MainWindow.h"

namespace
{
	MainWindow* WinPtr = nullptr;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return WinPtr->MsgProc(hwnd, msg, wParam, lParam);
}

extern LRESULT(ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam));
LRESULT MainWindow::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

MainWindow::MainWindow(HINSTANCE hInstance)
{
	WinInstance = hInstance;
	WindowHandle = nullptr;
	WinPtr = this;
}

MainWindow::~MainWindow()
{
}

bool MainWindow::Initialize()
{
	WNDCLASS wc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = WinInstance;
	wc.lpfnWndProc = MainWndProc;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"WindowApplication";

	if (!RegisterClass(&wc))
		return false;

	WindowHandle = CreateWindow(L"WindowApplication", Title.c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, WindowWidth, WindowHeight,
		0, 0, WinInstance, 0);

	if (!WindowHandle)
		return false;

	ShowWindow(WindowHandle, SW_SHOW);
	return true;
}

int MainWindow::Run()
{
	MSG msg = { 0 };

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}