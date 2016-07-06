#include <Shlobj.h>
#include "m3eDebug.h"
#include "NPFile.h"

#include "xui_timermgr.h"
#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_render_window.h"
#include "xui_global.h"
#include "xui_syswnd.h"
#include "onity_resource.h"
#include "onity_mainform.h"
#include "onity_console.h"

s32			DefaultWidth	= 1440;
s32			DefaultHeight	=  900;
HWND		gHWND			= NULL;
HINSTANCE	gHINSTANCE		= NULL;

#define WM_USER_FWATCHNOTIFY WM_USER+0x1000
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	if (xui_global::def_deviceproc(hwnd, message, wparam, lparam))
		return 0;

	switch (message)
	{
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
	wc.lpszClassName	= L"Onity";
	wc.lpszMenuName		= NULL;
	wc.style			= CS_VREDRAW | CS_HREDRAW | CS_OWNDC;

	if (!RegisterClass(&wc))
		return 0;

	gHWND = CreateWindow(L"Onity", L"Onity", WS_OVERLAPPEDWINDOW , 0, 0, DefaultWidth, DefaultHeight, NULL, NULL, hInstance, NULL);
	ShowWindow   (gHWND, SW_NORMAL);
	UpdateWindow (gHWND);

	std::string font = "Arial.TTF";
	NPFile font_config;
	if (font_config.Open("onity.font", NPFile::OM_READ))
	{
		font_config.ReadLine(font);
		font_config.Close();
	}

	xui_global::add_fontfile(font);
	xui_render_window::init(gHWND);
	xui_render_window* render_window = new xui_render_window(gHWND);
	xui_static_inscall(xui_convas,		init)();
	xui_static_inscall(xui_timermgr,	init)();
	xui_static_inscall(xui_desktop,		init)();
	xui_static_inscall(onity_resource,	init)();

	RECT rect;
	GetClientRect(gHWND, &rect);
	MoveWindow   (gHWND, 0, 0, 2*DefaultWidth-rect.right+rect.left, 2*DefaultHeight-rect.bottom+rect.top, TRUE);
	xui_desktop::get_ins()->add_child(new onity_mainform());
	xui_desktop::get_ins()->update(0.0f);

	extern user_print s_userprint;
	s_userprint = onity_console::game_print;

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
