#include "xui_desktop.h"
#include "xui_render_window.h"
#include "xui_window.h"
#include "xui_syswnd.h"
#include "xui_global.h"
#include "xui_timermgr.h"
#include "xui_native_window.h"
#include "xui_syswnd_event.h"
#include "xui_native_device.h"
#include "xui_bitmap_buffer.h"

#if xui_target_platform == xui_platform_macos
#include <locale>
#include <codecvt>
#endif

xui_bitmap*         xui_global::s_icon_pickcolour = NULL;
xui_bitmap*         xui_global::s_icon_pickselect = NULL;
xui_bitmap*         xui_global::s_icon_loop		  = NULL;
xui_native_window*  xui_global::s_hwnd_scolor     = NULL;
xui_syswnd_map      xui_global::s_syswnd_map;
xui_font_vec        xui_global::s_font_vec;

std::wstring xui_global::get_upper( const std::wstring& src )
{
	std::wstring result;
	for (u32 i = 0; i < src.length(); ++i)
		result.push_back((src[i] >= L'a' && src[i] <= L'z') ? (src[i]-32) : src[i]);

	return result;
}

std::vector<std::wstring> xui_global::get_split( const std::wstring& src, wchar_t key )
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

std::string xui_global::unicode_to_utf8( const std::wstring& src )
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

std::wstring xui_global::utf8_to_unicode( const std::string&  src )
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

std::string xui_global::unicode_to_ascii( const std::wstring& src )
{
#if xui_target_platform == xui_platform_macos
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(src);
#else
    char buffer[512];
    WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, buffer, 512, NULL, NULL);
    return std::string(buffer);
#endif
}

std::wstring xui_global::ascii_to_unicode( const std::string&  src )
{
#if xui_target_platform == xui_platform_macos
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.from_bytes(src);
#else
    wchar_t buffer[512];
    MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, buffer, 512);
    return std::wstring(buffer);
#endif
}

bool xui_global::was_scolorstart( void )
{
	return xui_native_device::has_screen_shot();
}

void xui_global::set_scolorstart( xui_syswnd* syswnd )
{
	if (!xui_native_device::has_screen_shot())
	{
		if (syswnd)
		{
			s_hwnd_scolor = syswnd->get_renderwnd()->get_hwnd();
		}
		else
		{
			s_hwnd_scolor = xui_native_window::get_global_window();
		}

        s_hwnd_scolor->set_floating(true);
		xui_native_device::capture_screenshot();
	}
}

xui_colour xui_global::get_scolor( void )
{
	if (xui_native_device::has_screen_shot())
	{
        xui_vector<s32> pt = xui_native_device::get_cursor_absolute();
		return xui_native_device::get_screen_color(pt);
	}

	return xui_colour::k_white;
}

std::vector<xui_colour> xui_global::get_scolor( const xui_vector<s32>& sz )
{
	std::vector<xui_colour> result;
	result.resize(sz.w * sz.h, xui_colour::k_black);
	if (xui_native_device::has_screen_shot())
	{
        xui_vector<s32> pt = xui_native_device::get_cursor_absolute();
		s32 sc = pt.x - sz.w / 2;
		s32 sr = pt.y - sz.h / 2;
		for (s32 ir = 0; ir < sz.h; ++ir)
		{
			for (s32 ic = 0; ic < sz.w; ++ic)
			{
				s32 x = sc + ic;
				s32 y = sr + ir;
				u32 index = ir * sz.w + ic;
				result[index] = xui_native_device::get_screen_color(xui_vector<s32>(x, y));
			}
		}
	}

	return result;
}

void xui_global::set_scolorclose( void )
{
	if (xui_native_device::has_screen_shot())
	{
        if (s_hwnd_scolor)
            s_hwnd_scolor->set_floating(false);

		//释放
        xui_native_device::del_screen_shot();
		s_hwnd_scolor = NULL;
	}
}

