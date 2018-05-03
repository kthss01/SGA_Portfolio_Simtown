#pragma once
class D2D
{
private:
	// ���̷�Ʈ �� �� �ִ��� ������ �Ǵ�
	LPDIRECT3D9 d3d;	// ��ǻ�� ����� ������ �� �༮ �׷��� ī�� ����
	// ���������� �����Ҷ� �����Ҷ� ���� �༮
	static LPDIRECT3DDEVICE9 device; // �׻� �� �༮�� �̿��������

	static D2D * instance;	

	D2D();
	~D2D();
public:
	static D2D * Get();
	static void Delete();

	static LPDIRECT3DDEVICE9 GetDevice() { return device; }

	void BeginScene(DWORD color);
	void EndScene();
};

