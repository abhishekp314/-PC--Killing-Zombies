
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include "Game.h"


#define APPTITLE "Speed v3.0 DX9"


//Function Prototypes
BOOL InitApp(HINSTANCE,int);
ATOM MyRegisterClass(HINSTANCE);
LRESULT CALLBACK WinProc(HWND,UINT,WPARAM,LPARAM);
HWND global_hwnd;
HDC global_hdc;

//NEW GAME
Game game;


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE prevInstance,LPSTR cmdLine,int showCMD)
{	
	MSG msg;

	//Register a new window
	MyRegisterClass(hInstance);

	//Init the app
	if(!InitApp(hInstance,showCMD))
	return FALSE;

	srand(time(NULL));
	
	//Init the game
	if(!game.Game_Init(global_hwnd))
	return 0;

	//main msg loop
	int done=1;
	while(done)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{	
			if(msg.message==WM_QUIT)
			{
				
				MessageBox(global_hwnd,"Exiting...","Speed v3 DX9",MB_OK);
				done=0;
			}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}
		else
		//Run Game Update and Draw
		game.Game_Update();
	}


	return msg.wParam;
}

//Windows Procedure
LRESULT CALLBACK WinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		game.Game_Shutdown();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);

}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	wc.cbSize=sizeof(WNDCLASSEX);

	//FILL STRUCTURE
	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=(WNDPROC)WinProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=hInstance;
	wc.hIcon=NULL;
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName=NULL;
	wc.lpszClassName=APPTITLE;
	wc.hIconSm=NULL;

	return RegisterClassEx(&wc);
}


BOOL InitApp(HINSTANCE hInstance,int nCmdShow)
{
	HWND hWnd;

	hWnd=CreateWindow(
		APPTITLE,
		APPTITLE,
		WS_EX_TOPMOST|WS_VISIBLE|WS_POPUP,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if(!hWnd)
	return FALSE;

	global_hwnd=hWnd;
	global_hdc=GetDC(hWnd);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}