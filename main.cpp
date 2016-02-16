#include <Shlobj.h>
#include "xui_family_create_win.h"
#include "xui_timermgr.h"
#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_render_window.h"
#include "xui_global.h"
#include "xui_demo.h"

HWND gHWND = NULL;

u08 VKToKey(WPARAM wParam)
{
	switch (wParam)
	{
	case 'A':				return KEY_A;
	case 'B':				return KEY_B;
	case 'C':				return KEY_C;
	case 'D':				return KEY_D;
	case 'E':				return KEY_E;
	case 'F':				return KEY_F;
	case 'G':				return KEY_G;
	case 'H':				return KEY_H;
	case 'I':				return KEY_I;
	case 'J':				return KEY_J;
	case 'K':				return KEY_K;
	case 'L':				return KEY_L;
	case 'M':				return KEY_M;
	case 'N':				return KEY_N;
	case 'O':				return KEY_O;
	case 'P':				return KEY_P;
	case 'Q':				return KEY_Q;
	case 'R':				return KEY_R;
	case 'S':				return KEY_S;
	case 'T':				return KEY_T;
	case 'U':				return KEY_U;
	case 'V':				return KEY_V;
	case 'W':				return KEY_W;
	case 'X':				return KEY_X;
	case 'Y':				return KEY_Y;
	case 'Z':				return KEY_Z;
	case VK_RETURN:			return KEY_ENTER;
	case VK_TAB:			return KEY_TAB;
	case VK_BACK:			return KEY_BACK;
	case VK_SHIFT:			return KEY_SHIFT;
	case VK_UP:				return KEY_UARROW;
	case VK_DOWN:			return KEY_DARROW;
	case VK_LEFT:			return KEY_LARROW;
	case VK_RIGHT:			return KEY_RARROW;
	case VK_HOME:			return KEY_HOME;
	case VK_END:			return KEY_END;
	case VK_F1:				return KEY_F1;
	case VK_F2:				return KEY_F2;
	case VK_F3:				return KEY_F3;
	case VK_F4:				return KEY_F4;
	case VK_F5:				return KEY_F5;
	case VK_F6:				return KEY_F6;
	case VK_F7:				return KEY_F7;
	case VK_F8:				return KEY_F8;
	case VK_F9:				return KEY_F9;
	default:				return KEY_NONE;
	}
}

