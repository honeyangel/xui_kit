#ifndef __xui_itemtag_h__
#define __xui_itemtag_h__

#include "xui_bitmap.h"
#include "xui_family_render.h"

class xui_itemtag
{
public:
	xui_itemtag( const std::wstring& text );
	xui_itemtag( const std::wstring& text, xui_bitmap* icon );
	virtual ~xui_itemtag( void ){};

	virtual std::wstring		get_text	( void );
	virtual xui_bitmap*			get_icon	( void );

protected:
	xui_bitmap*					m_icon;
	std::wstring				m_text;
};

#endif//__xui_itemtag_h__
