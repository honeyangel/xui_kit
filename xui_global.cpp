#include "xui_desktop.h"
#include "xui_render_window.h"
#include "xui_window.h"
#include "xui_syswnd.h"
#include "xui_global.h"

/*
//icon
*/
xui_bitmap* xui_global::icon_pickcolour = NULL;
xui_bitmap* xui_global::icon_pickselect = NULL;
xui_bitmap* xui_global::icon_loop		= NULL;

/*
//global
*/
bool def_deviceproc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );

/*
//string
*/
xui_method_explain(xui_global, get_upper,		std::wstring					)( const std::wstring& src )
{
	std::wstring result;
	for (u32 i = 0; i < src.length(); ++i)
		result.push_back((src[i] >= L'a' && src[i] <= L'z') ? (src[i]-32) : src[i]);

	return result;
}
xui_method_explain(xui_global, get_split,		std::vector<std::wstring>		)( const std::wstring& src, wchar_t key )
{
	std::vector<std::wstring> result;

	std::wstring temp(src);
	while(true)
	{
		s32 pos = temp.find(key);
		if (pos == -1)
			break;

		result.push_back(temp.substr(0, pos));
		temp.erase(0, pos+1);
	}

	if (!temp.empty())
		result.push_back(temp);

	return result;
}
xui_method_explain(xui_global, unicode_to_utf8, std::string						)( const std::wstring& src )
{
	std::string result;

	for (u32 i = 0; i < src.length(); ++i)
	{
		if (src[i] < 0x80)
		{
			result += (char)src[i];
		}
		else 
		if (src[i] < 0x800)
		{
			result += (char)(0xC0 |  (src[i] >> 6));
			result += (char)(0x80 |  (src[i]        & 0x3F));
		}
		else 
		if (src[i] < 0x10000)
		{
			result += (char)(0xE0 |  (src[i] >> 12));
			result += (char)(0x80 | ((src[i] >> 6 ) & 0x3F));
			result += (char)(0x80 |  (src[i]        & 0x3F));
		}
	}

	return result;
}
xui_method_explain(xui_global, utf8_to_unicode, std::wstring					)( const std::string&  src )
{
	std::wstring result;

	for (u32 i = 0; i < src.length(); )
	{
		wchar_t w;

		if ((src[i] & 0x80) == 0)							// U-00000000 - U-0000007F : 0xxxxxxx 
		{              
			w = (wchar_t)(src[i]);
			i += 1;
		}
		else 
		if ((src[i] & 0xe0) == 0xc0 && i+1 < src.length())	// U-00000080 - U-000007FF : 110xxxxx 10xxxxxx 
		{		  
			w  = (wchar_t)(src[i+0] & 0x3f) << 6;
			w |= (wchar_t)(src[i+1] & 0x3f);
			i += 2;
		}
		else 
		if ((src[i] & 0xf0) == 0xe0 && i+2 < src.length())	// U-00000800 - U-0000FFFF : 1110xxxx 10xxxxxx 10xxxxxx 
		{	
			w  = (wchar_t)(src[i+0] & 0x1f) << 12;
			w |= (wchar_t)(src[i+1] & 0x3f) << 6;
			w |= (wchar_t)(src[i+2] & 0x3f);
			i += 3;
		}
		else 
		if ((src[i] & 0xf8) == 0xf0)						// U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx , this is not Chinese
		{		   
			w  = 0x20;
			i += 4;
		}
		else 
		if ((src[i] & 0xfc) == 0xf8)						// U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx , this is not Chinese
		{		   
			w  = 0x20;
			i += 5;
		}
		else												// U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx , this is not Chinese
		{					   
			w  = 0x20;
			i += 6;
		}

		//add
		result += w;
	}

	return result;
}
xui_method_explain(xui_global, unicode_to_ascii,std::string						)( const std::wstring& src )
{
	char buffer[512];
	WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, buffer, 512, NULL, NULL);
	return std::string(buffer);
}
xui_method_explain(xui_global, ascii_to_unicode,std::wstring					)( const std::string&  src )
{
	wchar_t buffer[512];
	MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, buffer, 512);
	return std::wstring(buffer);
}

