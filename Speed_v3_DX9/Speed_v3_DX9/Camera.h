#include <d3dx9.h>
#include <Windows.h>
#include <stdlib.h>
#include <WinUser.h>
#include <time.h>
//#include "GameObjects.h"

class FirstPersonShooter
{
	LPDIRECT3DDEVICE9 d3ddev;
	
public: 
	FirstPersonShooter(LPDIRECT3DDEVICE9 dev);
	~FirstPersonShooter();

	void SetCamera(D3DXVECTOR3 source,D3DXVECTOR3 look);
	void SetPerspective(float,float,float,float);
	void SetSource(D3DXVECTOR3);
	void GetSource();


	//Transforming Cam Methods

	void getViewMatrix(D3DXMATRIX* V);
	void strafe(float units);
	void walk(float units);
	void yaw(float angle);
	void pitch(float angle);
	void getPosition(D3DXVECTOR3 *position);
	void setPosition(D3DXVECTOR3 *position);
	void getRight(D3DXVECTOR3 *right);
	void getUp(D3DXVECTOR3 *up);
	void getLook(D3DXVECTOR3 *look);

	void Update();
public:
	D3DXMATRIX matView;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;
	D3DXVECTOR3 look; D3DXVECTOR3 oldlook;
	D3DXVECTOR3 position;
	float angleOfYaw,angleOfPitch;

	friend float Vec3CosineAngle(D3DXVECTOR3 v1,D3DXVECTOR3 v2,float angle,float prevCoAngle);
};