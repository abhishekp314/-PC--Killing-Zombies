#include "GameObjects.h"
#include<iostream>



MainCharacter::MainCharacter(LPDIRECT3DDEVICE9 dev,char *file,char *fold)
{
	d3ddev=dev;
	myModel=new Model(d3ddev);
	filename=file;
	folder=fold;
	health=100;
}

MainCharacter::~MainCharacter()
{
	myModel->~Model();
}

void MainCharacter::LoadContent()
{
	myModel->LoadModel(filename,folder);
	if(myModel==NULL)
	{
		MessageBox(NULL,"error loading model main character","error",MB_OK);
		return;
	}
}

void MainCharacter::Update()
{
	d3ddev->SetTransform(D3DTS_WORLD,&world);
	myModel->DrawModel();
}

void MainCharacter::SetWorld(D3DXVECTOR3 angle,D3DXVECTOR3 scale,D3DXVECTOR3 pos)
{
	position=pos;
	D3DXMATRIX rotx,roty,rotz;
	D3DXMatrixRotationX(&rotx,angle.x);
	D3DXMatrixRotationY(&roty,angle.y);
	D3DXMatrixRotationZ(&rotz,angle.z);
	rotation=rotx*roty*rotz;
	D3DXMatrixScaling(&scaling,scale.x,scale.y,scale.z);
	D3DXMatrixTranslation(&translation,position.x,position.y,position.z);
	world=scaling*rotation*translation;
}

D3DXMATRIX MainCharacter::GetWorld()
{
	D3DXMATRIXA16 a;
	return a;
}

void MainCharacter::AttachToCam(D3DXMATRIX view,D3DXVECTOR3 p,D3DXVECTOR3 r,D3DXVECTOR3 l,D3DXVECTOR3 up,float angleOfYaw,float angleOfPitch)
{
	D3DXMATRIX w,w1,w2;
	D3DXMATRIX rot,scale,rot1;
	
	D3DXQUATERNION res,pitch,yaw;

	position=p;

	D3DXQuaternionRotationAxis(&yaw,&up,angleOfYaw);
	
	D3DXQuaternionRotationAxis(&pitch,&r,angleOfPitch);
	res=pitch*yaw;
	up=up*8;
	l=l*38;
	r=r*8;
	D3DXMatrixTranslation(&w2,p.x-up.x+l.x+r.x,p.y-up.y+l.y+r.y+sin(angleOfPitch+angleOfYaw),p.z-up.z+l.z+r.z);

	D3DXMatrixRotationQuaternion(&w,&res);
	
	D3DXMatrixScaling(&scale,0.1,0.1,0.1);

	world=scale*rotation*w*w2;
	/*
	char text[1024];
	sprintf(text,"%fang: %f vec: %f %f %f\t",angleOfYaw,res.x,res.y,res.z,res.w);
	OutputDebugString(text); */
}


void CrossHair::AttachToLookAxis(D3DXVECTOR3 p,D3DXVECTOR3 l)
{
	D3DXMATRIX scale,w;
	D3DXMatrixScaling(&scale,0.001,0.001,0.001);

	D3DXMatrixTranslation(&w,p.x+(l.x*3),p.y+(l.y*3),p.z+(l.z*3));
	world=scale*w;
}

void Bullets::AttachToLookAxis()
{
	D3DXMATRIX scale,w;
	//D3DXMatrixScaling(&scale,0.001,0.001,0.001);
	D3DXMatrixScaling(&scale,0.01,0.01,0.01);
	D3DXVec3Normalize(&look,&look);
	position=position+(look*speed);
	stopConstraint+=1;
	D3DXMatrixTranslation(&w,position.x,position.y,position.z);
	world=scale*w;
}


void EnemyMinion::rotate_to_death()
{
	D3DXMATRIX rot,trans;
	D3DXMatrixRotationX(&rot,1.57f);
	D3DXMatrixTranslation(&trans,position.x,position.y-20,position.z);
	world=scaling*rot*trans;
	disabled=true;
}



