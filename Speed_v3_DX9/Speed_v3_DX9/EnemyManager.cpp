#include "EnemyManager.h"
#include <iostream>


void EnemyManager::AddEnemyObject()
{
	//EnemyObjects.push_back(new EnemyMinion(dev,"Content/Models/Android/Android.X","Android"));
	EnemyObjects.push_back(new EnemyMinion(dev,"Content/Models/EnemyCharacter/character.X","EnemyCharacter"));
	EnemyObjects.at(noEnemyObject)->SetWorld(D3DXVECTOR3(0,0,0),D3DXVECTOR3(10,10,10),respawnLoc[noEnemyObject]);
	EnemyObjects.at(noEnemyObject)->LoadContent();
	EnemyObjects.at(noEnemyObject)->init();
	noEnemyObject++;

}

void EnemyManager::DisableEnemyObject(int id)
{
	EnemyObjects.at(id)->rotate_to_death();
}

void EnemyManager::RemoveEnemyObject()
{
}

void EnemyManager::Update()
{

	for(int i=0;i<noEnemyObject;i++)
	{
		if(EnemyObjects.at(i)->disabled==false)
		{
		EnemyObjects.at(i)->AI(followPosition,i);
		int d=EnemyObjects.at(i)->setDamage;
		if(d!=0)
		{
			totDamage+=d;
			EnemyObjects.at(i)->setDamage=0;
			msg="AH!";
		}
		font->DrawText(NULL,msg,-1,&rect,DT_TOP|DT_LEFT,0xffffffff);
		}

		EnemyObjects.at(i)->Update();
		EnemyObjects.at(i)->drawEnemyBullet();
	}

}

void EnemyManager::setFollowCharacter(D3DXVECTOR3 pos)
{

	followPosition=pos;
}