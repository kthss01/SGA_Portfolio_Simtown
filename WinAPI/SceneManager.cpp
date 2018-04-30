#include "stdafx.h"
#include "SceneManager.h"
#include "GameNode.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
}

GameNode* SceneManager::_currentScene = NULL;

void SceneManager::Init()
{
	return;
}

void SceneManager::Release()
{
	map<string, GameNode*>::iterator iter;
	iter = _mSceneList.begin();
	for (; iter != _mSceneList.end();) {
		// ����
		if (iter->second != NULL) {
			if (iter->second == _currentScene) iter->second->Release();
			SAFE_DELETE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else {
			++iter;
		}
	}
	_mSceneList.clear();
}

void SceneManager::Update()
{
	if (_currentScene) _currentScene->Update();
}

void SceneManager::Render()
{
	// hdc �ʿ��ѵ� �� ��
	if (_currentScene) _currentScene->Render();
}

GameNode * SceneManager::AddScene(string sceneName, GameNode * scene)
{
	// ���� ���� ���
	if (!scene) return NULL;
	// ������ �̸� �����ϴ� ���
	if (_mSceneList.count(sceneName) > 0) return NULL;

	_mSceneList[sceneName] = scene;

	return _mSceneList[sceneName];
}

HRESULT SceneManager::ChangeScene(string sceneName)
{
	// �� ���� ���� Ȯ��
	if (_mSceneList.count(sceneName) < 1) return E_FAIL;
	if (_mSceneList[sceneName] == _currentScene) return E_FAIL;

	// ������� ������ �� ����
	if (SUCCEEDED(_mSceneList[sceneName]->Init())) {
		// ���� ���� ���� �ϸ� ���� �� ������
		if (_currentScene) _currentScene->Release();
		_currentScene = _mSceneList[sceneName];

		return S_OK;
	}

	return E_FAIL;
}
