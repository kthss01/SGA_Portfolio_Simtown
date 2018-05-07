#include "stdafx.h"
#include "Simtown.h"


Simtown::Simtown()
{
}


Simtown::~Simtown()
{
}

HRESULT Simtown::Init()
{
	_isoX = 0;
	_isoY = 0;
	_center = 0;
	memset(_tileMap, 0, sizeof(_tileMap));

	_startX = INIT_X;
	_startY = INIT_Y;

	_isDebug = false;

	FindImg();

	_tileSize = TILESIZE_BIG;

	cell_width = CELL_WIDTH;
	cell_height = CELL_HEIGHT;
	radius_width = RADIUS_WIDTH;
	radius_height = RADIUS_HEIGHT;

	InitTile();

	UIInit();

	_isClicked = false;

	_population = 0;

	return S_OK;
}

void Simtown::Release()
{
}

void Simtown::Update()
{
	if (INPUT->GetKey('W')) { _startY += 5; }
	if (INPUT->GetKey('S')) { _startY -= 5; }
	if (INPUT->GetKey('A')) { _startX += 5; }
	if (INPUT->GetKey('D')) { _startX -= 5; }

	if (INPUT->GetKeyDown('1')) {
		switch (_tileSize)
		{
		case TILESIZE_SMALL:
			_startX -= 80;
			_startY -= 850;
			break;
		case TILESIZE_MID:
			_startX -= 50;
			_startY -= 550;
			break;
		case TILESIZE_BIG:
			break;
		}
		_tileSize = TILESIZE_BIG; 
		CheckSize(); 
	}
	if (INPUT->GetKeyDown('2')) { 
		switch (_tileSize)
		{
		case TILESIZE_SMALL:
			_startX -= 30;
			_startY -= 300;
			break;
		case TILESIZE_MID:
			break;
		case TILESIZE_BIG:
			_startX += 50;
			_startY += 550;
			break;
		}
		_tileSize = TILESIZE_MID; 
		CheckSize(); 
	}
	if (INPUT->GetKeyDown('3')) { 
		switch (_tileSize)
		{
		case TILESIZE_SMALL:
			break;
		case TILESIZE_MID:
			_startX += 30;
			_startY += 300;
			break;
		case TILESIZE_BIG:
			_startX += 80;
			_startY += 850;
			break;
		}
		_tileSize = TILESIZE_SMALL; 
		CheckSize(); 
	}
	if (INPUT->GetKey('Z')) {
		cell_width -= 2.0f;
		cell_height -= 1.0f;
		radius_width = cell_width / 2;
		radius_height = cell_height / 2;

		_startY += 10;

		if (cell_width > CELL_WIDTH2 + 24) {
			_tileSize = TILESIZE_BIG;
		}
		else if (cell_width > CELL_WIDTH3) {
			_tileSize = TILESIZE_MID;
		}
		else
			_tileSize = TILESIZE_SMALL;
	}
	if (INPUT->GetKey('X')) {
		cell_width += 2.0f;
		cell_height += 1.0f;
		radius_width = cell_width / 2;
		radius_height = cell_height / 2;

		_startY -= 10;

		if (cell_width > CELL_WIDTH2 + 24) {
			_tileSize = TILESIZE_BIG;
		}
		else if (cell_width > CELL_WIDTH3) {
			_tileSize = TILESIZE_MID;
		}
		else
			_tileSize = TILESIZE_SMALL;
	}
	if (INPUT->GetKeyDown('C')) { _ui._selectLeftUI = 6; }

	if (INPUT->GetKeyDown(VK_TAB)) {
		_isDebug = !_isDebug;
	}

	if (INPUT->GetKey(VK_LBUTTON)) {
		// tile Select
		{
			if (g_ptMouse.x > 100
				&& g_ptMouse.y > 35 && g_ptMouse.y < WINSIZEY - 100) {

				float cellX = (float)(g_ptMouse.x - _startX);

				if (cellX < 0) {
					cellX = (cellX - cell_width) / (float)cell_width;
				}
				else {
					cellX = cellX / (float)cell_width;
				}

				float cellY = fabs(g_ptMouse.y - _startY) / cell_height;

				cellY = (g_ptMouse.y < _startY) ? cellY * -1 : cellY;

				int isoX = (int)cellX + (int)cellY;
				int isoY = (int)cellY - (int)cellX;

				if (isoX >= 0 && isoX < TILE_COUNT_X &&
					isoY >= 0 && isoY < TILE_COUNT_Y) {
					int corner = GetCornerIndex(isoX, isoY);

					if (IsInRhombus(corner, isoX, isoY))
						corner = 0;

					switch (corner)
					{
					case 1:
						isoX = isoX - 1;
						break;
					case 2:
						isoY = isoY - 1;
						break;
					case 3:
						isoY = isoY + 1;
						break;
					case 4:
						isoX = isoX + 1;
						break;
					}

					if (isoX >= 0 && isoX < TILE_COUNT_X &&
						isoY >= 0 && isoY < TILE_COUNT_Y) {

						_center = corner;
						_isoX = isoX;
						_isoY = isoY;

						if (_ui._selectLeftUI == 5 && _tileMap[isoX][isoY].isStartDraw) {
							SOUND->Play("tile_destroy");
							_tileMap[isoX][isoY].isStartDraw = false;
							DeleteTile();
						}

						if (!_tileMap[isoX][isoY].isSelected && _isClicked) {
							_tileMap[isoX][isoY].isStartDraw = true;
							SelectTile();
							if (_tileMap[isoX][isoY].isStartDraw)
								SOUND->Play("tile_click");
						}
					}
				}
			}
		}
	}

	if (INPUT->GetKeyDown(VK_LBUTTON)) {
		// ui Select
		{
			// top ui
			if (PtInRect(&_ui._rcToolBarUI[0], g_ptMouse)) {
				InitTile();
			}
			if (PtInRect(&_ui._rcToolBarUI[1], g_ptMouse)) {
				SaveTile();
			}
			if (PtInRect(&_ui._rcToolBarUI[2], g_ptMouse)) {
				LoadTile();
			}

			if (PtInRect(&_ui._rcSongUI[0], g_ptMouse)) {
				PlaySong(0);
			}
			if (PtInRect(&_ui._rcSongUI[1], g_ptMouse)) {
				PlaySong(1);
			}
			if (PtInRect(&_ui._rcSongUI[2], g_ptMouse)) {
				PlaySong(2);
			}

			// left ui
			for (int i = 0; i < 14; i++) {
				if (PtInRect(&_ui._rcLeftUI[i], g_ptMouse)) {
					SOUND->Play("tile_select");
					if (i == 7 || i == 8 || i == 9) {
						_tileSize = (TileSize)(i - 7);
						CheckSize();
					}
					else if (i > 9) {
						_ui._selectLeftUI = i - 3;
						_ui._pageNum = 0;
						_ui._selectBottomUI = 0;
					}
					else {
						_ui._selectLeftUI = i;
						_ui._pageNum = 0;
						_ui._selectBottomUI = 0;

						if (i < 5) {
							_currentTileKind = (TileKind)i;
							_currentTileNum = 0;
						}
					}
					break;
				}
			}

			// bottom ui
			for (int i = 1; i < 8; i++) {
				if (PtInRect(&_ui._rcBottomUI[i], g_ptMouse)) {
					SOUND->Play("tile_select");
					_ui._selectBottomUI = i - 1;
					_currentTileNum = i - 1 + (_ui._pageNum * 7);
					break;
				}
			}

			// page ui
			if (PtInRect(&_ui._rcPageDownUI, g_ptMouse)) {
				if (_ui._selectLeftUI == 0 || _ui._selectLeftUI == 1 || _ui._selectLeftUI == 2) {
					SOUND->Play("tile_select");
					if (_ui._selectLeftUI == 0 && _ui._pageNum <= 1) {
						_currentTileNum += 7;
						_ui._pageNum++;
					}
					else if (_ui._pageNum <= 0) {
						_currentTileNum += 7;
						_ui._pageNum++;
					}
				}
			}
			if (PtInRect(&_ui._rcPageUpUI, g_ptMouse)) {
				if (_ui._selectLeftUI == 0 || _ui._selectLeftUI == 1 || _ui._selectLeftUI == 2) {
					SOUND->Play("tile_select");
					if (_ui._pageNum >= 1) {
						_currentTileNum -= 7;
						_ui._pageNum--;
					}
				}
			}
		}
	}

	if (_ui._selectLeftUI < 5) {
		_isClicked = true;
	}
	else
		_isClicked = false;
}

