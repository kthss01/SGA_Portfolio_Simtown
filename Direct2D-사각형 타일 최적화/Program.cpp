#include "stdafx.h"
#include "Program.h"


Program::Program()
{
	startX = startY = 0;
	endX = TILE_ROW - 1;
	endY = TILE_COL - 1;

	vEye = Vector2(0, 0);	// ī�޶��� ��ġ
	vLookAt = vEye + Vector2(0, 0, 1);
	vUp = Vector2(0, 1, 0);

	matView = Matrix::View(vEye, vLookAt, vUp);
	matProjection = Matrix::Ortho(0, WINSIZE_X, WINSIZE_Y, 0, 0.0, 1.0f);

	Init();

	D2D::GetDevice()->SetRenderState(
		D3DRS_LIGHTING, FALSE);

	// ���ĺ��� Enable True
	D2D::GetDevice()->SetRenderState(
		D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(
		D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(
		D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	
	cnt = 0;
}

Program::~Program()
{
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
}

void Program::Update()
{
	// ī�޶� ����°�
	//Vector2 vLookAt(0, 0, 1);	// �ٶ󺸴� ��ǥ��
	//vLookAt = vEye + Vector2(0, 0, 1);
	//Vector2 vUp(0, 1, 0);
	matView = Matrix::View(vEye, vLookAt, vUp);
	matProjection = Matrix::Ortho(0, WINSIZE_X, WINSIZE_Y, 0, 0.0, 1.0f);

	Matrix matViewProj = matView * matProjection;

	Util::GetMousePos(&mousePos);

	if (INPUT->GetKey(VK_UP)) { vEye.y += 1.0f; }
	if (INPUT->GetKey(VK_DOWN)) { vEye.y -= 1.0f; }
	if (INPUT->GetKey(VK_LEFT)) { vEye.x += 1.0f; }
	if (INPUT->GetKey(VK_RIGHT)) { vEye.x -= 1.0f; }

	if (INPUT->GetKeyDown(VK_LBUTTON)) {
		for (int i = startX; i < endX; i++) {
			for (int j = startY; j < endY; j++) {
				Vector2 position[6];

				for (int k = 0; k < 6; k++) {
					position[k] = mapTile[i][j].position[k].TransformCoord(matViewProj);
				}

				bool check = false;

				check |= Collision::IntersectTri(
					position[0], position[1], position[2], mousePos);
				check |= Collision::IntersectTri(
					position[3], position[4], position[5], mousePos);

				if (check) {
					mapTile[i][j].isClicked = 
						!mapTile[i][j].isClicked;
					break;
				}
			}
		}
	}

	cnt++;

	if (cnt % 100 == 0) {
		TileCollision();
	}
}

void Program::Render()
{
	D2D::GetDevice()->SetTransform(D3DTS_VIEW, &matView.ToDXMatrix());
	D2D::GetDevice()->SetTransform(D3DTS_PROJECTION, &matProjection.ToDXMatrix());

	D2D::GetDevice()->SetStreamSource(0, vb, 0, sizeof(Vertex));

	D2D::GetDevice()->SetFVF(D3DFVF_CUSTOMVERTEX);

	D2D::GetDevice()->SetIndices(this->ib);

	// test
	//int cnt = 0;
	for (int i = 0; i < TILE_SIZE; i++) {
		//if (i >= POKEMON_SIZE) continue;
		if (!mapTile[i / (TILE_COL - 1)][i % (TILE_COL - 1)].isShow) continue;
		if (mapTile[i / (TILE_COL - 1)][i % (TILE_COL - 1)].isClicked) continue;
		D2D::GetDevice()->SetTexture(0, pTex[i % POKEMON_SIZE]);
		D2D::GetDevice()->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST, // �׸� ����
			0,	// ���� vertex �迭 �ε���
			0,	// �ּ� vertex ���� ��ȣ
			VERTEX_SIZE,	// ������ ����
			i * 6,	// ���� index �迭
			2	// �ﰢ���� ����
		);
		// test
		//cnt++;
	}
	// test
	//int result = cnt;

	//for (int i = 0; i < (TILE_ROW - 1) * (TILE_COL - 1); i++) {
	//	D2D::GetDevice()->SetTexture(0, pTex[i % 4]);
	//	D2D::GetDevice()->DrawIndexedPrimitive(
	//		D3DPT_TRIANGLELIST,
	//		i * 6, i * 6, 6, i * 3, 2);
	//}

	TextDraw();
}

void Program::Init()
{
	vertices = new Vertex[VERTEX_SIZE];

	for (int i = 0; i < TILE_ROW; i++) {
		for (int j = 0; j < TILE_COL; j++) {
			vertices[i * TILE_COL + j].position = 
				Vector2(
					j * TILE_HEIGHT - (TILE_HEIGHT * (TILE_COL - 1)) / 2,
					i * TILE_WIDTH - (TILE_WIDTH * (TILE_ROW - 1)) / 2);
			vertices[i*TILE_COL + j].uv = Vector2(j, i);
		}
	}
	
	indices = new DWORD[INDEX_SIZE];

	Matrix matViewProj = matView * matProjection;

	int count = 0;
	for (int i = 0; i < TILE_ROW - 1; i++) {
		for (int j = 0; j < TILE_COL - 1; j++) {
			indices[count++] = i * TILE_COL + j;
			indices[count++] = i * TILE_COL + j + 1;
			indices[count++] = (i + 1) * TILE_COL + j;

			indices[count++] = i * TILE_COL + j + 1;
			indices[count++] = (i + 1) * TILE_COL + j + 1;
			indices[count++] = (i + 1) * TILE_COL + j;

			mapTile[i][j].x = i;
			mapTile[i][j].y = j;
			mapTile[i][j].position[0] = vertices[i * TILE_COL + j].position;
			mapTile[i][j].position[1] = vertices[i * TILE_COL + j + 1].position;
			mapTile[i][j].position[2] = vertices[(i + 1) * TILE_COL + j].position;
			mapTile[i][j].position[3] = vertices[i * TILE_COL + j + 1].position;
			mapTile[i][j].position[4] = vertices[(i + 1) * TILE_COL + j + 1].position;
			mapTile[i][j].position[5] = vertices[(i + 1) * TILE_COL + j].position;
			mapTile[i][j].isSelected = false;
			mapTile[i][j].isClicked = false;
			mapTile[i][j].isShow = true;

			Vector2 temp = mapTile[i][j].position[0].TransformCoord(matViewProj);
			if (temp.x < -1.0f && temp.y > 1.0f) {
				if (startX < i && startY < j) {
					startX = i;
					startY = j;
				}
			}
			if (temp.x < -1.0f || temp.y > 1.0f) {
				mapTile[i][j].isShow = false;
			}

			temp = mapTile[i][j].position[4].TransformCoord(matViewProj);
			if (temp.x > 1.0f && temp.y < -1.0f) {
				if (endX > i && endY > j) {
					endX = i;
					endY = j;
				}
			}
			if (temp.x > 1.0f || temp.y < -1.0f) {
				mapTile[i][j].isShow = false;
			}
		}
	}

	// vertex buffer
	HRESULT hr;
	hr = D2D::GetDevice()->CreateVertexBuffer(
		sizeof(Vertex) * VERTEX_SIZE,	
		0,	
		D3DFVF_CUSTOMVERTEX,	// 0�� ����ϸ� ���������� ����
		D3DPOOL_DEFAULT,
		&vb,
		NULL							// ������ �Ⱦ�
	);

	assert(SUCCEEDED(hr));

	void* vertexData = NULL;
	hr = vb->Lock(0, sizeof(Vertex) * VERTEX_SIZE, &vertexData, NULL);
	assert(SUCCEEDED(hr));

	memcpy(vertexData, vertices, sizeof(Vertex) * VERTEX_SIZE);
	hr = vb->Unlock();
	assert(SUCCEEDED(hr));

	// index buffer
	hr = D2D::GetDevice()->CreateIndexBuffer(
		sizeof(DWORD) * INDEX_SIZE,
		D3DUSAGE_WRITEONLY,	// ���� �뵵�θ� ����ϰڴ�
		D3DFMT_INDEX32,	// index ũ�� 4byte���̹Ƿ� 32
		D3DPOOL_DEFAULT, // �޸� ��� �����Ҳ���, 
		&ib,
		NULL
	);

	assert(SUCCEEDED(hr));
	void* pIndices = NULL;
	hr = ib->Lock(0, 0, &pIndices, 0);
	assert(SUCCEEDED(hr));

	memcpy(pIndices, indices, sizeof(DWORD) * INDEX_SIZE);

	hr = ib->Unlock();
	assert(SUCCEEDED(hr));

	for (int i = 0; i < POKEMON_SIZE; i++) {
		wstring temp;
		temp = L"Textures/pokemon/pokemon ";
		temp += L"(";
		temp += to_wstring(i + 1);
		temp += L").png";

		hr = D3DXCreateTextureFromFile(
			D2D::GetDevice(),
			temp.c_str(),
			&pTex[i]
		);
		assert(SUCCEEDED(hr));
	}

	hr = D3DXCreateFont(
		D2D::GetDevice(),
		15,					// �ʺ�
		0,					// ����
		FW_BOLD,			// �β�, �β��� ���ڴٴ°�
		1,					// �� ���� ũ�� �ٲ� �� ����
		FALSE,				// ���Ÿ�ü ��� ����
		DEFAULT_CHARSET,	// �� ���� ũ�� �ٲ� �� ����
		OUT_DEFAULT_PRECIS,	// �ܰ��� ó��
		DEFAULT_QUALITY,	// ����Ƽ
		DEFAULT_PITCH || FF_DONTCARE,		// �ɼ�, 
		L"����",
		&font
	);
	assert(SUCCEEDED(hr));
}

void Program::TextDraw()
{
	// RenderText
	RECT rc = { 10, 10, 0, 0 };

	wstring str;

	str = FRAME->Render();

	font->DrawTextW(
		NULL,
		str.c_str(),
		-1,
		&rc,
		DT_LEFT | DT_NOCLIP, 
		0xFFFF0000
	);

	rc.top += 50;

	str = to_wstring(mousePos.x);
	str += L" , ";
	str += to_wstring(mousePos.y);

	font->DrawTextW(
		NULL,
		str.c_str(),
		-1,	
		&rc,
		DT_LEFT | DT_NOCLIP, 
		0xFFFF0000
	);

	rc.top += 20;

	for (int i = startX; i < endX; i++) {
		for (int j = startY; j < endY; j++) {
			if (!mapTile[i][j].isSelected) continue;

			wstring temp;
			temp = L"(";
			temp += to_wstring(mapTile[i][j].x);
			temp += L",";
			temp += to_wstring(mapTile[i][j].y);
			temp += L")";

			temp += L" �浹";

			font->DrawTextW(
				NULL,
				temp.c_str(),
				-1,	
				&rc,
				DT_LEFT | DT_NOCLIP, 
				0xFFFF0000
			);
		}
	}
}

void Program::TileCollision()
{
	Matrix matViewProj = matView * matProjection;

	for (int i = startX; i < endX; i++) {
		for (int j = startY; j < endY; j++) {
			Vector2 position[6];

			for (int k = 0; k < 6; k++) {
				position[k] = mapTile[i][j].position[k].TransformCoord(matViewProj);
			}

			bool check = false;

			check |= Collision::IntersectTri(
				position[0], position[1], position[2], mousePos);
			check |= Collision::IntersectTri(
				position[3], position[4], position[5], mousePos);

			mapTile[i][j].isSelected = check;
		}
	}
}
