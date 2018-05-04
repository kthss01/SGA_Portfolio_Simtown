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

	_selectNum = 0;

	cell_width = CELL_WIDTH;
	cell_height = CELL_HEIGHT;
	radius_width = RADIUS_WIDTH;
	radius_height = RADIUS_HEIGHT;

	_rcTopUI = RectMake(100, 0, WINSIZEX, 35);

	_selectLeftUI = 0;

	//for (int i = 0; i < 13; i++) {
	//	_rcLeftUI[i] = RectMake(0, i * 50, 100, 50);
	//}
	//_rcLeftUI[13] = RectMake(0, 650, 100, 100);
	_rcLeftUI[0] = RectMake(0, 0, 100, 50);
	_rcLeftUI[1] = RectMake(0, 50, 100, 50);
	_rcLeftUI[2] = RectMake(0, 100, 100, 50);
	_rcLeftUI[3] = RectMake(0, 150, 100, 60);
	_rcLeftUI[4] = RectMake(0, 210, 100, 50);
	_rcLeftUI[5] = RectMake(0, 260, 100, 50);
	_rcLeftUI[6] = RectMake(0, 310, 100, 50);
	_rcLeftUI[7] = RectMake(0, 360, 100, 35);
	_rcLeftUI[8] = RectMake(0, 395, 100, 35);
	_rcLeftUI[9] = RectMake(0, 430, 100, 45);
	_rcLeftUI[10] = RectMake(0, 475, 100, 70);
	_rcLeftUI[11] = RectMake(0, 545, 100, 35);
	_rcLeftUI[12] = RectMake(0, 580, 100, 50);
	_rcLeftUI[13] = RectMake(0, 630, 100, 70);
	
	_rcBottomUI[0] = RectMake(100, WINSIZEY - 100, 300, 100);
	for (int i = 1; i < 9; i++) {
		_rcBottomUI[i] = RectMake(100 * i + 300, WINSIZEY - 100, 100, 100);
	}
	_selectBottomUI = 0;

	_rcDebug = RectMake(WINSIZEX - 320, 20, 320, 320);

	_pageNum = 0;

	_rcPageUpUI = RectMake(
		_rcBottomUI[8].left + 25, _rcBottomUI[8].top + 25, 50, 25);
	_rcPageDownUI = RectMake(
		_rcBottomUI[8].left + 25, _rcBottomUI[8].top + 75, 50, 25);

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
		_selectNum = 0; 
		CheckSize(); 
	}
	if (INPUT->GetKey('2')) { 
		_selectNum = 1; 
		CheckSize(); 
	}
	if (INPUT->GetKey('3')) { 
		_selectNum = 2; 
		CheckSize(); 
	}
	if (INPUT->GetKey('Z')) {
		cell_width -= 2.0f;
		cell_height -= 1.0f;
		radius_width = cell_width / 2;
		radius_height = cell_height / 2;
	}
	if (INPUT->GetKey('X')) {
		cell_width += 2.0f;
		cell_height += 1.0f;
		radius_width = cell_width / 2;
		radius_height = cell_height / 2;
	}

	if (INPUT->GetKeyDown(VK_TAB)) {
		_isDebug = !_isDebug;
	}

	if (INPUT->GetKey(VK_LBUTTON)) {

		// ui Select
		{
			// left ui
			for (int i = 0; i < 14; i++) {
				if (PtInRect(&_rcLeftUI[i], g_ptMouse)) {
					if (i == 7 || i == 8 || i == 9) {
						_selectNum = i - 7;
						CheckSize();
					}
					else if (i > 9)
						_selectLeftUI = i - 3;
					else 
						_selectLeftUI = i;
					break;
				}
			}
			
			// bottom ui
			for (int i = 1; i < 8; i++) {
				if (PtInRect(&_rcBottomUI[i], g_ptMouse)) {
					_selectBottomUI = i - 1;
					break;
				}
			}
		}

		// tile Select
		{
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

					_tileMap[isoX][isoY].isSelected = true;

					_center = corner;
					_isoX = isoX;
					_isoY = isoY;
				}
			}
		}
	}
}

void Simtown::Render()
{
	DrawTileMap();
	DrawUI();

	//sprintf_s(str, "isoX : %d, isoY : %d, corner : %d",
	//	_isoX, _isoY, _center);
	//TextOut(GetMemDC(), 700, 20, str, strlen(str));
	if (_isDebug) {
		RectangleMake(GetMemDC(), _rcTopUI);
		for (int i = 0; i < 14; i++) {
			RectangleMake(GetMemDC(), _rcLeftUI[i]);
		}
		for (int i = 0; i < 9; i++) {
			RectangleMake(GetMemDC(), _rcBottomUI[i]);
		}

		RectangleMake(GetMemDC(), _rcDebug);

		RectangleMake(GetMemDC(), _rcPageUpUI);
		RectangleMake(GetMemDC(), _rcPageDownUI);

		sprintf_s(str, "Debug Info");
		TextOut(GetMemDC(), WINSIZEX - 300, 120, str, strlen(str));

		sprintf_s(str, "cell_width : %.2f, cell_height : %.2f",
			cell_width, cell_height);
		TextOut(GetMemDC(), WINSIZEX - 300, 140, str, strlen(str));
		sprintf_s(str, "radius_width : %.2f, radius_height : %.2f",
			radius_width, radius_height);
		TextOut(GetMemDC(), WINSIZEX - 300, 160, str, strlen(str));

		sprintf_s(str, "show tile : %d", _tileCnt);
		TextOut(GetMemDC(), WINSIZEX - 300, 180, str, strlen(str));

		sprintf_s(str, "mouse pos : (%d, %d)", g_ptMouse.x, g_ptMouse.y);
		TextOut(GetMemDC(), WINSIZEX - 300, 200, str, strlen(str));

		sprintf_s(str, "start pos : (%d, %d)", _startX, _startY);
		TextOut(GetMemDC(), WINSIZEX - 300, 220, str, strlen(str));
	}

}