void Simtown::Render()
{
	DrawTileMap();

	if (_isClicked && g_ptMouse.x > 100
		&& g_ptMouse.y > 35 && g_ptMouse.y < WINSIZEY - 100) {
		float cellX = (float)(g_ptMouse.x - _startX);

		if (cellX < 0) {
			cellX = (cellX - cell_width) / (float)cell_width;
		}
		else {
			cellX = cellX / (float)cell_width;
		}

		float cellY = fabs(g_ptMouse.y - _startY) / cell_height;

		cellY = (g_ptMouse.y < _startY) ? cellY * -1 : cellY;

		int isoX = (int)cellX + (int)cellY;
		int isoY = (int)cellY - (int)cellX;

		if (isoX >= 0 && isoX < TILE_COUNT_X &&
			isoY >= 0 && isoY < TILE_COUNT_Y) {
			int corner = GetCornerIndex(isoX, isoY);

			if (IsInRhombus(corner, isoX, isoY))
				corner = 0;

			switch (corner)
			{
			case 1:
				isoX = isoX - 1;
				break;
			case 2:
				isoY = isoY - 1;
				break;
			case 3:
				isoY = isoY + 1;
				break;
			case 4:
				isoX = isoX + 1;
				break;
			}

			if (isoX >= 0 && isoX < TILE_COUNT_X &&
				isoY >= 0 && isoY < TILE_COUNT_Y) {
				_center = corner;
				_isoX = isoX;
				_isoY = isoY;
			}
		}

		float left = _startX + (_isoX * radius_width) - (_isoY * radius_width);
		float top = _startY + (_isoX * radius_height) + (_isoY * radius_height);

		DrawTile(left, top);
	}

	DrawUI();

	//sprintf_s(str, "isoX : %d, isoY : %d, corner : %d",
	//	_isoX, _isoY, _center);
	//TextOut(GetMemDC(), 700, 20, str, strlen(str));
	if (_isDebug) {
		RectangleMake(GetMemDC(), _ui._rcTopUI);
		for (int i = 0; i < 14; i++) {
			RectangleMake(GetMemDC(), _ui._rcLeftUI[i]);
		}
		for (int i = 0; i < 9; i++) {
			RectangleMake(GetMemDC(), _ui._rcBottomUI[i]);
		}

		for (int i = 0; i < 3; i++) {
			RectangleMake(GetMemDC(), _ui._rcToolBarUI[i]);
		}

		for (int i = 3; i < 6; i++) {
			RectangleMake(GetMemDC(), _ui._rcSongUI[i - 3]);
		}

		RectangleMake(GetMemDC(), _ui._rcTownNameUI);
		RectangleMake(GetMemDC(), _ui._rcPopulationUI[0]);
		RectangleMake(GetMemDC(), _ui._rcPopulationUI[1]);
								 
		RectangleMake(GetMemDC(), _ui._rcDebug);

		RectangleMake(GetMemDC(), _ui._rcPageUpUI);
		RectangleMake(GetMemDC(), _ui._rcPageDownUI);

		sprintf_s(str, "Debug Info");
		TextOut(GetMemDC(), WINSIZEX - 300, 60, str, strlen(str));

		sprintf_s(str, "worldTime : %f", FRAME->GetWorldTime());
		TextOut(GetMemDC(), WINSIZEX - 300, 80, str, strlen(str));

		sprintf_s(str, "ElapsedTime : %f", FRAME->GetElapsedTime());
		TextOut(GetMemDC(), WINSIZEX - 300, 100, str, strlen(str));

		sprintf_s(str, "cell_width : %.2f, cell_height : %.2f",
			cell_width, cell_height);
		TextOut(GetMemDC(), WINSIZEX - 300, 130, str, strlen(str));
		sprintf_s(str, "radius_width : %.2f, radius_height : %.2f",
			radius_width, radius_height);
		TextOut(GetMemDC(), WINSIZEX - 300, 150, str, strlen(str));

		sprintf_s(str, "show tile : %d", _tileCnt);
		TextOut(GetMemDC(), WINSIZEX - 300, 170, str, strlen(str));

		sprintf_s(str, "mouse pos : (%d, %d)", g_ptMouse.x, g_ptMouse.y);
		TextOut(GetMemDC(), WINSIZEX - 300, 190, str, strlen(str));

		sprintf_s(str, "start pos : (%d, %d)", _startX, _startY);
		TextOut(GetMemDC(), WINSIZEX - 300, 210, str, strlen(str));

		sprintf_s(str, "iso Tile pos : (%d, %d)", _isoX, _isoY);
		TextOut(GetMemDC(), WINSIZEX - 300, 230, str, strlen(str));
	}

}

void Simtown::UIInit()
{
	_ui._rcTopUI = RectMake(100, 0, WINSIZEX, 35);

	_ui._selectLeftUI = 0;

	_ui._rcLeftUI[0] = RectMake(0, 0, 100, 50);
	_ui._rcLeftUI[1] = RectMake(0, 50, 100, 50);
	_ui._rcLeftUI[2] = RectMake(0, 100, 100, 50);
	_ui._rcLeftUI[3] = RectMake(0, 150, 100, 60);
	_ui._rcLeftUI[4] = RectMake(0, 210, 100, 50);
	_ui._rcLeftUI[5] = RectMake(0, 260, 100, 50);
	_ui._rcLeftUI[6] = RectMake(0, 310, 100, 50);
	_ui._rcLeftUI[7] = RectMake(0, 360, 100, 35);
	_ui._rcLeftUI[8] = RectMake(0, 395, 100, 35);
	_ui._rcLeftUI[9] = RectMake(0, 430, 100, 45);
	_ui._rcLeftUI[10] = RectMake(0, 475, 100, 70);
	_ui._rcLeftUI[11] = RectMake(0, 545, 100, 35);
	_ui._rcLeftUI[12] = RectMake(0, 580, 100, 50);
	_ui._rcLeftUI[13] = RectMake(0, 630, 100, 70);

	_ui._rcBottomUI[0] = RectMake(100, WINSIZEY - 100, 300, 100);
	for (int i = 1; i < 9; i++) {
		_ui._rcBottomUI[i] = RectMake(100 * i + 300, WINSIZEY - 100, 100, 100);
	}
	_ui._selectBottomUI = 0;

	_ui._rcDebug = RectMake(WINSIZEX - 320, 40, 320, 300);

	_ui._pageNum = 0;

	_ui._rcPageUpUI = RectMake(
		_ui._rcBottomUI[8].left + 30, _ui._rcBottomUI[8].top + 10, 40, 40);
	_ui._rcPageDownUI = RectMake(
		_ui._rcBottomUI[8].left + 30, _ui._rcBottomUI[8].top + 50, 40, 40);

	for (int i = 0; i < 3; i++) {
		_ui._rcToolBarUI[i] = RectMake(i * 75 + 110, 5, 50, 25);
	}

	for (int i = 3; i < 6; i++) {
		_ui._rcSongUI[i - 3] = RectMake(i * 75 + 110, 5, 50, 25);
	}

	_ui._rcTownNameUI = RectMake(560, 5, 200, 25);
	_ui._rcPopulationUI[0] = RectMake(780, 5, 150, 25);
	_ui._rcPopulationUI[1] = RectMake(950, 0, 150, 35);
}

