#ifndef __xui_listview_h__
#define __xui_listview_h__

#include "xui_family.h"
#include "xui_container.h"

class xui_listview : public xui_container
{
public:
	/*
	//constructor
	*/
	xui_listview( const std::string& name, const xui_rect2d<s32>& rect, s32 lineheight, const xui_rect2d<s32>& itemborder, const xui_vector<s32>& iconoffset, const xui_vector<s32>& textoffset, xui_bitmap* flagbitmap = NULL );

	/*
	//multi select
	*/
	bool						was_allowmulti			( void ) const;
	void						set_allowmulti			( bool flag );

	/*
	//line height
	*/
	s32							get_lineheight			( void ) const;
	void						set_lineheight			( s32 height );

	/*
	//method
	*/
	const xui_family&			get_font				( void ) const;
	void						set_font				( const xui_family& font );
	const xui_colour&			get_movecolor			( void ) const;
	void						set_movecolor			( const xui_colour& color );
	const xui_colour&			get_flagcolor			( void ) const;
	void						set_flagcolor			( const xui_colour& color );
	const xui_colour&			get_textcolor			( void ) const;
	void						set_textcolor			( const xui_colour& color );
	u08							get_textalign			( void ) const;
	void						set_textalign			( u08 align );

	/*
	//selected
	*/
	void						set_selecteditem		( xui_listitem* item, bool selected );
	void						set_selecteditem		( u32 start, u32 final );
	void						non_selecteditem		( bool fireMethod = true );
	std::vector<xui_listitem*>	get_selecteditem		( void );
	std::vector<u32>			get_selecteditemindex	( void );

	/*
	//item
	*/
	u32							get_itemcount			( void ) const;
	u32							get_itemindex			( xui_listitem* item );
	xui_listitem*				get_item				( u32 index );
	xui_listitem*				add_item				( const std::wstring& text );
	xui_listitem*				add_item				( const std::wstring& text, xui_bitmap* icon );
	void						del_item				( xui_listitem* item );
	void						del_itemall				( void );
	void						set_itemvisible			( xui_listitem* item );

	/*
	//method
	*/
	xui_method<xui_method_args>	xm_selectedchange;

protected:
	/*
	//callback
	*/
	virtual void				on_invalid				( xui_method_args& args );
	virtual void				on_perform				( xui_method_args& args );

	/*
	//member
	*/
	xui_bitmap*					m_flagbitmap;
	s32							m_lineheight;
	bool						m_allowmulti;
	xui_rect2d<s32>				m_itemborder;
	xui_vector<s32>				m_iconoffset;
	xui_vector<s32>				m_textoffset;

	xui_family					m_font;
	u08							m_textalign;
	xui_colour					m_textcolor;
	xui_colour					m_movecolor;
	xui_colour					m_flagcolor;
};

#endif//__xui_listview_h__