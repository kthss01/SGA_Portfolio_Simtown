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

	_isDebug = true;

	FindImg();

	_selectNum = 0;

	cell_width = CELL_WIDTH;
	cell_height = CELL_HEIGHT;
	radius_width = RADIUS_WIDTH;
	radius_height = RADIUS_HEIGHT;

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

	if (INPUT->GetKey('1')) { _selectNum = 0; }
	if (INPUT->GetKey('2')) { _selectNum = 1; }
	if (INPUT->GetKey('3')) { _selectNum = 2; }
	if (INPUT->GetKey('Z')) {
		cell_width -= 2.0f;
		cell_height -= 1.0f;
		radius_width = cell_width / 2;
		radius_height = cell_height / 2;
		_selectNum = -1;
	}
	if (INPUT->GetKey('X')) {
		cell_width += 2.0f;
		cell_height += 1.0f;
		radius_width = cell_width / 2;
		radius_height = cell_height / 2;
		_selectNum = -1;
	}

	switch (_selectNum)
	{
	case -1:
		break;
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

	if (INPUT->GetKeyDown(VK_TAB)) {
		_isDebug = !_isDebug;
	}

	if (INPUT->GetKey(VK_LBUTTON)) {
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

			_tileMap[isoX][isoY].isSelected = true;

			_center = corner;
			_isoX = isoX;
			_isoY = isoY;
		}
	}
}

void Simtown::Render()
{
	DrawTileMap();

	//sprintf_s(str, "isoX : %d, isoY : %d, corner : %d",
	//	_isoX, _isoY, _center);
	//TextOut(GetMemDC(), 700, 20, str, strlen(str));
	sprintf_s(str, "cell_width : %.2f, cell_height : %.2f",
		cell_width, cell_height);
	TextOut(GetMemDC(), 0, 60, str, strlen(str));
	sprintf_s(str, "radius_width : %.2f, radius_height : %.2f",
		radius_width, radius_height);
	TextOut(GetMemDC(), 0, 80, str, strlen(str));
}

void Simtown::FindImg()
{
	_land = IMAGE->FindImage("land");
}

void Simtown::DrawTileMap()
{
	int tileCnt = 0;
	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			float left = _startX + (i * radius_width) - (j * radius_width);
			float top = _startY + (i * radius_height) + (j * radius_height);

			if (left + cell_width < WINSIZEX / 6 || left > WINSIZEX
				|| top + cell_height < WINSIZEX / 12 || top > WINSIZEY - WINSIZEY / 6) continue;

			// render 될 이미지 여기서 render
			// 함수 호출보다 포인트 받아서 하는게 더 빠름
			//IMAGE->Render("land_big", GetMemDC(), left, top);

			_land->Render(GetMemDC(), left, top, cell_width, cell_height);
			tileCnt++;

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

	sprintf_s(str, "show tile : %d", tileCnt);
	TextOut(GetMemDC(), 0, 100, str, strlen(str));
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
