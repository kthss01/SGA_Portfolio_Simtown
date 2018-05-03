#pragma once

#include "SingletonBase.h"
// ����� cpp ��ģ��
#include "./FMOD/inc/fmod.hpp"
#pragma comment(lib, "./FMOD/lib/fmodex_vc.lib")

// fmod
/*
�̸� ��������ִ°�
���� �÷��� �ϱ� ���� �����ִ� �༮
DirectX�� ����
������ ��������� ������ �ߴܵǾ����� ���򿡵� ���� ����ϰ� ����
�����ϰ� ����� �� �ְ� �����ϴ� Ȯ���ڰ� ���Ƽ� ���� ���� �ִ°�

���� ���� ����
fmod studio ����Ʈ ������ Ȯ�� ����
*/

// �ܺο��� include �ؾ� �Ǵ°͵��� ���� ���ӽ����̽��� ���� ����
using namespace FMOD;

// �̸� ���������� ��Ƶδ� ���尡 �� 20����� ����� + 5
// ���� ��Ƶδ°Ŵ� �پ��� ȿ������ ����
// �Ǵ� ���尡 ������ �ȳ����� ��� ������ ũ�� ũ�� ��°�
#define EXTRACHANNELBUFFER 5
#define SOUNDBUFFER 20

#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

class SoundManager : public SingletonBase<SoundManager>
{
private:
	map<string, Sound**> m_totalSounds;

	// fmod ������� ������ fopen ���� file ������
	// fmod ����ϱ����� ��������, ������
	System* m_system;
	// ���� �����͸� ���� ������
	// ����� ���� ���ڰ��� ���� �����ͷ� �Ǿ��־
	Sound** m_sound;		// ���� 1 �� (�ٹ��� �뷡 �ϳ�)
	Channel** m_channel;	// �ٹ� ��ü�� ��Ÿ���°�
public:
	SoundManager();
	~SoundManager();

	HRESULT Init();
	void Release();
	void Update();	// play�� �Ǵ� �༮

					// bgm�� bgm���� ������ ȿ�������� ������
					// loop�� �ݺ��Ұ���
	void AddSound(string keyName, string fileName,
		bool bgm = FALSE, bool loop = FALSE);
	// volume �Ҹ�ũ�� ����
	// volume = 0.0 ~ 1.0f 1.0f���� �ߴµ��� ������ü �Ҹ��� Ű������
	void Play(string keyName, float volume = 1.0f);
	void Stop(string keyName);		// ���� �� ���尪�� ó������ ���ư�
	void Pause(string keyName);		// �Ͻ����� �� ��ġ�� ���ߴ°�
	void Resume(string keyName);	// �ٽ� ���

	bool IsPlaySound(string keyName);
	bool isPauseSound(string keyName);
};

#define SOUND SoundManager::GetSingleton()