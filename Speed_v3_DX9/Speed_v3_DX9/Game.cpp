#include "Game.h"
#include<iostream>

int hitCount=0;
BOOL hit[10];
D3DXCOLOR bcolor;

Game::Game()
{
	d3d=NULL;
	d3ddev=NULL;
}

Game::~Game()
{
//	Game_Shutdown();
}

void Game::Game_Shutdown()
{
	MessageBox(hWnd,"SHUTING DOWN AND RELEASING","INTIMATE",MB_OK);

	char text[1024];
	sprintf(text,"Hit count %d\t",hitCount);
	OutputDebugString(text); 

	for(int i=0;i<10;i++)
	{
	sprintf(text,"Hit[%d] %d\t",i,hit[i]);
	OutputDebugString(text); 
	}

	mainCharacter->~MainCharacter();
	tunnel->~MainCharacter();
	//city->~MainCharacter();
	church->~MainCharacter();
	playground->~MainCharacter();
	crosshair->~CrossHair();
	enemyManager->~EnemyManager();
	left->~MainCharacter();
	right->~MainCharacter();
	//skyglobe->~MainCharacter();

	//release direct3d dev
	if(d3ddev!=NULL)
		d3ddev->Release();

	//release d3d
	if(d3d!=NULL)
		d3d->Release();

	if(font!=NULL)
	{
	font->Release();
	font=NULL;
	}
	ak47_bullet.clear();
	ak47_bullet.~vector();
}

bool Game::Game_Init(HWND hWnd)
{
	Game::hWnd=hWnd;

	//Init the Direct3D
	d3d=Direct3DCreate9(D3D_SDK_VERSION);
	GAME_OVER=false;


	if(d3d==NULL)
	{
		MessageBox(hWnd,"Cannot Init Direct3D","ERROR",MB_OK);
		return 0;
	}

	//Set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.Windowed=TRUE;
	d3dpp.SwapEffect=D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat=D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount=1;
	d3dpp.BackBufferWidth=SCREEN_WIDTH;
	d3dpp.BackBufferHeight=SCREEN_HEIGHT;
	d3dpp.hDeviceWindow=hWnd;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;
	//CREATE THE DIRECT3D DEVICE
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	if(d3ddev==NULL)
	{
		MessageBox(hWnd,"Cannot Create Direct3D Device","ERROR",MB_OK);
		return 0;
	}
	bcolor=D3DXCOLOR(0,0,0,0);

	//d3ddev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	srand(time(NULL));

	//Init the Input Manager Mouse
	inputManager=new InputManager(hWnd);
	inputManager->Init_Mouse();

	//Init Camera Class
	FPS=new FirstPersonShooter(d3ddev);
	FPS->SetCamera(D3DXVECTOR3(0.0f,20.0f,0.0f),D3DXVECTOR3(0,0,300));
	float ratio=(float)SCREEN_WIDTH/(float)SCREEN_WIDTH;
	FPS->SetPerspective(45.0f,ratio,0.1f,10000.0f);

	//turn Dynamic Lighting off, Z-BUFFERING ON
	
	d3ddev->SetRenderState(D3DRS_LIGHTING,true);
	d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS,true);
	
	d3ddev->SetRenderState(D3DRS_SPECULARENABLE,true);
	d3ddev->SetRenderState(D3DRS_AMBIENT,D3DXCOLOR(150.0f,100.0f,150.0f,0.0f));
	d3ddev->SetRenderState(D3DRS_ZENABLE,true);

	Game_LoadContent();
	return 1;
}

