#ifndef __xui_system_h__
#define __xui_system_h__

#include "xui_header.h"

class xui_system
{
public:
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
	static void							set_screenbegin ( void );
	static xui_colour					get_screencolor	( const xui_vector<s32>& pt );
	static void							set_screenclose	( void );

	/*
	//modify
	*/
	static void							set_fwatchbegin	( const std::wstring& path, HWND hwnd );
	static void							set_fwatchclose	( void );

	/*
	//file
	*/
	static std::vector<std::wstring>	get_path		( const std::wstring& path );
	static std::vector<std::wstring>	get_file		( const std::wstring& path );
	static void							del_path		( const std::wstring& path );
	static void							del_file		( const std::wstring& file );
	static void							mov_path		( const std::wstring& src, const std::wstring& dst );
	static void							mov_file		( const std::wstring& src, const std::wstring& dst );
	static void							cpy_path		( const std::wstring& src, const std::wstring& dst );
	static void							cpy_file		( const std::wstring& src, const std::wstring& dst );
};

#endif//__xui_system_h__