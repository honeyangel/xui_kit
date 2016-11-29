#ifndef __onity_pivotbox_h__
#define __onity_pivotbox_h__

#include "xui_component.h"

enum
{
	PIVOTSTYLE_NONE,
	PIVOTSTYLE_LC,
	PIVOTSTYLE_RC,
	PIVOTSTYLE_TC,
	PIVOTSTYLE_BC,
	PIVOTSTYLE_CC,
};

class onity_pivotbox
{
public:
	/*
	//constructor
	*/
	onity_pivotbox( xui_panel* owner );

	/*
	//method
	*/
	xui_vector<s32>	ori_pivot			( void );
	void			set_visible			( bool flag );
	void			set_rectupdate		( bool update_pivot = true );

protected:
	/*
	//event
	*/
	void			on_boundrenderself	( xui_component* sender, xui_method_args&  args );
	void			on_boundsetrenderrt	( xui_component* sender, xui_method_args&  args );
	void			on_pivotsetrenderpt	( xui_component* sender, xui_method_args&  args );
	void			on_pivotmousemove	( xui_component* sender, xui_method_mouse& args );
	void			on_pivotmouserise	( xui_component* sender, xui_method_mouse& args );
	void			on_arrowmousemove	( xui_component* sender, xui_method_mouse& args );
	void			on_arrowmouserise	( xui_component* sender, xui_method_mouse& args );
	
	/*
	//method
	*/
	xui_vector<s32>	get_pivotlimit		( const xui_rect2d<s32>& rt, const xui_vector<s32>& pt );
	xui_vector<s32> cal_pivotstyle		( const xui_rect2d<s32>& rt );

	/*
	//member
	*/
	u08				m_style;
	xui_panel*		m_owner;
	xui_drawer*		m_bound;
	xui_drawer*		m_pivot;
	xui_drawer*		m_lsize;
	xui_drawer*		m_rsize;
	xui_drawer*		m_tsize;
	xui_drawer*		m_bsize;
};

#endif//__onity_pivotbox_h__