#include <Shlobj.h>

#include "xui_timermgr.h"
#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_render_window.h"
#include "xui_global.h"
#include "xui_syswnd.h"
#include "xui_window.h"
#include "cocos_resource.h"
#include "cocos_mainform.h"
#include "cocos_console.h"

HWND			gHWND		= NULL;
HINSTANCE		gHINSTANCE	= NULL;
int 			WNDPOSX		= 0;
int				WNDPOSY		= 0;
int				WNDWIDTH	= 1600;
int				WNDHEIGHT	= 900;
std::wstring	TOOLPATH;

extern bool def_deviceproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );

#define WM_USER_FWATCHNOTIFY WM_USER+0x1000
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	if (def_deviceproc(hwnd, message, wparam, lparam))
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
			cocos_mainform::get_ptr()->set_saveshow();
		}
		break;
	case WM_MOVE:
		{
			int movex = (int)(short)LOWORD(lparam) - WNDPOSX;
			int movey = (int)(short)HIWORD(lparam) - WNDPOSY;
			std::vector<xui_syswnd*> vec = xui_global::get_syswndall();
			for (u32 i = 0; i < vec.size(); ++i)
			{
				xui_syswnd* syswnd = vec[i];
				xui_window* wnd = syswnd->get_popupctrl();
				xui_method_ptrcall(wnd, set_owner	)(NULL);
				xui_method_ptrcall(wnd, set_renderx	)(wnd->get_renderx()-movex);
				xui_method_ptrcall(wnd, set_rendery	)(wnd->get_rendery()-movey);
				xui_method_ptrcall(wnd, set_owner	)(syswnd);
			}

			WNDPOSX   = (int)(short)LOWORD(lparam);
			WNDPOSY   = (int)(short)HIWORD(lparam);
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

			WNDWIDTH  = (int)(short)LOWORD(lparam);
			WNDHEIGHT = (int)(short)HIWORD(lparam);
		}
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}

	return 0;
}

#include <windows.h>
#include <DbgHelp.h>
#include <stdlib.h>
#pragma comment(lib, "dbghelp.lib")

inline BOOL IsDataSectionNeeded(const WCHAR* pModuleName)
{
	if(pModuleName == 0)
	{
		return FALSE;
	}

	WCHAR szFileName[_MAX_FNAME] = L"";
	_wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);

	if(_wcsicmp(szFileName, L"ntdll") == 0)
		return TRUE;

	return FALSE;
}

BOOL CALLBACK MiniDumpCallback(PVOID pParam, const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput)
{
	if(pInput == 0 || pOutput == 0)
		return FALSE;

	switch(pInput->CallbackType)
	{
	case ModuleCallback:
		if(pOutput->ModuleWriteFlags & ModuleWriteDataSeg)
			if(!IsDataSectionNeeded(pInput->Module.FullPath))
				pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
	case IncludeModuleCallback:
	case IncludeThreadCallback:
	case ThreadCallback:
	case ThreadExCallback:
		return TRUE;
	default:;
	}

	return FALSE;
}

void CreateMiniDump(PEXCEPTION_POINTERS pep, LPCSTR strFileName)
{
	HANDLE hFile = CreateFileA(strFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
	{
		MINIDUMP_EXCEPTION_INFORMATION mdei;
		mdei.ThreadId           = GetCurrentThreadId();
		mdei.ExceptionPointers  = pep;
		mdei.ClientPointers     = NULL;

		MINIDUMP_CALLBACK_INFORMATION mci;
		mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;
		mci.CallbackParam       = 0;

		::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, (pep != 0) ? &mdei : 0, NULL, &mci);

		CloseHandle(hFile);
	}
}

LONG __stdcall MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	CreateMiniDump(pExceptionInfo, "cocos_editor/cocos_editor.crash");

	return EXCEPTION_EXECUTE_HANDLER;
}

// 此函数一旦成功调用，之后对 SetUnhandledExceptionFilter 的调用将无效
void DisableSetUnhandledExceptionFilter()
{
	void* addr = (void*)GetProcAddress(LoadLibraryA("kernel32.dll"),
		"SetUnhandledExceptionFilter");

	if (addr)
	{
		unsigned char code[16];
		int size = 0;

		code[size++] = 0x33;
		code[size++] = 0xC0;
		code[size++] = 0xC2;
		code[size++] = 0x04;
		code[size++] = 0x00;

		DWORD dwOldFlag, dwTempFlag;
		VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
		WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
		VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
	}
}

void InitMinDump()
{
	//注册异常处理函数
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	//使SetUnhandledExceptionFilter
	//DisableSetUnhandledExceptionFilter();
}

int CALLBACK WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	InitMinDump();

	gHINSTANCE = hInstance;

	WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS));
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance		= hInstance;
	wc.lpfnWndProc		= WndProc;
	wc.lpszClassName	= L"cocos editor";
	wc.lpszMenuName		= NULL;
	wc.style			= CS_VREDRAW | CS_HREDRAW | CS_OWNDC;

	if (!RegisterClass(&wc))
		return 0;

	FILE* file = NULL;
	file = fopen("cocos_editor.dock", "r");
	if (file)
	{
		std::string line = xui_global::get_fileline(file);
		sscanf(line.c_str(), "%d,%d,%d,%d", &WNDPOSX, &WNDPOSY, &WNDWIDTH, &WNDHEIGHT);
		fclose(file);
		file = NULL;
	}

	gHWND = CreateWindow(L"cocos editor", L"cocos editor", WS_OVERLAPPEDWINDOW, WNDPOSX, WNDPOSY, WNDWIDTH, WNDHEIGHT, NULL, NULL, hInstance, NULL);
	ShowWindow   (gHWND, SW_NORMAL);
	UpdateWindow (gHWND);

	std::string font = "Arial.TTF";
	file = fopen("cocos_editor.font", "r");
	if (file)
	{
		font = xui_global::get_fileline(file);
		fclose(file);
		file = NULL;
	}

	TOOLPATH = xui_global::get_workpath();

	xui_global::add_fontfile(font);
	xui_render_window::init(gHWND);
	xui_render_window* render_window = new xui_render_window(gHWND);
	xui_static_inscall(xui_convas,		init)();
	xui_static_inscall(xui_timermgr,	init)();
	xui_static_inscall(xui_desktop,		init)();
	xui_static_inscall(cocos_resource,	init)();
	RECT rect;
	GetClientRect(gHWND, &rect);
	int w = rect.right-rect.left;
	int h = rect.bottom-rect.top;
	xui_method_inscall(xui_desktop,		set_rendersz)(xui_vector<s32>(w, h));
	xui_method_inscall(xui_desktop,		add_child	)(new cocos_mainform());
	xui_method_inscall(xui_desktop,		update		)(0.0f);

	//extern user_print s_userprint;
	//s_userprint = cocos_console::editor_print;

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
				wnd->get_renderwnd()->current();
				wnd->get_popupctrl()->render ();
				wnd->get_renderwnd()->present();
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
