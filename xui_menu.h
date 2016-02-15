#ifndef __xui_menu_h__
#define __xui_menu_h__

#include "xui_control.h"

class xui_menu : public xui_control
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_menu*			create			( s32 width = 256 );

	/*
	//constructor
	*/
	xui_menu( const xui_vector<s32>& size );

	/*
	//method
	*/
	bool						was_series		( xui_component* comp );
	xui_toggle*					get_ownertoggle	( void );
	void						set_ownertoggle	( xui_toggle* toggle );
	xui_menu*					get_showsubmenu	( void );
	void						set_showsubmenu	( xui_menu* submenu );

	/*
	//item
	*/
	xui_component*				add_separate	( void );
	xui_menuitem*				add_item		( xui_bitmap* icon, const std::wstring& text, const std::wstring& hint = L"", const xui_family_render& draw = xui_family_render::default );
	void						add_item		( xui_menuitem* item );
	void						del_item		( xui_menuitem* item );

	/*
	//override
	*/
	virtual xui_component*		choose			( const xui_vector<s32>& pt );
	virtual void				update			( f32 delta );
	virtual void				render			( void ); 

protected:
	/*
	//callback
	*/
	virtual void				on_nonfocus		( xui_method_args& args );
	virtual void				on_invalid		( xui_method_args& args );
	virtual void				on_perform		( xui_method_args& args );

	/*
	//member
	*/
	xui_toggle*					m_ownertoggle;
	xui_menu*					m_showsubmenu;
};

#endif//__xui_menu_h__