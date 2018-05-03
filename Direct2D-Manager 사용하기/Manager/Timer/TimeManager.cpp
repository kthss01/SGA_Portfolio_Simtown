#include "stdafx.h"
#include "TimeManager.h"

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

wstring TimeManager::Render()
{
	wstring str;

	// �츮�� ����ϴ� if���̶� ���� �ϴٰ� ���� ��
	// _DEBUG ���־󿡼� �̸� ���� ���ǹ��̶�� �����
	// ���� ���°� Debug��� �����
#ifdef _DEBUG
	if (_frame != NULL) {
		
		str = L"FPS : ";
		str += to_wstring(_frame->GetFrameRate());
		str += L"\n";

		str += L"worldTime : ";
		str += to_wstring(_frame->GetWorldTime());
		str += L"\n";

		str += L"ElapsedTime : ";
		str += to_wstring(_frame->GetElapsedTime());

		return str;
	}
#else
	if (_frame != NULL) {


		str = L"FPS : ";
		str += to_wstring(_frame->GetFrameRate());

		return str;
	}
#endif

}