void Simtown::CheckSize()
{
	switch (_selectNum)
	{
	case 0:
		cell_width = CELL_WIDTH;
		cell_height = CELL_HEIGHT;
		radius_width = RADIUS_WIDTH;
		radius_height = RADIUS_HEIGHT;
		break;
	case 1:
		cell_width = CELL_WIDTH2;
		cell_height = CELL_HEIGHT2;
		radius_width = RADIUS_WIDTH2;
		radius_height = RADIUS_HEIGHT2;
		break;
	case 2:
		cell_width = CELL_WIDTH3;
		cell_height = CELL_HEIGHT3;
		radius_width = RADIUS_WIDTH3;
		radius_height = RADIUS_HEIGHT3;
		break;
	}
}

void Simtown::FindImg()
{
	_land = IMAGE->FindImage("land");
	_leftUI = IMAGE->FindImage("ui_left");

	_bottomUI[0] = IMAGE->FindImage("ui_bottom_1");
	_bottomUI[1] = IMAGE->FindImage("ui_bottom_2");
	_bottomUI[2] = IMAGE->FindImage("ui_bottom_3");
	_bottomUI[3] = IMAGE->FindImage("ui_bottom_4");
	_bottomUI[4] = IMAGE->FindImage("ui_bottom_5");

	_blankUI = IMAGE->FindImage("ui_blank");
}

void Simtown::DrawTileMap()
{
	_tileCnt = 0;
	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			float left = _startX + (i * radius_width) - (j * radius_width);
			float top = _startY + (i * radius_height) + (j * radius_height);

			if (left + cell_width < WINSIZEX / 10 || left > WINSIZEX
				|| top + cell_height < WINSIZEY / 16 || top > WINSIZEY - WINSIZEY / 7) continue;

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

	_blankUI->Render(GetMemDC(),
		_rcTopUI.left, _rcTopUI.top, WINSIZEX, 35);

	_leftUI->FrameRender(GetMemDC(),
		_rcLeftUI[0].left, _rcLeftUI[0].top, _selectLeftUI + _selectNum * 11, 0);

	// test
	//_bottomUI[0]->FrameRender(GetMemDC(),
	//	_rcBottomUI[1].left, _rcBottomUI[1].top, 0, 0);
	//_bottomUI[1]->FrameRender(GetMemDC(),
	//	_rcBottomUI[2].left, _rcBottomUI[2].top, 0, 1);
	//_bottomUI[2]->FrameRender(GetMemDC(),
	//	_rcBottomUI[3].left, _rcBottomUI[3].top, 0, 0);
	//_bottomUI[3]->FrameRender(GetMemDC(),
	//	_rcBottomUI[4].left, _rcBottomUI[4].top, 0, 0);
	//_bottomUI[4]->FrameRender(GetMemDC(),
	//	_rcBottomUI[5].left, _rcBottomUI[5].top, 0, 0);

	_blankUI->Render(GetMemDC(),
		_rcBottomUI[0].left, _rcBottomUI[0].top, 300, 100);
	_blankUI->Render(GetMemDC(),
		_rcBottomUI[8].left, _rcBottomUI[8].top);

	Image * temp = NULL;

	switch (_selectLeftUI)
	{
	case 0:
		temp = _bottomUI[0];
		break;
	case 1:
		temp = _bottomUI[1];
		break;
	case 2:
		temp = _bottomUI[2];
		break;
	case 3:
		temp = _bottomUI[3];
		break;
	case 4:
		temp = _bottomUI[4];
		break;
	default:
		temp = _bottomUI[0];
		break;
	}

	for (int i = 1; i < 8; i++) {
		if (!(_selectLeftUI == 0 || _selectLeftUI == 1 || _selectLeftUI == 2))
			_pageNum = 0;

		if(i - 1 == _selectBottomUI) 
			temp->FrameRender(GetMemDC(),
				_rcBottomUI[i].left, _rcBottomUI[i].top, 1, i - 1 + _pageNum * 7);
		else
			temp->FrameRender(GetMemDC(),
				_rcBottomUI[i].left, _rcBottomUI[i].top, 0, i - 1 + _pageNum * 7);
	}

	LineMake(GetMemDC(), PointMake(100, 35), PointMake(WINSIZEX, 35));
	LineMake(GetMemDC(), PointMake(400, WINSIZEY - 100), PointMake(400, WINSIZEY));
	LineMake(GetMemDC(), PointMake(100, WINSIZEY - 100), PointMake(WINSIZEX, WINSIZEY - 100));
	LineMake(GetMemDC(), PointMake(0, 0), PointMake(WINSIZEX, 0));
	LineMake(GetMemDC(), PointMake(100, 0), PointMake(100, WINSIZEY));
}
