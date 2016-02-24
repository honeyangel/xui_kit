#ifndef __xui_dockview_h__
#define __xui_dockview_h__

#include "xui_control.h"

class xui_dockview : public xui_control
{
	xui_declare_rtti

public:
	static xui_dockview*		create		( void );

	/*
	//constructor
	*/
	xui_dockview( const xui_vector<s32>& size, u08 dockstyle );

	/*
	//method
	*/
	xui_menu*							get_viewmenu			( void );
	xui_dockpage*						get_showpage			( void );
	void								set_showpage			( xui_dockpage* page );
	const std::vector<xui_dockpage*>&	get_pagelist			( void ) const;
	xui_rect2d<s32>						get_freerect			( void ) const;
	xui_rect2d<s32>						get_namerect			( void ) const;
	f32									get_portions			( void ) const;
	void								cal_portions			( void );
	void								use_portions			( void );

	/*
	//size
	*/
	xui_vector<s32>						get_minlimit			( void );
	xui_vector<s32>						get_maxlimit			( void );

	/*
	//page
	*/
	void								add_dockpage			( xui_dockpage* page, u08 dockstyle, bool autosize = true, bool merge = false );
	void								del_dockpage			( xui_dockpage* page );
	void								del_dockview			( xui_dockview* view );
	void								mov_dockview			( std::vector<xui_dockview*>& viewlist, xui_dockview* rootview );

protected:
	/*
	//callback
	*/
	virtual void						on_invalid				( xui_method_args& args );
	virtual void						on_perform				( xui_method_args& args );
	virtual void						on_setrendersz			( xui_method_args& args );

	/*
	//event
	*/
	void								on_sizectrlmousemove	( xui_component* sender, xui_method_mouse& args );
	void								on_sizectrltopdraw		( xui_component* sender, xui_method_args&  args );
	void								on_menuctrlrenderself	( xui_component* sender, xui_method_args&  args );
	void								on_viewmenucloseclick	( xui_component* sender, xui_method_args&  args );

	/*
	//method
	*/
	void								add_dockctrl			( xui_component* comp );
	void								del_dockctrl			( xui_component* comp );

	/*
	//member
	*/
	xui_dockpage*						m_showpage;
	std::vector<xui_dockpage*>			m_pagelist;
	std::vector<xui_dockview*>			m_viewlist;
	xui_component*						m_sizectrl;
	xui_toggle*							m_menuctrl;
	xui_menu*							m_viewmenu;
	f32									m_portions;
};

#endif//__xui_dockview_h__