void Simtown::CheckSize()
{
	switch (_tileSize)
	{
	case TILESIZE_BIG:
		cell_width = CELL_WIDTH;
		cell_height = CELL_HEIGHT;
		radius_width = RADIUS_WIDTH;
		radius_height = RADIUS_HEIGHT;
		break;
	case TILESIZE_MID:
		cell_width = CELL_WIDTH2;
		cell_height = CELL_HEIGHT2;
		radius_width = cell_width / 2;
		radius_height = cell_height / 2;
		break;
	case TILESIZE_SMALL:
		cell_width = CELL_WIDTH3;
		cell_height = CELL_HEIGHT3;
		radius_width = cell_width / 2;
		radius_height = cell_height / 2;
		break;
	}
}

void Simtown::FindImg()
{
	_land = IMAGE->FindImage("land");
	
	FindUIImg();
	FindTileImg();

}

void Simtown::FindUIImg()
{
	_ui._leftUI = IMAGE->FindImage("ui_left");

	_ui._bottomUI[0] = IMAGE->FindImage("ui_bottom_1");
	_ui._bottomUI[1] = IMAGE->FindImage("ui_bottom_2");
	_ui._bottomUI[2] = IMAGE->FindImage("ui_bottom_3");
	_ui._bottomUI[3] = IMAGE->FindImage("ui_bottom_4");
	_ui._bottomUI[4] = IMAGE->FindImage("ui_bottom_5");

	_ui._blankUI = IMAGE->FindImage("ui_blank");

	_ui._pageUpUI1 = IMAGE->FindImage("ui_click_up1");
	_ui._pageUpUI2 = IMAGE->FindImage("ui_click_up2");
	_ui._pageDownUI1 = IMAGE->FindImage("ui_click_down1");
	_ui._pageDownUI2 = IMAGE->FindImage("ui_click_down2");

	_ui._songUI = IMAGE->FindImage("ui_song");
	_ui._toolBarUI = IMAGE->FindImage("ui_toolBar");

	_ui._populationUI = IMAGE->FindImage("ui_population");
}

void Simtown::FindTileImg()
{
	_tile._houses[0][TILESIZE_BIG] =
		IMAGE->FindImage("houses_adobe_house_big");
	_tile._houses[0][TILESIZE_MID] =
		IMAGE->FindImage("houses_adobe_house_mid");
	_tile._houses[0][TILESIZE_SMALL] =
		IMAGE->FindImage("houses_adobe_house_small");
	_tile._houses[0][TILESIZE_OPEN] =
		IMAGE->FindImage("houses_adobe_house_open");

	_tile._houses[2][TILESIZE_BIG] =
		IMAGE->FindImage("houses_ranch_house_big");
	_tile._houses[2][TILESIZE_MID] =
		IMAGE->FindImage("houses_ranch_house_mid");
	_tile._houses[2][TILESIZE_SMALL] =
		IMAGE->FindImage("houses_ranch_house_small");
	_tile._houses[2][TILESIZE_OPEN] =
		IMAGE->FindImage("houses_ranch_house_open");

	_tile._houses[3][TILESIZE_BIG] =
		IMAGE->FindImage("houses_apartment_big");
	_tile._houses[3][TILESIZE_MID] =
		IMAGE->FindImage("houses_apartment_mid");
	_tile._houses[3][TILESIZE_SMALL] =
		IMAGE->FindImage("houses_apartment_small");
	_tile._houses[3][TILESIZE_OPEN] =
		IMAGE->FindImage("houses_apartment_open");

	_tile._houses[4][TILESIZE_BIG] =
		IMAGE->FindImage("houses_victorian_house_big");
	_tile._houses[4][TILESIZE_MID] =
		IMAGE->FindImage("houses_victorian_house_mid");
	_tile._houses[4][TILESIZE_SMALL] =
		IMAGE->FindImage("houses_victorian_house_small");
	_tile._houses[4][TILESIZE_OPEN] =
		IMAGE->FindImage("houses_victorian_house_open");

	_tile._houses[5][TILESIZE_BIG] =
		IMAGE->FindImage("houses_cottage_big");
	_tile._houses[5][TILESIZE_MID] =
		IMAGE->FindImage("houses_cottage_mid");
	_tile._houses[5][TILESIZE_SMALL] =
		IMAGE->FindImage("houses_cottage_small");
	_tile._houses[5][TILESIZE_OPEN] =
		IMAGE->FindImage("houses_cottage_open");

	_tile._businesses[0][TILESIZE_BIG] =
		IMAGE->FindImage("businesses_hardware_store_big");
	_tile._businesses[0][TILESIZE_MID] =
		IMAGE->FindImage("businesses_hardware_store_mid");
	_tile._businesses[0][TILESIZE_SMALL] =
		IMAGE->FindImage("businesses_hardware_store_small");

	_tile._businesses[1][TILESIZE_BIG] =
		IMAGE->FindImage("businesses_pizza_parlor_big");
	_tile._businesses[1][TILESIZE_MID] =
		IMAGE->FindImage("businesses_pizza_parlor_mid");
	_tile._businesses[1][TILESIZE_SMALL] =
		IMAGE->FindImage("businesses_pizza_parlor_small");

	_tile._businesses[3][TILESIZE_BIG] =
		IMAGE->FindImage("businesses_chinese_restaurant_big");
	_tile._businesses[3][TILESIZE_MID] =
		IMAGE->FindImage("businesses_chinese_restaurant_mid");
	_tile._businesses[3][TILESIZE_SMALL] =
		IMAGE->FindImage("businesses_chinese_restaurant_small");

	_tile._businesses[5][TILESIZE_BIG] =
		IMAGE->FindImage("businesses_burger_joint_big");
	_tile._businesses[5][TILESIZE_MID] =
		IMAGE->FindImage("businesses_burger_joint_mid");
	_tile._businesses[5][TILESIZE_SMALL] =
		IMAGE->FindImage("businesses_burger_joint_small");

	_tile._businesses[8][TILESIZE_BIG] =
		IMAGE->FindImage("businesses_pet_store_big");
	_tile._businesses[8][TILESIZE_MID] =
		IMAGE->FindImage("businesses_pet_store_mid");
	_tile._businesses[8][TILESIZE_SMALL] =
		IMAGE->FindImage("businesses_pet_store_small");

	_tile._tile[0][TILESIZE_BIG] =
		IMAGE->FindImage("road_big");
	_tile._tile[0][TILESIZE_MID] =
		IMAGE->FindImage("road_mid");
	_tile._tile[0][TILESIZE_SMALL] =
		IMAGE->FindImage("road_mid");

	_tile._tile[1][TILESIZE_BIG] =
		IMAGE->FindImage("bike_path_big");
	_tile._tile[1][TILESIZE_MID] =
		IMAGE->FindImage("bike_path_mid");
	_tile._tile[1][TILESIZE_SMALL] =
		IMAGE->FindImage("bike_path_mid");
}

