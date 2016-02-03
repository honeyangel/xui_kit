#include "xui_component.h"
#include "xui_global.h"

/*
//string
*/
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

/*
//cursor
*/
xui_method_explain(xui_global, set_cursor,		void							)( u32 cursor )
{
	switch (cursor)
	{
	case CURSOR_DEFAULT:	::SetCursor(::LoadCursor(NULL, IDC_ARROW	));	break;
	case CURSOR_NS:			::SetCursor(::LoadCursor(NULL, IDC_SIZENS	));	break;
	case CURSOR_WE:			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE	));	break;
	case CURSOR_TEXT:		::SetCursor(::LoadCursor(NULL, IDC_IBEAM	));	break;
	case CURSOR_HAND:		::SetCursor(::LoadCursor(NULL, IDC_HAND		));	break;
	case CURSOR_DRAG:		::SetCursor(::LoadCursor(NULL, IDC_HAND		));	break;
	case CURSOR_DRAGBAN:	::SetCursor(::LoadCursor(NULL, IDC_NO		));	break;
	}
}

/*
//screen
*/
HDC screen_hdc = NULL;
xui_method_explain(xui_global, set_scolorstart,	void							)( void )
{
	screen_hdc = ::GetDC(NULL);
}
xui_method_explain(xui_global, get_scolor,		xui_colour						)( const xui_vector<s32>& pt )
{
	COLORREF color = ::GetPixel(screen_hdc, pt.x, pt.y);
	BYTE r = GetRValue(color);
	BYTE g = GetGValue(color);
	BYTE b = GetBValue(color);
	return xui_colour(1.0f, r/255.0f, g/255.0f, b/255.0f);
}
xui_method_explain(xui_global, set_scolorclose,	void							)( void )
{
	::ReleaseDC(NULL, screen_hdc);
	screen_hdc = NULL;
}

/*
//modify
*/
#include <Shlobj.h>

extern HWND gHWND;
std::vector<std::wstring>	modify_path;
ULONG						notify_id = 0;
xui_method_explain(xui_global, set_fwatchstart,	void							)( const std::wstring& path )
{
	ITEMIDLIST* pidlist;
	SHParseDisplayName(path.c_str(), NULL, &pidlist, SFGAO_CANCOPY, NULL);

	SHChangeNotifyEntry shEntry;
	shEntry.fRecursive = TRUE;
	shEntry.pidl = pidlist;
	notify_id = SHChangeNotifyRegister(gHWND, SHCNRF_InterruptLevel|SHCNRF_ShellLevel|SHCNRF_RecursiveInterrupt, SHCNE_ALLEVENTS, WM_USER+0x1000, 1, &shEntry);
}
xui_method_explain(xui_global, set_fwatchclose,	void							)( void )
{
	SHChangeNotifyDeregister(notify_id);
	notify_id = 0;
}
xui_method_explain(xui_global, add_fwatch,		void							)( const std::wstring& path )
{
	for (u32 i = 0; i < modify_path.size(); ++i)
	{
		if (modify_path[i] == path)
			return;
	}

	modify_path.push_back(path);
}
xui_method_explain(xui_global, del_fwatch,		void							)( void )
{
	modify_path.clear();
}
xui_method_explain(xui_global, get_fwatch,		const std::vector<std::wstring>&)( void )
{
	return modify_path;
}

/*
//file
*/
xui_method_explain(xui_global, get_workpath,	std::wstring					)( void )
{
	wchar_t buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);
	return std::wstring(buffer);
}
xui_method_explain(xui_global, set_workpath,	void							)( const std::wstring& path )
{
	SetCurrentDirectory(path.c_str());
}
xui_method_explain(xui_global, get_path,		std::vector<std::wstring>		)( const std::wstring& path )
{
	std::vector<std::wstring> result;

	WIN32_FIND_DATAW findData;
	std::wstring temp = (path.length() == 0) ? L"*.*" : path+L"/*.*";
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
xui_method_explain(xui_global, get_file,		std::vector<std::wstring>		)( const std::wstring& path )
{
	std::vector<std::wstring> result;

	WIN32_FIND_DATAW findData;
	std::wstring temp = (path.length() == 0) ? L"*.*" : path+L"/*.*";
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