xui_vector<s32> xui_global::get_syswndmouse( xui_syswnd* syswnd )
{
	if (syswnd)
	{
        return syswnd->get_renderwnd()->get_hwnd()->get_cursor_position();
	}
	else
	{
        return xui_native_window::get_global_window()->get_cursor_position();
	}
}

void xui_global::set_syswndrect( xui_syswnd* syswnd, const xui_rect2d<s32>& rt )
{
	xui_vector<s32> pt = xui_desktop::get_ins()->get_mousedown();
	xui_desktop::get_ins()->set_mouselast(pt);
	xui_desktop::get_ins()->set_mousecurr(pt);

    xui_vector<s32> parent_pt = xui_native_window::get_global_window()->get_pt();

    xui_native_window* hwnd = syswnd->get_renderwnd()->get_hwnd();
    s32 x = parent_pt.x + rt.ax;
    s32 y = parent_pt.y + rt.ay;
    s32 w = rt.get_w();
    s32 h = rt.get_h();

    hwnd->set_pt(xui_vector<s32>(x, y));
    hwnd->set_sz(xui_vector<s32>(w, h));
}

std::vector<xui_syswnd*> xui_global::get_syswndall( void )
{
	std::vector<xui_syswnd*> vec;
	for (xui_syswnd_map::iterator itor = s_syswnd_map.begin(); itor != s_syswnd_map.end(); ++itor)
	{
		vec.push_back((*itor).second);
	}

	return vec;
}

xui_syswnd* xui_global::get_syswnd( xui_native_window* hwnd )
{
	xui_syswnd_map::iterator itor = s_syswnd_map.find(hwnd);
	if (itor != s_syswnd_map.end())
		return (*itor).second;

	return NULL;
}

xui_syswnd* xui_global::add_syswnd( xui_window* popupctrl, bool sizable )
{
    xui_syswnd* syswnd = nullptr;

    xui_native_window::create_info info;
    info.parent     = xui_native_window::get_global_window();
    info.shared     = xui_native_window::get_global_window();
    info.resizable  = sizable;

    xui_native_window* hwnd = new xui_native_window(info, new xui_syswnd_event());
    if (hwnd)
    {
        syswnd = new xui_syswnd(hwnd, popupctrl);
        s_syswnd_map[hwnd] = syswnd;
        set_syswndrect(syswnd, popupctrl->get_renderrt() + popupctrl->get_renderpt());
    }

    return syswnd;
}

void xui_global::set_syswndclose( xui_syswnd* syswnd )
{
    if (syswnd == NULL)
        return;

    xui_window* wnd = syswnd->get_popupctrl();
    if (wnd->was_modal())
        xui_desktop::get_ins()->del_modalwnd(wnd);

    for (xui_syswnd_map::iterator itor = s_syswnd_map.begin(); itor != s_syswnd_map.end(); ++itor)
    {
        if ((*itor).second == syswnd)
        {
            delete (*itor).second;
            s_syswnd_map.erase(itor);
            break;
        }
    }

    wnd->set_visible(false);
}

void xui_global::del_syswnd( xui_syswnd* syswnd )
{
	xui_native_window* hwnd = syswnd->get_renderwnd()->get_hwnd();
	if (hwnd == s_hwnd_scolor)
		set_scolorclose();

    hwnd->close();
}

void xui_global::mod_syswnd( xui_syswnd* syswnd )
{
    xui_native_window::get_global_window()->set_enable(false);

	for (xui_syswnd_map::iterator itor = s_syswnd_map.begin(); itor != s_syswnd_map.end(); ++itor)
	{
		xui_native_window* hwnd = (*itor).first;
        hwnd->set_enable((*itor).second == syswnd);
	}

    syswnd->get_renderwnd()->get_hwnd()->req_focus();
}