void Simtown::SelectTile()
{
	// 선택된 타일에 따라 isSelect true 
	switch (_currentTileKind)
	{
	case TILEKIND_TILESETS:
		switch (_currentTileNum)
		{
		case 0:
			if (_isoX + 1 >= TILE_COUNT_X || _isoY - 1 < 0 ||
				_tileMap[_isoX + 1][_isoY].isSelected ||
				_tileMap[_isoX][_isoY - 1].isSelected ||
				_tileMap[_isoX + 1][_isoY - 1].isSelected) {
				_tileMap[_isoX][_isoY].isStartDraw = false;
				return;
			}

			_tileMap[_isoX][_isoY].isSelected = true;
			_tileMap[_isoX + 1][_isoY].isSelected = true;
			_tileMap[_isoX][_isoY - 1].isSelected = true;
			_tileMap[_isoX + 1][_isoY - 1].isSelected = true;
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			break;
		case 15:
			break;
		case 16:
			break;
		case 17:
			break;
		case 18:
			break;
		case 19:
			break;
		case 20:
			break;
		}
		break;
	case TILEKIND_HOUSES:
		switch (_currentTileNum)
		{
		case 0:
			if (_isoY - 1 < 0 || 
				_tileMap[_isoX][_isoY - 1].isSelected) {
				_tileMap[_isoX][_isoY].isStartDraw = false;
				return;
			}
			_population += 2;

			_tileMap[_isoX][_isoY].isSelected = true;
			_tileMap[_isoX][_isoY - 1].isSelected = true;
			break;
		case 1:

			break;
		case 2:
			if (_isoX + 1 > TILE_COUNT_X ||
				_tileMap[_isoX + 1][_isoY].isSelected) {
				_tileMap[_isoX][_isoY].isStartDraw = false;
				return;
			}
			_population += 4;

			_tileMap[_isoX][_isoY].isSelected = true;
			_tileMap[_isoX + 1][_isoY].isSelected = true;
			break;
		case 3:
			if (_isoX + 1 > TILE_COUNT_X || _isoY - 1 < 0 ||
				_tileMap[_isoX + 1][_isoY].isSelected ||
				_tileMap[_isoX + 1][_isoY].isSelected ||
				_tileMap[_isoX + 1][_isoY - 1].isSelected) {
				_tileMap[_isoX][_isoY].isStartDraw = false;
				return;
			}
			_population += 8;

			_tileMap[_isoX][_isoY].isSelected = true;
			_tileMap[_isoX][_isoY - 1].isSelected = true;
			_tileMap[_isoX + 1][_isoY].isSelected = true;
			_tileMap[_isoX + 1][_isoY - 1].isSelected = true;
			break;
		case 4:
			if (_isoX + 1 > TILE_COUNT_X || _isoY - 1 < 0 ||
				_tileMap[_isoX + 1][_isoY].isSelected ||
				_tileMap[_isoX + 1][_isoY].isSelected ||
				_tileMap[_isoX + 1][_isoY - 1].isSelected) {
				_tileMap[_isoX][_isoY].isStartDraw = false;
				return;
			}
			_population += 4;

			_tileMap[_isoX][_isoY].isSelected = true;
			_tileMap[_isoX][_isoY - 1].isSelected = true;
			_tileMap[_isoX + 1][_isoY].isSelected = true;
			_tileMap[_isoX + 1][_isoY - 1].isSelected = true;
			break;
		case 5:
			if (_isoX + 1 > TILE_COUNT_X ||
				_tileMap[_isoX + 1][_isoY].isSelected) {
				_tileMap[_isoX][_isoY].isStartDraw = false;
				return;
			}
			_population += 3;

			_tileMap[_isoX][_isoY].isSelected = true;
			_tileMap[_isoX + 1][_isoY].isSelected = true;
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		}
		break;
	case TILEKIND_BUSINESSES:
		switch (_currentTileNum)
		{
		case 0:
			if (_isoX + 1 > TILE_COUNT_X ||
				_tileMap[_isoX + 1][_isoY].isSelected) {
				_tileMap[_isoX][_isoY].isStartDraw = false;
				return;
			}

			_tileMap[_isoX][_isoY].isSelected = true;
			_tileMap[_isoX + 1][_isoY].isSelected = true;
			break;
		case 1:
			if (_isoX + 1 > TILE_COUNT_X ||
				_tileMap[_isoX + 1][_isoY].isSelected) {
				_tileMap[_isoX][_isoY].isStartDraw = false;
				return;
			}

			_tileMap[_isoX][_isoY].isSelected = true;
			_tileMap[_isoX + 1][_isoY].isSelected = true;
			break;
		case 2:
			break;
		case 3:
			if (_isoX + 1 > TILE_COUNT_X ||
				_tileMap[_isoX + 1][_isoY].isSelected) {
				_tileMap[_isoX][_isoY].isStartDraw = false;
				return;
			}

			_tileMap[_isoX][_isoY].isSelected = true;
			_tileMap[_isoX + 1][_isoY].isSelected = true;
			break;
		case 4:
			break;
		case 5:
			if (_isoX + 1 > TILE_COUNT_X ||
				_tileMap[_isoX + 1][_isoY].isSelected) {
				_tileMap[_isoX][_isoY].isStartDraw = false;
				return;
			}

			_tileMap[_isoX][_isoY].isSelected = true;
			_tileMap[_isoX + 1][_isoY].isSelected = true;
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			if (_isoX + 1 > TILE_COUNT_X ||
				_tileMap[_isoX + 1][_isoY].isSelected) {
				_tileMap[_isoX][_isoY].isStartDraw = false;
				return;
			}

			_tileMap[_isoX][_isoY].isSelected = true;
			_tileMap[_isoX + 1][_isoY].isSelected = true;
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		}
		break;
	case TILEKIND_COMMUNITYBUILDINS:
		break;
	case TILEKIND_FUNPLACES:
		break;
	}

	// 타일에 선택된 타일 정보 저장
	_tileMap[_isoX][_isoY].tileKind = _currentTileKind;
	_tileMap[_isoX][_isoY].tileNum = _currentTileNum;
}

void Simtown::DeleteTile()
{
	// 선택된 타일에 따라 isSelect true 
	switch (_tileMap[_isoX][_isoY].tileKind)
	{
	case TILEKIND_TILESETS:
		switch (_tileMap[_isoX][_isoY].tileNum)
		{
		case 0:
			if (_isoX + 1 >= TILE_COUNT_X || _isoY - 1 < 0) {
				_tileMap[_isoX][_isoY].isStartDraw = true;
				return;
			}
			_tileMap[_isoX][_isoY].isSelected = false;
			_tileMap[_isoX + 1][_isoY].isSelected = false;
			_tileMap[_isoX][_isoY - 1].isSelected = false;
			_tileMap[_isoX + 1][_isoY - 1].isSelected = false;
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			break;
		case 15:
			break;
		case 16:
			break;
		case 17:
			break;
		case 18:
			break;
		case 19:
			break;
		case 20:
			break;
		}
		break;
	case TILEKIND_HOUSES:
		switch (_tileMap[_isoX][_isoY].tileNum)
		{
		case 0:
			if (_isoY - 1 < 0) {
				_tileMap[_isoX][_isoY].isStartDraw = true;
				return;
			}
			_population -= 2;

			_tileMap[_isoX][_isoY].isSelected = false;
			_tileMap[_isoX][_isoY - 1].isSelected = false;
			break;
		case 1:

			break;
		case 2:
			if (_isoX + 1 > TILE_COUNT_X) {
				_tileMap[_isoX][_isoY].isStartDraw = true;
				return;
			}
			_population -= 4;

			_tileMap[_isoX][_isoY].isSelected = false;
			_tileMap[_isoX + 1][_isoY].isSelected = false;
			break;
		case 3:
			if (_isoX + 1 > TILE_COUNT_X || _isoY - 1 < 0) {
				_tileMap[_isoX][_isoY].isStartDraw = true;
				return;
			}
			_population -= 8;

			_tileMap[_isoX][_isoY].isSelected = false;
			_tileMap[_isoX][_isoY - 1].isSelected = false;
			_tileMap[_isoX + 1][_isoY].isSelected = false;
			_tileMap[_isoX + 1][_isoY - 1].isSelected = false;
			break;
		case 4:
			if (_isoX + 1 > TILE_COUNT_X || _isoY - 1 < 0) {
				_tileMap[_isoX][_isoY].isStartDraw = true;
				return;
			}
			_population -= 4;

			_tileMap[_isoX][_isoY].isSelected = false;
			_tileMap[_isoX][_isoY - 1].isSelected = false;
			_tileMap[_isoX + 1][_isoY].isSelected = false;
			_tileMap[_isoX + 1][_isoY - 1].isSelected = false;
			break;
		case 5:
			if (_isoX + 1 > TILE_COUNT_X) {
				_tileMap[_isoX][_isoY].isStartDraw = true;
				return;
			}
			_population -= 3;

			_tileMap[_isoX][_isoY].isSelected = false;
			_tileMap[_isoX + 1][_isoY].isSelected = false;
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		}
		break;
	case TILEKIND_BUSINESSES:
		switch (_currentTileNum)
		{
		case 0:
			if (_isoX + 1 > TILE_COUNT_X) {
				_tileMap[_isoX][_isoY].isStartDraw = true;
				return;
			}

			_tileMap[_isoX][_isoY].isSelected = false;
			_tileMap[_isoX + 1][_isoY].isSelected = false;
			break;
		case 1:
			if (_isoX + 1 > TILE_COUNT_X) {
				_tileMap[_isoX][_isoY].isStartDraw = true;
				return;
			}

			_tileMap[_isoX][_isoY].isSelected = false;
			_tileMap[_isoX + 1][_isoY].isSelected = false;
			break;
		case 2:
			break;
		case 3:
			if (_isoX + 1 > TILE_COUNT_X) {
				_tileMap[_isoX][_isoY].isStartDraw = true;
				return;
			}

			_tileMap[_isoX][_isoY].isSelected = false;
			_tileMap[_isoX + 1][_isoY].isSelected = false;
			break;
		case 4:
			break;
		case 5:
			if (_isoX + 1 > TILE_COUNT_X) {
				_tileMap[_isoX][_isoY].isStartDraw = true;
				return;
			}

			_tileMap[_isoX][_isoY].isSelected = false;
			_tileMap[_isoX + 1][_isoY].isSelected = false;
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			if (_isoX + 1 > TILE_COUNT_X) {
				_tileMap[_isoX][_isoY].isStartDraw = true;
				return;
			}

			_tileMap[_isoX][_isoY].isSelected = false;
			_tileMap[_isoX + 1][_isoY].isSelected = false;
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		}
		break;
	case TILEKIND_COMMUNITYBUILDINS:
		break;
	case TILEKIND_FUNPLACES:
		break;
	}

	_tileMap[_isoX][_isoY].tileKind = -1;
	_tileMap[_isoX][_isoY].tileNum = -1;
}

