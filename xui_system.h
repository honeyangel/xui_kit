#ifndef __xui_system_h__
#define __xui_system_h__

#include "xui_header.h"

class xui_system
{
public:
	static std::string		unicode_to_utf8	( const std::wstring& src );
	static std::wstring		utf8_to_unicode	( const std::string&  src );
	static void				set_cursor		( u32 cursor );
};

#endif//__xui_system_h__