/*
//screen
*/
HWND	scolor_hwnd		= NULL;
CHAR*	screen_buffer	= NULL;
s32		screen_w		= 0;
s32		screen_h		= 0;
void capture_screen( void )
{
	HDC hdcScreen = ::GetDC(NULL);

	screen_w = GetSystemMetrics(SM_CXSCREEN);  // 屏幕宽
	screen_h = GetSystemMetrics(SM_CYSCREEN);  // 屏幕高
	HDC hdcMemory = CreateCompatibleDC(hdcScreen); // 创建兼容内存DC
	if (hdcMemory == NULL)
	{
		goto done;
	}

	// 通过窗口DC 创建一个兼容位图
	BITMAPINFO bi;
	bi.bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth			=  screen_w;
	bi.bmiHeader.biHeight			= -screen_h;
	bi.bmiHeader.biPlanes			= 1;
	bi.bmiHeader.biBitCount			= 24;
	bi.bmiHeader.biCompression		= BI_RGB;
	bi.bmiHeader.biSizeImage		= 0;
	bi.bmiHeader.biXPelsPerMeter	= 0;
	bi.bmiHeader.biYPelsPerMeter	= 0;
	bi.bmiHeader.biClrUsed			= 0;
	bi.bmiHeader.biClrImportant		= 0;

	char* buffer = NULL;
	HBITMAP hbmScreen = CreateDIBSection(hdcMemory, &bi, DIB_RGB_COLORS, (void**)(&buffer), 0, 0);
	if (hbmScreen == NULL)
	{
		goto done;
	}

	// 将位图块传送到我们兼容的内存DC中
	SelectObject(hdcMemory, hbmScreen);
	if (!BitBlt(
		hdcMemory,   // 目的DC
		0, 0,        // 目的DC的 x,y 坐标
		screen_w, screen_h, // 目的 DC 的宽高
		hdcScreen,   // 来源DC
		0, 0,        // 来源DC的 x,y 坐标
		SRCCOPY))    // 粘贴方式
	{
		goto done;
	}

	screen_buffer = new char[screen_w*screen_h*3];
	memcpy(screen_buffer, buffer, screen_w*screen_h*3);

	// 清理资源
done:
	DeleteObject(hbmScreen);
	DeleteObject(hdcMemory);
	ReleaseDC(NULL, hdcScreen);
}

