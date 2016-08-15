#ifndef __onity_condition_h__
#define __onity_condition_h__

#include "xui_control.h"

class NP2DSCondition;
class onity_condition : public xui_control
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static const s32	default_height;

	/*
	//constructor
	*/
	onity_condition( NP2DSCondition* condition );

	/*
	//method
	*/
	bool				was_selected	( void ) const;
	NP2DSCondition*		get_condition	( void );

protected:
	/*
	//callback
	*/
	virtual void		on_perform				( xui_method_args& args );

	/*
	//event
	*/
	void				on_ctrlnonfocus			( xui_component* sender, xui_method_args& args );
	void				on_ctrlgetfocus			( xui_component* sender, xui_method_args& args );
	void				on_paramnameselection	( xui_component* sender, xui_method_args& args );
	void				on_boolvalueselection	( xui_component* sender, xui_method_args& args );
	void				on_numboperaselection	( xui_component* sender, xui_method_args& args );
	void				on_numbvaluetextchanged	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_toggle*			m_flagcheck;
	xui_dropbox*		m_paramname;
	xui_dropbox*		m_boolvalue;
	xui_dropbox*		m_numbopera;
	xui_textbox*		m_numbvalue;
	NP2DSCondition*		m_condition;
};

#endif//__onity_condition_h__