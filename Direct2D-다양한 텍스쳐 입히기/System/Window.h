#pragma once
class Program;

// â�� ����� ���Ҹ� �Ҳ�

class Window
{
public:
	Window();
	~Window();

	WPARAM Run();

	float color[4];
private:
	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	static Program* program;

	bool bFullScreen = false;
};