xui_method_explain(xui_global, was_scolorstart, bool							)( void )
{
	return screen_buffer != NULL;
}
xui_method_explain(xui_global, set_scolorstart,	void							)( xui_syswnd* syswnd )
{
	if (screen_buffer == NULL)
	{
		if (syswnd)
		{
			scolor_hwnd = syswnd->get_renderwnd()->get_hwnd();
		}
		else
		{
			extern HWND gHWND;
			scolor_hwnd = gHWND;
		}

		::SetWindowPos(scolor_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		capture_screen();
	}
}

xui_colour pick_colour( s32 x, s32 y )
{
	u32 index = (y * screen_w + x) * 3;
	BYTE b = screen_buffer[index  ];
	BYTE g = screen_buffer[index+1];
	BYTE r = screen_buffer[index+2];
	return xui_colour(1.0f, r/255.0f, g/255.0f, b/255.0f);
}
xui_method_explain(xui_global, get_scolor,		xui_colour						)( void )
{
	if (screen_buffer)
	{
		POINT pt;
		GetCursorPos(&pt);
		return pick_colour(pt.x, pt.y);
	}

	return xui_colour::transparent;
}
xui_method_explain(xui_global, get_scolor,		std::vector<xui_colour>			)( const xui_vector<s32>& sz )
{
	std::vector<xui_colour> result;
	result.resize(sz.w*sz.h, xui_colour::black);
	if (screen_buffer)
	{
		POINT pt;
		GetCursorPos(&pt);
		s32 sc = pt.x - sz.w/2;
		s32 sr = pt.y - sz.h/2;
		for (s32 ir = 0; ir < sz.h; ++ir)
		{
			for (s32 ic = 0; ic < sz.w; ++ic)
			{
				s32 x = sc + ic;
				s32 y = sr + ir;
				u32 index = ir * sz.w + ic;
				result[index] = pick_colour(x, y);
			}
		}
	}

	return result;
}
xui_method_explain(xui_global, set_scolorclose,	void							)( void )
{
	if (screen_buffer)
	{
		if (scolor_hwnd)
			::SetWindowPos(scolor_hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

		//释放
		delete [] screen_buffer;
		screen_buffer = NULL;
		scolor_hwnd   = NULL;
	}
}

/*
//system
*/
std::map<HWND, xui_syswnd*> syswnd_map;
LRESULT CALLBACK xui_syswnd_proc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	if (def_deviceproc(hwnd, message, wparam, lparam))
		return 0;

	switch (message)
	{
	case WM_CLOSE:
		{
			xui_syswnd* syswnd = xui_global::get_syswnd(hwnd);
			if (syswnd)
			{
				xui_window* wnd = syswnd->get_popupctrl();
				if (wnd->was_modal())
					xui_desktop::get_ins()->del_modalwnd(wnd);

				std::map<HWND, xui_syswnd*>::iterator itor = syswnd_map.find(hwnd);
				if (itor != syswnd_map.end())
				{
					delete (*itor).second;
					syswnd_map.erase(itor);
				}

				wnd->set_visible(false);
			}

			DestroyWindow(hwnd);
		}
		break;
	case WM_SIZE:
		{
			xui_syswnd* syswnd = xui_global::get_syswnd(hwnd);
			if (syswnd)
			{
				extern HWND gHWND;
				RECT rect;
				GetWindowRect(gHWND, &rect);
				s32 title = GetSystemMetrics(SM_CYCAPTION);
				s32 frame = GetSystemMetrics(SM_CXSIZEFRAME);

				HWND hwnd = syswnd->get_renderwnd()->get_hwnd();
				RECT window;
				RECT client;
				GetWindowRect(hwnd, &window);
				GetClientRect(hwnd, &client);
				s32 edgex = (window.right-window.left) - (client.right-client.left);
				s32 edgey = (window.bottom-window.top) - (client.bottom-client.top);

				s32 x = (window.left + edgex/2) - (rect.left + frame);
				s32 y = (window.top  + edgey/2) - (rect.top  + title);
				s32 w =  client.right  - client.left;
				s32 h =  client.bottom - client.top ;
				xui_window* wnd = syswnd->get_popupctrl();
				xui_method_ptrcall(wnd, set_owner	)(NULL);
				xui_method_ptrcall(wnd, set_renderpt)(xui_vector<s32>(x, y));
				xui_method_ptrcall(wnd, set_rendersz)(xui_vector<s32>(w, h));
				xui_method_ptrcall(wnd, set_owner	)(syswnd);
			}
		}
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}

	return 0;
}

void reg_syswndclass( void )
{
	static bool has_register = false;
	if (has_register == false)
	{
		WNDCLASS wc;
		memset(&wc, 0, sizeof(WNDCLASS));

		extern HINSTANCE gHINSTANCE;
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
		wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
		wc.hInstance		= gHINSTANCE;
		wc.lpfnWndProc		= xui_syswnd_proc;
		wc.lpszClassName	= L"xui_syswnd";
		wc.lpszMenuName		= NULL;
		wc.style			= CS_VREDRAW | CS_HREDRAW | CS_OWNDC;

		if (RegisterClass(&wc))
			has_register = true;
	}
}
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
	case VK_ESCAPE:			return KEY_ESC;
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
	case VK_DELETE:			return KEY_DELETE;
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
xui_method_explain(xui_global, get_syswndmouse,	xui_vector<s32>					)( xui_syswnd* syswnd )
{
	POINT pt;
	GetCursorPos(&pt);
	if (syswnd)
	{
		ScreenToClient(syswnd->get_renderwnd()->get_hwnd(), &pt);
	}
	else
	{
		extern HWND gHWND;
		ScreenToClient(gHWND, &pt);
	}

	return xui_vector<s32>(pt.x, pt.y);
}
xui_method_explain(xui_global, set_syswndrect,	void							)( xui_syswnd* syswnd, const xui_rect2d<s32>& rt )
{
	xui_vector<s32> pt = xui_desktop::get_ins()->get_mousedown();
	xui_desktop::get_ins()->set_mouselast(pt);
	xui_desktop::get_ins()->set_mousecurr(pt);

	extern HWND gHWND;
	RECT rect;
	GetWindowRect(gHWND, &rect);
	s32 title = GetSystemMetrics(SM_CYCAPTION);
	s32 frame = GetSystemMetrics(SM_CXSIZEFRAME);

	HWND hwnd = syswnd->get_renderwnd()->get_hwnd();
	RECT window;
	RECT client;
	GetWindowRect(hwnd, &window);
	GetClientRect(hwnd, &client);
	s32 edgex = (window.right-window.left) - (client.right-client.left);
	s32 edgey = (window.bottom-window.top) - (client.bottom-client.top);

	s32 x = rect.left  + frame + rt.ax - edgex/2;
	s32 y = rect.top   + title + rt.ay - edgey/2;
	s32 w = rt.get_w() + edgex;
	s32 h = rt.get_h() + edgey;
	MoveWindow(hwnd, x, y, w, h, FALSE);
}
xui_method_explain(xui_global, get_syswndall,	std::vector<xui_syswnd*>		)( void )
{
	std::vector<xui_syswnd*> vec;
	for (std::map<HWND, xui_syswnd*>::iterator itor = syswnd_map.begin(); itor != syswnd_map.end(); ++itor)
	{
		vec.push_back((*itor).second);
	}

	return vec;
}
xui_method_explain(xui_global, get_syswnd,		xui_syswnd*						)( void* hwnd )
{
	std::map<HWND, xui_syswnd*>::iterator itor = syswnd_map.find((HWND)hwnd);
	if (itor != syswnd_map.end())
		return (*itor).second;

	return NULL;
}
xui_method_explain(xui_global, add_syswnd,		xui_syswnd*						)( xui_window* popupctrl, bool sizable )
{
	reg_syswndclass();

	DWORD style = WS_POPUP |
				  (sizable ? WS_THICKFRAME : WS_DLGFRAME);

	extern HINSTANCE gHINSTANCE;
	extern HWND      gHWND;
	HWND hwnd = CreateWindow(L"xui_syswnd", L"", style , 0, 0, 0, 0, gHWND, NULL, gHINSTANCE, NULL);
	xui_syswnd* syswnd = new xui_syswnd(hwnd, popupctrl);
	syswnd_map[hwnd] = syswnd;
	set_syswndrect(syswnd, popupctrl->get_renderrt() + popupctrl->get_renderpt());

	ShowWindow   (hwnd, SW_NORMAL);
	UpdateWindow (hwnd);

	return syswnd;
}
xui_method_explain(xui_global, del_syswnd,		void							)( xui_syswnd* syswnd )
{
	HWND hwnd = syswnd->get_renderwnd()->get_hwnd();
	if (hwnd == scolor_hwnd)
		set_scolorclose();

	SendMessage(hwnd, WM_CLOSE, 0, 0);
}
xui_method_explain(xui_global, mod_syswnd,		void							)( xui_syswnd* syswnd )
{
	extern HWND  gHWND;
	EnableWindow(gHWND, FALSE);

	std::map<HWND, xui_syswnd*>::iterator itor = syswnd_map.begin();
	for (; itor != syswnd_map.end(); ++itor)
	{
		HWND		 hwnd = (*itor).first;
		xui_syswnd*  swnd = (*itor).second;
		EnableWindow(hwnd, (swnd == syswnd) ? TRUE : FALSE);
	}

	SetForegroundWindow(syswnd->get_renderwnd()->get_hwnd());
}
xui_method_explain(xui_global, res_syswnd,		void							)( void )
{
	std::map<HWND, xui_syswnd*>::iterator itor = syswnd_map.begin();
	for (; itor != syswnd_map.end(); ++itor)
	{
		HWND hwnd = (*itor).first;
		EnableWindow(hwnd, TRUE);
	}

	extern HWND  gHWND;
	EnableWindow(gHWND, TRUE);
	SetForegroundWindow(gHWND);
}

std::map<u32, HCURSOR> cursor_map;
xui_method_explain(xui_global, add_cursor,		void							)( u32 cursor, const std::wstring& filename )
{
	std::map<u32, HCURSOR>::iterator itor = cursor_map.find(cursor);
	if (itor != cursor_map.end())
		::DestroyCursor((*itor).second);

	cursor_map[cursor] = ::LoadCursorFromFile(filename.c_str());
}
xui_method_explain(xui_global, set_cursor,		void							)( u32 cursor )
{
	std::map<u32, HCURSOR>::iterator itor = cursor_map.find(cursor);
	if (itor != cursor_map.end())
	{
		::SetCursor((*itor).second);
	}
	else
	{
		switch (cursor)
		{
		case CURSOR_DEFAULT:	::SetCursor(::LoadCursor(NULL, IDC_ARROW	));	break;
		case CURSOR_NS:			::SetCursor(::LoadCursor(NULL, IDC_SIZENS	));	break;
		case CURSOR_WE:			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE	));	break;
		case CURSOR_NWSE:		::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE	));	break;
		case CURSOR_NESW:		::SetCursor(::LoadCursor(NULL, IDC_SIZENESW	)); break;
		case CURSOR_TEXT:		::SetCursor(::LoadCursor(NULL, IDC_IBEAM	));	break;
		case CURSOR_HAND:		::SetCursor(::LoadCursor(NULL, IDC_HAND		));	break;
		case CURSOR_DRAG:		::SetCursor(::LoadCursor(NULL, IDC_HAND		));	break;
		case CURSOR_FORBID:		::SetCursor(::LoadCursor(NULL, IDC_NO		));	break;
		}
	}
}
xui_method_explain(xui_global, cpy_string,		void						)( const std::wstring& text )
{
	HGLOBAL hGlobal = GlobalAlloc(GHND, (text.length() + 1) * sizeof(wchar_t));
	wchar_t* buffer = (wchar_t*)GlobalLock(hGlobal);
	wcscpy(buffer, text.c_str());
	GlobalUnlock(hGlobal);

	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hGlobal);
	CloseClipboard();
}
xui_method_explain(xui_global, pst_string,		std::wstring				)( void )
{
	std::wstring result;

	OpenClipboard(NULL);
	HGLOBAL hGlobal = GetClipboardData(CF_UNICODETEXT);
	if (hGlobal)
	{
		wchar_t* buffer = (wchar_t*)GlobalLock(hGlobal);
		result = std::wstring(buffer);
		GlobalUnlock(hGlobal);
	}
	CloseClipboard();

	return result;
}

