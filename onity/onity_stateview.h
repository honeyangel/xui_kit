#ifndef __onity_stateview_h__
#define __onity_stateview_h__

#include "xui_control.h"

class NP2DSStateCtrl;
class NP2DSState;
class NP2DSParam;
class onity_state;
class onity_propcontroller;
class onity_stateview : public xui_control
{
	xui_declare_rtti
	
public:
	/*
	//constructor
	*/
	onity_stateview( void );

	/*
	//destructor
	*/
	virtual ~onity_stateview( void );

	/*
	//method
	*/
	void						set_editprop		( onity_propcontroller* editprop );
	void						add_state			( const xui_vector<s32>& pt );

	void						add_statectrl		( NP2DSState* state, const xui_vector<s32>& pt );
	onity_state*				get_statectrl		( NP2DSState* state );
	onity_state*				get_statectrl		( const xui_vector<s32>& pt );
	void						del_statectrl		( NP2DSState* state );
	void						del_statectrlall	( void );

	/*
	//select
	*/
	void						sel_statectrl		( onity_state* statectrl );
	void						sel_transprop		( const xui_vector<s32>& pt );

	/*
	//notity
	*/
	void						on_delstate			( NP2DSState* state );
	void						on_delparam			( NP2DSParam* param );
	void						on_addparam			( NP2DSParam* param );

	/*
	//override
	*/
	virtual xui_component*		choose_else			( const xui_vector<s32>& pt );

protected:
	/*
	//callback
	*/
	virtual void				on_noncatch			( xui_method_args&  args );
	virtual void				on_invalid			( xui_method_args&  args );
	virtual void				on_renderself		( xui_method_args&  args );
	virtual void				on_keybddown		( xui_method_keybd& args );
	virtual void				on_mousedown		( xui_method_mouse& args );
	virtual void				on_mousemove		( xui_method_mouse& args );
	virtual void				on_mouserise		( xui_method_mouse& args );

	/*
	//event
	*/
	void						on_createclick		( xui_component* sender, xui_method_args&  args );
	void						on_defaultclick		( xui_component* sender, xui_method_args&  args );
	void						on_transitionclick	( xui_component* sender, xui_method_args&  args );
	void						on_deleteclick		( xui_component* sender, xui_method_args&  args );
	void						on_statemousedown	( xui_component* sender, xui_method_mouse& args );
	void						on_statemouserise	( xui_component* sender, xui_method_mouse& args );

	/*
	//method
	*/
	void						show_menu			( onity_state* statectrl );
	void						drag_view			( const xui_vector<s32>& delta );
	NP2DSTransition*			find_transition		( onity_state* selfstate, const xui_vector<s32>& pt );
	void						draw_transition		( onity_state* selfstate, onity_state* nextstate, const xui_colour& color );
	void						draw_transition		( const xui_vector<s32>& self, const xui_vector<s32>& next, const xui_colour& color );
	xui_vector<s32>				calc_offset			( const xui_vector<s32>& self, const xui_vector<s32>& next );

	/*
	//member
	*/
	xui_menu*					m_menu;
	xui_menuitem*				m_create;
	xui_menuitem*				m_default;
	xui_menuitem*				m_transition;
	xui_menuitem*				m_delete;
	bool						m_mousedrag;
	xui_vector<s32>				m_dragvalue;
	bool						m_maketrans;
	std::vector<onity_state*>	m_statectrl;
	onity_state*				m_actstate;
	onity_state*				m_selstate;
	NP2DSTransition*			m_seltrans;
	onity_propcontroller*		m_editprop;
};

#endif//__onity_stateview_h__