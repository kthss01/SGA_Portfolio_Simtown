#include "stdafx.h"
#include "Frame.h"

// ����� �Լ��� �ִٰ� �����
#include <mmsystem.h>
// ���̺귯���� cpp�� ������ �ִٰ� �����
#pragma comment(lib, "winmm.lib")

Frame::Frame()
{
}


Frame::~Frame()
{
}

HRESULT Frame::Init()
{
	// ���� Ÿ�̸� �� �� �ִ��� Ȯ�����ִ� �Լ�
	// ���� �����ͷ� �Ǿ� ����
	// ���� ���ڰ����� void * �� �Ǿ� �ִ°͵� ����
	if (QueryPerformanceFrequency(
		(LARGE_INTEGER*)&_periodFrequency)) {

		// ����ũ�� ������ (1 / 1000000 (�鸸) ) ��

		_isHardWare = true;
		// GetTickCount�� ���� �༮�̶�� �����
		// lastTime�� ���� �ð� ������
		QueryPerformanceCounter(
			(LARGE_INTEGER*)&_lastTime);

		// �ʴ� �ð� ��� ����
		_timeScale = 1.0f / _periodFrequency;
	}
	else {

		// �и� ����Ʈ (1 / 1000) �� 

		_isHardWare = false;

		_lastTime = timeGetTime();
		_timeScale = 1.0f / 1000;
	}

	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0;
	_worldTime = 0;

	return S_OK;
}

void Frame::Tick(float lockFPS)
{
	if (_isHardWare) {
		QueryPerformanceCounter(
			(LARGE_INTEGER*)&_curTime);
	}
	else {
		_curTime = timeGetTime();
	}

	// curTime �� lastTime ���̴� �� ������ (���) �׸��� �� ��Ȳ
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	if (lockFPS > 0.0f) {
		// ���⼭ �ݺ��� ���鼭 ���� �Ŵ°�
		while (_timeElapsed < (1.0f / lockFPS)) {
			if (_isHardWare)
				QueryPerformanceCounter(
				(LARGE_INTEGER*)&_curTime);
			else
				_curTime = timeGetTime();

			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	_lastTime = _curTime;
	// �������Ӵ� ����Ǵ� ���
	_FPSFrameCount++; // �� �༮�� ���̰� �׿��� �������� fps ��ȯ
	_FPSTimeElapsed += _timeElapsed;
	_worldTime += _timeElapsed;

	// 1�� ������ ���� ������ �� �ʱ�ȭ
	if (_FPSTimeElapsed > 1.0f) {
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}
}

unsigned long Frame::GetFrameRate(char * str) const
{
	//// ���ڿ� ������
	//if (str != NULL) {
	//	// ���� ���ڿ��� ���ڿ� �������� ��ȯ
	//	wsprintf(str, "FPS : %d", _frameRate);
	//}

	return _frameRate;
}
