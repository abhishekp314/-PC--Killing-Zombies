#include "GameObjects.h"
#include<vector>

using namespace std;

class EnemyManager
{
public:
	D3DXVECTOR3 respawnLoc[10];
	int noRespawnLoc,noEnemyObject,totDamage;
	vector<EnemyMinion *> EnemyObjects;
	LPDIRECT3DDEVICE9 dev;
	//AI Variables
	D3DXVECTOR3 followPosition,oldFollowPosition;
	ID3DXFont* font;
	RECT rect;
	char *msg;

	EnemyManager(LPDIRECT3DDEVICE9 device)
	{
		//Set Respawn Loactions
		dev=device;
		noRespawnLoc=0;
		noEnemyObject=0;

		FILE *respFile;
		respFile=fopen("RespawnLocation.txt","r");

			float f1=0,f2=0,f3=0;
			while(fscanf(respFile,"%f %f %f\n",&f1,&f2,&f3)!=EOF)
			{
					respawnLoc[noRespawnLoc++]=D3DXVECTOR3(f1,f2,f3);
					AddEnemyObject();
			}
			fclose(respFile);

			followPosition=D3DXVECTOR3(0,0,0);
			totDamage=0;

			D3DXFONT_DESC logfont={24,
                          0,
                          400,
                          0,
                          false,
                          DEFAULT_CHARSET,
                          OUT_TT_PRECIS,
                          CLIP_DEFAULT_PRECIS,
                          DEFAULT_PITCH,
                          "Calibri"};
	font=0;
	D3DXCreateFontIndirectA(dev,&logfont,&font);
	
	rect.top=350;
	rect.left=400;
	rect.bottom=450;
	rect.right=700;
	msg="";
	}

	~EnemyManager()
	{
		EnemyObjects.clear();
		EnemyObjects.~vector();
	}

	void AddEnemyObject();
	void DisableEnemyObject(int id);
	void RemoveEnemyObject();
	void Update();
	void setFollowCharacter(D3DXVECTOR3);
};