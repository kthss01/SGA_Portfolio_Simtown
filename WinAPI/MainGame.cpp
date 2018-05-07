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

	IMAGE->AddImage("bg", "images/bg2.bmp", 0, 0,
		WINSIZEX, WINSIZEY, false, RGB(255, 0, 255));

	LoadSimtownImg();

	//TestScene * test = new TestScene;
	//SCENE->AddScene("Test", test);
	SCENE->AddScene("Simtown", new Simtown);

	SOUND->Init();
	//SOUND->AddSound("Test", "sounds/����3.wav", true, true);

	SOUND->AddSound("song1", "sounds/song/SimTown - Song 1.mp3", true, true);
	SOUND->AddSound("song2", "sounds/song/SimTown - Song 2.mp3", true, true);
	SOUND->AddSound("song3", "sounds/song/SimTown - Song 3.mp3", true, true);

	SOUND->AddSound("tile_click", "sounds/effect/tile click.mp3", false, false);
	SOUND->AddSound("tile_destroy", "sounds/effect/tile destroy.mp3", false, false);
	SOUND->AddSound("tile_error", "sounds/effect/tile error.mp3", false, false);
	SOUND->AddSound("tile_select", "sounds/effect/tile select.mp3", false, false);

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
		IMAGE->Render("bg", GetMemDC(), 0, 0);

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

	IMAGE->AddImage("road_big", "images/tile/road_big_15x1.bmp", 0, 0,
		CELL_WIDTH * 15 * 2, CELL_HEIGHT * 1 * 2, 15, 1, true, RGB(255, 0, 255));
	IMAGE->AddImage("road_mid", "images/tile/road_mid_15x1.bmp", 0, 0,
		CELL_WIDTH * 15, CELL_HEIGHT * 1, 15, 1, true, RGB(255, 0, 255));

	IMAGE->AddImage("bike_path_big", "images/tile/bike_path_big_11x4.bmp", 0, 0,
		CELL_WIDTH * 11, CELL_HEIGHT * 4, 11, 4, true, RGB(255, 0, 255));
	IMAGE->AddImage("bike_path_mid", "images/tile/bike_path_mid_22x1.bmp", 0, 0,
		CELL_WIDTH2 * 22, CELL_HEIGHT2, 22, 1, true, RGB(255, 0, 255));
}

void MainGame::LoadBuilding()
{
	LoadHouses();
	LoadCommunityBuildings();
	LoadBusinesses();
	LoadFunPlaces();
}

void MainGame::LoadHouses()
{
	IMAGE->AddImage("houses_adobe_house_open",
		"images/houses/open/adobe_house.bmp", 0, 0,
		CELL_WIDTH + 50, CELL_HEIGHT * 2, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_adobe_house_big",
		"images/houses/big/adobe_house.bmp", 0, 0,
		CELL_WIDTH + 50, CELL_HEIGHT * 2, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_adobe_house_mid",
		"images/houses/mid/adobe_house.bmp", 0, 0,
		CELL_WIDTH / 2 + 25, CELL_HEIGHT, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_adobe_house_small",
		"images/houses/small/adobe_house.bmp", 0, 0,
		CELL_WIDTH / 4 + 12.5, CELL_HEIGHT / 2, true, RGB(255, 0, 255));
	
	IMAGE->AddImage("houses_ranch_house_open",
		"images/houses/open/ranch_house.bmp", 0, 0,
		CELL_WIDTH + 50, CELL_HEIGHT * 2, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_ranch_house_big",
		"images/houses/big/ranch_house.bmp", 0, 0,
		CELL_WIDTH + 50, CELL_HEIGHT * 2, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_ranch_house_mid",
		"images/houses/mid/ranch_house.bmp", 0, 0,
		CELL_WIDTH / 2 + 25, CELL_HEIGHT, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_ranch_house_small",
		"images/houses/small/ranch_house.bmp", 0, 0,
		CELL_WIDTH / 4 + 12.5, CELL_HEIGHT / 2, true, RGB(255, 0, 255));

	IMAGE->AddImage("houses_apartment_open",
		"images/houses/open/apartment.bmp", 0, 0,
		CELL_WIDTH + 100, CELL_HEIGHT * 3 + 25, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_apartment_big",
		"images/houses/big/apartment.bmp", 0, 0,
		CELL_WIDTH + 100, CELL_HEIGHT * 3 + 25, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_apartment_mid",
		"images/houses/mid/apartment.bmp", 0, 0,
		CELL_WIDTH / 2 + 50, CELL_HEIGHT / 2 * 3 + 12.5, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_apartment_small",
		"images/houses/small/apartment.bmp", 0, 0,
		CELL_WIDTH / 4 + 25, CELL_HEIGHT / 4 * 3 + 6.25, true, RGB(255, 0, 255));

	IMAGE->AddImage("houses_victorian_house_open",
		"images/houses/open/victorian_house.bmp", 0, 0,
		CELL_WIDTH + CELL_WIDTH / 2 + CELL_WIDTH / 8, CELL_HEIGHT * 3 + CELL_HEIGHT / 4, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_victorian_house_big",
		"images/houses/big/victorian_house.bmp", 0, 0,
		CELL_WIDTH + CELL_WIDTH / 2 + CELL_WIDTH / 8, CELL_HEIGHT * 3 + CELL_HEIGHT / 4, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_victorian_house_mid",
		"images/houses/mid/victorian_house.bmp", 0, 0,
		CELL_WIDTH / 2 + CELL_WIDTH / 4 + CELL_WIDTH / 16, CELL_HEIGHT / 2 * 3 + CELL_HEIGHT / 8, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_victorian_house_small",
		"images/houses/small/victorian_house.bmp", 0, 0,
		CELL_WIDTH / 4 + CELL_WIDTH / 8 + CELL_WIDTH / 32, CELL_HEIGHT / 4 * 3 + CELL_HEIGHT / 16, true, RGB(255, 0, 255));

	IMAGE->AddImage("houses_cottage_open",
		"images/houses/open/cottage.bmp", 0, 0,
		CELL_WIDTH + CELL_WIDTH / 4, CELL_HEIGHT * 2 + CELL_HEIGHT / 4, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_cottage_big",
		"images/houses/big/cottage.bmp", 0, 0,
		CELL_WIDTH + CELL_WIDTH / 4, CELL_HEIGHT * 2 + CELL_HEIGHT / 4, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_cottage_mid",
		"images/houses/mid/cottage.bmp", 0, 0,
		CELL_WIDTH / 2 + CELL_WIDTH / 8, CELL_HEIGHT + CELL_HEIGHT / 8, true, RGB(255, 0, 255));
	IMAGE->AddImage("houses_cottage_small",
		"images/houses/small/cottage.bmp", 0, 0,
		CELL_WIDTH / 4 + CELL_WIDTH / 16, CELL_HEIGHT / 2 + CELL_HEIGHT / 16, true, RGB(255, 0, 255));

}

void MainGame::LoadCommunityBuildings()
{
}

void MainGame::LoadBusinesses()
{
}

void MainGame::LoadFunPlaces()
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
