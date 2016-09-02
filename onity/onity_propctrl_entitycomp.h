#ifndef __onity_propctrl_jsonescomp_h__
#define __onity_propctrl_jsonescomp_h__

#include "xui_propctrl.h"

class onity_propkind_entitycomp;
class onity_compattr : public xui_control
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static const s32			default_height;

	/*
	//constructor
	*/
	onity_compattr( onity_propkind_entitycomp* compkind, const std::string& attrname );

	/*
	//method
	*/
	bool						was_selected			( void ) const;
	const std::string&			get_attrname			( void ) const;

protected:
	/*
	//callback
	*/
	virtual void				on_perform				( xui_method_args& args );

	/*
	//event
	*/
	void						on_ctrlnonfocus			( xui_component* sender, xui_method_args& args );
	void						on_ctrlgetfocus			( xui_component* sender, xui_method_args& args );
	void						on_namectrltextenter	( xui_component* sender, xui_method_args& args );
	void						on_typectrlselection	( xui_component* sender, xui_method_args& args );
	void						on_boolctrlselection	( xui_component* sender, xui_method_args& args );
	void						on_numbctrltextchanged	( xui_component* sender, xui_method_args& args );
	void						on_textctrltextchanged	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_toggle*					m_flagctrl;
	xui_textbox*				m_namectrl;
	xui_dropbox*				m_typectrl;
	xui_dropbox*				m_boolctrl;
	xui_textbox*				m_numbctrl;
	xui_textbox*				m_textctrl;
	std::string					m_attrname;
	onity_propkind_entitycomp*	m_compkind;
};

class onity_propctrl_compattr : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_propctrl*		create				( xui_propdata* propdata );

	/*
	//constructor
	*/
	onity_propctrl_compattr( void );

	/*
	//override
	*/
	virtual void				on_linkpropdata		( void );
	virtual void				on_editvalue		( xui_propedit* sender );

protected:
	/*
	//callback
	*/
	virtual void				on_renderback		( xui_method_args& args );
	virtual void				on_invalid			( xui_method_args& args );
	virtual void				on_perform			( xui_method_args& args );

	/*
	//event
	*/
	void						on_middleinvalid	( xui_component* sender, xui_method_args& args );
	void						on_middleperform	( xui_component* sender, xui_method_args& args );
	void						on_deleteclick		( xui_component* sender, xui_method_args& args );
	void						on_deleterenderself	( xui_component* sender, xui_method_args& args );
	void						on_insertclick		( xui_component* sender, xui_method_args& args );
	void						on_insertrenderself	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*					m_header;
	xui_panel*					m_middle;
	xui_drawer*					m_nontip;
	xui_button*					m_insert;
	xui_button*					m_delete;
};

#endif//__onity_propctrl_entitycomp_h__