bool def_deviceproc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	switch (message)
	{
	case WM_KILLFOCUS:
		{
			if (hwnd == scolor_hwnd)
			{
				xui_desktop::get_ins()->set_focusctrl(NULL);
				::SetFocus(hwnd);
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			xui_method_mouse args;
			args.wheel = (s32)(s16)HIWORD(wparam) / 3;
			xui_desktop::get_ins()->os_mousewheel(args);
		}
		break;
	case WM_CAPTURECHANGED:
		{
			HWND other_hwnd = (HWND)lparam;
			if (other_hwnd)
			{
				extern HWND gHWND;
				if (xui_global::get_syswnd(other_hwnd) == NULL || other_hwnd != gHWND)
				{
					if (xui_desktop::get_ins()->get_catchctrl() != NULL)
					{
						POINT pt;
						GetCursorPos(&pt);
						ScreenToClient(hwnd, &pt);

						xui_method_mouse args;
						args.wparam = hwnd;
						args.mouse  = MB_L;
						args.point  = xui_vector<s32>((s32)pt.x, (s32)pt.y);
						args.ctrl   = false;
						args.shift  = false;
						args.alt    = false;
						xui_desktop::get_ins()->os_mouserise(args);
					}
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);

			xui_method_mouse args;
			args.wparam = hwnd;
			args.point  = xui_vector<s32>((s32)pt.x, (s32)pt.y);
			args.ctrl   = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
			args.shift  = (GetKeyState(VK_SHIFT)   & 0x8000) != 0;
			args.alt    = (GetKeyState(VK_MENU)    & 0x8000) != 0;

			switch (message)
			{
			case WM_LBUTTONDOWN:	args.mouse = MB_L;	break;
			case WM_RBUTTONDOWN:	args.mouse = MB_R;	break;
			case WM_MBUTTONDOWN:	args.mouse = MB_M;	break;
			}

			xui_desktop::get_ins()->os_mousedown(args);
			if (xui_desktop::get_ins()->get_catchctrl())
				SetCapture(hwnd);
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);
			xui_method_mouse args;
			args.wparam = hwnd;
			args.point  = xui_vector<s32>((s32)pt.x, (s32)pt.y);
			args.ctrl   = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
			args.shift  = (GetKeyState(VK_SHIFT)   & 0x8000) != 0;
			args.alt    = (GetKeyState(VK_MENU)    & 0x8000) != 0;
			xui_desktop::get_ins()->os_mousemove(args);
		}
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);

			xui_method_mouse args;
			args.wparam = hwnd;
			args.point  = xui_vector<s32>((s32)pt.x, (s32)pt.y);
			args.ctrl   = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
			args.shift  = (GetKeyState(VK_SHIFT)   & 0x8000) != 0;
			args.alt    = (GetKeyState(VK_MENU)    & 0x8000) != 0;

			switch (message)
			{
			case WM_LBUTTONUP:		args.mouse = MB_L;	break;
			case WM_RBUTTONUP:		args.mouse = MB_R;	break;
			case WM_MBUTTONUP:		args.mouse = MB_M;	break;
			}

			xui_desktop::get_ins()->os_mouserise(args);
			if (xui_desktop::get_ins()->get_catchctrl() == NULL)
				ReleaseCapture();
		}
		break;
	case WM_KEYDOWN:
		{
			xui_method_keybd args;
			args.ctrl  = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
			args.shift = (GetKeyState(VK_SHIFT)   & 0x8000) != 0;
			args.alt   = (GetKeyState(VK_MENU)    & 0x8000) != 0;
			args.kcode = VKToKey(wparam);
			xui_desktop::get_ins()->os_keybddown(args);
		}
		break;
	case WM_KEYUP:
		{
			xui_method_keybd args;
			args.ctrl  = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
			args.shift = (GetKeyState(VK_SHIFT)   & 0x8000) != 0;
			args.alt   = (GetKeyState(VK_MENU)    & 0x8000) != 0;
			args.kcode = VKToKey(wparam);
			xui_desktop::get_ins()->os_keybdrise(args);
		}
		break;
	case WM_CHAR:
		{
			xui_desktop::get_ins()->os_keybdchar((u16)wparam);
		}
		break;
	default:
		return false;
	}

	return true;
}

