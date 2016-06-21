#ifndef __onity_fileview_h__
#define __onity_fileview_h__

#include "xui_control.h"

class onity_tileview;
class onity_fileview : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_fileview( void );

	/*
	//method
	*/
	void							set_locknode			( xui_treenode* locknode );
	xui_treeview*					get_lineview			( void );
	onity_tileview*					get_tileview			( void );
	void							clear					( void );

	/*
	//method
	*/
	xui_method<xui_method_args >	xm_fileviewselection;
	xui_method<xui_method_mouse>	xm_fileviewdoubleclk;
	xui_method<xui_method_mouse>	xm_fileviewnodeclick;
	xui_method<xui_method_dragdrop>	xm_fileviewassetdrag;

protected:
	/*
	//callback
	*/
	virtual void					on_updateself			( xui_method_update& args );

	/*
	//event
	*/
	void							on_lineviewshow			( xui_component* sender, xui_method_args&	  args );
	void							on_lineviewrenderelse	( xui_component* sender, xui_method_args&     args );
	void							on_tileviewrenderelse	( xui_component* sender, xui_method_args&     args );
	void							on_fileviewselection	( xui_component* sender, xui_method_args&	  args );
	void							on_fileviewnodeclick	( xui_component* sender, xui_method_mouse&	  args );
	void							on_fileviewdoubleclk	( xui_component* sender, xui_method_mouse&	  args );
	void							on_fileviewassetdrag	( xui_component* sender, xui_method_dragdrop& args );

	/*
	//member
	*/
	xui_treeview*					m_lineview;
	onity_tileview*					m_tileview;
	xui_treenode*					m_locknode;
	f32								m_locktime;
};

#endif//__onity_fileview_h__