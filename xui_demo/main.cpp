#include <Shlobj.h>
#include "xui_timermgr.h"
#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_render_window.h"
#include "xui_global.h"
#include "xui_button.h"
#include "xui_syswnd.h"
#include "xui_demo.h"

HINSTANCE	gHINSTANCE	= NULL;
HWND		gHWND		= NULL;

#define WM_USER_FWATCHNOTIFY WM_USER+0x1000
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	if (xui_global::def_deviceproc(hwnd, message, wparam, lparam))
		return 0;

	switch (message)
	{
	case WM_KILLFOCUS:
		{
			if (xui_global::was_scolorstart())
			{
				xui_desktop::get_ins()->set_focusctrl(NULL);
				::SetFocus(hwnd);
			}
		}
		break;
	case WM_USER_FWATCHNOTIFY:
		{
			long eventid;
			PIDLIST_ABSOLUTE *rgpidl;
			HANDLE notify_lock = SHChangeNotification_Lock((HANDLE)wparam, (DWORD)lparam, &rgpidl, &eventid);
			if (notify_lock)
			{
				std::wstring srcpath;
				std::wstring dstpath;

				wchar_t buffer[MAX_PATH];
				if (rgpidl[0])
				{
					SHGetPathFromIDList(rgpidl[0], buffer);
					srcpath = buffer;
				}
				if (rgpidl[1])
				{
					SHGetPathFromIDList(rgpidl[1], buffer);
					dstpath = buffer;
				}

				if (eventid == SHCNE_UPDATEITEM && srcpath.length() > 0)
				{
					xui_global::add_fwatch(eventid, srcpath, dstpath);
				}
			}
			SHChangeNotification_Unlock(notify_lock);
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
			GetClientRect(hwnd, &rect);
			int w = rect.right-rect.left;
			int h = rect.bottom-rect.top;
			if (xui_desktop::get_ins() && w > 0 && h > 0)
				xui_desktop::get_ins()->set_rendersz(xui_vector<s32>(w, h));
		}
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}

	return 0;
}

int CALLBACK WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	gHINSTANCE = hInstance;

	WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS));
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance		= hInstance;
	wc.lpfnWndProc		= WndProc;
	wc.lpszClassName	= L"Demo";
	wc.lpszMenuName		= NULL;
	wc.style			= CS_VREDRAW | CS_HREDRAW | CS_OWNDC;

	if (!RegisterClass(&wc))
		return 0;

	gHWND = CreateWindow(L"Demo", L"Demo", WS_OVERLAPPEDWINDOW , 100, 100, 1024, 768, NULL, NULL, hInstance, NULL);
	ShowWindow   (gHWND, SW_NORMAL);
	UpdateWindow (gHWND);

	xui_global::add_fontfile("arial.ttf");
	xui_render_window::init(gHWND);
	xui_render_window* render_window = new xui_render_window(gHWND);
	xui_static_inscall(xui_convas,			init)();
	xui_static_inscall(xui_timermgr,		init)();
	xui_static_inscall(xui_desktop,			init)();

	RECT rect;
	GetClientRect(gHWND, &rect);
	MoveWindow   (gHWND, 100, 100, 2*1024-rect.right+rect.left, 2*768-rect.bottom+rect.top, TRUE);

	xui_window* window = new xui_window(xui_vector<s32>(500, 500), false);
	window->ini_component(0, 0, DOCKSTYLE_F);
	window->set_borderrt(xui_rect2d<s32>(8));
	xui_demo::test_button	(window);
	xui_demo::test_toggle	(window);
	xui_demo::test_textbox	(window);
	xui_demo::test_linebox	(window);
	xui_demo::test_gridbox	(window);
	xui_demo::test_toolbar	(window);
	xui_demo::test_scroll	(window);
	xui_demo::test_slider	(window);
	xui_demo::test_listview	(window);
	xui_demo::test_dropbox	(window);
	xui_demo::test_treeview	(window);
	xui_demo::test_timeview	(window);
	xui_demo::test_propview	(window);
	xui_demo::test_menu		(window);
	//xui_demo::test_dockview(window);
	xui_desktop::get_ins()->add_child(window);
	xui_global::set_fwatchstart(xui_global::get_workpath());

	xui_window* dlg1 = xui_desktop::get_ins()->show_message(L"111", 1);
	xui_window* dlg2 = xui_desktop::get_ins()->show_message(L"222", 2);
	//xui_global::add_syswnd(L"Test1", window, true);
	//xui_global::add_syswnd(L"Test2", window, true);

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
			static int total_time = 0;
			static int delta_time = 0;

			total_time = timeGetTime();

			render_window->current();
			xui_method_inscall(xui_timermgr,	update	)(delta_time/1000.0f);
			xui_method_inscall(xui_desktop,		update	)(delta_time/1000.0f);
			xui_method_inscall(xui_desktop,		render	)();
			render_window->present();

			std::vector<xui_syswnd*> vec = xui_global::get_syswndall();
			for (u32 i = 0; i < vec.size(); ++i)
			{
				xui_syswnd* wnd = vec[i];
				wnd->update(delta_time/1000.0f);
				wnd->render();
			}

			delta_time = timeGetTime() - total_time;
			if (delta_time < 16)
			{
				Sleep(16-delta_time);
				delta_time = 16;
			}
		}
	}

	return 0;
}
