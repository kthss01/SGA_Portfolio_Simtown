#pragma once

namespace MY_UTIL {
	float GetAngle(float startX, float startY, float endX, float endY);

	float GetDistance(float startX, float startY, float endX, float endY);

	bool IntersectLine(POINT FLineP1, POINT FLineP2,
		POINT SLineP1, POINT SLineP2);

	// Ray ������ - ���������� �ִ� ���� (���� ������ �����°�)
	// �������� ���ΰ� �ٸ� ���� ����� ���� �����ִ°� �ƴ� 
	// �����Ҷ��� ��� (����׿뵵�� ���)
	typedef struct tagRay {
		POINT origin;		// ���� ��ġ
		POINT direction;	// ����

		tagRay() {
			origin = { 0, 0 };
			direction = { 0, 0 };
		}
		tagRay(POINT origin, POINT direction) {
			this->origin = origin;
			this->direction = direction;
		}
		tagRay(int originX, int originY, int directionX, int directionY) {
			origin = { originX, originY };
			direction = { directionX, directionY };
		}

		void DrawRay(HDC hdc);
		bool CollisionRect(RECT rc);
	} Ray, *LPRay;
}