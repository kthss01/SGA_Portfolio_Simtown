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
	IMAGE->AddImage("ui_click_up1", "images/ui/ui_click_up_1.bmp", 0, 0,
		40, 40, false, RGB(255, 0, 255));
	IMAGE->AddImage("ui_click_up2", "images/ui/ui_click_up_2.bmp", 0, 0,
		40, 40, false, RGB(255, 0, 255));
	IMAGE->AddImage("ui_click_down1", "images/ui/ui_click_down_1.bmp", 0, 0,
		40, 40, false, RGB(255, 0, 255));
	IMAGE->AddImage("ui_click_down2", "images/ui/ui_click_down_2.bmp", 0, 0,
		40, 40, false, RGB(255, 0, 255));
	IMAGE->AddImage("ui_toolBar", "images/ui/toolbar_3x1.bmp", 0, 0,
		50 * 3, 25, 3, 1, true, RGB(255, 0, 255));
	IMAGE->AddImage("ui_song", "images/ui/song_3x1.bmp", 0, 0,
		50 * 3, 25, 3, 1, true, RGB(255, 0, 255));
	IMAGE->AddImage("ui_population", "images/ui/ui_population.bmp", 0, 0,
		150 * 3, 35, 3, 1, false, RGB(255, 0, 255));
}
