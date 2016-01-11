#ifndef __xui_itemtag_h__
#define __xui_itemtag_h__

#include "xui_bitmap.h"

class xui_itemtag
{
public:
	/*
	//constructor
	*/
	xui_itemtag( void );
	xui_itemtag( const std::wstring& text );

	/*
	//virtual
	*/
	virtual std::wstring	get_text( void );

protected:
	/*
	//member
	*/
	std::wstring			m_text;
};

#endif//__xui_itemtag_h__
