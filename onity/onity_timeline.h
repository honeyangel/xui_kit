#ifndef __onity_timeline_h__
#define __onity_timeline_h__

#include "xui_dockpage.h"
#include "xui_treeview.h"

class onity_propactor;
class onity_preview;
class onity_lineview;
class onity_timeline : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_timeline( void );

	/*
	//method
	*/
	onity_preview*		get_drawview				( void );
	xui_timeview*		get_timeview				( void );
	onity_propactor*	get_editprop				( void );
	void				set_editprop				( onity_propactor* editprop );

	/*
	//override
	*/
	virtual void		update_else                 ( f32 delta );
	virtual void		render_else					( void );

protected:
	/*
	//callback
	*/
	virtual void		on_perform					( xui_method_args& args );

	/*
	//event
	*/
	void				on_linectrlclick			( xui_component* sender, xui_method_args&			args );
	void				on_sizectrlmousemove		( xui_component* sender, xui_method_mouse&			args );
	void				on_drawviewrenderself		( xui_component* sender, xui_method_args&			args );
	void				on_timeviewcurframechange	( xui_component* sender, xui_method_args&			args );
	void				on_timeviewlinemouseclick	( xui_component* sender, xui_method_mouse&			args );
	void				on_timeviewselecteddrag		( xui_component* sender, xui_method_args&			args );
	void				on_timeviewaddframe			( xui_component* sender, xui_method_args&			args );
	void				on_timeviewdelframe			( xui_component* sender, xui_method_args&			args );
	void				on_timeviewaddlayer			( xui_component* sender, xui_method_args&			args );
	void				on_timeviewdellayer			( xui_component* sender, xui_method_args&			args );
	void				on_timetreedragover			( xui_component* sender, xui_method_treedragdrop&	args );
	void				on_timetreedragdrop			( xui_component* sender, xui_method_treedragdrop&	args );
	void				on_timetreemouseclick		( xui_component* sender, xui_method_mouse&			args );

	/*
	//method
	*/
	void				refresh_timeview			( void );

	/*
	//member
	*/
	xui_timeview*		m_timeview;
	onity_preview*		m_drawview;
	onity_lineview*		m_lineview;
	xui_toggle*			m_linectrl;
	xui_control*		m_sizectrl;
	onity_propactor*	m_editprop;
};

#endif//__onity_timeline_h__