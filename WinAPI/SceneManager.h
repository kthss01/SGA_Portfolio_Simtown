#pragma once
#include "SingletonBase.h"

// ���� ������ ����
// ���� ����� include �ϴ°� ���� ����� �ƴ�
// �����ؼ��� ����� include ���ϴ°� ����
/*
GameNode���� �̹��� include �ϰ� �̹������� ���ӳ�� include �ϸ�
�Ӵٰ��� �ϰ� �Ǽ� ����
������ cpp�� ���� �� ����� ���� �����ϴ� ����� ��
*/
class GameNode;
class SceneManager : public SingletonBase<SceneManager>
{
private:
	typedef map<string, GameNode*> mSceneList;
private:
	static GameNode* _currentScene; // ���� �÷��� ��
	mSceneList _mSceneList; // �� ���
public:
	SceneManager();
	~SceneManager();

	void Init();
	void Release();
	void Update();
	void Render();

	GameNode* AddScene(string sceneName, GameNode* scene);
	HRESULT ChangeScene(string sceneName);
};

#define SCENE SceneManager::GetSingleton()

/*
i++ -> i ���� �� + 1
++i -> +1 �� ����
*/