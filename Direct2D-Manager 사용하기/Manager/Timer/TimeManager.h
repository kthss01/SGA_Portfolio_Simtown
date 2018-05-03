#pragma once

#include "../SingletonBase.h"
#include "Frame.h"

class TimeManager : public SingletonBase<TimeManager>
{
private:
	Frame * _frame;
public:
	TimeManager();
	~TimeManager();

	HRESULT Init();
	void Release();
	void Update(float lock = 0.0f);
	// GameNode ��� ���� �ʰ� �־ hdc �ʿ���
	// ���� GetMemDC() �Լ� ��� �ް� ������ GameNode ���� ��ӹ����� �ǳ� 
	// ���� ��� �Ⱦ��°� ����
	wstring Render();

	float GetElapsedTime() const { return _frame->GetElapsedTime(); }
	float GetWorldTime() const { return _frame->GetWorldTime(); }
};

#define FRAME TimeManager::GetSingleton()

