#include "stdafx.h"
#include "Program.h"


Program::Program()
{
	Init();

	D2D::GetDevice()->SetRenderState(
		// ����Ʈ ����
		D3DRS_LIGHTING,
		// ��� ����
		FALSE
	);

	// ���ĺ��� Enable True
	D2D::GetDevice()->SetRenderState(
		D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(
		D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(
		D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	vEye = Vector2(0, 0);	// ī�޶��� ��ġ
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
	Vector2 vLookAt(0, 0, 1);	// �ٶ󺸴� ��ǥ��
	vLookAt = vEye + Vector2(0, 0, 1);
	Vector2 vUp(0, 1, 0);
	matView = Matrix::View(vEye, vLookAt, vUp);
	matProjection = Matrix::Ortho(0, WINSIZE_X, WINSIZE_Y, 0, 0.0, 1.0f);

	if (GetKeyState(VK_UP) & 0x8000) { vEye.y += 1.0f; }
	if (GetKeyState(VK_DOWN) & 0x8000) { vEye.y -= 1.0f; }

	Util::GetMousePos(&mousePos);

	Matrix matViewProj = matView * matProjection;

	TileCollision(matViewProj);
}

void Program::Render()
{
	// ���������� ����ϴ°� Device�� �Ѱ��ְԵ�
	D2D::GetDevice()->SetTransform(D3DTS_VIEW, &matView.ToDXMatrix());
	D2D::GetDevice()->SetTransform(D3DTS_PROJECTION, &matProjection.ToDXMatrix());

	D2D::GetDevice()->SetStreamSource(0, vb, 0, sizeof(Vertex));

	// FVF ������ �����͸� � �������� ������
	// position, color �̰� ���������� �ѰŶ� �˷������
	// ����ü �� ������ �����ϰ� �������
	// XYZ ������ DIFFUSE color
	// color���̶� ������ ���� �� �־������
	// texture ������ texture coord�� �־������
	// color �ȳ־��ָ� �������� ������ �ȵ� ��ǥ�� ������
	//D2D::GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	// texture �� ����Ҷ�
	D2D::GetDevice()->SetFVF(D3DFVF_CUSTOMVERTEX);

	// ���� �̿�, �ε��� �̿� �簢�� �׸���
	// �ε��� ������ ����̽��� �־������
	D2D::GetDevice()->SetIndices(this->ib);


	D2D::GetDevice()->SetTexture(0, pTex[1]);
	D2D::GetDevice()->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST, // �׸� ����
		0,	// ���� vertex �迭 �ε���
		0,	// �ּ� vertex ���� ��ȣ
		VERTEX_SIZE,	// ������ ����
		0,	// ���� index �迭
		INDEX_SIZE / 3	// �ﰢ���� ����
	);

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
			//vertices[i * TILE_COL + j].color = 0xFFFF000;
			//if (i % 2 == 0 && j % 2 == 0)
			//	vertices[i * TILE_COL + j].uv = Vector2(0, 0);
			//else if (i % 2 == 0 && j % 2 != 0)
			//	vertices[i * TILE_COL + j].uv = Vector2(1, 0);
			//else if (i % 2 != 0 && j % 2 == 0)
			//	vertices[i * TILE_COL + j].uv = Vector2(0, 1);
			//else
			//	vertices[i * TILE_COL + j].uv = Vector2(1, 1);
			vertices[i*TILE_COL + j].uv = Vector2(j, i);
		}
	}
	
	indices = new DWORD[INDEX_SIZE];

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
		}
	}

	// vertex buffer
	HRESULT hr;
	hr = D2D::GetDevice()->CreateVertexBuffer(
		//sizeof(Vertex) * 3,				// ������ �� ũ��
		// �ﰢ���� �ƴ� �簢������ ����Ƿ� ���� �ʿ�
		sizeof(Vertex) * VERTEX_SIZE,				// ������ �� ũ��
										// default�� �ְ� dynamic�� ����
										// default�� cpu �Ⱦ��°�
										// dynamic���� �ϸ� ������ ��� �������� �� ���� (update���� ����)
										// �����ϴ°� ��ü�� �׷��ȿ� ���ϸ� �ɾ��༭ ���� �Ҽ��� ������ ������
		0,								// GPU ��� �߿� CPU �������
										// texture ���� �ٸ�
										//D3DFVF_XYZ | D3DFVF_DIFFUSE,	
										// 0���� ȭ�� ����ִ°ɷ� �̹� ����ϰ� ����
		D3DFVF_CUSTOMVERTEX,	// 0�� ����ϸ� ���������� ����
		D3DPOOL_DEFAULT,
		&vb,
		NULL							// ������ �Ⱦ�
	);
	// direct9 2010�⵵�� ���� �Ϸ�ǰ� ���׳� ������ fix��
	// �׷��� ���� �Ⱦ��°� ����
	assert(SUCCEEDED(hr));

	// ���� �ݴ°�
	// update���� ���̽��ָ� ������ ���ϰ� ����
	// �ʸ��� �̷� ���� ������ ���ϰ� �ɸ��ٴ°�
	// vertex ���� ���ϸ� �� ���� �ٽ� ���ۿ� �־������
	// ���� �ٸ� ���� 3�� �ʿ��ϸ� ���� 3�� �ʿ���
	void* vertexData = NULL;
	//hr = vb->Lock(0, sizeof(Vertex) * 3, &vertexData, NULL);
	// �簢�� �׸��Ƿ�
	hr = vb->Lock(0, sizeof(Vertex) * VERTEX_SIZE, &vertexData, NULL);
	assert(SUCCEEDED(hr));

	//memcpy(vertexData, vertices, sizeof(Vertex) * 3);
	// �簢�� �׸��Ƿ�
	memcpy(vertexData, vertices, sizeof(Vertex) * VERTEX_SIZE);
	hr = vb->Unlock();
	assert(SUCCEEDED(hr));

	// index buffer
	hr = D2D::GetDevice()->CreateIndexBuffer(
		sizeof(DWORD) * INDEX_SIZE,
		D3DUSAGE_WRITEONLY,	// ���� �뵵�θ� ����ϰڴ�
		D3DFMT_INDEX32,	// index ũ�� 4byte���̹Ƿ� 32
						//�������� �ʰ� �׳� ������ �� ���� �����ڴٴ°�
		D3DPOOL_DEFAULT, // �޸� ��� �����Ҳ���, 
		&ib,
		NULL
	);

	assert(SUCCEEDED(hr));
	void* pIndices = NULL;
	hr = ib->Lock(0, 0, &pIndices, 0);
	assert(SUCCEEDED(hr));

	// lock �ʹ� �������� �ϴ°� ������
	// memcpy ��õ
	memcpy(pIndices, indices, sizeof(DWORD) * INDEX_SIZE);

	hr = ib->Unlock();
	assert(SUCCEEDED(hr));

	// texture load
	// d3d9�� �ִ°�
	//D2D::GetDevice()->CreateTexture() // texture load�� �ƴ϶� ������ִ°�
	// win api���� ���Ŷ� �ؼ� �ؾ���
	// CreateFile() // ���� ��ü�� �ҷ��ͼ� texture ���� �ؾߵ�  
	// ���ϻ󿡼� �ҷ��ͼ� ������ִ°� 
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/pokemon_1.png",
		&pTex[0]
	);
	assert(SUCCEEDED(hr));
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/pokemon_2.png",
		&pTex[1]
	);
	assert(SUCCEEDED(hr));
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/pokemon_3.png",
		&pTex[2]
	);
	assert(SUCCEEDED(hr));
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/pokemon_4.png",
		&pTex[3]
	);
	assert(SUCCEEDED(hr));
	// �̹����� ���� ���� �ʿ��ϸ� (ex�� Ȯ����)
	// �ؽ�ó ���Ͽ��� �ҷ����µ� �ɼ��� �߰��ؼ� �ҷ����ٴٴ°�
	// �̳༮ �̿��ؼ� ����Ÿ ������ �� �� ����
	// ���ڿ��� Colorkey (���� �� ��, ����Ÿ�� �� �༮),
	// *pSrcInfo �̹����� ũ�� ���� ������ image�� ���� ���� ���°�
	// width, height�� �̹��� ��ü�� ũ�⸦ �������ִ°� �Ƚᵵ ��
	// �̹��� ���� ��� �ΰ��� ���� vertex �� sprite
	//D3DXCreateTextureFromFileEx();

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
							// FF_DONTCARE ���� ũ�� ������� ū �༮ �������� ����ִ°� �ʺ�� ���� �߿�
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

	str = to_wstring(mousePos.x);
	str += L" , ";
	str += to_wstring(mousePos.y);

	// ��Ƽ����Ʈ�� DrawTextA
	font->DrawTextW(
		// �̹��� 2D ��ǥ���� ���°� sprite��� ��
		NULL,
		str.c_str(),
		-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
		&rc,
		// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
		// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
		DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
							 // 0x~~ �̰� �������� �Լ��� ����
							 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
		0xFFFF0000
	);

	rc.top += 20;
	//if (check) {
	//	// ��Ƽ����Ʈ�� DrawTextA
	//	font->DrawTextW(
	//		// �̹��� 2D ��ǥ���� ���°� sprite��� ��
	//		NULL,
	//		L"�浹",
	//		-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
	//		&rc,
	//		// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
	//		// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
	//		DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
	//							 // 0x~~ �̰� �������� �Լ��� ����
	//							 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	//		0xFFFF0000
	//	);
	//}
	//else {
	//	// ��Ƽ����Ʈ�� DrawTextA
	//	font->DrawTextW(
	//		// �̹��� 2D ��ǥ���� ���°� sprite��� ��
	//		NULL,
	//		L"���浹",
	//		-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
	//		&rc,
	//		// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
	//		// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
	//		DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
	//							 // 0x~~ �̰� �������� �Լ��� ����
	//							 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	//		0xFFFF0000
	//	);
	//}

	for (int i = 0; i < TILE_ROW - 1; i++) {
		for (int j = 0; j < TILE_COL - 1; j++) {
			if (!mapTile[i][j].isSelected) continue;

			wstring temp;
			temp = L"(";
			temp += to_wstring(mapTile[i][j].x);
			temp += L",";
			temp += to_wstring(mapTile[i][j].y);
			temp += L")";

			temp += L" �浹";
			// ��Ƽ����Ʈ�� DrawTextA
			font->DrawTextW(
				// �̹��� 2D ��ǥ���� ���°� sprite��� ��
				NULL,
				temp.c_str(),
				-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
				&rc,
				// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
				// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
				DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
										// 0x~~ �̰� �������� �Լ��� ����
										//D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
				0xFFFF0000
			);
		}
	}
}

void Program::TileCollision(Matrix& matViewProj)
{
	for (int i = 0; i < TILE_ROW - 1; i++) {
		for (int j = 0; j < TILE_COL - 1; j++) {
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
