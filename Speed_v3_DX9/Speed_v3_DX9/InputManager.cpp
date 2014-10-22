#include "InputManager.h"
#include<iostream>

#define BUTTON_DOWN(obj,button) (obj.rgbButtons[button]&0x80)

InputManager::InputManager(HWND h)
{
	hwnd=h;
	HRESULT result=DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL);

	if(result!=DI_OK)
	{
		MessageBox(hwnd,"Input prob","Error",MB_OK);
		return;
	}
	//Init mouse

	result=dinput->CreateDevice(GUID_SysMouse,&dimouse,NULL);
	if(result!=DI_OK)
	{
		MessageBox(hwnd,"MOuse prob","Error",MB_OK);
		return;
	}

}

InputManager::~InputManager()
{
	if(dimouse!=NULL)
	{
		dimouse->Unacquire();
		dimouse->Release();
		dimouse=NULL;
	}
}

int InputManager::Init_Mouse()
{
	HRESULT result=dimouse->SetDataFormat(&c_dfDIMouse);
	if(result!=DI_OK)
		return 0;

	result=dimouse->SetCooperativeLevel(hwnd,DISCL_EXCLUSIVE|DISCL_FOREGROUND);

	if(result!=DI_OK)
	{
		MessageBox(hwnd,"Coop prob","Error",MB_OK);
		return 0;
	}
	//acquire mouse

	result=dimouse->Acquire();

	if(result!=DI_OK)
	{
		MessageBox(hwnd,"Acquiring prob","Error",MB_OK);
		return 0;
	}
	return 1;
}

float xpos=0;

int InputManager::GetMouseX()
{
	return mouse_state.lX;
}

int InputManager::GetMouseY()
{
	return mouse_state.lY;
}

int InputManager::Mouse_Button(int button)
{
	return mouse_state.rgbButtons[button];
}


void InputManager::CheckMouseInput()
{
	dimouse->GetDeviceState(sizeof(mouse_state),(LPVOID)&mouse_state);
}



