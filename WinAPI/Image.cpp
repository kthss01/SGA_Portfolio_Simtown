#include "stdafx.h"
#include "Image.h"

// ���� ���� ����� ���� ���̺귯��(lib)
#pragma comment(lib, "msimg32.lib")

Image::Image()
	:m_imageInfo(NULL)
	, fileName(NULL)
	, isTrans(false)
	, transColor(RGB(0, 0, 0))
	, blendImage(NULL)
{
}

Image::~Image() {
	this->Release();
}

HRESULT Image::Init(int width, int height) {
	if (m_imageInfo != NULL) Release();

	HDC hdc = GetDC(g_hWnd);

	m_imageInfo = new IMAGE_INFO;
	m_imageInfo->loadType = LOAD_EMPTY;
	m_imageInfo->resID = 0;
	m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	m_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	m_imageInfo->hOBit = (HBITMAP)SelectObject(
		m_imageInfo->hMemDC, m_imageInfo->hBit);
	m_imageInfo->width = width;
	m_imageInfo->height = height;

	if (m_imageInfo->hBit == 0) {
		Release();

		return E_FAIL;
	}

	// �⺻������ �ʱ�ȭ ���شٰ� ���� ��
	blendImage = new IMAGE_INFO;
	blendImage->loadType = LOAD_EMPTY;
	blendImage->resID = 0;
	blendImage->hMemDC = CreateCompatibleDC(hdc);
	blendImage->hBit =
		(HBITMAP)CreateCompatibleBitmap(hdc,
			m_imageInfo->width, m_imageInfo->height);
	blendImage->hOBit =
		(HBITMAP)SelectObject(blendImage->hMemDC, blendImage->hBit);
	blendImage->width = WINSIZEX;
	blendImage->height = WINSIZEY;

	// alpha blend �ɼ�
	// ��Ÿ �ɼ�
	blendFunc.BlendFlags = 0;
	// ��Ʈ�� �̹��� 32��Ʈ �ΰ�� AC_SRC_ALPHA
	// �� �ܿ� 16��Ʈ 24��Ʈ ��� �������� 0���� �ʱ�ȭ
	blendFunc.AlphaFormat = 0;
	// ���� ������ �̰͸� ����ϸ� �ȴ� �˰� ����
	blendFunc.BlendOp = AC_SRC_OVER;

	if (blendImage == NULL) {
		Release();
		return E_FAIL;
	}

	// �������� �����ִ� �̹����� ���İ� �� �� ����
	// ���İ� �� �� �ִ� �̹����� ����Ÿ �� �� ����
	// �׷��� ����Ÿ ���� ���İ� �ִ� ������ �Ҳ�

	tempDC = CreateCompatibleDC(hdc);

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	if (m_imageInfo != NULL) Release();

	HDC hdc = GetDC(g_hWnd);

	m_imageInfo = new IMAGE_INFO;
	m_imageInfo->loadType = LOAD_FILE;
	m_imageInfo->resID = 0;
	m_imageInfo->hMemDC = CreateCompatibleDC(hdc);

	m_imageInfo->hBit =
		(HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height,
			LR_LOADFROMFILE);

	m_imageInfo->hOBit = (HBITMAP)SelectObject(
		m_imageInfo->hMemDC, m_imageInfo->hBit);
	m_imageInfo->width = width;
	m_imageInfo->height = height;

	int len = strlen(fileName);
	// ���̰��� +1 �ѰŴ� null���� ������ �Ѱ�
	this->fileName = new char[len + 1];
	strcpy_s(this->fileName, len + 1, fileName);

	this->isTrans = isTrans;
	this->transColor = transColor;

	if (m_imageInfo->hBit == 0) {
		Release();

		return E_FAIL;
	}

	// �⺻������ �ʱ�ȭ ���شٰ� ���� ��
	blendImage = new IMAGE_INFO;
	blendImage->loadType = LOAD_EMPTY;
	blendImage->resID = 0;
	blendImage->hMemDC = CreateCompatibleDC(hdc);
	blendImage->hBit =
		(HBITMAP)CreateCompatibleBitmap(hdc,
			m_imageInfo->width, m_imageInfo->height);
	blendImage->hOBit =
		(HBITMAP)SelectObject(blendImage->hMemDC, blendImage->hBit);
	blendImage->width = WINSIZEX;
	blendImage->height = WINSIZEY;

	// alpha blend �ɼ�
	// ��Ÿ �ɼ�
	blendFunc.BlendFlags = 0;
	// ��Ʈ�� �̹��� 32��Ʈ �ΰ�� AC_SRC_ALPHA
	// �� �ܿ� 16��Ʈ 24��Ʈ ��� �������� 0���� �ʱ�ȭ
	blendFunc.AlphaFormat = 0;
	// ���� ������ �̰͸� ����ϸ� �ȴ� �˰� ����
	blendFunc.BlendOp = AC_SRC_OVER;

	if (blendImage == NULL) {
		Release();
		return E_FAIL;
	}

	// �������� �����ִ� �̹����� ���İ� �� �� ����
	// ���İ� �� �� �ִ� �̹����� ����Ÿ �� �� ����
	// �׷��� ����Ÿ ���� ���İ� �ִ� ������ �Ҳ�

	tempDC = CreateCompatibleDC(hdc);

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	if (m_imageInfo != NULL) Release();

	HDC hdc = GetDC(g_hWnd);

	m_imageInfo = new IMAGE_INFO;
	m_imageInfo->loadType = LOAD_FILE;
	m_imageInfo->resID = 0;
	m_imageInfo->hMemDC = CreateCompatibleDC(hdc);

	m_imageInfo->hBit =
		(HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height,
			LR_LOADFROMFILE);

	m_imageInfo->hOBit = (HBITMAP)SelectObject(
		m_imageInfo->hMemDC, m_imageInfo->hBit);

	m_imageInfo->x = x - (width / width / 2);
	m_imageInfo->y = y - (height / height / 2);

	m_imageInfo->width = width;
	m_imageInfo->height = height;

	int len = strlen(fileName);
	// ���̰��� +1 �ѰŴ� null���� ������ �Ѱ�
	this->fileName = new char[len + 1];
	strcpy_s(this->fileName, len + 1, fileName);

	this->isTrans = isTrans;
	this->transColor = transColor;

	if (m_imageInfo->hBit == 0) {
		Release();

		return E_FAIL;
	}

	// �⺻������ �ʱ�ȭ ���شٰ� ���� ��
	blendImage = new IMAGE_INFO;
	blendImage->loadType = LOAD_EMPTY;
	blendImage->resID = 0;
	blendImage->hMemDC = CreateCompatibleDC(hdc);
	blendImage->hBit =
		(HBITMAP)CreateCompatibleBitmap(hdc,
			m_imageInfo->width, m_imageInfo->height);
	blendImage->hOBit =
		(HBITMAP)SelectObject(blendImage->hMemDC, blendImage->hBit);
	blendImage->width = WINSIZEX;
	blendImage->height = WINSIZEY;

	// alpha blend �ɼ�
	// ��Ÿ �ɼ�
	blendFunc.BlendFlags = 0;
	// ��Ʈ�� �̹��� 32��Ʈ �ΰ�� AC_SRC_ALPHA
	// �� �ܿ� 16��Ʈ 24��Ʈ ��� �������� 0���� �ʱ�ȭ
	blendFunc.AlphaFormat = 0;
	// ���� ������ �̰͸� ����ϸ� �ȴ� �˰� ����
	blendFunc.BlendOp = AC_SRC_OVER;

	if (blendImage == NULL) {
		Release();
		return E_FAIL;
	}

	// �������� �����ִ� �̹����� ���İ� �� �� ����
	// ���İ� �� �� �ִ� �̹����� ����Ÿ �� �� ����
	// �׷��� ����Ÿ ���� ���İ� �ִ� ������ �Ҳ�

	tempDC = CreateCompatibleDC(hdc);

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	if (m_imageInfo != NULL) Release();

	HDC hdc = GetDC(g_hWnd);

	m_imageInfo = new IMAGE_INFO;
	m_imageInfo->loadType = LOAD_FILE;
	m_imageInfo->resID = 0;
	m_imageInfo->hMemDC = CreateCompatibleDC(hdc);

	m_imageInfo->hBit =
		(HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height,
			LR_LOADFROMFILE);

	m_imageInfo->hOBit = (HBITMAP)SelectObject(
		m_imageInfo->hMemDC, m_imageInfo->hBit);

	m_imageInfo->width = width;
	m_imageInfo->height = height;

	m_imageInfo->maxFrameX = frameX - 1;
	m_imageInfo->maxFrameY = frameY - 1;
	m_imageInfo->frameWidth = width / frameX;
	m_imageInfo->frameHeight = height / frameY;

	int len = strlen(fileName);
	// ���̰��� +1 �ѰŴ� null���� ������ �Ѱ�
	this->fileName = new char[len + 1];
	strcpy_s(this->fileName, len + 1, fileName);

	this->isTrans = isTrans;
	this->transColor = transColor;

	if (m_imageInfo->hBit == 0) {
		Release();

		return E_FAIL;
	}

	// �⺻������ �ʱ�ȭ ���شٰ� ���� ��
	blendImage = new IMAGE_INFO;
	blendImage->loadType = LOAD_EMPTY;
	blendImage->resID = 0;
	blendImage->hMemDC = CreateCompatibleDC(hdc);
	blendImage->hBit =
		(HBITMAP)CreateCompatibleBitmap(hdc,
			m_imageInfo->width, m_imageInfo->height);
	blendImage->hOBit =
		(HBITMAP)SelectObject(blendImage->hMemDC, blendImage->hBit);
	blendImage->width = WINSIZEX;
	blendImage->height = WINSIZEY;

	// alpha blend �ɼ�
	// ��Ÿ �ɼ�
	blendFunc.BlendFlags = 0;
	// ��Ʈ�� �̹��� 32��Ʈ �ΰ�� AC_SRC_ALPHA
	// �� �ܿ� 16��Ʈ 24��Ʈ ��� �������� 0���� �ʱ�ȭ
	blendFunc.AlphaFormat = 0;
	// ���� ������ �̰͸� ����ϸ� �ȴ� �˰� ����
	blendFunc.BlendOp = AC_SRC_OVER;

	if (blendImage == NULL) {
		Release();
		return E_FAIL;
	}

	// �������� �����ִ� �̹����� ���İ� �� �� ����
	// ���İ� �� �� �ִ� �̹����� ����Ÿ �� �� ����
	// �׷��� ����Ÿ ���� ���İ� �ִ� ������ �Ҳ�

	tempDC = CreateCompatibleDC(hdc);

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	if (m_imageInfo != NULL) Release();

	HDC hdc = GetDC(g_hWnd);

	m_imageInfo = new IMAGE_INFO;
	m_imageInfo->loadType = LOAD_FILE;
	m_imageInfo->resID = 0;
	m_imageInfo->hMemDC = CreateCompatibleDC(hdc);

	m_imageInfo->hBit =
		(HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height,
			LR_LOADFROMFILE);

	m_imageInfo->hOBit = (HBITMAP)SelectObject(
		m_imageInfo->hMemDC, m_imageInfo->hBit);

	// ������ ������ �߽����� x�� �ǰԲ�
	m_imageInfo->x = x - (width / frameX / 2);
	m_imageInfo->y = y - (height / frameY / 2);

	m_imageInfo->width = width;
	m_imageInfo->height = height;

	m_imageInfo->maxFrameX = frameX - 1;
	m_imageInfo->maxFrameY = frameY - 1;
	m_imageInfo->frameWidth = width / frameX;
	m_imageInfo->frameHeight = height / frameY;

	int len = strlen(fileName);
	// ���̰��� +1 �ѰŴ� null���� ������ �Ѱ�
	this->fileName = new char[len + 1];
	strcpy_s(this->fileName, len + 1, fileName);

	this->isTrans = isTrans;
	this->transColor = transColor;

	if (m_imageInfo->hBit == 0) {
		Release();

		return E_FAIL;
	}

	// �⺻������ �ʱ�ȭ ���شٰ� ���� ��
	blendImage = new IMAGE_INFO;
	blendImage->loadType = LOAD_EMPTY;
	blendImage->resID = 0;
	blendImage->hMemDC = CreateCompatibleDC(hdc);
	blendImage->hBit =
		(HBITMAP)CreateCompatibleBitmap(hdc,
			m_imageInfo->width, m_imageInfo->height);
	blendImage->hOBit =
		(HBITMAP)SelectObject(blendImage->hMemDC, blendImage->hBit);
	blendImage->width = WINSIZEX;
	blendImage->height = WINSIZEY;

	// alpha blend �ɼ�
	// ��Ÿ �ɼ�
	blendFunc.BlendFlags = 0;
	// ��Ʈ�� �̹��� 32��Ʈ �ΰ�� AC_SRC_ALPHA
	// �� �ܿ� 16��Ʈ 24��Ʈ ��� �������� 0���� �ʱ�ȭ
	blendFunc.AlphaFormat = 0;
	// ���� ������ �̰͸� ����ϸ� �ȴ� �˰� ����
	blendFunc.BlendOp = AC_SRC_OVER;

	if (blendImage == NULL) {
		Release();
		return E_FAIL;
	}

	// �������� �����ִ� �̹����� ���İ� �� �� ����
	// ���İ� �� �� �ִ� �̹����� ����Ÿ �� �� ����
	// �׷��� ����Ÿ ���� ���İ� �ִ� ������ �Ҳ�

	tempDC = CreateCompatibleDC(hdc);

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

void Image::Release() {
	if (m_imageInfo) {
		SelectObject(m_imageInfo->hMemDC, m_imageInfo->hOBit);
		DeleteObject(m_imageInfo->hOBit);
		DeleteDC(m_imageInfo->hMemDC);

		SAFE_DELETE(m_imageInfo);
		SAFE_DELETE(fileName);

		isTrans = false;
		transColor = RGB(0, 0, 0);

		DeleteDC(tempDC);
	}
}

void Image::Render(HDC hdc) {

	// Ư�� ������ ���� �׸��ڴٸ�
	if (isTrans) {
		//GdiTransparentBlt : ��Ʈ�� ��½� Ư�� ������ ���� �ϰ� ���
		/*
		bitmap�� alpha���� ���� �����ϰ� ���� ���� ����
		���� ����ϴ� ĳ�����ϰ� ����ϰ� ���ÿ� ������ ĳ���� �ܺλ��� ��������
		*/
		GdiTransparentBlt
		(
			// ȭ�� �κ�
			hdc,					// ����� ����� DC
			0,						// ����� ��ǥ�� ������ X
			0,						// ����� ��ǥ�� ������ Y
			m_imageInfo->width,		// ����� �̹����� ���� ũ��
			m_imageInfo->height,	// ����� �̹����� ���� ũ��

									// �̹��� �κ�
			m_imageInfo->hMemDC,	// ����� ����� DC
									// ����κ� �����ϸ� ���ϴ� �κи� �׸� �� ����
			0,						// ����� �̹��� ������ X
			0,						// ����� �̹��� ������ Y
									// �̰� �̹��� �������� ũ�� ������ �κ� ������� ä����
			m_imageInfo->width,		// ����� �̹��� ����ũ��
			m_imageInfo->height,	// ����� �̹��� ����ũ��
			transColor				// �����Ҷ� ������ ����(�⺻������ ����Ÿ ��)
		);
	}
	// ���� �̹��� �״�� �׳� �׸��ڴٸ�
	else {
		BitBlt(hdc, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void Image::Render(HDC hdc, int destX, int destY)
{
	// Ư�� ������ ���� �׸��ڴٸ�
	if (isTrans) {
		//GdiTransparentBlt : ��Ʈ�� ��½� Ư�� ������ ���� �ϰ� ���
		// transparent : ������
		/*
		bitmap�� alpha���� ���� �����ϰ� ���� ���� ����
		���� ����ϴ� ĳ�����ϰ� ����ϰ� ���ÿ� ������ ĳ���� �ܺλ��� ��������
		*/

		GdiTransparentBlt
		(
			// ȭ�� �κ�
			hdc,					// ����� ����� DC
			destX,					// ����� ��ǥ�� ������ X
			destY,					// ����� ��ǥ�� ������ Y
			m_imageInfo->width,		// ����� �̹����� ���� ũ��
			m_imageInfo->height,	// ����� �̹����� ���� ũ��

									// �̹��� �κ�
			m_imageInfo->hMemDC,	// ����� ����� DC
									// ����κ� �����ϸ� ���ϴ� �κи� �׸� �� ����
			0,						// ����� �̹��� ������ X
			0,						// ����� �̹��� ������ Y
									// �̰� �̹��� �������� ũ�� ������ �κ� ������� ä����
			m_imageInfo->width,		// ����� �̹��� ����ũ��
			m_imageInfo->height,	// ����� �̹��� ����ũ��
			transColor				// �����Ҷ� ������ ����(�⺻������ ����Ÿ ��)
		);
	}
	// ���� �̹��� �״�� �׳� �׸��ڴٸ�
	else {
		BitBlt(hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void Image::Render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	// Ư�� ������ ���� �׸��ڴٸ�
	if (isTrans) {
		//GdiTransparentBlt : ��Ʈ�� ��½� Ư�� ������ ���� �ϰ� ���
		// transparent : ������
		/*
		bitmap�� alpha���� ���� �����ϰ� ���� ���� ����
		���� ����ϴ� ĳ�����ϰ� ����ϰ� ���ÿ� ������ ĳ���� �ܺλ��� ��������
		*/

		GdiTransparentBlt
		(
			// ȭ�� �κ�
			hdc,					// ����� ����� DC
			destX,					// ����� ��ǥ�� ������ X
			destY,					// ����� ��ǥ�� ������ Y
			sourWidth,				// ����� �̹����� ���� ũ��
			sourHeight,				// ����� �̹����� ���� ũ��

									// �̹��� �κ�
			m_imageInfo->hMemDC,	// ����� ����� DC
									// ����κ� �����ϸ� ���ϴ� �κи� �׸� �� ����
			sourX,					// ����� �̹��� ������ X
			sourY,					// ����� �̹��� ������ Y
									// �̰� �̹��� �������� ũ�� ������ �κ� ������� ä����
			sourWidth,				// ����� �̹��� ����ũ��
			sourHeight,				// ����� �̹��� ����ũ��
			transColor				// �����Ҷ� ������ ����(�⺻������ ����Ÿ ��)
		);
	}
	// ���� �̹��� �״�� �׳� �׸��ڴٸ�
	else {
		BitBlt(hdc, destX, destY, sourWidth, sourHeight,
			m_imageInfo->hMemDC, sourX, sourY, SRCCOPY);
	}
}

void Image::AlphaRender(HDC hdc, BYTE alpha)
{
	// ������ �󸶸�ŭ �ְڴ��� ����
	blendFunc.SourceConstantAlpha = alpha;
	// ����Ÿ ����� �� ����
	if (isTrans) {
		// AlphaBlend() Ư������ �� �� ���� ������ ����

		// 1. ��� �ؾߵ� DC�� �׷��� �ִ� ������ ���� �̹����� �׷��ش�
		BitBlt(blendImage->hMemDC, 0, 0, blendImage->width, blendImage->height,
			hdc, 0, 0, SRCCOPY); // ��� ���� SRCCOPY

		// 2. ��� �ؾߵ� �̹����� ���忡 �׷��ش�. (����Ÿ ���� ����)
		GdiTransparentBlt(
			blendImage->hMemDC,
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			m_imageInfo->hMemDC,
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			transColor);
		// 3. ���� DC�� ����ؾߵ� DC�� �׸���.
		AlphaBlend(
			hdc,
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			blendImage->hMemDC, 
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			blendFunc);
	}
	else {
		AlphaBlend(
			hdc, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			blendFunc);
	}
}

void Image::AlphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{
	// ������ �󸶸�ŭ �ְڴ��� ����
	blendFunc.SourceConstantAlpha = alpha;
	// ����Ÿ ����� �� ����
	if (isTrans) {
		// AlphaBlend() Ư������ �� �� ���� ������ ����

		// 1. ��� �ؾߵ� DC�� �׷��� �ִ� ������ ���� �̹����� �׷��ش�
		BitBlt(blendImage->hMemDC, 0, 0, blendImage->width, blendImage->height,
			hdc, destX, destY, SRCCOPY); // ��� ���� SRCCOPY

		// 2. ��� �ؾߵ� �̹����� ���忡 �׷��ش�. (����Ÿ ���� ����)
		GdiTransparentBlt(
			blendImage->hMemDC,
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			m_imageInfo->hMemDC,
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			transColor);
		// 3. ���� DC�� ����ؾߵ� DC�� �׸���.
		AlphaBlend(
			hdc,
			destX,
			destY,
			m_imageInfo->width,
			m_imageInfo->height,
			blendImage->hMemDC, 
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			blendFunc);
	}
	else {
		AlphaBlend(
			hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			blendFunc);
	}
}

void Image::AlphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
	// ������ �󸶸�ŭ �ְڴ��� ����
	blendFunc.SourceConstantAlpha = alpha;
	// ����Ÿ ����� �� ����
	if (isTrans) {
		// AlphaBlend() Ư������ �� �� ���� ������ ����

		// 1. ��� �ؾߵ� DC�� �׷��� �ִ� ������ ���� �̹����� �׷��ش�
		BitBlt(blendImage->hMemDC, 0, 0, blendImage->width, blendImage->height,
			hdc, destX, destY, SRCCOPY); // ��� ���� SRCCOPY

		// 2. ��� �ؾߵ� �̹����� ���忡 �׷��ش�. (����Ÿ ���� ����)
		GdiTransparentBlt(
			blendImage->hMemDC,
			0,
			0,
			sourWidth,
			sourHeight,
			m_imageInfo->hMemDC,
			sourX,
			sourY,
			sourWidth,
			sourHeight,
			transColor);
		// 3. ���� DC�� ����ؾߵ� DC�� �׸���.
		AlphaBlend(
			hdc,
			destX,
			destY,
			sourWidth,
			sourHeight,
			blendImage->hMemDC, 
			0,
			0,
			sourWidth,
			sourHeight,
			blendFunc);
	}
	else {
		AlphaBlend(
			hdc, destX, destY, sourWidth, sourHeight,
			m_imageInfo->hMemDC, sourX, sourY, sourWidth, sourHeight,
			blendFunc);
	}
}

void Image::FrameRender(HDC hdc, int destX, int destY)
{
	// Ư�� ������ ���� �׸��ڴٸ�
	if (isTrans) {
		//GdiTransparentBlt : ��Ʈ�� ��½� Ư�� ������ ���� �ϰ� ���
		// transparent : ������
		/*
		bitmap�� alpha���� ���� �����ϰ� ���� ���� ����
		���� ����ϴ� ĳ�����ϰ� ����ϰ� ���ÿ� ������ ĳ���� �ܺλ��� ��������
		*/

		GdiTransparentBlt
		(
			// ȭ�� �κ�
			hdc,						// ����� ����� DC
			destX,						// ����� ��ǥ�� ������ X
			destY,						// ����� ��ǥ�� ������ Y
			m_imageInfo->frameWidth,	// ����� �̹����� ���� ũ��
			m_imageInfo->frameHeight,	// ����� �̹����� ���� ũ��

										// �̹��� �κ�
			m_imageInfo->hMemDC,		// ����� ����� DC
										// ����κ� �����ϸ� ���ϴ� �κи� �׸� �� ����
			// ����� �̹��� ������ X
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
			// ����� �̹��� ������ Y
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
			// �̰� �̹��� �������� ũ�� ������ �κ� ������� ä����
			m_imageInfo->frameWidth,	// ����� �̹��� ����ũ��
			m_imageInfo->frameHeight,	// ����� �̹��� ����ũ��
			transColor					// �����Ҷ� ������ ����(�⺻������ ����Ÿ ��)
		);
	}
	// ���� �̹��� �״�� �׳� �׸��ڴٸ�
	else {
		BitBlt(hdc, destX, destY,
			m_imageInfo->frameWidth, m_imageInfo->frameHeight,
			m_imageInfo->hMemDC,
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight, SRCCOPY);
	}
}

void Image::FrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	m_imageInfo->currentFrameX = currentFrameX;
	m_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > m_imageInfo->maxFrameX)
		m_imageInfo->currentFrameX = m_imageInfo->maxFrameX;
	if (currentFrameY > m_imageInfo->maxFrameY)
		m_imageInfo->currentFrameY = m_imageInfo->maxFrameY;



	// Ư�� ������ ���� �׸��ڴٸ�
	if (isTrans) {
		//GdiTransparentBlt : ��Ʈ�� ��½� Ư�� ������ ���� �ϰ� ���
		// transparent : ������
		/*
		bitmap�� alpha���� ���� �����ϰ� ���� ���� ����
		���� ����ϴ� ĳ�����ϰ� ����ϰ� ���ÿ� ������ ĳ���� �ܺλ��� ��������
		*/

		GdiTransparentBlt
		(
			// ȭ�� �κ�
			hdc,						// ����� ����� DC
			destX,						// ����� ��ǥ�� ������ X
			destY,						// ����� ��ǥ�� ������ Y
			m_imageInfo->frameWidth,	// ����� �̹����� ���� ũ��
			m_imageInfo->frameHeight,	// ����� �̹����� ���� ũ��

										// �̹��� �κ�
			m_imageInfo->hMemDC,		// ����� ����� DC
										// ����κ� �����ϸ� ���ϴ� �κи� �׸� �� ����
			// ����� �̹��� ������ X
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
			// ����� �̹��� ������ Y
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
			// �̰� �̹��� �������� ũ�� ������ �κ� ������� ä����
			m_imageInfo->frameWidth,	// ����� �̹��� ����ũ��
			m_imageInfo->frameHeight,	// ����� �̹��� ����ũ��
			transColor					// �����Ҷ� ������ ����(�⺻������ ����Ÿ ��)
		);
	}
	// ���� �̹��� �״�� �׳� �׸��ڴٸ�
	else {
		BitBlt(hdc, destX, destY,
			m_imageInfo->frameWidth, m_imageInfo->frameHeight,
			m_imageInfo->hMemDC,
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight, SRCCOPY);
	}
}

void Image::FrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha)
{
	m_imageInfo->currentFrameX = currentFrameX;
	m_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > m_imageInfo->maxFrameX)
		m_imageInfo->currentFrameX = m_imageInfo->maxFrameX;
	if (currentFrameY > m_imageInfo->maxFrameY)
		m_imageInfo->currentFrameY = m_imageInfo->maxFrameY;

	AlphaRender(hdc, destX, destY,
		currentFrameX * m_imageInfo->frameWidth,
		currentFrameY * m_imageInfo->frameHeight,
		m_imageInfo->frameWidth, m_imageInfo->frameHeight, alpha);
}

// drawArea �׷��� �κ�
void Image::LoopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY)
{
	// offset �� ������ ������ �ȵǼ� ���� �ϱ�
	if (offsetX < 0)
		offsetX = m_imageInfo->width + (offsetX % m_imageInfo->width);
	if (offsetY < 0)
		offsetY = m_imageInfo->height + (offsetY % m_imageInfo->height);

	// �׷����� ����(�̹���)�� ������ ����
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	// �׷����� DC�� ����
	RECT rcDest;
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	// ���� ���� ���� ���ϱ�
	for (int y = 0; y < drawAreaH; y += sourHeight) {
		// ������ ���� ���
		rcSour.top = (y + offsetY) % m_imageInfo->height;
		rcSour.bottom = m_imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		// sour������ �׸��� ȭ���� �Ѿ� ���ٸ�
		if (y + sourHeight > drawAreaH) {
			// �Ѿ �׸��� ����ŭ bottom�� ���� �÷��ش�
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}
		// �׷����� ����
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		// ���� ���� ����
		for (int x = 0; x < drawAreaW; x += sourWidth) {
			rcSour.left = (x + offsetX) % m_imageInfo->width;
			rcSour.right = m_imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			// sour������ �׸��� ȭ���� �Ѿ��
			if (x + sourWidth > drawAreaW) {
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			// �׸���
			Render(hdc, rcDest.left, rcDest.top,
				rcSour.left, rcSour.top,
				rcSour.right - rcSour.left,
				rcSour.bottom - rcSour.top);
		}
	}
}

void Image::LoopAlphaRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha)
{
	// offset �� ������ ������ �ȵǼ� ���� �ϱ�
	if (offsetX < 0)
		offsetX = m_imageInfo->width + (offsetX % m_imageInfo->width);
	if (offsetY < 0)
		offsetY = m_imageInfo->height + (offsetY % m_imageInfo->height);

	// �׷����� ����(�̹���)�� ������ ����
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	// �׷����� DC�� ����
	RECT rcDest = {};
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	// ���� ���� ���� ���ϱ�
	for (int y = 0; y < drawAreaH; y += sourHeight) {
		// ������ ���� ���
		rcSour.top = (y + offsetY) % m_imageInfo->height;
		rcSour.bottom = m_imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		// sour������ �׸��� ȭ���� �Ѿ� ���ٸ�
		if (y + sourHeight > drawAreaH) {
			// �Ѿ �׸��� ����ŭ bottom�� ���� �÷��ش�
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}
		// �׷����� ����
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		// ���� ���� ����
		for (int x = 0; x < drawAreaW; x += sourWidth) {
			rcSour.left = (x + offsetX) % m_imageInfo->width;
			rcSour.right = m_imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			// sour������ �׸��� ȭ���� �Ѿ��
			if (x + sourWidth > drawAreaW) {
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			// �׸���
			AlphaRender(hdc, rcDest.left, rcDest.top,
				rcSour.left, rcSour.top,
				rcSour.right - rcSour.left,
				rcSour.bottom - rcSour.top, alpha);
		}
	}
}

void Image::Render(HDC hdc, int destX, int destY, int width, int height)
{
	// Ư�� ������ ���� �׸��ڴٸ�
	if (isTrans) {

		GdiTransparentBlt
		(
			// ȭ�� �κ�
			hdc,					// ����� ����� DC
			destX,					// ����� ��ǥ�� ������ X
			destY,					// ����� ��ǥ�� ������ Y
			width,					// ����� �̹����� ���� ũ��
			height,					// ����� �̹����� ���� ũ��

									// �̹��� �κ�
			m_imageInfo->hMemDC,	// ����� ����� DC
									// ����κ� �����ϸ� ���ϴ� �κи� �׸� �� ����
			0,						// ����� �̹��� ������ X
			0,						// ����� �̹��� ������ Y
									// �̰� �̹��� �������� ũ�� ������ �κ� ������� ä����
			m_imageInfo->width,		// ����� �̹��� ����ũ��
			m_imageInfo->height,	// ����� �̹��� ����ũ��
			transColor				// �����Ҷ� ������ ����(�⺻������ ����Ÿ ��)
		);
	}
	// ���� �̹��� �״�� �׳� �׸��ڴٸ�
	else {
		StretchBlt(hdc, destX, destY, width, height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			SRCCOPY);
	}
}

void Image::AlphaRender(HDC hdc, int destX, int destY, int width, int height, BYTE alpha)
{
	// ������ �󸶸�ŭ �ְڴ��� ����
	blendFunc.SourceConstantAlpha = alpha;
	// ����Ÿ ����� �� ����
	if (isTrans) {
		// AlphaBlend() Ư������ �� �� ���� ������ ����

		// blendImage���� hMemDC�� hBit�� 
		// m_imageInfo�� width height�� �����Ǽ� �׷���
		HBITMAP tempBitmap =
			(HBITMAP)CreateCompatibleBitmap(hdc, width, height);
		HBITMAP temOldBitmap = (HBITMAP)SelectObject(tempDC, tempBitmap);
	
		// 1. ��� �ؾߵ� DC�� �׷��� �ִ� ������ ���� �̹����� �׷��ش�
		BitBlt(tempDC, 0, 0, width, height,
			hdc, destX, destY, SRCCOPY); // ��� ���� SRCCOPY

		// 2. ��� �ؾߵ� �̹����� ���忡 �׷��ش�. (����Ÿ ���� ����)
		GdiTransparentBlt(
			tempDC,
			0,
			0,
			width,
			height,
			m_imageInfo->hMemDC,
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			transColor);
		// 3. ���� DC�� ����ؾߵ� DC�� �׸���.
		AlphaBlend(
			hdc,
			destX,
			destY,
			width,
			height,
			tempDC,
			0,
			0,
			width,
			height,
			blendFunc);

		SelectObject(tempDC, temOldBitmap);
		DeleteObject(tempBitmap);
	}
	else {
		AlphaBlend(
			hdc, destX, destY, width, height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			blendFunc);
	}
}

void Image::FrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, int width, int height)
{
	m_imageInfo->currentFrameX = currentFrameX;
	m_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > m_imageInfo->maxFrameX)
		m_imageInfo->currentFrameX = m_imageInfo->maxFrameX;
	if (currentFrameY > m_imageInfo->maxFrameY)
		m_imageInfo->currentFrameY = m_imageInfo->maxFrameY;

	// Ư�� ������ ���� �׸��ڴٸ�
	if (isTrans) {
		//GdiTransparentBlt : ��Ʈ�� ��½� Ư�� ������ ���� �ϰ� ���
		// transparent : ������
		/*
		bitmap�� alpha���� ���� �����ϰ� ���� ���� ����
		���� ����ϴ� ĳ�����ϰ� ����ϰ� ���ÿ� ������ ĳ���� �ܺλ��� ��������
		*/

		GdiTransparentBlt
		(
			// ȭ�� �κ�
			hdc,						// ����� ����� DC
			destX,						// ����� ��ǥ�� ������ X
			destY,						// ����� ��ǥ�� ������ Y
			width,						// ����� �̹����� ���� ũ��
			height,						// ����� �̹����� ���� ũ��

										// �̹��� �κ�
			m_imageInfo->hMemDC,		// ����� ����� DC
										// ����κ� �����ϸ� ���ϴ� �κи� �׸� �� ����
			// ����� �̹��� ������ X
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
			// ����� �̹��� ������ Y
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
			// �̰� �̹��� �������� ũ�� ������ �κ� ������� ä����
			m_imageInfo->frameWidth,	// ����� �̹��� ����ũ��
			m_imageInfo->frameHeight,	// ����� �̹��� ����ũ��
			transColor					// �����Ҷ� ������ ����(�⺻������ ����Ÿ ��)
		);
	}
	// ���� �̹��� �״�� �׳� �׸��ڴٸ�
	else {
		StretchBlt(hdc, destX, destY, width, height,
			m_imageInfo->hMemDC, 
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth, 
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
			m_imageInfo->frameWidth,
			m_imageInfo->frameHeight,
			SRCCOPY);
	}
}

