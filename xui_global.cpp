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
//system
*/
xui_method_explain(xui_global, set_capture,		void							)( void )
{
	extern HWND gHWND;
	SetCapture(gHWND);
}
xui_method_explain(xui_global, non_capture,		void							)( void )
{
	ReleaseCapture();
}
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
#include <shellapi.h>

extern HWND gHWND;
notify_change_map			modify_map;
ULONG						notify_id = 0;
xui_method_explain(xui_global, set_fwatchstart,	void							)( const std::wstring& path )
{
	ITEMIDLIST* pidlist;
	SHParseDisplayName(path.c_str(), NULL, &pidlist, SFGAO_CANCOPY, NULL);

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
xui_method_explain(xui_global, add_fwatch,		void							)( const std::wstring& path, const std::wstring& misc )
{
	modify_map[path] = misc;
}
xui_method_explain(xui_global, del_fwatch,		void							)( void )
{
	modify_map.clear();
}
xui_method_explain(xui_global, get_fwatch,		const notify_change_map&		)( void )
{
	return modify_map;
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

std::wstring g_workpath;
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
	g_workpath = path + L"/";
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