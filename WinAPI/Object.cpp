#include "stdafx.h"
#include "Object.h"


void Object::ObjectEnable()
{
	//_player->SetX(RND->GetFromInto(100, WINSIZEX - 100));
	//_player->SetY(RND->GetFromInto(100, WINSIZEY - 100));
	position.x = RND->GetFromInto(100, WINSIZEX - 100);
	position.y = RND->GetFromInto(100, WINSIZEY - 100);
	deltaTime = 0;
}

void Object::ObjectDisable()
{
	deltaTime = 0;
}

void Object::ObjectUpdate()
{
	// animation������ animation ������ ����
	// �� �����Ӵ� ����ð� �ֱ�
	deltaTime += FRAME->GetElapsedTime();

	if (deltaTime < 0)
		deltaTime = 0;

	if (deltaTime > 0.1f) {
		currentFrameX = (currentFrameX + 1) % (_player->GetMaxFrameX() + 1);
		//_player->SetFrameX(
		//	(_player->GetFrameX() + 1) %
		//	(_player->GetMaxFrameX() + 1));
		deltaTime = 0;
	}
}

void Object::ObjectNoActiveUpdate()
{
	deltaTime += FRAME->GetElapsedTime();

	// ������ �ð�����
	if (deltaTime > _respawnTime) {
		this->SetActive(true);
	}
}

void Object::ObjectRender()
{
	//_player->FrameRender(GetMemDC(),
	//	_player->GetX(), _player->GetY());
	_player->FrameRender(GetMemDC(), position.x, position.y,
		currentFrameX, 0);
}

// bool ���� NULL ������ false �� 0�̶�

Object::Object()
	: bActive(NULL), _player(NULL), _respawnTime(3.0f), currentFrameX(0)
{
	//bActive = NULL;
}


Object::~Object()
{
}

HRESULT Object::Init()
{
	return S_OK;
}

void Object::Release()
{
}

void Object::Update()
{
	if (this->bActive)
		this->ObjectUpdate();
	else
		this->ObjectNoActiveUpdate();
}

void Object::Render()
{
	if (this->bActive)
		this->ObjectRender();

	char str[128];

	if (this->bActive)
		sprintf_s(str, "pos : %d, %d, %d : True",
			position.x, position.y, _playerID);
	else
		sprintf_s(str, "pos : %d, %d, %d : False, %f",
			position.x, position.y, _playerID, _respawnTime - deltaTime);

	TextOut(GetMemDC(), 0, _playerID * 20 + 60, str, strlen(str));
}

void Object::SetActive(bool bActive)
{
	// ��Ȱ��ȭ -> Ȱ��ȭ
	if (this->bActive == false && bActive == true) {
		this->ObjectEnable();
		this->bActive = true;
	}
	else if (this->bActive == true && bActive == false) {
		this->ObjectDisable();
		this->bActive = false;
	}
}
