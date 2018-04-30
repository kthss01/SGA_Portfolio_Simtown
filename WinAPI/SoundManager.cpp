#include "stdafx.h"
#include "SoundManager.h"


SoundManager::SoundManager()
	:m_system(NULL), m_channel(NULL), m_sound(NULL)
{
}


SoundManager::~SoundManager()
{
}

HRESULT SoundManager::Init()
{
	// ���� �ý��� ����
	System_Create(&m_system);

	// flag�� �ɼ�
	m_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	m_sound = new Sound*[TOTALSOUNDBUFFER];
	m_channel = new Channel*[TOTALSOUNDBUFFER];

	// ZeroMemory�� �ص���
	/// memset ZeroMemory�� �ʱ�ȭ�� ���� �߻���
	/// �׳� �ݺ��� ������ �ʱ�ȭ ��Ŵ
	//memset(m_sound, 0, sizeof(Sound*) * TOTALSOUNDBUFFER);
	//memset(m_channel, 0, sizeof(Channel*) * TOTALSOUNDBUFFER);
	//ZeroMemory(m_sound, sizeof(Sound*) * TOTALSOUNDBUFFER);
	//ZeroMemory(m_channel, sizeof(Channel*) * TOTALSOUNDBUFFER);
	for (int i = 0; i < TOTALSOUNDBUFFER; i++) {
		m_sound[i] = NULL;
		m_channel[i] = NULL;
	}

	return S_OK;
}

void SoundManager::Release()
{
	if (m_channel != NULL || m_sound != NULL) {
		for (int i = 0; i < TOTALSOUNDBUFFER; i++) {
			if (m_channel != NULL) {
				// fmod�� ���� �����ؾ��� �κ��� 
				// channel�� ������ �ִ� ��찡 ����
				if (m_channel[i]) m_channel[i]->stop();
			}
			if (m_sound != NULL) {
				if (m_sound[i]) m_sound[i]->release();
			}
		}
	}
	SAFE_DELETE_ARRAY(m_sound);
	SAFE_DELETE_ARRAY(m_channel);

	if (m_system != NULL) {
		m_system->release();
		m_system->close();
	}

	// test
	// fmod ���� 3d ���� ������ �̷��� �����ϸ� �ȴٰ� ��
	// 3d ���� �Ÿ��� ���� ����
	//m_channel[0]->set3DMinMaxDistance()
	// 3d ���� ��ġ�� ���� ����
	//m_channel[0]->set3DCustomRolloff()
	// ���÷� ȿ���� �� �� ����
}

void SoundManager::Update()
{
	m_system->update();
}

void SoundManager::AddSound(string keyName, string fileName, bool bgm, bool loop)
{
	// stream sound ���̴� ũ�� ���µ�
	// stream�� sound�� �Ϸķ� �� ����Ǵµ�
	// sound�� �׳� ����ȴٰ� �����
	// ����Ǵ� ����� �ٸ�

	if (loop) {
		if (bgm) {
			// string.c_str() string char* ������ ���� ���ִ°�
			m_system->createStream(fileName.c_str(),
				FMOD_LOOP_NORMAL, NULL,
				&m_sound[m_totalSounds.size()]);
		}
		else {
			m_system->createSound(fileName.c_str(),
				FMOD_LOOP_NORMAL, NULL,
				&m_sound[m_totalSounds.size()]);
		}
	}
	else {
		m_system->createSound(fileName.c_str(),
			FMOD_DEFAULT, NULL,
			&m_sound[m_totalSounds.size()]);
	}
	m_totalSounds[keyName] = &m_sound[m_totalSounds.size()];
}

void SoundManager::Play(string keyName, float volume)
{
	// channel���� �÷����ؾߵǼ� count�� �����°�
	int count = 0;
	map<string, Sound**>::iterator iter;
	for (iter = m_totalSounds.begin();
		iter != m_totalSounds.end(); ++iter, count++) {
		if (keyName == iter->first) {
			// false�� �ٷ� �Ͻ����� ��ų�� ����
			m_system->playSound(FMOD_CHANNEL_FREE,
				*iter->second, false, &m_channel[count]);
			m_channel[count]->setVolume(volume);
			break;
		}
	}
}

void SoundManager::Stop(string keyName)
{
	int count = 0;
	map<string, Sound**>::iterator iter;
	for (iter = m_totalSounds.begin();
		iter != m_totalSounds.end(); ++iter, count++) {
		if (keyName == iter->first) {
			m_channel[count]->stop();
			break;
		}
	}
}

void SoundManager::Pause(string keyName)
{
	int count = 0;
	map<string, Sound**>::iterator iter;
	for (iter = m_totalSounds.begin();
		iter != m_totalSounds.end(); ++iter, count++) {
		if (keyName == iter->first) {
			m_channel[count]->setPaused(true);
			break;
		}
	}
}

void SoundManager::Resume(string keyName)
{
	int count = 0;
	map<string, Sound**>::iterator iter;
	for (iter = m_totalSounds.begin();
		iter != m_totalSounds.end(); ++iter, count++) {
		if (keyName == iter->first) {
			m_channel[count]->setPaused(false);
			break;
		}
	}
}

bool SoundManager::IsPlaySound(string keyName)
{
	bool isPlay;

	int count = 0;
	map<string, Sound**>::iterator iter;
	for (iter = m_totalSounds.begin();
		iter != m_totalSounds.end(); ++iter, count++) {
		if (keyName == iter->first) {
			m_channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool SoundManager::isPauseSound(string keyName)
{
	bool isPaused;

	int count = 0;
	map<string, Sound**>::iterator iter;
	for (iter = m_totalSounds.begin();
		iter != m_totalSounds.end(); ++iter, count++) {
		if (keyName == iter->first) {
			m_channel[count]->getPaused(&isPaused);
			break;
		}
	}

	return isPaused;
}
