#include "xui_native_device.h"
#include "xui_colour.h"
#include "xui_native_window.h"
#include "glfw3.h"
#include "glfw3native.h"

xui_vector<s32> xui_native_device::get_screen_size( void )
{
    xui_vector<s32> size;
    size.w = GetSystemMetrics(SM_CXSCREEN);
    size.h = GetSystemMetrics(SM_CYSCREEN);
    return size;
}

ULONG notify_id = 0;
void xui_native_device::set_fwatchstart( const std::wstring& path )
{
    if (notify_id != 0)
    {
        SHChangeNotifyDeregister(notify_id);
        notify_id  = 0;
    }

    std::wstring temp = pathstyle_replace(path, L'/', L'\\');
    ITEMIDLIST* pidlist;
    SHParseDisplayName(temp.c_str(), NULL, &pidlist, SFGAO_CANCOPY, NULL);

    SHChangeNotifyEntry shEntry;
    shEntry.fRecursive = TRUE;
    shEntry.pidl = pidlist;
    LONG lEvent = SHCNE_CREATE | SHCNE_DELETE | SHCNE_MKDIR | SHCNE_RENAMEFOLDER | SHCNE_RENAMEITEM | SHCNE_RMDIR | SHCNE_UPDATEITEM;
    notify_id = SHChangeNotifyRegister(xui_native_window::get_global_window()->get_win32_handler(), SHCNRF_InterruptLevel | SHCNRF_ShellLevel | SHCNRF_RecursiveInterrupt | SHCNRF_NewDelivery, lEvent, WM_USER + 0x1000, 1, &shEntry);
}

void xui_native_device::set_fwatchclose( void )
{
    SHChangeNotifyDeregister(notify_id);
    notify_id = 0;
}

INT CALLBACK BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData )
{
    switch (uMsg)
    {
    case BFFM_INITIALIZED:
        {
            TCHAR szDir[MAX_PATH] = { 0 };
            GetCurrentDirectory(sizeof(szDir) / sizeof(TCHAR), szDir);
            SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir);
        }
        break;
    }
    return 0;
}

void xui_native_device::set_openpath_dialog( xui_method<const std::wstring>* callback )
{
    BROWSEINFO info;
    memset(&info, 0, sizeof(BROWSEINFO));
    info.hwndOwner  = xui_native_window::get_global_window()->get_win32_handler();
    info.lpszTitle  = L"Select Path";
    info.ulFlags    = BIF_USENEWUI;
    info.lpfn       = BrowseCallbackProc;

    LPITEMIDLIST lpitem = SHBrowseForFolder(&info);
    if (lpitem == NULL)
        return;

    wchar_t buffer[MAX_PATH];
    SHGetPathFromIDList(lpitem, buffer);
    (*callback)(NULL, std::wstring(buffer));
}

std::string xui_native_device::get_temppath( void )
{
    return std::string("");
}

const std::wstring& xui_native_device::get_workpath( void )
{
    if (s_work_path.empty())
    {
        wchar_t buffer[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, buffer);
        s_work_path = pathstyle_replace(std::wstring(buffer), L'\\', L'/') + L"/";
    }

    return s_work_path;
}

void xui_native_device::set_workpath( const std::wstring& path )
{
    std::wstring temp = pathstyle_replace(path, L'/', L'\\');
    SetCurrentDirectory(temp.c_str());
    s_work_path = pathstyle_replace(path, L'\\', L'/') + L"/";
}

void xui_native_device::set_showfind( const std::wstring& path )
{
    std::wstring full = /*s_work_path + */path;
    std::wstring temp = L"/select," + pathstyle_replace(full, L'/', L'\\');
    ShellExecute(xui_native_window::get_global_window()->get_win32_handler(), L"open", L"Explorer.exe", temp.c_str(), NULL, SW_SHOW);
}

