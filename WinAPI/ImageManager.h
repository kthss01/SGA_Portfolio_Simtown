#pragma once

#include "SingletonBase.h"
#include "Image.h"

// �̹��� ���� �� ȣ��
// �̹��� �Ŵ����� �̹��� ���� �ϰ� ������ ���� ������ �ϴ� �Ŷ�� ���� ��

class ImageManager : public SingletonBase<ImageManager>
{
private:
	map<string, Image*> _mImageList;

public:
	ImageManager();
	~ImageManager();

	// init�� �� ������ ���� ����
	void Release();
	Image* AddImage(string strKey, const char* fileName,
		float x, float y, int width, int height,
		bool isTrans, COLORREF transColor);
	Image* AddImage(string strKey, const char* fileName,
		float x, float y, int width, int height,
		int frameX, int frameY, bool isTrans, COLORREF transColor);

	Image* FindImage(string strKey);
	bool DeleteImage(string strKey);
	bool DeleteAll();

	// �Ϲ� ����
	void Render(string strKey, HDC hdc, int destX, int destY);
	void Render(string strKey, HDC hdc, int destX, int destY,
		int sourX, int sourY, int sourWidth, int sourHeight);
	
	// ���� ����
	void AlphaRender(string strKey, HDC hdc, int destX, int destY, BYTE alpha);

	// ������ ����
	void FrameRender(string strKey, HDC hdc, int destX, int destY,
		int currentFrameX, int currentFrameY);
};

#define IMAGE ImageManager::GetSingleton()