void Simtown::DrawTileMap()
{
	_tileCnt = 0;
	// land 그리기
	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			float left = _startX + (i * radius_width) - (j * radius_width);
			float top = _startY + (i * radius_height) + (j * radius_height);

			if (left + cell_width < WINSIZEX / 12 || left > WINSIZEX
				|| top + cell_height < WINSIZEY / 18 || top > WINSIZEY - WINSIZEY / 7) continue;

			// render 될 이미지 여기서 render
			// 함수 호출보다 포인트 받아서 하는게 더 빠름
			//IMAGE->Render("land_big", GetMemDC(), left, top);

			_land->Render(GetMemDC(), left, top, cell_width, cell_height);
			
			_tileCnt++;

			if (_isDebug) {
				if (_tileMap[i][j].isSelected) {
					HPEN myPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
					HPEN oldPen = (HPEN)SelectObject(GetMemDC(), myPen);
					DrawRhombus(left, top);
					DeleteObject(SelectObject(GetMemDC(), oldPen));
				}
				else {
					DrawRhombus(left, top);
					//SetTextColor(GetMemDC(), RGB(0, 0, 0));
					//sprintf_s(str, "(%d,%d)", i, j);
					//TextOut(GetMemDC(),
					//	left + RADIUS_WIDTH / 2 + 8,
					//	top + RADIUS_HEIGHT / 2 + 5, str, strlen(str));
				}
			}
		}
	}

	// tile 그리기
	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			float left = _startX + (i * radius_width) - (j * radius_width);
			float top = _startY + (i * radius_height) + (j * radius_height);

			if (left + cell_width < WINSIZEX / 12 || left > WINSIZEX
				|| top + cell_height < WINSIZEY / 18 || top > WINSIZEY - WINSIZEY / 7) continue;

			if (_tileMap[i][j].isStartDraw) {
				switch (_tileMap[i][j].tileKind)
				{
				case TILEKIND_TILESETS:
					_tile._tile[_tileMap[i][j].tileNum][_tileSize]
						->FrameRender(GetMemDC(), left,
							top - cell_height / 2,
							MakeRoad(i,j), 0, cell_width * 2, cell_height * 2);
					break;
				case TILEKIND_HOUSES:
					switch (_tileMap[i][j].tileNum)
					{
					case 0:
						if (_ui._selectLeftUI == 6 && cell_width > CELL_WIDTH) {
							_tile._houses[_tileMap[i][j].tileNum][TILESIZE_OPEN]
								->Render(GetMemDC(), left, top - cell_height,
									cell_width + cell_width / 4,
									cell_height * 2);
						}
						else
							_tile._houses[_tileMap[i][j].tileNum][_tileSize]
							->Render(GetMemDC(), left, top - cell_height,
								cell_width + cell_width / 4,
								cell_height * 2);
						break;
					case 1:

						break;
					case 2:
						if (_ui._selectLeftUI == 6 && cell_width > CELL_WIDTH) {
							_tile._houses[_tileMap[i][j].tileNum][TILESIZE_OPEN]
								->Render(GetMemDC(), left,
									top - cell_height / 2 - cell_height / 4,
									cell_width + cell_width / 4,
									cell_height * 2);
						}
						else
							_tile._houses[_tileMap[i][j].tileNum][_tileSize]
							->Render(GetMemDC(), left,
								top - cell_height / 2 - cell_height / 4,
								cell_width + cell_width / 4,
								cell_height * 2);
						break;
					case 3:
						if (_ui._selectLeftUI == 6 && cell_width > CELL_WIDTH) {
							_tile._houses[_tileMap[i][j].tileNum][TILESIZE_OPEN]
								->Render(GetMemDC(), left,
									top - cell_height - cell_height / 2 - cell_height / 4 - cell_height / 8,
									cell_width + cell_width / 2,
									cell_height * 3 + cell_height / 4);
						}
						else
							_tile._houses[_tileMap[i][j].tileNum][_tileSize]
							->Render(GetMemDC(), left,
								top - cell_height - cell_height / 2 - cell_height / 4 - cell_height / 8,
								cell_width + cell_width / 2,
								cell_height * 3 + cell_height / 4);
						break;
					case 4:
						if (_ui._selectLeftUI == 6 && cell_width > CELL_WIDTH) {
							_tile._houses[_tileMap[i][j].tileNum][TILESIZE_OPEN]
								->Render(GetMemDC(), left,
									top - cell_height - cell_height / 2 - cell_height / 4 - cell_height / 4,
									cell_width + cell_width / 2 + cell_width / 8,
									cell_height * 3 + cell_height / 4);
						}
						else
							_tile._houses[_tileMap[i][j].tileNum][_tileSize]
							->Render(GetMemDC(), left,
								top - cell_height - cell_height / 2 - cell_height / 4 - cell_height / 4,
								cell_width + cell_width / 2 + cell_width / 8,
								cell_height * 3 + cell_height / 4);
						break;
					case 5:
						if (_ui._selectLeftUI == 6 && cell_width > CELL_WIDTH) {
							_tile._houses[_tileMap[i][j].tileNum][TILESIZE_OPEN]
								->Render(GetMemDC(), left, top - cell_height,
									cell_width + cell_width / 4,
									cell_height * 2 + cell_height / 4);
						}
						else
							_tile._houses[_tileMap[i][j].tileNum][_tileSize]
							->Render(GetMemDC(), left, top - cell_height,
								cell_width + cell_width / 4,
								cell_height * 2 + cell_height / 4);
						break;
					case 6:
						break;
					case 7:
						break;
					case 8:
						break;
					case 9:
						break;
					case 10:
						break;
					case 11:
						break;
					case 12:
						break;
					case 13:
						break;
					}
					
					break;
				case TILEKIND_BUSINESSES:
					switch (_tileMap[i][j].tileNum)
					{
					case 0:
						_tile._businesses[_tileMap[i][j].tileNum][_tileSize]
							->Render(GetMemDC(), left, top - cell_height + cell_height / 16,
								cell_width,
								cell_height * 2);
						break;
					case 1:
						_tile._businesses[_tileMap[i][j].tileNum][_tileSize]
							->Render(GetMemDC(), left, top - cell_height + cell_height / 16,
								cell_width,
								cell_height * 2);
						break;
					case 2:
						break;
					case 3:
						_tile._businesses[_tileMap[i][j].tileNum][_tileSize]
							->Render(GetMemDC(), left, top - cell_height,
								cell_width,
								cell_height * 2);
						break;
					case 4:
						break;
					case 5:
						_tile._businesses[_tileMap[i][j].tileNum][_tileSize]
							->Render(GetMemDC(), left, top - cell_height + cell_height / 16,
								cell_width,
								cell_height * 2);
						break;
					case 6:
						break;
					case 7:
						break;
					case 8:
						_tile._businesses[_tileMap[i][j].tileNum][_tileSize]
							->Render(GetMemDC(), left, top - cell_height + cell_height / 16,
								cell_width,
								cell_height * 2);
						break;
					case 9:
						break;
					case 10:
						break;
					case 11:
						break;
					case 12:
						break;
					case 13:
						break;
					}
					break;
				case TILEKIND_COMMUNITYBUILDINS:
					break;
				case TILEKIND_FUNPLACES:
					break;
				}
			}
		}
	}
}