std::vector<std::wstring> xui_native_device::get_path( const std::wstring& path )
{
    std::vector<std::wstring> result;

    WIN32_FIND_DATAW findData;
    std::wstring full = (path.length() == 0) ? L"*.*" : path + L"/*.*";
    std::wstring temp = pathstyle_replace(full, L'/', L'\\');
    HANDLE handle = FindFirstFile(temp.c_str(), &findData);
    if (handle != INVALID_HANDLE_VALUE)
    {
        if ((findData.cFileName[0] != L'.') &&
            (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 &&
            (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0)
            result.push_back(std::wstring(findData.cFileName));

        while (FindNextFile(handle, &findData))
        {
            if ((findData.cFileName[0] != L'.') &&
                (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 &&
                (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0)
                result.push_back(std::wstring(findData.cFileName));
        }

        FindClose(handle);
    }

    return result;
}

std::vector<std::wstring> xui_native_device::get_file( const std::wstring& path, const std::wstring& filter )
{
    std::vector<std::wstring> result;

    WIN32_FIND_DATAW findData;
    std::wstring full = (path.length() == 0) ? filter : path + L"/" + filter;
    std::wstring temp = pathstyle_replace(full, L'/', L'\\');
    HANDLE handle = FindFirstFile(temp.c_str(), &findData);
    if (handle != INVALID_HANDLE_VALUE)
    {
        if ((findData.cFileName[0] != L'.') &&
            (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 &&
            (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0 &&
            (findData.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) == 0)
            result.push_back(std::wstring(findData.cFileName));

        while (FindNextFile(handle, &findData))
        {
            if ((findData.cFileName[0] != L'.') &&
                (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 &&
                (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0 &&
                (findData.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) == 0)
                result.push_back(std::wstring(findData.cFileName));
        }

        FindClose(handle);
    }

    return result;
}

bool xui_native_device::has_path( const std::wstring& path )
{
    WIN32_FIND_DATAW findData;
    std::wstring full = (path.length() == 0) ? L"*.*" : path + L"/*.*";
    std::wstring temp = pathstyle_replace(full, L'/', L'\\');
    HANDLE handle = FindFirstFile(temp.c_str(), &findData);
    if (handle != INVALID_HANDLE_VALUE)
    {
        FindClose(handle);
        return true;
    }

    return false;
}

bool xui_native_device::add_path( const std::wstring& path )
{
    std::wstring full = path;
    std::wstring temp = pathstyle_replace(full, L'/', L'\\');
    return CreateDirectory(temp.c_str(), NULL) != 0;
}

bool xui_native_device::del_file( const std::wstring& path )
{
    std::wstring temp = pathstyle_replace(path + L'\0', L'/', L'\\');
    SHFILEOPSTRUCTW fileOP;
    memset(&fileOP, 0, sizeof(SHFILEOPSTRUCTW));
    fileOP.hwnd = xui_native_window::get_global_window()->get_win32_handler();
    fileOP.wFunc = FO_DELETE;
    fileOP.pFrom = temp.c_str();
    fileOP.pTo = NULL;
    fileOP.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
    return SHFileOperation(&fileOP) == 0;
}

bool xui_native_device::mov_file( const std::wstring& src, const std::wstring& dst )
{
    std::wstring tempsrc = pathstyle_replace(src + L'\0', L'/', L'\\');
    std::wstring tempdst = pathstyle_replace(dst + L'\0', L'/', L'\\');
    SHFILEOPSTRUCTW fileOP;
    memset(&fileOP, 0, sizeof(SHFILEOPSTRUCTW));
    fileOP.hwnd = xui_native_window::get_global_window()->get_win32_handler();
    fileOP.wFunc = FO_MOVE;
    fileOP.pFrom = tempsrc.c_str();
    fileOP.pTo = tempdst.c_str();
    fileOP.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
    return SHFileOperation(&fileOP) == 0;
}

bool xui_native_device::cpy_file( const std::wstring& src, const std::wstring& dst )
{
    std::wstring tempsrc = pathstyle_replace(src + L'\0', L'/', L'\\');
    std::wstring tempdst = pathstyle_replace(dst + L'\0', L'/', L'\\');
    SHFILEOPSTRUCTW fileOP;
    memset(&fileOP, 0, sizeof(SHFILEOPSTRUCTW));
    fileOP.hwnd = xui_native_window::get_global_window()->get_win32_handler();
    fileOP.wFunc = FO_COPY;
    fileOP.pFrom = tempsrc.c_str();
    fileOP.pTo = tempdst.c_str();
    fileOP.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
    return SHFileOperation(&fileOP) == 0;
}

bool xui_native_device::rna_file( const std::wstring& src, const std::wstring& dst )
{
    std::wstring tempsrc = pathstyle_replace(src + L'\0', L'/', L'\\');
    std::wstring tempdst = pathstyle_replace(dst + L'\0', L'/', L'\\');
    SHFILEOPSTRUCTW fileOP;
    memset(&fileOP, 0, sizeof(SHFILEOPSTRUCTW));
    fileOP.hwnd = xui_native_window::get_global_window()->get_win32_handler();
    fileOP.wFunc = FO_RENAME;
    fileOP.pFrom = tempsrc.c_str();
    fileOP.pTo = tempdst.c_str();
    fileOP.fFlags = FOF_NOCONFIRMATION;
    return SHFileOperation(&fileOP) == 0;
}

void xui_native_device::sleep( f32 seconds )
{
    ::Sleep((s32)(seconds * 1000.0f));
}

void xui_native_device::release_capture( void )
{
    ::ReleaseCapture();
}

void xui_native_device::capture_screenshot( void )
{
    xui_vector<s32> screen_size = get_screen_size();

    HDC hdcScreen = ::GetDC(NULL);
    HDC hdcMemory = CreateCompatibleDC(hdcScreen);
    if (hdcMemory == NULL)
    {
        goto done;
    }

    BITMAPINFO bi;
    bi.bmiHeader.biSize             = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth            =  screen_size.w;
    bi.bmiHeader.biHeight           = -screen_size.h;
    bi.bmiHeader.biPlanes           = 1;
    bi.bmiHeader.biBitCount         = 24;
    bi.bmiHeader.biCompression      = BI_RGB;
    bi.bmiHeader.biSizeImage        = 0;
    bi.bmiHeader.biXPelsPerMeter    = 0;
    bi.bmiHeader.biYPelsPerMeter    = 0;
    bi.bmiHeader.biClrUsed          = 0;
    bi.bmiHeader.biClrImportant     = 0;

    char* buffer = NULL;
    HBITMAP hbmScreen = CreateDIBSection(hdcMemory, &bi, DIB_RGB_COLORS, (void**)(&buffer), 0, 0);
    if (hbmScreen == NULL)
    {
        goto done;
    }

    SelectObject(hdcMemory, hbmScreen);
    if (!BitBlt(
        hdcMemory,   
        0, 0,        
        screen_size.w, 
        screen_size.h, 
        hdcScreen,   
        0, 0,        
        SRCCOPY))    
    {
        goto done;
    }

    s32 buffer_size = screen_size.w * screen_size.h * 3;
    s_screen_buf = new u08[buffer_size];
    memcpy(s_screen_buf, buffer, buffer_size);

done:
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemory);
    ReleaseDC(NULL, hdcScreen);
}