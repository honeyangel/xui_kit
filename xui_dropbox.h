#ifndef __xui_dropbox_h__
#define __xui_dropbox_h__

#include "xui_textbox.h"

class xui_dropbox : public xui_textbox
{
public:
	/*
	//constructor
	*/
	xui_dropbox( const std::string& name, const xui_rect2d<s32>& rect );

	/*
	//destructor
	*/
	virtual ~xui_dropbox( void );

	/*
	//init
	*/
	void						ini_dropbox				( u32 selectedindex );

	/*
	//drop max count
	*/
	u32							get_maxdrop				( void ) const;
	void						set_maxdrop				( u32 count );

	/*
	//item
	*/
	u32							get_itemcount			( void ) const;
	xui_itemtag*				get_item				( u32 index );
	void						add_item				( xui_itemtag* item );
	void						add_item				( const std::wstring& text );
	void						del_item				( xui_itemtag* item );
	void						del_itemall				( void );

	/*
	//selected item
	*/
	u32							get_selectedindex		( void );
	xui_itemtag*				get_selecteditem		( void );
	void						set_selecteditem		( xui_itemtag* item );
	void						non_selecteditem		( void );

	/*
	//virtual
	*/
	virtual xui_rect2d<s32>		get_renderrtins			( void ) const;
	virtual xui_colour			get_rendercolor			( void ) const;
	virtual void				update					( f32 delta );

	/*
	//method
	*/
	xui_method<xui_method_args>	xm_selection;

protected:
	/*
	//override
	*/
	virtual void				on_keybddown			( xui_method_keybd& args );
	virtual void				on_mousedown			( xui_method_mouse& args );
	virtual void				on_nonfocus				( xui_method_args&  args );
	virtual void				on_getfocus				( xui_method_args&  args );
	virtual void				on_textchanged			( xui_method_args&  args );

	/*
	//event
	*/
	void						on_droptogmousedown		( xui_component* sender, xui_method_mouse& args );
	void						on_droptogrenderself	( xui_component* sender, xui_method_args&  args );
	void						on_droplstselection		( xui_component* sender, xui_method_args&  args );
	void						on_dropallnonfocus		( xui_component* sender, xui_method_args&  args );
	void						on_dropallkeybddown		( xui_component* sender, xui_method_keybd& args );
	void						on_dropallmousedown		( xui_component* sender, xui_method_mouse& args );
	void						on_droplstsetclientsz	( xui_component* sender, xui_method_args& args );
	void						on_droplstsetrendersz	( xui_component* sender, xui_method_args& args );

	/*
	//method
	*/
	void						set_droplistshow		( const std::wstring& text );
	void						set_droplisthide		( void );

	/*
	//member
	*/
	u32							m_maxdrop;
	std::vector<xui_itemtag*>	m_itemvec;
	xui_itemtag*				m_selitem;
	xui_toggle*					m_droptog;
	xui_listview*				m_droplst;
};

#endif//__xui_dropbox_h__