void Game::Game_LoadContent()
{
	//GUN MODEL
	//mainCharacter=new MainCharacter(d3ddev,"Content/Models/AKS/AKS-74.X","AKS");
	mainCharacter=new MainCharacter(d3ddev,"Content/Models/Guns/M4/M4.X","Guns\\\\M4");
	mainCharacter->SetWorld(D3DXVECTOR3(0,-1.7,-0.15),D3DXVECTOR3(0.3,0.3,0.3),D3DXVECTOR3(0,0,0));
	mainCharacter->LoadContent(); 
	//Init ak47 bullets count
	ak47BulletCount=0;
	availBullet=9999;
	availBulletCount=0;
	
	//City SAMPLE
	tunnel=new MainCharacter(d3ddev,"Content/Models/EntryWay/EntryWayFinal3ds.X","EntryWay");
	tunnel->SetWorld(D3DXVECTOR3(0,0,0),D3DXVECTOR3(100,100,100),D3DXVECTOR3(0,-70,0));
	tunnel->LoadContent();
	
	/*city=new MainCharacter(d3ddev,"Content/Models/City/Xcity.X","City");
	city->SetWorld(D3DXVECTOR3(0,0,0),150,D3DXVECTOR3(0,-20,100));
	city->LoadContent();*/
	
	church=new MainCharacter(d3ddev,"Content/Models/Church/church.X","Church");
	church->SetWorld(D3DXVECTOR3(0,0,0),D3DXVECTOR3(10,10,10),D3DXVECTOR3(0,0,-200));
	church->LoadContent();

	//Playground
	playground=new MainCharacter(d3ddev,"Content/Models/Playground/playground3ds.X","Playground");
	playground->SetWorld(D3DXVECTOR3(0,0,0),D3DXVECTOR3(500,50,100),D3DXVECTOR3(0,10,0));
	playground->LoadContent();

	//Crosshair
	crosshair=new CrossHair(d3ddev,"Content/Models/Crosshair/Crosshair_Sphere.X","Crosshair");
	crosshair->SetWorld(D3DXVECTOR3(0,0,0),D3DXVECTOR3(1,1,1),D3DXVECTOR3(0,0,0));
	crosshair->LoadContent();

	//Skyglobe
	/*skyglobe=new MainCharacter(d3ddev,"Content/Models/SkyGlobe/skyglobe3DS.X","SkyGlobe");
	skyglobe->SetWorld(D3DXVECTOR3(0,0,0),D3DXVECTOR3(1000,2000,1000),D3DXVECTOR3(0,100,0));
	skyglobe->LoadContent();
	*/
	//Bounds model
	left=new MainCharacter(d3ddev,"Content/Models/Crosshair/Crosshair_Sphere.X","Crosshair");
	left->SetWorld(D3DXVECTOR3(0,0,0),D3DXVECTOR3(0.1,0.1,0.1),D3DXVECTOR3(-798.568359,30.000000,229.235138));
	left->LoadContent();
	
	right=new MainCharacter(d3ddev,"Content/Models/Crosshair/Crosshair_Sphere.X","Crosshair");
	right->SetWorld(D3DXVECTOR3(0,0,0),D3DXVECTOR3(0.1,0.1,0.1),D3DXVECTOR3(-818.568359,-15.000000,249.235138));
	right->LoadContent();

	//Create the new EnemyManager Object
	enemyManager=new EnemyManager(d3ddev);

	//Head Up Display
	D3DXFONT_DESC logfont={24,
                          0,
                          400,
                          0,
                          false,
                          DEFAULT_CHARSET,
                          OUT_TT_PRECIS,
                          CLIP_DEFAULT_PRECIS,
                          DEFAULT_PITCH,
                          "Arial"};
	font=0;
	D3DXCreateFontIndirectA(d3ddev,&logfont,&font);
	
	rect.top=0;
	rect.left=0;
	rect.bottom=400;
	rect.right=700;
	

}

void Game::Game_Update()
{

	if(KEY_DOWN(VK_ESCAPE))
		PostMessage(hWnd,WM_DESTROY,0,0);

	if(d3ddev==NULL)
		return;

	//clear backbuffer
	d3ddev->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,bcolor,1.0f,0);

	
	if(GAME_OVER==false)
	{
	//Start Rendering
	if(d3ddev->BeginScene())
	{
		mainCharacter->health-=enemyManager->totDamage;
		enemyManager->totDamage=0; //After reducing health, init for next damages from enemy
		
		if(mainCharacter->health<=0)
		{
			RECT go;
			go.top=(SCREEN_HEIGHT/2)-100;
			go.left=(SCREEN_WIDTH/2)-200;
			go.right=700;
			go.bottom=go.top+100;
			font->DrawTextA(NULL,"GAME OVER",-1,&go,DT_CENTER,D3DXCOLOR(1,0,0,1));
			mainCharacter->health=0;
			GAME_OVER=true;
		}

		//Health Display
		rect.top=0;
		char hud[1024];
		sprintf(hud,"HEALTH : %d",mainCharacter->health);
		font->DrawText(NULL,hud,-1,&rect,DT_TOP|DT_LEFT,0xffffffff);

		//Bullets Display
		rect.top=50;
		sprintf(hud,"BULLETS : %d",(availBullet-availBulletCount));
		font->DrawText(NULL,hud,-1,&rect,DT_TOP|DT_LEFT,0xffffffff);

		//Empty Bullets Display
		if(noBullet!=NULL)
		{
			RECT rect1;
			rect1.top=350; rect1.right=700; rect1.bottom=450; rect1.left=400;
			font->DrawText(NULL,noBullet,-1,&rect1,DT_CENTER,0xffffffff);
		}


		//Main Character set
		mainCharacter->Update();
		//tunnel->Update();
		//city->Update();
		//church->Update();
		playground->Update();

		crosshair->Update();
		//skyglobe
		//skyglobe->Update();

		//Bullets update
		for(int i=0;i<ak47BulletCount;i++)
			ak47_bullet.at(i)->Update();

		//Enemy Updates
		
		enemyManager->Update();
		//Bounds model update debug - to be removed
		left->Update();
		right->Update();

		d3ddev->EndScene();
	}

	d3ddev->Present(NULL,NULL,NULL,NULL);

	//Check Key Inputs from user
	Game_Input();

	//Cam update
	FPS->Update();


	//ATTACH CAMERA TO MAIN CHARACTER
	mainCharacter->AttachToCam(FPS->matView,FPS->position,FPS->right,FPS->look,FPS->up,FPS->angleOfYaw,FPS->angleOfPitch);

	//Attach Crosshair on Look Axis
	crosshair->AttachToLookAxis(FPS->position,FPS->look);

	enemyManager->setFollowCharacter(mainCharacter->position);
	//Track and update the bullets
	for(int i=0;i<ak47_bullet.size();i++)
	{
		BOOL res=false;
		ak47_bullet.at(i)->AttachToLookAxis();
		res=checkCollision(ak47_bullet.at(i)->position,ak47_bullet.at(i)->look,i);
		
		if(ak47_bullet.at(i)->stopConstraint>200 || res==true)
		{
				ak47BulletCount--;
				ak47_bullet.at(i)->~Bullets();
				ak47_bullet.erase(ak47_bullet.begin()+i);
		}
	}
	}
}



