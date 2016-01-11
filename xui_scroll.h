#ifndef __xui_scroll_h__
#define __xui_scroll_h__

#include "xui_control.h"

class xui_scroll : public xui_control
{
	friend class xui_scrollthumb;

public:
	/*
	//constructor
	*/
	xui_scroll( const std::string& name, const xui_rect2d<s32>& rect, u08 style );

	/*
	//init
	*/
	void						ini_scroll		( s32 range, s32 value );

	/*
	//arrow&thumb
	*/
	xui_scrollarrow*			get_startarrow	( void ) const;
	xui_scrollarrow*			get_finalarrow	( void ) const;
	xui_scrollthumb*			get_thumb		( void ) const;

	/*
	//method
	*/
	s32							get_smallchange ( void ) const;
	void						set_smallchange ( s32 value );
	s32							get_largechange	( void ) const;
	void						set_largechange	( s32 value );
	bool						was_thumbresize	( void ) const;
	void						set_thumbresize	( bool flag );

	/*
	//range
	*/
	s32							get_range	( void ) const;
	void						set_range	( s32 range );

	/*
	//value
	*/
	s32							get_value	( void ) const;
	void						set_value	( s32 value );

	/*
	//rectangle
	*/
	virtual xui_rect2d<s32>		get_hallowrt( void ) const;

	/*
	//method
	*/
	xui_method<xui_method_args>	xm_scroll;

protected:
	/*
	//callback
	*/
	virtual void				on_lock		( xui_method_args&  args );
	virtual void				on_free		( xui_method_args&  args );
	virtual void				on_perform	( xui_method_args&  args );
	virtual void				on_mousedown( xui_method_mouse& args );

	/*
	//update
	*/
	void						resize_thumb( void );
	void						update_thumb( void );
	void						update_value( void );

	/*
	//member
	*/
	s32							m_smallchange;
	s32							m_largechange;
	bool						m_thumbresize;
	u08							m_style;
	s32							m_value;
	s32							m_range;
	xui_scrollthumb*			m_thumb;
	xui_scrollarrow*			m_arrow[2];
};

#endif//__xui_scroll_h__