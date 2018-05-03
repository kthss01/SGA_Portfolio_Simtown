#include "stdafx.h"
#include "D2D.h"

D2D* D2D::instance = NULL;
LPDIRECT3DDEVICE9 D2D::device = NULL;

D2D::D2D()
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	// �ȿ� �� �� false�� error �� ���
	assert(d3d != NULL);

	// �Ӽ��� ������� direct ��������
	D3DPRESENT_PARAMETERS param = { 0 };
	// â��� true, ��üȭ���̸� ����� ����� �˾ƾ���
	// �̰� âũ�� �����ص� âũ�� �����
	param.Windowed = TRUE;	// â���� ���� ��üȭ������ ���� false�� ��ü
	// swap effect (���۰� ��ü�Ǵ� ����) 
	// ���� ���� �� ���۷� ȭ�� ü���� �Ǵ� �� swap chain effect
	// �޸� ���� ������ �� ����
	// DISCARD�� �⺻�� ������ ���� �ʰ� ���� ���۷� �Ѱ���
	param.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// Depth ���̰� Stencil ��� ��� ���̿� ����Ʈ ĥ�ϸ� ���°�
	// Stencil Direct������ ��ޱ�� D2D������ ���� ������
	// Depth�� 16��Ʈ 2Byte������ ���ڴٴ°�
	// ����
	// �� ���� ���°� 
	// �̰� �÷��� �����Ҷ� ���� ���°�
	// D3DFMT_A32B32G32R32F 
	// ���ڰ� 4�� ���µ� �ϳ��� 4����Ʈ�� ���� float�̶�°�
	param.AutoDepthStencilFormat = D3DFMT_D16;
	// Depth �� Stencil �� �ڵ����� ����ϰڳĴ°�
	param.EnableAutoDepthStencil = TRUE;

	HRESULT hr;

	hr = d3d->CreateDevice(
		// �� ���� ����Ͱ� �ְ� �Ǹ� �� ����Ϳ� ���ڴ�
		// ����� ���� ����� �� �� ��üȭ�� ���ϴ°� ��õ
		// ����� ũ�� �ڵ����� ������ٶ� �������� �ȸ¾Ƽ� ȭ�� ���������� ����
		D3DADAPTER_DEFAULT,	// �� ����͸� ����ϰڴ�
		D3DDEVTYPE_HAL,
		g_hWnd,
		// hardware vertex processing 
		// ������ ���� ����Ʈ ó�� ���� ���� �����ϴ°� �ϵ���� / ����Ʈ����
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&param,
		&device
	);

	// ������ ������ ��� cpp ��� ��ġ���� � ������ �߸鼭 �ߴܵ�
	// ������ �پ� �� �� ����
	// �ȿ� bool���� �� �� �ִµ� ���ڿ��� ������ true�� && �����ϸ��
	assert(SUCCEEDED(hr) && "Not CreateDevice");

}


D2D::~D2D()
{
	SAFE_RELEASE(device);
	SAFE_RELEASE(d3d);
}

// �̱��� ó��
D2D * D2D::Get()
{
	if (instance == NULL)
		instance = new D2D;
	return instance;
}

void D2D::Delete()
{
	if (instance != NULL)
		SAFE_DELETE(instance);
}

void D2D::BeginScene(DWORD color)
{
	// Clear ȭ�� �����ִ°�, 
	// 2��° ���ڴ� ���� ũ�� NULL�̸� ȭ�� ��ü
	// CLEAR_TARGET ȭ�鿡 ��µ� �༮
	// ZBUFFER clear ���̰� �ʱ�ȭ
	// 1.0f ZBUFFER ������ �ʱ�ȭ ����
	// z ���� 0.1 ���� ~ 1 ���̰� ������ ������ �����°�
 	// 0.0f Stancil ������ �ʱ�ȭ ����
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		color, 1.0f, 0.0f);

	device->BeginScene(); // �ĸ� ���ۿ� ����.
}

void D2D::EndScene()
{
	device->EndScene();	// ������ �����ٴ°�
	// present �ϸ� endScene �������� ������ ���Ե�
	device->Present(NULL, NULL, NULL, NULL); // �ĸ� -> ����
}