void Image::FrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, int width, int height, BYTE alpha)
{
	m_imageInfo->currentFrameX = currentFrameX;
	m_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > m_imageInfo->maxFrameX)
		m_imageInfo->currentFrameX = m_imageInfo->maxFrameX;
	if (currentFrameY > m_imageInfo->maxFrameY)
		m_imageInfo->currentFrameY = m_imageInfo->maxFrameY;

	// ������ �󸶸�ŭ �ְڴ��� ����
	blendFunc.SourceConstantAlpha = alpha;
	// ����Ÿ ����� �� ����
	if (isTrans) {
		// AlphaBlend() Ư������ �� �� ���� ������ ����

		// blendImage���� hMemDC�� hBit�� 
		// m_imageInfo�� width height�� �����Ǽ� �׷���
		HBITMAP tempBitmap =
			(HBITMAP)CreateCompatibleBitmap(hdc, width, height);
		HBITMAP temOldBitmap = (HBITMAP)SelectObject(tempDC, tempBitmap);

		// 1. ��� �ؾߵ� DC�� �׷��� �ִ� ������ ���� �̹����� �׷��ش�
		BitBlt(tempDC, 0, 0, width, height,
			hdc, destX, destY, SRCCOPY); // ��� ���� SRCCOPY

		// 2. ��� �ؾߵ� �̹����� ���忡 �׷��ش�. (����Ÿ ���� ����)
		GdiTransparentBlt(
			tempDC,
			0,
			0,
			width,
			height,
			m_imageInfo->hMemDC,
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
			m_imageInfo->frameWidth,
			m_imageInfo->frameHeight,
			transColor);
		// 3. ���� DC�� ����ؾߵ� DC�� �׸���.
		AlphaBlend(
			hdc,
			destX,
			destY,
			width,
			height,
			tempDC,
			0,
			0,
			width,
			height,
			blendFunc);

		SelectObject(tempDC, temOldBitmap);
		DeleteObject(tempBitmap);
	}
	else {
		AlphaBlend(
			hdc, destX, destY, width, height,
			m_imageInfo->hMemDC, 
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth, 
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
			m_imageInfo->frameWidth,
			m_imageInfo->frameHeight,
			blendFunc);
	}
}