void Simtown::DrawRhombus(float left, float top)
{
	float centerX = left + radius_width;
	float centerY = top + radius_height;

	POINT p[5];
	p[0].x = centerX;
	p[0].y = centerY - radius_height;

	p[1].x = centerX + radius_width;
	p[1].y = centerY;

	p[2].x = centerX;
	p[2].y = centerY + radius_height;

	p[3].x = centerX - radius_width;
	p[3].y = centerY;

	p[4].x = centerX;
	p[4].y = centerY - radius_height;

	for (int i = 1; i < 5; i++) {
		LineMake(GetMemDC(), p[i - 1], p[i]);
	}
}

int Simtown::GetCornerIndex(int isoX, int isoY)
{
	float left = _startX + (isoX * radius_width) - (isoY * radius_width);
	float top = _startY + (isoX * radius_height) + (isoY * radius_height);

	float dx = (g_ptMouse.x - left) / (float)cell_width;
	float dy = (g_ptMouse.y - top) / (float)cell_height;

	if (dx < 0.5f && dy < 0.5f) return 1;
	if (dx >= 0.5f && dy < 0.5f) return 2;
	if (dx < 0.5f && dy >= 0.5f) return 3;
	if (dx >= 0.5f && dy >= 0.5f) return 4;

	return 0;
}

bool Simtown::IsInRhombus(int corner, int isoX, int isoY)
{
	// lefttop
	if (corner == 1) {
		float left = _startX + (isoX * radius_width) - (isoY * radius_width);
		float top = _startY + (isoX * radius_height) + (isoY * radius_height);

		float dx = (float)(g_ptMouse.x - left) / radius_width;
		float dy = (float)(g_ptMouse.y - top) / radius_height;

		if ((1.0f - dy) <= dx) return true;
		else return false;
	}
	// righttop
	else if (corner == 2) {
		float left = _startX + (isoX * radius_width) - (isoY * radius_width);
		left += radius_width;
		float top = _startY + (isoX * radius_height) + (isoY * radius_height);

		float dx = (float)(g_ptMouse.x - left) / radius_width;
		float dy = (float)(g_ptMouse.y - top) / radius_height;

		if (dy >= dx) return true;
		else return false;
	}
	// leftdown
	else if (corner == 3) {
		float left = _startX + (isoX * radius_width) - (isoY * radius_width);
		float top = _startY + (isoX * radius_height) + (isoY * radius_height);
		top += radius_height;

		float dx = (float)(g_ptMouse.x - left) / radius_width;
		float dy = (float)(g_ptMouse.y - top) / radius_height;

		if (dy < dx) return true;
		else return false;
	}
	// rightdown
	else if (corner == 4) {
		float left = _startX + (isoX * radius_width) - (isoY * radius_width);
		left += radius_width;
		float top = _startY + (isoX * radius_height) + (isoY * radius_height);
		top += radius_height;

		float dx = (float)(g_ptMouse.x - left) / radius_width;
		float dy = (float)(g_ptMouse.y - top) / radius_height;

		if ((1.0f - dy) > dx) return true;
		else return false;
	}

	return false;

}

void Simtown::DrawUI()
{
	// top
	_ui._blankUI->Render(GetMemDC(),
		_ui._rcTopUI.left, _ui._rcTopUI.top, WINSIZEX, 35);

	for (int i = 0; i < 3; i++) {
		_ui._toolBarUI->FrameRender(GetMemDC(),
			_ui._rcToolBarUI[i].left, _ui._rcToolBarUI[i].top, i, 0);
		_ui._songUI->FrameRender(GetMemDC(),
			_ui._rcSongUI[i].left, _ui._rcSongUI[i].top, i, 0);
	}

	SetTextColor(GetMemDC(), RGB(0, 0, 0));

	BeginCreateFont(GetMemDC(), &hfont, 14);

	sprintf_s(str, "마을 이름 : 심타운");
	TextOut(GetMemDC(), _ui._rcTownNameUI.left + 15,
		_ui._rcTownNameUI.top + 5, str, strlen(str));

	sprintf_s(str, "인구 : %d명", _population);
	TextOut(GetMemDC(), _ui._rcPopulationUI[0].left,
		_ui._rcPopulationUI[0].top + 5, str, strlen(str));

	_ui._populationUI->FrameRender(GetMemDC(),
		_ui._rcPopulationUI[1].left, _ui._rcPopulationUI[1].top, 0, 0);

	LineMake(GetMemDC(), PointMake(560, 0), PointMake(560, 35));
	LineMake(GetMemDC(), PointMake(760, 0), PointMake(760, 35));
	LineMake(GetMemDC(), PointMake(950, 0), PointMake(950, 35));

	// left
	_ui._leftUI->FrameRender(GetMemDC(),
		_ui._rcLeftUI[0].left, _ui._rcLeftUI[0].top, 
		_ui._selectLeftUI + _tileSize * 11, 0);

	// bottom

	if (_isClicked) {

		_ui._blankUI->Render(GetMemDC(),
			_ui._rcBottomUI[0].left, _ui._rcBottomUI[0].top, 300, 100);
		_ui._blankUI->Render(GetMemDC(),
			_ui._rcBottomUI[8].left, _ui._rcBottomUI[8].top);

		Image * temp = NULL;

		switch (_ui._selectLeftUI)
		{
		case 0:
			temp = _ui._bottomUI[0];
			break;
		case 1:
			temp = _ui._bottomUI[1];
			break;
		case 2:
			temp = _ui._bottomUI[2];
			break;
		case 3:
			temp = _ui._bottomUI[3];
			break;
		case 4:
			temp = _ui._bottomUI[4];
			break;
		}

		for (int i = 1; i < 8; i++) {
			if (!(_ui._selectLeftUI == 0 || _ui._selectLeftUI == 1 || _ui._selectLeftUI == 2))
				_ui._pageNum = 0;

			if (i - 1 == _ui._selectBottomUI)
				temp->FrameRender(GetMemDC(),
					_ui._rcBottomUI[i].left, _ui._rcBottomUI[i].top, 1, i - 1 + _ui._pageNum * 7);
			else
				temp->FrameRender(GetMemDC(),
					_ui._rcBottomUI[i].left, _ui._rcBottomUI[i].top, 0, i - 1 + _ui._pageNum * 7);
		}

		// page

		if (_ui._selectLeftUI == 0 || _ui._selectLeftUI == 1 || _ui._selectLeftUI == 2) {
			_ui._pageUpUI2->Render(GetMemDC(), _ui._rcPageUpUI.left, _ui._rcPageUpUI.top);
			_ui._pageDownUI2->Render(GetMemDC(), _ui._rcPageDownUI.left, _ui._rcPageDownUI.top);
		}
		else {
			_ui._pageUpUI1->Render(GetMemDC(), _ui._rcPageUpUI.left, _ui._rcPageUpUI.top);
			_ui._pageDownUI1->Render(GetMemDC(), _ui._rcPageDownUI.left, _ui._rcPageDownUI.top);
		}
	}
	else {
		_ui._blankUI->Render(GetMemDC(),
			_ui._rcBottomUI[0].left, _ui._rcBottomUI[0].top, WINSIZEX, 100);
	}

	LineMake(GetMemDC(), PointMake(100, 35), PointMake(WINSIZEX, 35));
	LineMake(GetMemDC(), PointMake(400, WINSIZEY - 100), PointMake(400, WINSIZEY));
	LineMake(GetMemDC(), PointMake(100, WINSIZEY - 100), PointMake(WINSIZEX, WINSIZEY - 100));
	LineMake(GetMemDC(), PointMake(0, 0), PointMake(WINSIZEX, 0));
	LineMake(GetMemDC(), PointMake(100, 0), PointMake(100, WINSIZEY));
}

