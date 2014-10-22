#include "Camera.h"
#include<math.h>
#include<stdio.h>
FirstPersonShooter::FirstPersonShooter(LPDIRECT3DDEVICE9 dev)
{
	d3ddev=dev;
	position=D3DXVECTOR3(0,10,0);
	look=D3DXVECTOR3(0,0,1);
	up=D3DXVECTOR3(0,1,0);
	right=D3DXVECTOR3(1,0,0);
	angleOfYaw=angleOfPitch=0;
}

FirstPersonShooter::~FirstPersonShooter()
{
}

void FirstPersonShooter::SetCamera(D3DXVECTOR3 source,D3DXVECTOR3 luk)
{
	
	D3DXVECTOR3 up(0.0f,1.0f,0.0f);

	position=source;
	look=luk;
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V,&position,&look,&up);
	d3ddev->SetTransform(D3DTS_VIEW,&V);
}

void FirstPersonShooter::SetPerspective(float fov,float aspectRatio,float nearRange,float farRange)
{
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj,fov,aspectRatio,nearRange,farRange);
	d3ddev->SetTransform(D3DTS_PROJECTION,&matProj);
}

void FirstPersonShooter::SetSource(D3DXVECTOR3 src)
{
}

void FirstPersonShooter::GetSource()
{
}

void FirstPersonShooter::getViewMatrix(D3DXMATRIX *V)
{
	//Keep camera's axes orthogonal to each other
	D3DXVec3Normalize(&look,&look);

	D3DXVec3Cross(&up,&look,&right);
	D3DXVec3Normalize(&up,&up);

	D3DXVec3Cross(&right,&up,&look);
	D3DXVec3Normalize(&right,&right);

	float x=-D3DXVec3Dot(&right,&position);
	float y=-D3DXVec3Dot(&up,&position);
	float z=-D3DXVec3Dot(&look,&position);

	(*V)(0,0) = right.x; (*V)(0, 1) = up.x; (*V)(0, 2) = look.x; (*V)(0, 3) = 0.0f;
	(*V)(1,0) = right.y; (*V)(1, 1) = up.y; (*V)(1, 2) = look.y; (*V)(1, 3) = 0.0f;
	(*V)(2,0) = right.z; (*V)(2, 1) = up.z; (*V)(2, 2) = look.z; (*V)(2, 3) = 0.0f;
	(*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
	matView=*V;
	//char text[1024];
	//sprintf(text,"pos:%f %f %f\t",position.x,position.y,position.z);
	//sprintf(text,"pos:%f %f %f\t",up.x,up.y,up.z);
	//OutputDebugString(text);
}

void FirstPersonShooter::yaw(float angle)
{
	D3DXMATRIX T;
	//angleOfYaw=angle;
	//rotate around world(0,1,0)
	D3DXMatrixRotationY(&T,angle);
	oldlook=right; //to calculate cosine angle between the old look vector and new look to find the angle of movement

	D3DXVec3TransformCoord(&look,&look,&T);
	D3DXVec3TransformCoord(&right,&right,&T);
	
	angleOfYaw=Vec3CosineAngle(oldlook,right,angle,angleOfYaw);
	
}

static float acc_angle=0.0f;
void FirstPersonShooter::pitch(float angle)
{
	D3DXMATRIX T;
	acc_angle+=angle;
	if(acc_angle<1.57 && acc_angle>-1.57)
	{
	D3DXMatrixRotationAxis(&T,&right,angle);
	
	D3DXVECTOR3 oldup=look;
	//rotate up and look around right vector
	D3DXVec3TransformCoord(&up,&up,&T);
	
	D3DXVec3TransformCoord(&look,&look,&T);
	angleOfPitch=Vec3CosineAngle(oldup,look,angle,angleOfPitch);
	}
	else
	{
		if(acc_angle<0)
			acc_angle=-1.57;
		else
			acc_angle=1.57;
	}

}

float Vec3CosineAngle(D3DXVECTOR3 v1,D3DXVECTOR3 v2,float angle,float prevCoAngle)
{
	float CoAngle=prevCoAngle;

	if(angle>0)
	CoAngle+=acosf(D3DXVec3Dot(&v1,&v2)/(sqrt(((v1.x*v1.x)+(v1.y*v1.y)+(v1.z*v1.z))*((v2.x*v2.x)+(v2.y*v2.y)+(v2.z*v2.z)))));
	else
	CoAngle+=-acosf(D3DXVec3Dot(&v1,&v2)/(sqrt(((v1.x*v1.x)+(v1.y*v1.y)+(v1.z*v1.z))*((v2.x*v2.x)+(v2.y*v2.y)+(v2.z*v2.z)))));
	if((CoAngle>6.28f && angle>0)||(CoAngle<-6.28f && angle<0))
		CoAngle=0;

	return CoAngle;
}

//float walkSlerp=0.0;
void FirstPersonShooter::walk(float units)
{
	//if(walkSlerp<2.0)
	{
	position+=D3DXVECTOR3(look.x,5.0f,look.z)*units;
	//walkSlerp+=0.5;
	}
	//else
	{
	position+=D3DXVECTOR3(look.x,0.0f,look.z)*units;
	position.y=20;
	//walkSlerp=0;
	}
}

void FirstPersonShooter::strafe(float units)
{
	position+=D3DXVECTOR3(right.x,0.0f,right.z)*units;
}

void FirstPersonShooter::Update()
{
	D3DXMATRIX V;
	getViewMatrix(&V);
	d3ddev->SetTransform(D3DTS_VIEW,&V);
}

void FirstPersonShooter::getLook(D3DXVECTOR3 *l)
{
	l=&look;
}

void FirstPersonShooter::getPosition(D3DXVECTOR3 *v)
{
	v=&position;
}

void FirstPersonShooter::getRight(D3DXVECTOR3 *r)
{
	r=&right;
}