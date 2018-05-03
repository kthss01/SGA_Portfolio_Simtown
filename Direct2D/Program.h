#pragma once

// MainGame대신 돌아갈꺼

#define TILE_ROW 10
#define TILE_COL 10
#define TILE_WIDTH 50
#define TILE_HEIGHT 50

#define VERTEX_SIZE (TILE_ROW * TILE_COL)
#define INDEX_SIZE ((TILE_ROW - 1) * (TILE_COL - 1) * 2 * 3)

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

class Program
{
private:
	// 기본적으로 도형 만들려면 vertex 필요
	struct Vertex {
		Vector2 position;
		//DWORD color;
		Vector2 uv;
	};

	Vertex* vertices;
	DWORD* indices;

	Matrix matView;
	Matrix matProjection;
	// viewPort 필요없음 
	// 윈도우 쓰겠다고 선언된 순간부터 window 기준으로 되어있음

	LPDIRECT3DVERTEXBUFFER9 vb;
	LPDIRECT3DINDEXBUFFER9 ib;

	LPDIRECT3DTEXTURE9 pTex;

	Vector2 vEye;
	
	LPD3DXFONT font;

	Vector2 mousePos;

	bool check;

public:
	Program();
	~Program();

	void Update();
	void Render();

	void Init();
	void TextDraw();
};

