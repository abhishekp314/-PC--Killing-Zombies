
#include <d3d9.h>
#include <DirectXCollision.h>
#include <Windows.h>
#include <stdlib.h>
#include <WinUser.h>
#include <time.h>
#include "Camera.h"
#include "GameObjects.h"
#include "InputManager.h"
#include "EnemyManager.h"
#include <vector>

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)??1:0)

using namespace std;
using namespace DirectX;

class Game
{
public:
	HWND hWnd;
	
	//BIG BOSS IDirect3D
	LPDIRECT3D9 d3d;
	//IDirect3DDevice is the video card reference
	LPDIRECT3DDEVICE9 d3ddev;

	//Input Manager
	InputManager *inputManager;

	//GAME OBJECTS WITH MESH
	MainCharacter *mainCharacter;
	MainCharacter *tunnel,*city,*church,*playground;

	//CrossHair
	CrossHair *crosshair;

	//Bullets
	vector<Bullets *> ak47_bullet;
	int ak47BulletCount,availBullet,availBulletCount;

	//First Person Camera
	FirstPersonShooter *FPS;

	//Enemy Manager- Creates Enemy Models, Updates, Draws and AI
	EnemyManager *enemyManager;

	//SkyGlobe
	MainCharacter *skyglobe;

	//Font Variables
	ID3DXFont* font;
	char* noBullet;
	RECT rect;

	//GAME HANDLER
	BOOL GAME_OVER;

	//Bounds object
	MainCharacter *left,*right;

	Game();
	~Game();

	bool Game_Init(HWND);
	void Game_Update();
	void Game_LoadContent();
	void Game_Shutdown();
	void Game_Input();
	BOOL checkCollision(D3DXVECTOR3 pos,D3DXVECTOR3 look,int bulletNo);
	void Insert_to_file();
};
