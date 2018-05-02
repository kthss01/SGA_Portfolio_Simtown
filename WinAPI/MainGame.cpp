#include "stdafx.h"
#include "MainGame.h"

#include "TestScene.h"
#include "Simtown.h"

#include "IsoTile.h"

MainGame::MainGame()
{
#ifdef SUBWINOPEN
	SUBWIN->Init();
#endif // SUBWINDOW 필요시 MainGame.h 주석 제거
}

MainGame::~MainGame()
{
}

HRESULT MainGame::Init()
{
	GameNode::Init();
	isDebug = false;

	LoadSimtownImg();

	//TestScene * test = new TestScene;
	//SCENE->AddScene("Test", test);
	SCENE->AddScene("Simtown", new Simtown);

	SOUND->Init();
	//SOUND->AddSound("Test", "sounds/영전3.wav", true, true);

#ifdef SUBWINOPEN
	SUBWIN->SetScene(test);
#endif // SUBWINDOW 필요시 MainGame.h 주석 제거

	//SCENE->ChangeScene("Test");
	SCENE->ChangeScene("Simtown");

	return S_OK;
}

void MainGame::Release()
{
	GameNode::Release();
}

void MainGame::Update()
{
	GameNode::Update();
	SCENE->Update();

#ifdef SUBWINOPEN
	SUBWIN->Update();
#endif // SUBWINDOW 필요시 MainGame.h 주석 제거

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_F11)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void MainGame::Render()
{
	PatBlt(GetMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	{
		SCENE->Render();

#ifdef SUBWINOPEN
		SUBWIN->Render();
#endif // SUBWINDOW 필요시 MainGame.h 주석 제거
		
	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
	FRAME->Render(GetMemDC());
	this->SetBackBuffer()->Render(GetHDC());
}

void MainGame::LoadSimtownImg()
{
	LoadTile();
	LoadBuilding();
}

void MainGame::LoadTile()
{
	IMAGE->AddImage("land", "images/tile/land_big.bmp", 0, 0,
		CELL_WIDTH, CELL_HEIGHT, true, RGB(255, 0, 255));
}

void MainGame::LoadBuilding()
{
}
