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

	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			_tileMap[i][j].isSelected = false;
			_tileMap[i][j].isStartDraw = false;
		}
	}

	UIInit();

	_isClicked = false;

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

	if (INPUT->GetKey('1')) {
		switch (_tileSize)
		{
		case TILESIZE_SMALL:
			_startX -= 80;
			_startY -= 495;
			break;
		case TILESIZE_MID:
			_startX -= 50;
			_startY -= 330;
			break;
		case TILESIZE_BIG:
			break;
		}
		_tileSize = TILESIZE_BIG; 
		CheckSize(); 
	}
	if (INPUT->GetKey('2')) { 
		switch (_tileSize)
		{
		case TILESIZE_SMALL:
			_startX -= 30;
			_startY -= 165;
			break;
		case TILESIZE_MID:
			break;
		case TILESIZE_BIG:
			_startX += 50;
			_startY += 330;
			break;
		}
		_tileSize = TILESIZE_MID; 
		CheckSize(); 
	}
	if (INPUT->GetKey('3')) { 
		switch (_tileSize)
		{
		case TILESIZE_SMALL:
			break;
		case TILESIZE_MID:
			_startX += 30;
			_startY += 165;
			break;
		case TILESIZE_BIG:
			_startX += 80;
			_startY += 495;
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

		_startY += 6;

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

		_startY -= 6;

		if (cell_width > CELL_WIDTH2 + 24) {
			_tileSize = TILESIZE_BIG;
		}
		else if (cell_width > CELL_WIDTH3) {
			_tileSize = TILESIZE_MID;
		}
		else
			_tileSize = TILESIZE_SMALL;
	}

	if (INPUT->GetKeyDown(VK_TAB)) {
		_isDebug = !_isDebug;
	}

	if (INPUT->GetKey(VK_LBUTTON)) {
		// tile Select
		if(g_ptMouse.x > 100 
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
						_tileMap[isoX][isoY].isStartDraw = false;
						DeleteTile();
					}

					if (!_tileMap[isoX][isoY].isSelected && _isClicked) {
						_tileMap[isoX][isoY].isStartDraw = true;
						SelectTile();
						//_tileMap[isoX][isoY].isSelected = true;
					}
				}
			}
		}
	}

	if (INPUT->GetKeyDown(VK_LBUTTON)) {
		// ui Select
		{
			// left ui
			for (int i = 0; i < 14; i++) {
				if (PtInRect(&_ui._rcLeftUI[i], g_ptMouse)) {
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
					_ui._selectBottomUI = i - 1;
					_currentTileNum = i - 1 + (_ui._pageNum * 7);
					break;
				}
			}

			// page ui
			if (PtInRect(&_ui._rcPageDownUI, g_ptMouse)) {
				if (_ui._selectLeftUI == 0 || _ui._selectLeftUI == 1 || _ui._selectLeftUI == 2) {
					if (_ui._selectLeftUI == 0 && _ui._pageNum <= 1)
						_ui._pageNum++;
					else if (_ui._pageNum <= 0)
						_ui._pageNum++;
				}
			}
			if (PtInRect(&_ui._rcPageUpUI, g_ptMouse)) {
				if (_ui._selectLeftUI == 0 || _ui._selectLeftUI == 1 || _ui._selectLeftUI == 2) {
					if (_ui._pageNum >= 1)
						_ui._pageNum--;
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
	FindBuildingImg();

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

void Simtown::FindBuildingImg()
{
	_building._houses[0][TILESIZE_BIG] =
		IMAGE->FindImage("houses_adobe_house_big");
	_building._houses[0][TILESIZE_MID] =
		IMAGE->FindImage("houses_adobe_house_mid");
	_building._houses[0][TILESIZE_SMALL] =
		IMAGE->FindImage("houses_adobe_house_small");
	_building._houses[0][TILESIZE_OPEN] =
		IMAGE->FindImage("houses_adobe_house_open");
}

void Simtown::SelectTile()
{
	// 선택된 타일에 따라 isSelect true 
	switch (_currentTileKind)
	{
	case TILEKIND_TILESETS:
		break;
	case TILEKIND_HOUSES:
		switch (_currentTileNum)
		{
		case 0:
			if (_tileMap[_isoX][_isoY - 1].isSelected) {
				_tileMap[_isoX][_isoY].isStartDraw = false;
				return;
			}

			_tileMap[_isoX][_isoY].isSelected = true;
			_tileMap[_isoX][_isoY - 1].isSelected = true;
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
	case TILEKIND_BUSINESSES:
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
	switch (_currentTileKind)
	{
	case TILEKIND_TILESETS:
		break;
	case TILEKIND_HOUSES:
		switch (_currentTileNum)
		{
		case 0:
			_tileMap[_isoX][_isoY].isSelected = false;
			_tileMap[_isoX][_isoY - 1].isSelected = false;
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
	case TILEKIND_BUSINESSES:
		break;
	case TILEKIND_COMMUNITYBUILDINS:
		break;
	case TILEKIND_FUNPLACES:
		break;
	}
}

void Simtown::DrawTileMap()
{
	_tileCnt = 0;
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
			if (_tileMap[i][j].isStartDraw) {
				switch (_tileMap[i][j].tileKind)
				{
				case TILEKIND_TILESETS:
					break;
				case TILEKIND_HOUSES:
					_building._houses[_currentTileNum][_tileSize]
						->Render(GetMemDC(), left, top - cell_height,
							cell_width + cell_width / 4,
							cell_height * 2);
					break;
				case TILEKIND_BUSINESSES:
					break;
				case TILEKIND_COMMUNITYBUILDINS:
					break;
				case TILEKIND_FUNPLACES:
					break;
				}
			}
			
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

	sprintf_s(str, "인구 : %d명", 0);
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
		break;
	case TILEKIND_HOUSES:
		_building._houses[_currentTileNum][_tileSize]
			->Render(GetMemDC(), left, top - cell_height,
				cell_width + cell_width / 4,
				cell_height * 2);
		break; 
	case TILEKIND_BUSINESSES:
		break;
	case TILEKIND_COMMUNITYBUILDINS:
		break;
	case TILEKIND_FUNPLACES:
		break;
	}
}
