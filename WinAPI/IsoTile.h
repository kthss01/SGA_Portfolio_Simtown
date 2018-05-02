#pragma once

#define CELL_WIDTH 200
#define CELL_HEIGHT 100

#define RADIUS_WIDTH (CELL_WIDTH / 2)
#define RADIUS_HEIGHT (CELL_HEIGHT / 2)

#define CELL_WIDTH2 (CELL_WIDTH / 2)
#define CELL_HEIGHT2 (CELL_HEIGHT / 2)

#define RADIUS_WIDTH2 (CELL_WIDTH2 / 2)
#define RADIUS_HEIGHT2 (CELL_HEIGHT2 / 2)

#define CELL_WIDTH3 (CELL_WIDTH2 / 2)
#define CELL_HEIGHT3 (CELL_HEIGHT2 / 2)

#define RADIUS_WIDTH3 (CELL_WIDTH3 / 2)
#define RADIUS_HEIGHT3 (CELL_HEIGHT3 / 2)

// ÃÊ±â ÁÂÇ¥
#define INIT_X 400
#define INIT_Y 20

#define TILE_COUNT_X 1000
#define TILE_COUNT_Y 1000

struct tagTile {
	bool isSelected;
};