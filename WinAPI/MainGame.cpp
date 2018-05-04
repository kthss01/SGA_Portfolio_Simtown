#include "stdafx.h"
#include "MainGame.h"

#include "TestScene.h"
#include "Simtown.h"

#include "IsoTile.h"

MainGame::MainGame()
{
#ifdef SUBWINOPEN
	SUBWIN->Init();
#endif // SUBWINDOW �ʿ�� MainGame.h �ּ� ����
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
	//SOUND->AddSound("Test", "sounds/����3.wav", true, true);

#ifdef SUBWINOPEN
	SUBWIN->SetScene(test);
#endif // SUBWINDOW �ʿ�� MainGame.h �ּ� ����

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
#endif // SUBWINDOW �ʿ�� MainGame.h �ּ� ����

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
#endif // SUBWINDOW �ʿ�� MainGame.h �ּ� ����
		
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
	LoadUI();
}

void MainGame::LoadTile()
{
	IMAGE->AddImage("land", "images/tile/land_big.bmp", 0, 0,
		CELL_WIDTH, CELL_HEIGHT, true, RGB(255, 0, 255));
}

void MainGame::LoadBuilding()
{
}

void MainGame::LoadUI()
{
	IMAGE->AddImage("ui_left", "images/ui/ui_left.bmp", 0, 0,
		100 * 11 * 3, 700, 33, 1, false, RGB(255, 0, 255));
	IMAGE->AddImage("ui_bottom_1", "images/ui/ui_bottom_1.bmp", 0, 0,
		100 * 2, 100 * 21, 2, 21, false, RGB(255, 0, 255));
	IMAGE->AddImage("ui_bottom_2", "images/ui/ui_bottom_2.bmp", 0, 0,
		100 * 2, 100 * 14, 2, 14, false, RGB(255, 0, 255));
	IMAGE->AddImage("ui_bottom_3", "images/ui/ui_bottom_3.bmp", 0, 0,
		100 * 2, 100 * 14, 2, 14, false, RGB(255, 0, 255));
	IMAGE->AddImage("ui_bottom_4", "images/ui/ui_bottom_4.bmp", 0, 0,
		100 * 2, 100 * 7, 2, 7, false, RGB(255, 0, 255));
	IMAGE->AddImage("ui_bottom_5", "images/ui/ui_bottom_5.bmp", 0, 0,
		100 * 2, 100 * 7, 2, 7, false, RGB(255, 0, 255));
	IMAGE->AddImage("ui_blank", "images/ui/ui_blank.bmp", 0, 0, 
		100, 100, false, RGB(255, 0, 255));
}
