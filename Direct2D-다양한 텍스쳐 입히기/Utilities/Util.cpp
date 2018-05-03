#include "stdafx.h"
#include "Util.h"


Util::Util()
{
}


Util::~Util()
{
}

void Util::GetMousePos(Vector2 * out)
{
	// API �Լ��� ���ؼ� �޾ƿ� ��
	POINT cursorPos;

	GetCursorPos(&cursorPos);	// ���� ������ â�� �������� ������

	ScreenToClient(g_hWnd, &cursorPos);

	// ���콺 �������� ȭ�鿡 ����� ���������� ����Ҳ�
	Vector2 temp;

	// 0 ~ 1 ���� ������ ���� ����
	temp.x = (float)cursorPos.x / (float)WINSIZE_X;
	// y���� D3D�� �ϰ� �־ ���� +�̰� �Ʒ��� -�̾ �������� ���� ��
	temp.y = 1.0f - (cursorPos.y / (float)WINSIZE_Y);

	// x���� ���� ������ ������ -1 �������� 1�̶� ���ϰ� ���ذ�
	temp.x = temp.x * 2.0f - 1.0f;
	temp.y = temp.y * 2.0f - 1.0f;

	// �̷��� �ϸ� ȭ�� �߽��� 0,0 ���� -1 ������ 1 �� 1 �Ʒ� -1��

	*out = temp;
}
