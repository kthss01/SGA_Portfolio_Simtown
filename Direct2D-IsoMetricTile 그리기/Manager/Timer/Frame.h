#pragma once

// fps
/*
�帣�� ���Ҷ��� 1��Ī ���� ���� (�ѽο� ������)

frame pre second 1�ʴ� �� �������� ����������
���ӿ� ���� ����
�ּ� ���� 30 fps
�ܼ� ���� ��� �׷��� ���� ���̰� �ּ� ������ ���߰� ��
��� ������ 60 fps �Ϲ������� �ν��ϴ� �ִ� �������� 60fps

30 fps
1�� ���� ���� �������� 30�� ���°�
60 �̻� 120 240�� ���°� ���� ���µ� ��ǻ�� ���ɸ� ���� ��ƸԴ°�

animation -> frame ������ ����
�� ������ 1�� ���� �������� ������ ����
*/

class Frame
{
private:
	// �Ϲ� Ÿ�̸� 1/1000 �� ���� Ÿ�̸� 1/�鸸
	// ��ǻ�ͳ� ��Ʈ�� cpu�� ����Ǿ����� ���� ���� ���� �����ִ°� ����
	// ���� Ÿ�̸� ���ٰ� ������ ������ �ƴ� cpu �δ��� �ñ� ������

	// ����� ���� �༮
	bool _isHardWare;			// ���� Ÿ�̸Ӹ� ���� �ϴ���?
								// �� ������ �� �ʴ����� ��� 
	float _timeScale;			// �ð� ����� 
								// gettickCount() 1/1000 �� ���� 1/�鸸 ��
	float _timeElapsed;			// �� �����Ӵ� �����
								// __int64 �⺻ 2^32 �ε� 2^64�� �ٲ۰� long�� ����
	__int64 _curTime;			// ���� �ð�
	__int64 _lastTime;			// ������ �ð�
								// getTickcount�� 1000 �����̸� �鸸
	__int64 _periodFrequency;	// �ð� �ֱ�

								// timeElapsed = curTime - lastTime;

								// ������ �� �༮
	unsigned long _frameRate; // fps
							  // �� ���� fps�� �Ѱ��ٲ�
	unsigned long _FPSFrameCount; // ������ ������ ������ ������ �۾����� count++; 
	float _FPSTimeElapsed;
	float _worldTime;
public:
	Frame();
	~Frame();

	HRESULT Init();

	// fps ������ �ɾ��� �༮
	// �갡 �յ��ϰ� �������� ����
	// ���� Ǯ�� ������ 0���� �ƴ� ��� �����ϰ� ���� ������ ���ڷ�
	void Tick(float lockFPS = 0.0f);

	// �Լ��� �ڿ� const ���ΰ�
	// ���� �������� ����, 
	// �ʱ�ȭ�ؼ� (new �ؼ�) �ٷ� �־��ִ� ��� ���� 
	// ���� �ȵǴ� ��찡 �ִµ� const�� �ϸ� �ߵ�
	// FPS
	unsigned long GetFrameRate(char* str = NULL) const;
	// �� �����Ӵ� ��� �ð�
	// 2D������ ������µ� 3D������ �����ִ°� ����
	float GetElapsedTime() const { return _timeElapsed; }
	// ��ü ��� �ð� (â�� Ű�� �� �ĺ��� ����ؼ� �����Ǵ� �ð�)
	float GetWorldTime() const { return _worldTime; }
};