void Simtown::DrawTile(int left, int top)
{
	switch (_currentTileKind)
	{
	case TILEKIND_TILESETS:
		_tile._tile[_currentTileNum][_tileSize]
			->FrameRender(GetMemDC(), left, 
				top - cell_height / 2,
				0, 0, cell_width * 2, cell_height * 2);
		break;
	case TILEKIND_HOUSES:
		switch (_currentTileNum)
		{
		case 0:		
			_tile._houses[_currentTileNum][_tileSize]
			->Render(GetMemDC(), left, top - cell_height,
				cell_width + cell_width / 4,
				cell_height * 2);
			break;
		case 1:

			break;
		case 2:
			_tile._houses[_currentTileNum][_tileSize]
				->Render(GetMemDC(), left, 
					top - cell_height / 2 - cell_height / 4,
					cell_width + cell_width / 4,
					cell_height * 2);
			break;
		case 3:
			_tile._houses[_currentTileNum][_tileSize]
				->Render(GetMemDC(), left,
					top - cell_height - cell_height / 2 - cell_height / 4 - cell_height / 8,
					cell_width + cell_width / 2,
					cell_height * 3 + cell_height / 4);
			break;
		case 4:
			_tile._houses[_currentTileNum][_tileSize]
				->Render(GetMemDC(), left,
					top - cell_height - cell_height / 2 - cell_height / 4 - cell_height / 4,
					cell_width + cell_width / 2 + cell_width / 8,
					cell_height * 3 + cell_height / 4);
			break;
		case 5:
			_tile._houses[_currentTileNum][_tileSize]
				->Render(GetMemDC(), left, top - cell_height,
					cell_width + cell_width / 4,
					cell_height * 2 + cell_height / 4);
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		}

		break; 
	case TILEKIND_BUSINESSES:
		switch (_currentTileNum)
		{
		case 0:
			_tile._businesses[_currentTileNum][_tileSize]
				->Render(GetMemDC(), left, top - cell_height + cell_height / 16,
					cell_width,
					cell_height * 2);
			break;
		case 1:
			_tile._businesses[_currentTileNum][_tileSize]
				->Render(GetMemDC(), left, top - cell_height + cell_height / 16,
					cell_width,
					cell_height * 2);
			break;
		case 2:
			break;
		case 3:
			_tile._businesses[_currentTileNum][_tileSize]
				->Render(GetMemDC(), left, top - cell_height,
					cell_width,
					cell_height * 2);
			break;
		case 4:
			break;
		case 5:
			_tile._businesses[_currentTileNum][_tileSize]
				->Render(GetMemDC(), left, top - cell_height + cell_height / 16,
					cell_width,
					cell_height * 2);
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			_tile._businesses[_currentTileNum][_tileSize]
				->Render(GetMemDC(), left, top - cell_height + cell_height / 16,
					cell_width,
					cell_height * 2);
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		}
		break;
	case TILEKIND_COMMUNITYBUILDINS:
		switch (_currentTileNum)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		}
		break;
	case TILEKIND_FUNPLACES:
		switch (_currentTileNum)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		}
		break;
	}
}

void Simtown::SaveTile()
{
	// api가 가지고있는 save 함수 사용
	// 구조체로 넣을 수도 있고
	// 마찬가지로 배열로도 넣을 수 있음
	// 바이너리 파일이라 이미지 같은 웬만한거 다됨
	HANDLE file;
	DWORD write;

	// CreateFile Binary 형식으로 들어감
	// Binary 16진수 데이터 형식으로 들어감
	// Binary 한글 관련으로 문제가 있었음
	// binary라 일반적인 fopen 보다 빠름 대신 한줄로 쭉 써짐
	file = CreateFile(
		"save/tileMap.map",	// 생성할 파일 또는 로드할 파일의 이름
		GENERIC_WRITE,	// 파일이나 장치를 만들거나 열때의 권한
		0,				// 파일 공유 모드 입력
		NULL,			// 파일 또는 장치를 열 때 보안 및 특성
						// 항상 처음부터 작성하겠다는거 다 지우고
						// 여기 바꿔주면 라인별로 넣을 수 있음
		CREATE_ALWAYS,	// 파일이나 장치를 열 때 취할 행동
		FILE_ATTRIBUTE_NORMAL,	// 파일, 장치를 열 때, 만들 때 갖는 특성
		NULL			// 만들어질 파일이 갖게 될 특성 etc 저장되는 핸들
	);

	// 구분점은 따로 없고 쭉 들어가므로 사이즈로 구분해서 읽어와야함
	WriteFile(file, _tileMap,
		sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y, &write, NULL);

	CloseHandle(file);
}

void Simtown::LoadTile()
{
	// api가 가지고있는 save 함수 사용
	// 구조체로 넣을 수도 있고
	// 마찬가지로 배열로도 넣을 수 있음
	HANDLE file;
	DWORD read;

	// CreateFile Binary 형식으로 들어감
	// Binary 16진수 데이터 형식으로 들어감
	// Binary 한글 관련으로 문제가 있었음
	// binary라 일반적인 fopen 보다 빠름 대신 한줄로 쭉 써짐
	file = CreateFile(
		"save/tileMap.map",	// 생성할 파일 또는 로드할 파일의 이름
		GENERIC_READ, /// 수정	// 파일이나 장치를 만들거나 열때의 권한
		0,				// 파일 공유 모드 입력
		NULL,			// 파일 또는 장치를 열 때 보안 및 특성
						// 항상 처음부터 작성하겠다는거 다 지우고
						// 여기 바꿔주면 라인별로 넣을 수 있음
		OPEN_EXISTING, /// 수정	// 파일이나 장치를 열 때 취할 행동
		FILE_ATTRIBUTE_NORMAL,	// 파일, 장치를 열 때, 만들 때 갖는 특성
		NULL			// 만들어질 파일이 갖게 될 특성 etc 저장되는 핸들
	);

	// 구분점은 따로 없고 쭉 들어가므로 사이즈로 구분해서 읽어와야함
	ReadFile(file, _tileMap,
		sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y, &read, NULL);

	CloseHandle(file);
}

void Simtown::InitTile()
{
	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			_tileMap[i][j].isSelected = false;
			_tileMap[i][j].isStartDraw = false;

			_tileMap[i][j].tileKind = -1;
			_tileMap[i][j].tileNum = -1;
		}
	}
}