void xui_global::res_syswnd( void )
{
	for (xui_syswnd_map::iterator itor = s_syswnd_map.begin(); itor != s_syswnd_map.end(); ++itor)
	{
		xui_native_window* hwnd = (*itor).first;
        hwnd->set_enable(true);
	}

    xui_native_window::get_global_window()->set_enable(true);
    xui_native_window::get_global_window()->req_focus();
}

void xui_global::add_cursor( u32 cursor, const std::wstring& filename )
{
    xui_bitmap_buffer* buffer = xui_bitmap_buffer::create(unicode_to_utf8(filename));
    if (buffer)
    {
        xui_native_device::add_cursor(cursor, buffer);
        delete buffer;
    }
}

void xui_global::set_cursor( u32 cursor )
{
    xui_native_device::set_cursor(cursor);
}

void xui_global::cpy_string( const std::wstring& text )
{
    std::string utf8 = xui_global::unicode_to_utf8(text);
    xui_native_device::set_clipboard_string(utf8.c_str());
}

std::wstring xui_global::pst_string( void )
{
    std::wstring result;
    const char* text = xui_native_device::get_clipboard_string();
    if (text)
    {
        result = xui_global::utf8_to_unicode(std::string(text));
    }

    return result;
}

void xui_global::set_fwatchstart( const std::wstring& path )
{
    xui_native_device::set_fwatchstart(path);
}

void xui_global::set_fwatchclose( void )
{
    xui_native_device::set_fwatchclose();
}

void xui_global::add_fwatch( u32 eventid, const std::wstring& srcpath, const std::wstring& dstpath )
{
    xui_native_device::add_fwatch(eventid, srcpath, dstpath);
}

void xui_global::del_fwatch( void )
{
    xui_native_device::del_fwatch();
}

const xui_notify_vec& xui_global::get_fwatch( void )
{
	return xui_native_device::get_fwatch();
}

s32 xui_global::get_fontface( const std::string& file )
{
	for (s32 i = 0; i < (s32)s_font_vec.size(); ++i)
	{
		if (s_font_vec[i] == file)
			return i;
	}

	return 0;
}

const std::string& xui_global::get_fontfile( s32 face )
{
	return s_font_vec[face];
}

void xui_global::add_fontfile( const std::string& file )
{
	s_font_vec.push_back(file);
}

std::string xui_global::get_fileline( FILE* file )
{
	std::string line;
	while (1)
	{
		char c = 0;
		fread(&c, 1, 1, file);
		if (c == '\n' || c == 0)
			break;
        if (c <  32)
            continue;

		line.append(1, c);
	}

	return line;
}

//std::wstring xui_global::get_openpath( void )
//{
//    return xui_native_device::get_openpath();
//}

std::wstring xui_global::get_workpath( void )
{
    return xui_native_device::get_workpath();
}

void xui_global::set_workpath( const std::wstring& path )
{
    xui_native_device::set_workpath(path);
}

void xui_global::set_showfind( const std::wstring& path )
{
    xui_native_device::set_showfind(path);
}

std::vector<std::wstring> xui_global::get_path( const std::wstring& path )
{
    return xui_native_device::get_path(path);
}

std::vector<std::wstring> xui_global::get_file( const std::wstring& path, const std::wstring& filter )
{
    return xui_native_device::get_file(path, filter);
}

bool xui_global::has_path( const std::wstring& path )
{
    return xui_native_device::has_path(path);
}

bool xui_global::has_file( const std::wstring& file )
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

bool xui_global::add_path( const std::wstring& path )
{
    return xui_native_device::add_path(path);
}

bool xui_global::del_file( const std::wstring& path )
{
    return xui_native_device::del_file(path);
}

bool xui_global::mov_file( const std::wstring& src, const std::wstring& dst )
{
    return xui_native_device::mov_file(src, dst);
}

bool xui_global::cpy_file( const std::wstring& src, const std::wstring& dst )
{
    return xui_native_device::cpy_file(src, dst);
}

bool xui_global::rna_file( const std::wstring& src, const std::wstring& dst )
{
    return xui_native_device::rna_file(src, dst);
}