/*
//modify
*/
std::wstring pathstyle_replace( const std::wstring& path, wchar_t src, wchar_t dst )
{
	std::wstring result = path;
	for (u32 i = 0; i < result.length(); ++i)
	{
		if (result[i] == src)
			result[i] =  dst;
	}

	return result;
}

#include <Shlobj.h>
#include <shellapi.h>

std::wstring g_workpath;

extern HWND gHWND;
xui_notify_vec				notify_vec;
ULONG						notify_id = 0;
xui_method_explain(xui_global, set_fwatchstart,	void							)( const std::wstring& path )
{
	std::wstring temp = pathstyle_replace(path, L'/', L'\\');
	ITEMIDLIST* pidlist;
	SHParseDisplayName(temp.c_str(), NULL, &pidlist, SFGAO_CANCOPY, NULL);

	SHChangeNotifyEntry shEntry;
	shEntry.fRecursive = TRUE;
	shEntry.pidl = pidlist;
	LONG lEvent = SHCNE_CREATE|SHCNE_DELETE|SHCNE_MKDIR|SHCNE_RENAMEFOLDER|SHCNE_RENAMEITEM|SHCNE_RMDIR|SHCNE_UPDATEITEM;
	notify_id = SHChangeNotifyRegister(gHWND, SHCNRF_InterruptLevel|SHCNRF_ShellLevel|SHCNRF_RecursiveInterrupt|SHCNRF_NewDelivery, lEvent, WM_USER+0x1000, 1, &shEntry);
}
xui_method_explain(xui_global, set_fwatchclose,	void							)( void )
{
	SHChangeNotifyDeregister(notify_id);
	notify_id = 0;
}
xui_method_explain(xui_global, add_fwatch,		void							)( u32 eventid, const std::wstring& srcpath, const std::wstring& dstpath )
{
	xui_notify_info info;
	info.eventid = eventid;
	info.srcpath = pathstyle_replace(srcpath, L'\\', L'/');
	info.dstpath = pathstyle_replace(dstpath, L'\\', L'/');
	if (info.srcpath.length() > 0) info.srcpath.erase(0, g_workpath.length());
	if (info.dstpath.length() > 0) info.dstpath.erase(0, g_workpath.length());

	notify_vec.push_back(info);
}
xui_method_explain(xui_global, del_fwatch,		void							)( void )
{
	notify_vec.clear();
}
xui_method_explain(xui_global, get_fwatch,		const xui_notify_vec&			)( void )
{
	return notify_vec;
}

