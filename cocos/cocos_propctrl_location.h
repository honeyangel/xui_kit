#ifndef __cocos_propdata_location_h__
#define __cocos_propdata_location_h__

#include "xui_propctrl.h"

class cocos_value_location
{
public:
	u08				horzedge;
	u08				vertedge;
	bool			horzfill;
	bool			vertfill;
	xui_vector<f32>	horzgrap;
	xui_vector<f32>	vertgrap;
};

class cocos_propnodebase;
class cocos_propdata_location : public xui_propdata
{
public:
	/*
	//constructor
	*/
	cocos_propdata_location( xui_propkind* kind, cocos_propnodebase* propnode, bool editsize );

	/*
	//method
	*/
	bool					can_editsize			( void );
	cocos_value_location	get_value				( void );
	void					set_value				( const cocos_value_location& value );

protected:
	/*
	//override
	*/
	virtual u08*			do_serialize			( void );
	virtual void			un_serialize			( u08* byte );

	/*
	//member
	*/
	cocos_propnodebase*		m_propnode;
	bool					m_editsize;
};

class cocos_propctrl_location : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_propctrl_location( xui_propdata* propdata );

	/*
	//destructor
	*/
	virtual ~cocos_propctrl_location( void );

	/*
	//create
	*/
	static xui_propctrl*	create					( xui_propdata* propdata );

	/*
	//propdata
	*/
	virtual void			on_linkpropdata			( bool selfupdate = false );
	virtual void			on_editvalue			( xui_propedit* sender );

protected:
	/*
	//override
	*/
	virtual void			on_invalid				( xui_method_args&   args );
	virtual void			on_perform				( xui_method_args&   args );
	virtual void			on_updateself			( xui_method_update& args );

	/*
	//event
	*/
	void					on_numbctrlnonfocus		( xui_component* sender, xui_method_args& args );
	void					on_numbctrlgetfocus		( xui_component* sender, xui_method_args& args );
	void					on_numbctrltextchanged	( xui_component* sender, xui_method_args& args );
	void					on_editctrlgetfocus		( xui_component* sender, xui_method_args& args );
	void					on_toggctrlclick		( xui_component* sender, xui_method_args& args );
	void					on_editpaneperform		( xui_component* sender, xui_method_args& args );
	void					on_editpanerenderself	( xui_component* sender, xui_method_args& args );
	void					on_viewpanerenderself	( xui_component* sender, xui_method_args& args );
	void					on_horzctrlrenderself	( xui_component* sender, xui_method_args& args );
	void					on_vertctrlrenderself	( xui_component* sender, xui_method_args& args );

	/*
	//method
	*/
	void					set_margin				( xui_component* sender, f64 value );
	void					set_horzalign			( void );
	void					set_vertalign			( void );
	void					set_horzstretch			( void );
	void					set_vertstretch			( void );

	/*
	//member
	*/
	xui_panel*				m_editpane;
	xui_panel*				m_viewpane;
	xui_numbbox*			m_lnumctrl;
	xui_numbbox*			m_rnumctrl;
	xui_numbbox*			m_tnumctrl;
	xui_numbbox*			m_bnumctrl;
	xui_toggle*				m_ltogctrl;
	xui_toggle*				m_rtogctrl;
	xui_toggle*				m_ttogctrl;
	xui_toggle*				m_btogctrl;
	xui_toggle*				m_horzctrl;
	xui_toggle*				m_vertctrl;
	xui_action_ctrl*		m_animctrl;
};

#endif//__cocos_propdata_location_h__