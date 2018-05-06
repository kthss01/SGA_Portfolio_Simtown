#pragma once

#include "GameNode.h"
#include "IsoTile.h"

struct tagUIInfo {
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

	RECT _rcPageUpUI;
	RECT _rcPageDownUI;

	int _pageNum;

	Image * _pageUpUI1;
	Image * _pageUpUI2;
	Image * _pageDownUI1;
	Image * _pageDownUI2;

	RECT _rcToolBarUI[3];
	RECT _rcSongUI[3];
	Image * _toolBarUI;
	Image * _songUI;

	RECT _rcTownNameUI;
	RECT _rcPopulationUI[2];

	Image * _populationUI;
};

enum TileSize {
	TILESIZE_SMALL,
	TILESIZE_MID,
	TILESIZE_BIG,
	TILESIZE_OPEN,
	TILESIZE_END
};

struct tagBuildingInfo {
	Image * _houses[14][TILESIZE_END];
};

enum TileKind {
	TILEKIND_TILESETS,
	TILEKIND_HOUSES,
	TILEKIND_BUSINESSES,
	TILEKIND_COMMUNITYBUILDINS,
	TILEKIND_FUNPLACES
};

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

	TileSize _tileSize;
	float cell_width, cell_height;
	float radius_width, radius_height;

	int _tileCnt;

	tagUIInfo _ui;

	tagBuildingInfo _building;

	TileKind _currentTileKind;
	int _currentTileNum;

	HFONT hfont;

	bool _isClicked;
public:
	Simtown();
	~Simtown();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	void UIInit();

	void CheckSize();

	void FindImg();
	void FindUIImg();
	void FindBuildingImg();

	void SelectTile();
	void DeleteTile();

	void DrawTileMap();
	void DrawRhombus(float left, float top);
	int GetCornerIndex(int isoX, int isoY);
	bool IsInRhombus(int corner, int isoX, int isoY);

	void DrawUI();
	void DrawTile(int left, int top);

	void SaveTile();
	void LoadTile();
	void InitTile();

	void PlaySong(int num);
};