void EnemyMinion::AI(D3DXVECTOR3 fpos,int id)
{
	float distanceToTarget=D3DXVec3Length(&(position-fpos));
	
	if(distanceToTarget>100)
	{
	/* traditional method to follow the character
	if(fpos.x>position.x)
		position.x+=0.1;
	else
		position.x-=0.1;
	if(fpos.z>position.z)
		position.z+=0.1;
	else
		position.z-=0.1;
		*/
		//Calculate circle of enemies
		D3DXVECTOR3 targetPosition=fpos;
		D3DXMATRIX aroundRot;
		D3DXVECTOR3 targetAxis;
		D3DXVec3Cross(&targetAxis,&D3DXVECTOR3(0,0,-1),&fpos);
		D3DXVec3Normalize(&targetAxis,&targetAxis);
		D3DXMatrixRotationAxis(&aroundRot,&targetAxis,0.5*id);
		D3DXVec3TransformCoord(&targetPosition,&targetPosition,&aroundRot);

		D3DXVECTOR3 lookAtTarget=fpos;
		
		//We have the forward vector
		//Now transform to local space
		D3DXMATRIX mTolocal;
		D3DXMatrixInverse(&mTolocal,NULL,&world);

		D3DXVECTOR3 localLookAt;
		//lookAtTarget.y=20;

		D3DXVec3TransformCoord(&localLookAt,&lookAtTarget,&mTolocal);
		D3DXVec3Normalize(&localLookAt,&localLookAt);

		D3DXVECTOR3 localRotationAxis;
		D3DXVec3Cross(&localRotationAxis,&enemyForward,&localLookAt);
		D3DXVec3Normalize(&localRotationAxis,&localRotationAxis);

		//float localAngle=acosf(D3DXVec3Dot(&enemyForward,&localLookAt));
		//localAngle=localAngle/64;
		float localAngle=Vec3CosineAngle(enemyForward,localLookAt,1,0);
		
		D3DXMatrixRotationAxis(&rotation,&localRotationAxis,localAngle);

	//Movement
	float ori=position.y;
	fpos.y=0; 
	//P=P+t(Q-P) equation to follow the character
	//position=position+0.0008*(fpos-position);
	position=position+0.0008*(targetPosition-position);
	position.y=ori;
	
	world=rotation*world;
	world._41=position.x; world._42=position.y; world._43=position.z; 

	//Set Bounding Box
	D3DXVECTOR3 pos,left,right;
	pos=position;
	left.x=pos.x+10;	left.y=pos.y+10;	left.z=pos.z-10;
		//left side corner
		right.x=pos.x-10;	right.y=pos.y-35;	right.z=pos.z+10;
	char s[1024];
	/*sprintf(s,"P[%d]=%f %f %f -> L[%d]=%f %f %f -> R[%d]=%f %f %f\n",id,pos.x,pos.y,pos.z,id,leftBound.x,leftBound.y,leftBound.z,
		id,rightBound.x,rightBound.y,rightBound.z);
	OutputDebugString(s);*/
	sprintf(s,"P[%d]=%f %f %f -> L[%d]=%f %f %f -> R[%d]=%f %f %f Angle: %f\n",id,pos.x,pos.y,pos.z,id,left.x,left.y,left.z,id,right.x,
		right.y,right.z,localAngle);
	OutputDebugString(s);
	}
	AIhitBullets(distanceToTarget,position,fpos);
}

void EnemyMinion::init()
{
	D3DXMATRIX enemyMatrix=world;
	enemyMatrix._41=0; enemyMatrix._42=0; enemyMatrix._43=0; enemyMatrix._44=1;
	float localAngle=0.0;
	D3DXMATRIX toEnemySpace;

	if(D3DXMatrixInverse(&toEnemySpace,NULL,&enemyMatrix)!=NULL)
	{
		D3DXVECTOR4 vec;
		D3DXVec3Transform(&vec,&D3DXVECTOR3(0,0,-1),&toEnemySpace);

		enemyForward=D3DXVECTOR3(vec.x,vec.y,vec.z);
	}
	setDamage=0;
	for(int ct=0;ct<10;ct++)
		damage[ct]=0;
	//set specific health reduction
	damage[3]=1;
	damage[4]=3;
	damage[5]=2;
	myModel->SetModelAttributes();
}

float EnemyMinion::Vec3CosineAngle(D3DXVECTOR3 v1,D3DXVECTOR3 v2,float angle,float prevCoAngle)
{
	float CoAngle=prevCoAngle;

	if(angle>0)
	CoAngle=acosf(D3DXVec3Dot(&v1,&v2)/(sqrt(((v1.x*v1.x)+(v1.y*v1.y)+(v1.z*v1.z))*((v2.x*v2.x)+(v2.y*v2.y)+(v2.z*v2.z)))));
	else
	CoAngle=-acosf(D3DXVec3Dot(&v1,&v2)/(sqrt(((v1.x*v1.x)+(v1.y*v1.y)+(v1.z*v1.z))*((v2.x*v2.x)+(v2.y*v2.y)+(v2.z*v2.z)))));


	return CoAngle;
}

void EnemyMinion::AIhitBullets(float distance,D3DXVECTOR3 position,D3DXVECTOR3 fpos)
{
	if(bulletDisabled==false)
	{
		enemyBullet->look=fpos-position;
		enemyBullet->position=position;
		bulletDisabled=true;

		int ct=rand()%10;
		setDamage=damage[ct];
	}
}
void EnemyMinion::drawEnemyBullet()
{
	if(bulletDisabled==true)
	{
		enemyBullet->AttachToLookAxis();
		
		if(enemyBullet->stopConstraint>1000)
		{
			bulletDisabled=false;
			enemyBullet->stopConstraint=0;
		}
	}
	enemyBullet->Update();
}