#include "stdafx.h"
#include "Collision.h"


Collision::Collision()
{
}


Collision::~Collision()
{
}

bool Collision::IntersectTri(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 mousePos)
{
	Vector2 cross;	// ������ ����ϴ� ���� vector

	float dotA, dotB;	// ���� ����

	Vector2 temp[3];	// ����� �༮, ���� ������

	temp[0].x = p2.x - p0.x;
	temp[0].y = p2.y - p0.y;

	temp[1].x = p1.x - p0.x;
	temp[1].y = p1.y - p0.y;

	temp[2].x = mousePos.x - p0.x;
	temp[2].y = mousePos.y - p0.y;

	// 2D���� ����ϴ� ���� 
	// temp[0]�� ������ ���Ͱ� ������� (�븻����, ��������)
	cross.x = -temp[0].y;
	cross.y = temp[0].x;

	// dot < 0 �а��̶�� 180���� ����
	// dot�� ���� 90���� ������ > 0 180���� ũ�� ����
	dotA = Vector2::Dot(cross, temp[1]);
	dotB = Vector2::Dot(cross, temp[2]);

	// �� ����� ��� �ﰢ�� ������ �����Ե�
	// ��� ���� ��� �� ���ָ� �ǰ�
	if (dotA * dotB < 0)
		return false;

	temp[0].x = p0.x - p1.x;
	temp[0].y = p0.y - p1.y;

	temp[1].x = p2.x - p1.x;
	temp[1].y = p2.y - p1.y;

	temp[2].x = mousePos.x - p1.x;
	temp[2].y = mousePos.y - p1.y;

	// 2D���� ����ϴ� ���� 
	// temp[0]�� ������ ���Ͱ� ������� (�븻����, ��������)
	cross.x = -temp[0].y;
	cross.y = temp[0].x;

	// dot < 0 �а��̶�� 180���� ����
	// dot�� ���� 90���� ������ > 0 180���� ũ�� ����
	dotA = Vector2::Dot(cross, temp[1]);
	dotB = Vector2::Dot(cross, temp[2]);

	// �� ����� ��� �ﰢ�� ������ �����Ե�
	// ��� ���� ��� �� ���ָ� �ǰ�
	if (dotA * dotB < 0)
		return false;

	temp[0].x = p1.x - p2.x;
	temp[0].y = p1.y - p2.y;

	temp[1].x = p0.x - p2.x;
	temp[1].y = p0.y - p2.y;

	temp[2].x = mousePos.x - p2.x;
	temp[2].y = mousePos.y - p2.y;

	// 2D���� ����ϴ� ���� 
	// temp[0]�� ������ ���Ͱ� ������� (�븻����, ��������)
	cross.x = -temp[0].y;
	cross.y = temp[0].x;

	// dot < 0 �а��̶�� 180���� ����
	// dot�� ���� 90���� ������ > 0 180���� ũ�� ����
	dotA = Vector2::Dot(cross, temp[1]);
	dotB = Vector2::Dot(cross, temp[2]);

	// �� ����� ��� �ﰢ�� ������ �����Ե�
	// ��� ���� ��� �� ���ָ� �ǰ�
	if (dotA * dotB < 0)
		return false;

	return true;
}

bool Collision::IntersectRayToLine(Ray ray, Vector2 start, Vector2 end, Vector2 * outPos, Vector2 * outNormal)
{
	Vector2 normal;
	normal.x = -(end.y - start.y);
	normal.y = (end.x - start.x);

	// ������ �� ���� �ְ�
	// ������ �����ε� �� �� ����
	// normal �� ���������� �ִ� �Ÿ� ���Ѱ�
	float dist = -Vector2::Dot(normal, end);
	float t1 = -Vector2::Dot(normal, ray.origin);
	float t2 = Vector2::Dot(normal, ray.direction);
	float t = (t1 - dist) / t2;

	// ������ ������ ��Ȳ�� �� (����, ���� �� �� ����)
	if (t >= 0.0f && t <= 1.0f) {
		if (outPos != NULL) {
			*outPos = ray.origin + ray.direction * t;

			if (outNormal != NULL) {
				*outNormal = normal * -1;
			}
		}
		return true;
	}
	return false;
}
