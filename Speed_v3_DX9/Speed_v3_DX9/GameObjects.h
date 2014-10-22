#include<d3dx9.h>
#include "Model.h"

#ifndef _Game_Objects_H
#define _Game_Objects_H

class MainCharacter
{
	
public:
	D3DXMATRIX world,rotation,scaling,translation;
	D3DXVECTOR3 position;
	int health;
	LPDIRECT3DDEVICE9 d3ddev;
	Model *myModel;
	char *filename;
	char *folder;

	MainCharacter(LPDIRECT3DDEVICE9 dev,char *filename,char *folder);
	~MainCharacter();

	void LoadContent();
	void Update();

	void SetWorld(D3DXVECTOR3 rotAngle,D3DXVECTOR3 scale,D3DXVECTOR3 position);
	D3DXMATRIX GetWorld();

	void AttachToCam(D3DXMATRIX v,D3DXVECTOR3 p,D3DXVECTOR3 r,D3DXVECTOR3 l,D3DXVECTOR3 up,float angleOfYaw,float angleOfPitch);
	
};




class CrossHair:public MainCharacter
{
public:
	CrossHair(LPDIRECT3DDEVICE9 dev,char *filename,char *folder):MainCharacter(dev,filename,folder)
	{
	}
	~CrossHair()
	{
	
	}
	void AttachToLookAxis(D3DXVECTOR3 p,D3DXVECTOR3 l);
};

class Bullets:public MainCharacter
{
public:
	D3DXVECTOR3 look;
	float stopConstraint,speed;
	D3DXVECTOR3 position;

	Bullets(D3DXVECTOR3 Position,D3DXVECTOR3 Look,float Speed,LPDIRECT3DDEVICE9 dev,char *filename,char *folder):MainCharacter(dev,filename,folder)
	{
		position=Position;
		look=Look;
		speed=Speed;
		stopConstraint=0.0f;
		position=position+look*1;
	}
	~Bullets()
	{
		
	}
	void AttachToLookAxis();
};


//class enemy characters

class EnemyMinion:public MainCharacter
{
public:
	BOOL disabled,bulletDisabled;
	D3DXVECTOR3 enemyLook,oldFollowPosition,enemyForward;
	Bullets *enemyBullet;
	int damage[10],setDamage;
	D3DXVECTOR3 leftBound,rightBound;

	EnemyMinion(LPDIRECT3DDEVICE9 dev,char *filename,char *folder):MainCharacter(dev,filename,folder)
	{
		disabled=false;
		enemyLook=D3DXVECTOR3(0,0,-1);
		oldFollowPosition=enemyLook;
		enemyForward=D3DXVECTOR3(0,0,1);

		bulletDisabled=false;
		enemyBullet=new Bullets(D3DXVECTOR3(0,0,0),D3DXVECTOR3(0,0,-1),2,d3ddev,"Content/Models/Crosshair/Crosshair_Sphere.X","Crosshair");
		enemyBullet->SetWorld(D3DXVECTOR3(0,0,0),D3DXVECTOR3(2,2,2),D3DXVECTOR3(0,0,0));
		enemyBullet->LoadContent();
		memset(damage,0,sizeof(int));

		leftBound=position;
		rightBound=position;
	}
	~EnemyMinion()
	{
	}
	float Vec3CosineAngle(D3DXVECTOR3 v1,D3DXVECTOR3 v2,float angle,float prevCoAngle);
	void rotate_to_death();
	void AI(D3DXVECTOR3,int id);
	void init();
	void AIhitBullets(float distance,D3DXVECTOR3,D3DXVECTOR3);
	void drawEnemyBullet();
};

#endif