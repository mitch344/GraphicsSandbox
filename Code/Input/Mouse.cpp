#include "Mouse.h"

Mouse::Mouse()
{
	DirectInput = 0;
	Device = 0;
}

Mouse::~Mouse()
{
	if (Device != 0)
	{
		Device->Unacquire();
		Device->Release();
		Device = 0;
	}
}

void Mouse::Initialize(HINSTANCE hInstance, HWND hWnd)
{
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, nullptr);
	DirectInput->CreateDevice(GUID_SysMouse, &Device, 0);
	Device->SetDataFormat(&c_dfDIMouse);
	Device->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	Device->Acquire();
}

void Mouse::GetState()
{
	if (Device != 0)
	{
		memcpy(&LastState, &CurrentState, sizeof(CurrentState));

		if (FAILED(Device->GetDeviceState(sizeof(CurrentState), &CurrentState)))
		{
			if (SUCCEEDED(Device->Acquire()))
				Device->GetDeviceState(sizeof(CurrentState), &CurrentState);
		}
	}
}

bool Mouse::ChangedPosition()
{
	if ((CurrentState.lX != LastState.lX) || (CurrentState.lY != LastState.lY))
		return true;
}

LONG Mouse::GetCurrentStateX() const
{
	return CurrentState.lX;
}

LONG Mouse::GetCurrentStateY() const
{
	return CurrentState.lY;
}

LONG Mouse::GetCurrentStateZ() const
{
	return CurrentState.lZ;
}

DIMOUSESTATE Mouse::GetCurrentState() const
{
	return CurrentState;
}

void Mouse::SetLastState(const DIMOUSESTATE LastState)
{
	this->LastState = LastState;
}