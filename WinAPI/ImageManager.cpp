#include "stdafx.h"
#include "ImageManager.h"


ImageManager::ImageManager()
{
}


ImageManager::~ImageManager()
{
}

void ImageManager::Release()
{
	DeleteAll();
}

Image * ImageManager::AddImage(string strKey, const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	Image* img = FindImage(strKey);

	if (img) return img;

	img = new Image;

	if (FAILED(img->Init(fileName, x, y, width, height, isTrans, transColor))) {
		SAFE_DELETE(img);
		return NULL;
	}

	_mImageList[strKey] = img;

	return img;
}

Image * ImageManager::AddImage(string strKey, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	// �˻��ؼ� �̹� ������ �߰����� �ʰ� �׳� ����
	Image* img = FindImage(strKey);

	if (img) return img;

	// SUCCEEDED() ���ڰ��� true false -> true�̸� true false�̸� false ��ȯ
	// FAILED() ���ڰ� true or false -> true�̸� false false�̸� true ��ȯ
	// HRESULT �� ��� ���
	// S_OK E_FAILED, INVALID DATA(��ȿ���� �ʴ� ������) �̷��͵� ����

	img = new Image;
	// �����ϸ� true�� �����ϸ� false
	if (FAILED(
		img->Init(fileName, x, y, width, height,
			frameX, frameY, isTrans, transColor))) {
		SAFE_DELETE(img);
		return NULL;
	}

	_mImageList[strKey] = img;

	return img;
}

Image * ImageManager::FindImage(string strKey)
{
	// �ڵ� �����δ� �̰� ��������
	// iterator�� ã���� �޸� �����δ� ���� count�� �� Ȯ���ؾߵǴ�
	if (_mImageList.count(strKey) < 1) return NULL;

	return _mImageList[strKey];
}

bool ImageManager::DeleteImage(string strKey)
{
	return false;
}

bool ImageManager::DeleteAll()
{
	map<string, Image*>::iterator iter = _mImageList.begin();

	for (; iter != _mImageList.end();) {
		if (iter->second != NULL) {
			iter->second->Release(); // IMAGE_INFO ���� ����
			SAFE_DELETE(iter->second); // image* ����
			iter = _mImageList.erase(iter); // map���� ����
		}
		else {
			iter++;
		}
	}
	_mImageList.clear(); // ����� �ѹ� �� Ȯ���ϰ� ����� ���ؼ�

	return true;
}

void ImageManager::Render(string strKey, HDC hdc, int destX, int destY)
{
	Image* img = FindImage(strKey);
	if (img) img->Render(hdc, destX, destY);
}

void ImageManager::Render(string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	Image* img = FindImage(strKey);
	if (img) img->Render(hdc, destX, destY, sourX, sourY, sourWidth, sourHeight);
}

void ImageManager::AlphaRender(string strKey, HDC hdc, int destX, int destY, BYTE alpha)
{
	Image* img = FindImage(strKey);
	if (img) img->AlphaRender(hdc, destX, destY, alpha);
}

void ImageManager::FrameRender(string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	Image* img = FindImage(strKey);
	if (img) img->FrameRender(hdc, destX, destY, currentFrameX, currentFrameY);
}
