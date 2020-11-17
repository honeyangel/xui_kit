#ifndef __xui_global_h__
#define __xui_global_h__

#include "xui_header.h"
#include "xui_vector.h"
#include "xui_colour.h"
#include "xui_method.h"
#include "xui_native_device.h"

typedef std::map<xui_native_window*, xui_syswnd*>   xui_syswnd_map;
typedef std::vector<std::string>                    xui_font_vec;    

class xui_global
{
public:
	static xui_bitmap*						s_icon_pickcolour;
	static xui_bitmap*						s_icon_pickselect;
	static xui_bitmap*						s_icon_loop;
    static xui_native_window*               s_hwnd_scolor;

	static std::wstring						get_upper		( const std::wstring& src );
	static std::vector<std::wstring>		get_split		( const std::wstring& src, wchar_t key );
	static std::string						unicode_to_utf8	( const std::wstring& src );
	static std::wstring						utf8_to_unicode	( const std::string&  src );
    static std::string                      unicode_to_ascii( const std::wstring& src );
    static std::wstring                     ascii_to_unicode( const std::string&  src );
    
	static xui_vector<s32>					get_syswndmouse	( xui_syswnd* syswnd );
    static void                             set_syswndclose ( xui_syswnd* syswnd );
	static void								set_syswndrect	( xui_syswnd* syswnd, const xui_rect2d<s32>& rt );
	static std::vector<xui_syswnd*>			get_syswndall	( void );
	static xui_syswnd*						get_syswnd		( xui_native_window* hwnd );
	static xui_syswnd*						add_syswnd		( xui_window* popupctrl, bool sizable );
	static void								del_syswnd		( xui_syswnd* syswnd );
	static void								mod_syswnd		( xui_syswnd* syswnd );
	static void								res_syswnd		( void );
	static void								set_cursor		( u32 cursor );
	static void								add_cursor		( u32 cursor, const std::wstring& filename );
	static void								cpy_string		( const std::wstring& text );
	static std::wstring						pst_string		( void );

	static bool								was_scolorstart	( void );
	static void								set_scolorstart ( xui_syswnd* syswnd );
	static xui_colour						get_scolor		( void );
	static std::vector<xui_colour>			get_scolor		( const xui_vector<s32>& sz );
	static void								set_scolorclose	( void );

	static void								set_fwatchstart	( const std::wstring& path );
	static void								set_fwatchclose	( void );
	static void								add_fwatch		( u32 eventid, const std::wstring& srcpath, const std::wstring& dstpath );
	static void								del_fwatch		( void );
	static const xui_notify_vec&			get_fwatch		( void );

	static s32								get_fontface	( const std::string& file );
	static const std::string&				get_fontfile	( s32 face );
	static void								add_fontfile	( const std::string& file );

	static std::string						get_fileline	( FILE* file );
	//static std::wstring						get_openpath	( void );
	static std::wstring						get_workpath	( void );
	static void								set_workpath	( const std::wstring& path );
	static void								set_showfind	( const std::wstring& path );
	static std::vector<std::wstring>		get_path		( const std::wstring& path );
	static std::vector<std::wstring>		get_file		( const std::wstring& path, const std::wstring& filter = L"*.*" );
	static bool								has_path		( const std::wstring& path );
	static bool								has_file		( const std::wstring& file );
	static bool								add_path		( const std::wstring& path );
	static bool								del_file		( const std::wstring& file );
	static bool								mov_file		( const std::wstring& src, const std::wstring& dst );
	static bool								cpy_file		( const std::wstring& src, const std::wstring& dst );
	static bool								rna_file		( const std::wstring& src, const std::wstring& dst );

protected:
    static xui_syswnd_map                   s_syswnd_map;
    static xui_font_vec                     s_font_vec;
};

#endif//__xui_global_h__