/*
//font
*/
std::vector<std::string> g_fontfilevec;
xui_method_explain(xui_global, get_fontfile,	const std::string&				)( s32 face )
{
	return g_fontfilevec[face];
}
xui_method_explain(xui_global, add_fontfile,	void							)( const std::string& file )
{
	g_fontfilevec.push_back(file);
}

/*
//file
*/
INT CALLBACK BrowseCallbackProc(HWND hwnd,  UINT uMsg, LPARAM lp,  LPARAM pData) 
{
	switch(uMsg) 
	{
	case BFFM_INITIALIZED: 
		{
			// WParam is TRUE since you are passing a path.
			// It would be FALSE if you were passing a pidl.
			TCHAR szDir[MAX_PATH]={0};
			GetCurrentDirectory(sizeof(szDir)/sizeof(TCHAR), szDir);
			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir);
		}
		break;
	}
	return 0;
}
xui_method_explain(xui_global, get_fileline,	std::string						)( FILE* file )
{
	std::string line;
	while (1)
	{
		char c = 0;
		fread(&c, 1, 1, file);
		if (c == '\n' || c == 0)
			break;

		line.append(1, c);
	}

	return line;
}
xui_method_explain(xui_global, get_openpath,	std::wstring					)( void )
{
	BROWSEINFO info;
	memset(&info, 0, sizeof(BROWSEINFO));
	info.hwndOwner	= gHWND;
	info.lpszTitle	= L"Select Path";
	info.ulFlags	= BIF_USENEWUI;
	info.lpfn		= BrowseCallbackProc;
	LPITEMIDLIST lpitem = SHBrowseForFolder(&info);
	if (lpitem == NULL)
		return std::wstring();

	wchar_t buffer[MAX_PATH];
	SHGetPathFromIDList(lpitem, buffer);
	return std::wstring(buffer);
}

