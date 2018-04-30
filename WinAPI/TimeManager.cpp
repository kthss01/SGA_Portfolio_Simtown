#include "stdafx.h"
#include "TimeManager.h"

#define Test 

TimeManager::TimeManager()
{
}


TimeManager::~TimeManager()
{
}

HRESULT TimeManager::Init()
{
	_frame = new Frame;
	_frame->Init();

	return S_OK;
}

void TimeManager::Release()
{
	if (_frame != NULL)
		SAFE_DELETE(_frame);
}

void TimeManager::Update(float lock)
{
	if (_frame != NULL) {
		_frame->Tick(lock);
	}
}

void TimeManager::Render(HDC hdc)
{
	char str[256];

	string strFrame;
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 0, 0));

	// �츮�� ����ϴ� if���̶� ���� �ϴٰ� ���� ��
	// _DEBUG ���־󿡼� �̸� ���� ���ǹ��̶�� �����
	// ���� ���°� Debug��� �����
#ifdef _DEBUG
	if (_frame != NULL) {
		sprintf_s(str, "FPS : %d", _frame->GetFrameRate());
		TextOut(hdc, 0, 0, str, strlen(str));

		sprintf_s(str, "worldTime : %f", _frame->GetWorldTime());
		TextOut(hdc, 0, 20, str, strlen(str));

		sprintf_s(str, "ElapsedTime : %f", _frame->GetElapsedTime());
		TextOut(hdc, 0, 40, str, strlen(str));
	}
#else
	if (_frame != NULL) {
		sprintf_s(str, "FPS : %d", _frame->GetFrameRate());
		TextOut(hdc, 0, 0, str, strlen(str));
	}
#endif

	// �̰� ���� ����ϴ°� 
	// ����Ͽ��� VIP ����� �������� ���� ��
	//#ifdef Test
	//	cout << Test << endl;
	//#endif // Test

#ifdef vip0
	cout << ����� << endl;
#else
	//cout << ���� << endl;
#endif // vip0

}
