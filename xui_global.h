#ifndef __xui_global_h__
#define __xui_global_h__

#include "xui_header.h"
#include "xui_vector.h"
#include "xui_colour.h"
#include "xui_method.h"

class xui_global
{
public:
	/*
	//method
	*/
	static xui_method<std::wstring>		xm_changenotify;

	/*
	//string
	*/
	static std::string					unicode_to_utf8	( const std::wstring& src );
	static std::wstring					utf8_to_unicode	( const std::string&  src );

	/*
	//cursor
	*/
	static void							set_cursor		( u32 cursor );

	/*
	//screen
	*/
	static void							set_scolorstart ( void );
	static xui_colour					get_scolor		( const xui_vector<s32>& pt );
	static void							set_scolorclose	( void );

	/*
	//modify
	*/
	static void							set_fwatchstart	( const std::wstring& path, void* hwnd );
	static void							set_fwatchclose	( void );

	/*
	//file
	*/
	static std::wstring					get_workpath	( void );
	static std::vector<std::wstring>	get_path		( const std::wstring& path );
	static std::vector<std::wstring>	get_file		( const std::wstring& path );
	static void							del_path		( const std::wstring& path );
	static void							del_file		( const std::wstring& file );
	static void							mov_path		( const std::wstring& src, const std::wstring& dst );
	static void							mov_file		( const std::wstring& src, const std::wstring& dst );
	static void							cpy_path		( const std::wstring& src, const std::wstring& dst );
	static void							cpy_file		( const std::wstring& src, const std::wstring& dst );
};

#endif//__xui_global_h__