xui_method_explain(xui_global, get_workpath,	std::wstring					)( void )
{
	if (g_workpath.empty())
	{
		wchar_t buffer[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, buffer);
		g_workpath = pathstyle_replace(std::wstring(buffer), L'\\', L'/') + L"/";
	}

	return g_workpath;
}
xui_method_explain(xui_global, set_workpath,	void							)( const std::wstring& path )
{
	std::wstring temp = pathstyle_replace(path, L'/', L'\\');
	SetCurrentDirectory(temp.c_str());
	g_workpath = pathstyle_replace(path, L'\\', L'/') + L"/";
}
xui_method_explain(xui_global, set_showfind,	void							)( const std::wstring& path )
{
	std::wstring full = g_workpath + path;
	std::wstring temp = L"/select," + pathstyle_replace(full, L'/', L'\\');
	ShellExecute(gHWND, L"open", L"Explorer.exe", temp.c_str(), NULL, SW_SHOW);
}
xui_method_explain(xui_global, get_path,		std::vector<std::wstring>		)( const std::wstring& path )
{
	std::vector<std::wstring> result;

	WIN32_FIND_DATAW findData;
	std::wstring full = (path.length() == 0) ? L"*.*" : path+L"/*.*";
	std::wstring temp = pathstyle_replace(full, L'/', L'\\');
	HANDLE handle = FindFirstFile(temp.c_str(), &findData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		if ((findData.cFileName[0] != L'.') &&
			(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 &&
			(findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)    == 0)
			result.push_back(std::wstring(findData.cFileName));

		while (FindNextFile(handle, &findData))
		{
			if ((findData.cFileName[0] != L'.') &&
				(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 &&
				(findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)    == 0)
				result.push_back(std::wstring(findData.cFileName));
		}

		FindClose(handle);
	}

	return result;
}
xui_method_explain(xui_global, get_file,		std::vector<std::wstring>		)( const std::wstring& path, const std::wstring& filter )
{
	std::vector<std::wstring> result;

	WIN32_FIND_DATAW findData;
	std::wstring full = (path.length() == 0) ? filter : path+L"/"+filter;
	std::wstring temp = pathstyle_replace(full, L'/', L'\\');
	HANDLE handle = FindFirstFile(temp.c_str(), &findData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		if ((findData.cFileName[0] != L'.') &&
			(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 &&
			(findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)    == 0 &&
			(findData.dwFileAttributes & FILE_ATTRIBUTE_DEVICE)	   == 0)
			result.push_back(std::wstring(findData.cFileName));

		while (FindNextFile(handle, &findData))
		{
			if ((findData.cFileName[0] != L'.') &&
				(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 &&
				(findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)    == 0 &&
				(findData.dwFileAttributes & FILE_ATTRIBUTE_DEVICE)	   == 0)
				result.push_back(std::wstring(findData.cFileName));
		}

		FindClose(handle);
	}

	return result;
}
xui_method_explain(xui_global, has_path,		bool							)( const std::wstring& path )
{
	WIN32_FIND_DATAW findData;
	std::wstring full = (path.length() == 0) ? L"*.*" : path+L"/*.*";
	std::wstring temp = pathstyle_replace(full, L'/', L'\\');
	HANDLE handle = FindFirstFile(temp.c_str(), &findData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		FindClose(handle);
		return true;
	}

	return false;
}
xui_method_explain(xui_global, has_file,		bool							)( const std::wstring& file )
{
	std::string temp = unicode_to_ascii(file);
	FILE* handle = fopen(temp.c_str(), "rb");
	if (handle)
	{
		fclose(handle);
		return true;
	}

	return false;
}
xui_method_explain(xui_global, add_path,		bool							)( const std::wstring& path )
{
	std::wstring full = path;
	std::wstring temp = pathstyle_replace(full, L'/', L'\\');
	return CreateDirectory(temp.c_str(), NULL) != 0;
}
xui_method_explain(xui_global, del_file,		bool							)( const std::wstring& path )
{
	std::wstring temp = pathstyle_replace(path+L'\0', L'/', L'\\');
	SHFILEOPSTRUCTW fileOP;
	memset(&fileOP, 0, sizeof(SHFILEOPSTRUCTW));
	fileOP.hwnd		= gHWND;
	fileOP.wFunc	= FO_DELETE;
	fileOP.pFrom	= temp.c_str();
	fileOP.pTo		= NULL;
	fileOP.fFlags	= FOF_ALLOWUNDO|FOF_NOCONFIRMATION;
	return SHFileOperation(&fileOP) == 0;
}
xui_method_explain(xui_global, mov_file,		bool							)( const std::wstring& src, const std::wstring& dst )
{
	std::wstring tempsrc = pathstyle_replace(src+L'\0', L'/', L'\\');
	std::wstring tempdst = pathstyle_replace(dst+L'\0', L'/', L'\\');
	SHFILEOPSTRUCTW fileOP;
	memset(&fileOP, 0, sizeof(SHFILEOPSTRUCTW));
	fileOP.hwnd		= gHWND;
	fileOP.wFunc	= FO_MOVE;
	fileOP.pFrom	= tempsrc.c_str();
	fileOP.pTo		= tempdst.c_str();
	fileOP.fFlags	= FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR;
	return SHFileOperation(&fileOP) == 0;
}
xui_method_explain(xui_global, cpy_file,		bool							)( const std::wstring& src, const std::wstring& dst )
{
	std::wstring tempsrc = pathstyle_replace(src+L'\0', L'/', L'\\');
	std::wstring tempdst = pathstyle_replace(dst+L'\0', L'/', L'\\');
	SHFILEOPSTRUCTW fileOP;
	memset(&fileOP, 0, sizeof(SHFILEOPSTRUCTW));
	fileOP.hwnd		= gHWND;
	fileOP.wFunc	= FO_COPY;
	fileOP.pFrom	= tempsrc.c_str();
	fileOP.pTo		= tempdst.c_str();
	fileOP.fFlags	= FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR;
	return SHFileOperation(&fileOP) == 0;
}
xui_method_explain(xui_global, rna_file,		bool							)( const std::wstring& src, const std::wstring& dst )
{
	std::wstring tempsrc = pathstyle_replace(src+L'\0', L'/', L'\\');
	std::wstring tempdst = pathstyle_replace(dst+L'\0', L'/', L'\\');
	SHFILEOPSTRUCTW fileOP;
	memset(&fileOP, 0, sizeof(SHFILEOPSTRUCTW));
	fileOP.hwnd		= gHWND;
	fileOP.wFunc	= FO_RENAME;
	fileOP.pFrom	= tempsrc.c_str();
	fileOP.pTo		= tempdst.c_str();
	fileOP.fFlags	= FOF_NOCONFIRMATION;
	return SHFileOperation(&fileOP) == 0;
}