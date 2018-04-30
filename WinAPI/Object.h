#pragma once

#include "GameNode.h"

// Ŭ���� ������ include ������ �ϴ°͵� �ð�������
// �ѹ��� include �ϰ� �ϴ� ����� ����
// stdafx.h ��� �־��
// ������ stdafx.h include �ϴ� ��찡 �ִµ�
// Ŭ������ id�� �־�ΰ� id���� ������ ȣ���ϴ°� ������

// #ifdef Ȱ��
// ��Ȯ���� #ifndef �̿��ؼ� ���� �ȵ������� ���� �ϴ½�����

#ifdef Test

#endif // Test

// �̰� �ݴ���
// �̰ɷ� Ȱ��
#ifndef Test

#define Test

#endif // !1

class Object : public GameNode
{
private:
	bool bActive;
	Image* _player;

	float deltaTime;
	float _respawnTime;

	int _playerID;
	POINT position;

	int currentFrameX;
private:
	void ObjectEnable();			// ��Ȱ��ȭ -> Ȱ��ȭ
	void ObjectDisable();			// Ȱ��ȭ -> ��Ȱ��ȭ

									// �ȿ��� ���� �� �� �༮
	void ObjectUpdate();			// Ȱ��ȭ�� ������Ʈ
									// ������ �ð� ������شٵ���
	void ObjectNoActiveUpdate();	// ��Ȱ��ȭ�� ������Ʈ
	void ObjectRender();
public:
	Object();
	~Object();

	HRESULT Init() override;
	void Release() override;

	// �ܺο��� ������ �� �༮
	void Update() override;
	void Render() override;

	void SetActive(bool bActive);
	bool GetActive() { return bActive; }

	void SetImage(Image* image) { _player = image; }
	void SetID(int id) { _playerID = id; }
};

