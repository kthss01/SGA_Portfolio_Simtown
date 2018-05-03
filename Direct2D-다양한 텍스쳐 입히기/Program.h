#pragma once

// MainGame��� ���ư���

// vertex 10 x 10 �̸� tile 9 x 9 Ÿ�� �������

#define TILE_ROW 13
#define TILE_COL 14
#define TILE_WIDTH 50
#define TILE_HEIGHT 50

#define TILE_SIZE ((TILE_ROW - 1) * (TILE_COL - 1))
#define VERTEX_SIZE (TILE_ROW * TILE_COL)
#define INDEX_SIZE (TILE_SIZE * 2 * 3)

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

#define POKEMON_SIZE 151

struct tagTile {
	int x, y;
	Vector2 position[6];

	bool isSelected;
};

class Program
{
private:
	// �⺻������ ���� ������� vertex �ʿ�
	struct Vertex {
		Vector2 position;
		//DWORD color;
		Vector2 uv;
	};

	Vertex* vertices;
	DWORD* indices;

	Matrix matView;
	Matrix matProjection;
	// viewPort �ʿ���� 
	// ������ ���ڴٰ� ����� �������� window �������� �Ǿ�����

	LPDIRECT3DVERTEXBUFFER9 vb;
	LPDIRECT3DINDEXBUFFER9 ib;

	LPDIRECT3DTEXTURE9 pTex[POKEMON_SIZE];

	Vector2 vEye;
	
	LPD3DXFONT font;

	Vector2 mousePos;

	bool check;

	tagTile mapTile[TILE_ROW - 1][TILE_COL - 1];
	int tileX, tileY;
public:
	Program();
	~Program();

	void Update();
	void Render();

	void Init();
	void TextDraw();
	void TileCollision(Matrix& matViewProj);
};

