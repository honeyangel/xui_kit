#ifndef __xui_listview_h__
#define __xui_listview_h__

#include "xui_family.h"
#include "xui_family_render.h"
#include "xui_container.h"

class xui_listview : public xui_container
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static const xui_colour		default_movecolor;
	static const xui_colour		default_tickcolor;
	static xui_listview*		create					( const xui_vector<s32>& size, bool drawtick );

	/*
	//constructor
	*/
	xui_listview( const xui_vector<s32>& size, bool drawtick, xui_component* parent = NULL );

	/*
	//multi select
	*/
	bool						was_allowmulti			( void ) const;
	void						set_allowmulti			( bool flag );

	/*
	//method
	*/
	const xui_vector<s32>&		get_iconsize			( void ) const;
	void						set_iconsize			( const xui_vector<s32>& iconsize );
	const xui_family&			get_textfont			( void ) const;
	void						set_textfont			( const xui_family& textfont );
	const xui_family_render&	get_textdraw			( void ) const;
	void						set_textdraw			( const xui_family_render& textdraw );
	u08							get_textalign			( void ) const;
	void						set_textalign			( u08 textalign );
	u08							get_iconalign			( void ) const;
	void						set_iconalign			( u08 iconalign );
	const xui_colour&			get_movecolor			( void ) const;
	void						set_movecolor			( const xui_colour& movecolor );
	const xui_colour&			get_tickcolor			( void ) const;
	void						set_tickcolor			( const xui_colour& tickcolor );
	const xui_rect2d<s32>&		get_itemborder			( void ) const;
	void						set_itemborder			( const xui_rect2d<s32>& itemborder );
	const xui_vector<s32>&		get_iconoffset			( void ) const;
	void						set_iconoffset			( const xui_vector<s32>& iconoffset );
	const xui_vector<s32>&		get_textoffset			( void ) const;
	void						set_textoffset			( const xui_vector<s32>& textoffset );
	s32							get_lineheight			( void ) const;
	void						set_lineheight			( s32 height );

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
	bool						m_drawtick;
	xui_vector<s32>				m_iconsize;
	xui_family					m_textfont;
	xui_family_render			m_textdraw;
	u08							m_textalign;
	u08							m_iconalign;
	xui_colour					m_movecolor;
	xui_colour					m_tickcolor;
	xui_rect2d<s32>				m_itemborder;
	xui_vector<s32>				m_iconoffset;
	xui_vector<s32>				m_textoffset;
	s32							m_lineheight;
	bool						m_allowmulti;
};

#endif//__xui_listview_h__