float reload=1.2f;
void Game::Game_Input()
{
	float timedelta=0.01;
	
	if(GetAsyncKeyState('W')&0x8000f)
		FPS->walk(4.0f*timedelta);

	if(GetAsyncKeyState('S')&0x8000f)
		FPS->walk(-4.0*timedelta);

	if(GetAsyncKeyState('A')&0x8000f)
		FPS->strafe(-4.0*timedelta);

	if(GetAsyncKeyState('D')&0x8000f)
		FPS->strafe(4.0*timedelta);

	if(GetAsyncKeyState(VK_LSHIFT)&0x8000f)
		FPS->walk(2);

	//Respawn Location points
	if(GetAsyncKeyState('I')&0x8000f)
		Insert_to_file();


	//Check Mouse Inputs

	inputManager->CheckMouseInput();

	if(inputManager->GetMouseX()<0)
		//Move left
			FPS->yaw(-2.0*timedelta);

	if(inputManager->GetMouseX()>0)
		//Move right
			FPS->yaw(2.0*timedelta);

	if(inputManager->GetMouseY()<0)
		//Move Down
			FPS->pitch(-2.0*timedelta);

	if(inputManager->GetMouseY()>0)
		//Move Down
			FPS->pitch(2.0*timedelta);

	//0-Mouse Left Button
	//1-Mouse Right Button
	//2-Mouse Scroll/Middle Button
	
	if(inputManager->Mouse_Button(0))
	{
		if(availBullet-availBulletCount<0)
		{
			noBullet="Out of Ammo";
		}
		else if(reload<0)
		{
		ak47_bullet.push_back(new Bullets(FPS->position,FPS->look,1,d3ddev,"Content/Models/Crosshair/Crosshair_Sphere.X","Crosshair"));
		ak47_bullet.at(ak47BulletCount)->SetWorld(D3DXVECTOR3(0,0,0),D3DXVECTOR3(1,1,1),D3DXVECTOR3(0,0,0));
		ak47_bullet.at(ak47BulletCount)->LoadContent();
		ak47BulletCount++;
		availBulletCount++;
			reload=1.2f;
		}
	}
	reload=reload-0.1f;
	if(inputManager->Mouse_Button(1))
	{
	
	}
	if(inputManager->Mouse_Button(2))
	{
	
	}

}


void Game::Insert_to_file()
{
	FILE *RespawnFile;
	RespawnFile=fopen("BoundLocation.txt","a");
	fprintf(RespawnFile,"%f %f %f\n",FPS->position.x,FPS->position.y,FPS->position.z);
	fclose(RespawnFile);
}

BOOL Game::checkCollision(D3DXVECTOR3 pos,D3DXVECTOR3 look,int bulletNo)
{
	
	for(int i=0;i<enemyManager->noEnemyObject;i++)
	{
		D3DXVECTOR3 p=enemyManager->EnemyObjects.at(i)->position;

		D3DXVECTOR3 modelPosruc=p,modelPosllc=p,modelPosruc1,modelPosllc1;
		
		//-378.749969 20.000000 -56.944309
		/*modelPosllc.x=-323.443420;	modelPosllc.y=0;	modelPosllc.z=-76.979378;	
		modelPosruc.x=-392.181702;	modelPosruc.y=100;	modelPosruc.z=-21.086603; */

		//right side corner
		modelPosruc.x=modelPosllc.x+10;	modelPosruc.y=modelPosllc.y+10;	modelPosruc.z=modelPosllc.z-10;
		//left side corner
		modelPosllc.x=modelPosllc.x-10;	modelPosllc.y=modelPosllc.y-35;	modelPosllc.z=modelPosllc.z+10;	
		modelPosllc1=modelPosruc; modelPosruc1=modelPosllc;
		
		if(D3DXBoxBoundProbe(&modelPosllc,&modelPosruc,&pos,&look)||D3DXBoxBoundProbe(&modelPosruc,&modelPosruc,&pos,&look) && enemyManager->EnemyObjects.at(i)->disabled==false)
		{
			hitCount++;
			hit[i]=true;
			enemyManager->DisableEnemyObject(i);
			if(bcolor==D3DXCOLOR(0.5,0.0,0.5,0))
				bcolor=D3DXCOLOR(0,0.5,0.5,0);
			else
				bcolor=D3DXCOLOR(0.5,0,0.5,0);
			return true;
		}
	
	}
	return false;
}