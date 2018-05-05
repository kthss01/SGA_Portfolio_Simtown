#pragma once

#define CELL_WIDTH 200
#define CELL_HEIGHT 100

#define RADIUS_WIDTH (CELL_WIDTH / 2)
#define RADIUS_HEIGHT (CELL_HEIGHT / 2)

#define CELL_WIDTH2 (CELL_WIDTH / 2)
#define CELL_HEIGHT2 (CELL_HEIGHT / 2)

#define CELL_WIDTH3 (CELL_WIDTH / 4)
#define CELL_HEIGHT3 (CELL_HEIGHT / 4)

// ÃÊ±â ÁÂÇ¥
#define INIT_X 555
#define INIT_Y -330

#define TILE_COUNT_X 1000
#define TILE_COUNT_Y 1000

struct tagTile {
	bool isSelected;
};