#ifndef __xui_global_h__
#define __xui_global_h__

#include "xui_header.h"
#include "xui_vector.h"
#include "xui_colour.h"
#include "xui_method.h"

typedef std::map<std::wstring, std::wstring> notify_change_map;
class xui_global
{
public:
	/*
	//icon
	*/
	static xui_bitmap*						icon_pickcolour;
	static xui_bitmap*						icon_pickselect;
	static xui_bitmap*						icon_loop;

	/*
	//string
	*/
	static std::string						unicode_to_utf8	( const std::wstring& src );
	static std::wstring						utf8_to_unicode	( const std::string&  src );
	static std::string						unicode_to_ascii( const std::wstring& src );
	static std::wstring						ascii_to_unicode( const std::string&  src );

	/*
	//system
	*/
	static void								set_cursor		( u32 cursor );
	static void								add_cursor		( u32 cursor, const std::wstring& filename );
	static void								set_capture		( void );
	static void								non_capture		( void );

	/*
	//screen
	*/
	static bool								was_scolorstart	( void );
	static void								set_scolorstart ( void );
	static xui_colour						get_scolor		( void );
	static std::vector<xui_colour>			get_scolor		( const xui_vector<s32>& sz );
	static void								set_scolorclose	( void );

	/*
	//modify
	*/
	static void								set_fwatchstart	( const std::wstring& path );
	static void								set_fwatchclose	( void );
	static void								add_fwatch		( const std::wstring& path, const std::wstring& misc = L"" );
	static void								del_fwatch		( void );
	static const notify_change_map&			get_fwatch		( void );

	/*
	//font
	*/
	static const std::string&				get_fontfile	( s32 face );
	static void								add_fontfile	( const std::string& file );

	/*
	//file
	*/
	static std::wstring						get_openpath	( void );
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
};

#endif//__xui_global_h__