int Simtown::MakeRoad(int i, int j)
{
	// 기본 이미지
	int frameX = 0;

	// check
	bool isClicked[4];
	bool isSame[4];

	tagTile temp;
	// left
	if (i - 2 >= 0) {
		temp = _tileMap[i - 2][j];
		isSame[0] =
			temp.tileKind == _tileMap[i][j].tileKind &&
			temp.tileNum == _tileMap[i][j].tileNum &&
			temp.tileKind != -1 ?
			true : false;
	}
	else
		isSame[0] = false;
	if (i - 1 >= 0) {
		temp = _tileMap[i - 1][j];
		isClicked[0] = temp.isSelected;
	}
	else
		isClicked[0] = true;
	// top
	if (j - 2 >= 0) {
		temp = _tileMap[i][j - 2];
		isSame[1] =
			temp.tileKind == _tileMap[i][j].tileKind &&
			temp.tileNum == _tileMap[i][j].tileNum &&
			temp.tileKind != -1 ?
			true : false;
	}
	else
		isSame[1] = false;
	if (j - 2 >= 0) {
		temp = _tileMap[i][j - 2];
		isClicked[1] = temp.isSelected;
	}
	else
		isClicked[1] = true;
	// right
	if (i + 2 < TILE_COUNT_X) {
		temp = _tileMap[i + 2][j];
		isSame[2] =
			temp.tileKind == _tileMap[i][j].tileKind &&
			temp.tileNum == _tileMap[i][j].tileNum &&
			temp.tileKind != -1 ?
			true : false;
	}
	else
		isSame[2] = false;
	if (i + 2 < TILE_COUNT_X) {
		temp = _tileMap[i + 2][j];
		isClicked[2] = temp.isSelected;
	}
	else
		isClicked[2] = true;
	// bottom
	if (j + 2 < TILE_COUNT_Y) {
		temp = _tileMap[i][j + 2];
		isSame[3] =
			temp.tileKind == _tileMap[i][j].tileKind &&
			temp.tileNum == _tileMap[i][j].tileNum &&
			temp.tileKind != -1 ?
			true : false;
	}
	else
		isSame[3] = false;
	if (j + 1 < TILE_COUNT_Y) {
		temp = _tileMap[i][j + 1];
		isClicked[3] = temp.isSelected;
	}
	else
		isClicked[3] = true;
	
	// 왼 타일 있을 때
	if(isSame[0]) {
		frameX = 1;
		// 위 타일 있을 때
		if (isSame[1]) {
			frameX = 14;
			// 오른 타일이나 아래 타일이 있을 때
			if (isSame[2] || isSame[3]) {
				frameX = 2;
				// 아래 타일이 막혀 있을 때
				if (!isSame[3] && isClicked[3])
					frameX = 14;
			}
			// 오른 타일이 막혀있을 때
			if (!isSame[2] && isClicked[2]) {
				frameX = 3;
			}
		}
		// 아래 타일 있을 때
		else if (isSame[3]) {
			frameX = 11;
			// 위 타일이나 오른 타일이 있을 때
			if (isSame[1] || isSame[2]) {
				frameX = 2;
				// 위 타일이 막혀 있을 때
				if (!isSame[1] && isClicked[1])
					frameX = 11;
			}
			// 오른 타일이 막혀있을 때
			if (!isSame[2] && isClicked[2]) {
				frameX = 10;
			}
		}
		// 오른 타일이 막혀있을 때
		if (!isSame[2] && isClicked[2] &&
			frameX != 3 && frameX != 10) {
			frameX = 7;
		}
	}

	// 위 타일 있을 때
	if (isSame[1]) {
		frameX = 0;
		// 왼 타일 있을 때
		if (isSame[0]) {
			frameX = 13;
			// 오른 타일이나 아래 타일이 있을 때
			if (isSame[2] || isSame[3]) {
				frameX = 2;
				// 오른 타일이 막혀있을 때
				if (!isSame[2] && isClicked[2])
					frameX = 13;
			}
			// 아래 타일이 막혀있을 때
			if (!isSame[3] && isClicked[3]) {
				frameX = 3;
			}
		}
		// 오른 타일 있을 때
		else if (isSame[2]) {
			frameX = 12;
			// 왼 타일이나 아래 타일이 있을 때
			if (isSame[0] || isSame[3]) {
				frameX = 2;
				// 왼 타일이 막혀있을 때
				if(!isSame[0] && isClicked[0])
					frameX = 12;
			}
			// 아래 타일이 막혀있을 때
			if (!isSame[3] && isClicked[3]) {
				frameX = 9;
			}
		}
		// 아래 타일이 막혀있을 때
		if (!isSame[3] && isClicked[3] &&
			frameX != 3 && frameX != 9) {
			frameX = 8;
		}
	}

	// 오른 타일 있을 때
	if (isSame[2]) {
		frameX = 1;
		// 위 타일 있을 때
		if (isSame[1]) {
			frameX = 14;
			// 왼 타일이나 아래 타일이 있을 때
			if (isSame[0] || isSame[3]) {
				frameX = 2;
				// 아래 타일이 막혀있을 때
				if (!isSame[3] && isClicked[3])
					frameX = 14;
			}
			// 왼 타일이 막혀있을 때
			if (!isSame[0] && isClicked[0]) {
				frameX = 9;
			}
		}
		// 아래 타일 있을 때
		else if (isSame[3]) {
			frameX = 11;
			// 왼 타일이나 위 타일이 있을 때
			if (isSame[0] || isSame[1]) {
				frameX = 2;
				// 위 타일이 막혀있을 때
				if (!isSame[1] && isClicked[1])
					frameX = 11;
			}
			// 왼 타일이 막혀있을 때
			if (!isSame[0] && isClicked[0]) {
				frameX = 6;
			}
		}
		// 왼 타일이 막혀있을 때
		if (!isSame[0] && isClicked[0] &&
			frameX != 9 && frameX != 6) {
			frameX = 5;
		}
	}

	// 아래 타일 있을 때
	if (isSame[3]) {
		frameX = 0;
		// 왼 타일 있을 때
		if (isSame[0]) {
			frameX = 13;
			// 위 타일이나 오른 타일이 있을 때
			if (isSame[1] || isSame[2]) {
				frameX = 2;
				// 오른 타일이 막혀있을 때
				if (!isSame[2] && isClicked[2])
					frameX = 13;
			}
			// 위 타일이 막혀있을 때
			if (!isSame[1] && isClicked[1]) {
				frameX = 10;
			}
		}
		// 오른 타일 있을 때
		else if (isSame[2]) {
			frameX = 12;
			// 왼 타일이나 위 타일이 있을 때
			if (isSame[0] || isSame[1]) {
				frameX = 2;
				// 왼 타일이 막혀있을 때
				if (!isSame[0] && isClicked[0])
					frameX = 12;
			}
			// 위 타일이 막혀있을 때
			if (!isSame[1] && isClicked[1]) {
				frameX = 6;
			}
		}
		// 위 타일이 막혀있을 때
		if (!isSame[1] && isClicked[1] && 
			frameX != 10 && frameX != 6) {
			frameX = 4;
		}
	}

	return frameX;
}

void Simtown::PlaySong(int num)
{
	switch (num)
	{
	case 0:
		if (SOUND->IsPlaySound("song2")) SOUND->Stop("song2");
		if (SOUND->IsPlaySound("song3")) SOUND->Stop("song3");
		if (SOUND->IsPlaySound("song1")) SOUND->Stop("song1");
		else
			SOUND->Play("song1", 0.75f);
		break;
	case 1:
		if (SOUND->IsPlaySound("song1")) SOUND->Stop("song1");
		if (SOUND->IsPlaySound("song3")) SOUND->Stop("song3");
		if (SOUND->IsPlaySound("song2")) SOUND->Stop("song2");
		else
			SOUND->Play("song2", 0.75f);
		break;
	case 2:
		if (SOUND->IsPlaySound("song1")) SOUND->Stop("song1");
		if (SOUND->IsPlaySound("song2")) SOUND->Stop("song2");
		if (SOUND->IsPlaySound("song3")) SOUND->Stop("song3");
		else
			SOUND->Play("song3", 0.75f);
		break;
	}
}
