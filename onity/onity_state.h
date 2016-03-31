#ifndef __onity_state_h__
#define __onity_state_h__

#include "xui_drawer.h"

class onity_propstate;
class onity_state : public xui_drawer
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static const s32	default_width ;
	static const s32	default_height;

	/*
	//constructor
	*/
	onity_state( NP2DSState* state );

	/*
	//destructor
	*/
	virtual ~onity_state( void );

	/*
	//method
	*/
	onity_propstate*	get_propstate	( void );
	NP2DSState*			get_state		( void );

	/*
	//notify
	*/
	void				on_delstate		( NP2DSState* state );
	void				on_delparam		( NP2DSParam* param );
	void				on_addparam		( NP2DSParam* param );

	/*
	//override
	*/
	virtual xui_colour	get_rendercolor	( void ) const;

protected:
	/*
	//callback
	*/
	virtual void		on_setrenderpt	( xui_method_args&  args );
	virtual void		on_mousedown	( xui_method_mouse& args );
	virtual void		on_mousemove	( xui_method_mouse& args );
	virtual void		on_keybddown	( xui_method_keybd& args );

	/*
	//member
	*/
	NP2DSState*			m_state;
	xui_vector<s32>		m_mousedown;
	onity_propstate*	m_propstate;
};

#endif//__onity_state_h__