#define WM_USER_FWATCHNOTIFY WM_USER+0x1000
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_USER_FWATCHNOTIFY:
		{
			long lEvent;
			PIDLIST_ABSOLUTE *rgpidl;
			HANDLE hNotifyLock = SHChangeNotification_Lock((HANDLE)wParam, (DWORD)lParam, &rgpidl, &lEvent);
			if (hNotifyLock)
			{
				std::wstring path;
				std::wstring misc;

				wchar_t buffer[MAX_PATH];
				if (rgpidl[0])
				{
					SHGetPathFromIDList(rgpidl[0], buffer);
					path = buffer;
				}
				if (rgpidl[1])
				{
					SHGetPathFromIDList(rgpidl[1], buffer);
					misc = buffer;
				}

				if (path.length() > 0)
					xui_global::add_fwatch(path, misc);
			}
			SHChangeNotification_Unlock(hNotifyLock);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			xui_method_mouse args;
			args.wheel = (s32)(s16)HIWORD(wParam);
			xui_desktop::get_ins()->os_mousewheel(args);
		}
		break;
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
		{
			xui_method_mouse args;
			args.point = xui_vector<s32>((s32)LOWORD(lParam), (s32)HIWORD(lParam));
			args.ctrl  = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
			args.shift = (GetKeyState(VK_SHIFT)   & 0x8000) != 0;
			args.alt   = (GetKeyState(VK_MENU)    & 0x8000) != 0;

			switch (message)
			{
			case WM_LBUTTONDBLCLK:	args.mouse = MB_L;	break;
			case WM_RBUTTONDBLCLK:	args.mouse = MB_R;	break;
			case WM_MBUTTONDBLCLK:	args.mouse = MB_M;	break;
			}

			xui_desktop::get_ins()->os_mousedclick(args);
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			//::SetCapture(hWnd);

			xui_method_mouse args;
			args.point = xui_vector<s32>((s32)LOWORD(lParam), (s32)HIWORD(lParam));
			args.ctrl  = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
			args.shift = (GetKeyState(VK_SHIFT)   & 0x8000) != 0;
			args.alt   = (GetKeyState(VK_MENU)    & 0x8000) != 0;
			
			switch (message)
			{
			case WM_LBUTTONDOWN:	args.mouse = MB_L;	break;
			case WM_RBUTTONDOWN:	args.mouse = MB_R;	break;
			case WM_MBUTTONDOWN:	args.mouse = MB_M;	break;
			}
			
			xui_desktop::get_ins()->os_mousedown(args);
		}
		break;
	case WM_MOUSEMOVE:
		{
			xui_method_mouse args;
			args.point = xui_vector<s32>((s32)LOWORD(lParam), (s32)HIWORD(lParam));
			xui_desktop::get_ins()->os_mousemove(args);
		}
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		{
			xui_method_mouse args;
			args.point = xui_vector<s32>((s32)LOWORD(lParam), (s32)HIWORD(lParam));
			args.ctrl  = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
			args.shift = (GetKeyState(VK_SHIFT)   & 0x8000) != 0;
			args.alt   = (GetKeyState(VK_MENU)    & 0x8000) != 0;

			switch (message)
			{
			case WM_LBUTTONUP:		args.mouse = MB_L;	break;
			case WM_RBUTTONUP:		args.mouse = MB_R;	break;
			case WM_MBUTTONUP:		args.mouse = MB_M;	break;
			}

			xui_desktop::get_ins()->os_mouserise(args);
		}
		break;
	case WM_KEYDOWN:
		{
			xui_method_keybd args;
			args.ctrl  = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
			args.shift = (GetKeyState(VK_SHIFT)   & 0x8000) != 0;
			args.alt   = (GetKeyState(VK_MENU)    & 0x8000) != 0;
			args.kcode = VKToKey(wParam);
			xui_desktop::get_ins()->os_keybddown(args);
		}
		break;
	case WM_KEYUP:
		{
			xui_method_keybd args;
			args.ctrl  = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
			args.shift = (GetKeyState(VK_SHIFT)   & 0x8000) != 0;
			args.alt   = (GetKeyState(VK_MENU)    & 0x8000) != 0;
			args.kcode = VKToKey(wParam);
			xui_desktop::get_ins()->os_keybdrise(args);
		}
		break;
	case WM_CHAR:
		{
			xui_desktop::get_ins()->os_keybdchar((u16)wParam);
		}
		break;
	case WM_CLOSE:
		{
			PostQuitMessage(0);
		}
		break;
	case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			int w = rect.right-rect.left;
			int h = rect.bottom-rect.top;
			if (xui_convas::get_ins())
			{
				xui_method_inscall(xui_convas,  set_viewport)(xui_rect2d<s32>(0, 0, w, h));
				xui_method_inscall(xui_desktop, set_rendersz)(xui_vector<s32>(w, h));
			}
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int CALLBACK WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS));

	//create console 
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance		= hInstance;
	wc.lpfnWndProc		= WndProc;
	wc.lpszClassName	= L"XUI";
	wc.lpszMenuName		= NULL;
	wc.style			= CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_DBLCLKS;

	if (!RegisterClass(&wc))
		return 0;

	gHWND = CreateWindow(L"XUI", L"XUI", WS_OVERLAPPEDWINDOW , 0, 0, 1024, 768, NULL, NULL, hInstance, NULL);
	//SetCapture   (gHWND);
	ShowWindow   (gHWND, SW_NORMAL);
	UpdateWindow (gHWND);

	xui_render_window* render_window = new xui_render_window(gHWND);
	g_family_create = new xui_family_create_win();
	xui_static_inscall(xui_timermgr,	init)();
	xui_static_inscall(xui_desktop,		init)();
	xui_static_inscall(xui_convas,		init)();

	RECT rect;
	GetClientRect(gHWND, &rect);
	MoveWindow   (gHWND, 0, 0, 2*1024-rect.right+rect.left, 2*768-rect.bottom+rect.top, TRUE);

	xui_window* window = new xui_window(xui_vector<s32>(500, 500));
	window->ini_component(0, 0, DOCKSTYLE_F);
	window->set_borderrt(xui_rect2d<s32>(8));
	//xui_demo::test_button	(window);
	//xui_demo::test_toggle	(window);
	//xui_demo::test_textbox	(window);
	//xui_demo::test_linebox	(window);
	//xui_demo::test_gridbox	(window);
	//xui_demo::test_toolbar	(window);
	//xui_demo::test_scroll	(window);
	//xui_demo::test_slider	(window);
	//xui_demo::test_listview	(window);
	//xui_demo::test_dropbox	(window);
	//xui_demo::test_treeview	(window);
	//xui_demo::test_timeview	(window);
	//xui_demo::test_propview	(window);
	//xui_demo::test_menu		(window);
	xui_demo::test_dockview(window);
	xui_desktop::get_ins()->add_child(window);
	xui_global::set_fwatchstart(xui_global::get_workpath());

	MSG msg;
	memset(&msg, 0, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage (&msg);
		}
		else
		{
			int time  = timeGetTime();

			xui_method_inscall(xui_convas,		clear	)(xui_colour(1.0f, 0.1f, 0.1f, 0.1f));
			xui_method_inscall(xui_convas,		begin	)();
			xui_method_inscall(xui_timermgr,	update	)(0.016f);
			xui_method_inscall(xui_desktop,		update	)(0.016f);
			xui_method_inscall(xui_desktop,		render	)();
			render_window->present();

			int delta = timeGetTime() - time;
			if (delta < 16)
				Sleep(16-delta);
		}
	}

	return 0;
}
