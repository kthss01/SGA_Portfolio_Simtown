// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once


#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

#include <string>
#include <vector>

using namespace std;
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "./Math/Matrix.h"
#include "./Math/Vector2.h"

#include "./System/D2D.h"

using namespace MyDirectX;

#include "./Utilities/Util.h"

#include "./Physics/Collision.h"

#include "./Manager/Timer/TimeManager.h"
#include "./Manager/KeyManager.h"

#define EPSILON 0.0001f

#define SAFE_RELEASE(p){ if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p){ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] (p); (p) = NULL; } }
// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

// 창 이름
static wstring appName = L"Direct2D";

#define WINSIZE_X 1024
#define WINSIZE_Y 764