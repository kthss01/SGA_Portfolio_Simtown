#pragma once

#include "GameNode.h"
#include "IsoTile.h"

class Simtown : public GameNode
{
private:
	tagTile _tileMap[TILE_COUNT_X][TILE_COUNT_Y];
	int _isoX, _isoY;
	int _center;
	char str[128];

	int _startX;
	int _startY;

	bool _isDebug;

	Image * _land;

	int _selectNum;

	float cell_width, cell_height;
	float radius_width, radius_height;

	RECT _rcLeftUI[14];
	Image * _leftUI;
	int _selectLeftUI;

	RECT _rcTopUI;
	Image * _topUI;

	RECT _rcBottomUI[9];
	Image * _bottomUI[5];
	int _selectBottomUI;

	Image *_blankUI;

	RECT _rcDebug;
	int _tileCnt;

	RECT _rcPageUpUI;
	RECT _rcPageDownUI;

	bool _pageNum;
public:
	Simtown();
	~Simtown();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	void CheckSize();

	void FindImg();
	void DrawTileMap();
	void DrawRhombus(float left, float top);
	int GetCornerIndex(int isoX, int isoY);
	bool IsInRhombus(int corner, int isoX, int isoY);

	void DrawUI();
};

