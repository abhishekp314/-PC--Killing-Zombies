#include <dinput.h>

class InputManager
{
public:
	LPDIRECTINPUT8 dinput;
	LPDIRECTINPUTDEVICE8 dimouse;
	DIMOUSESTATE mouse_state;

	HWND hwnd;
	InputManager(HWND hwnd);
	~InputManager();

	int Init_Mouse();

	void CheckMouseInput();

	int GetMouseX();
	int GetMouseY();
	int Mouse_Button(int);
};