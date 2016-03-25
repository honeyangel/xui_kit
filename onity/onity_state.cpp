#include "NP2DSState.h"
#include "NP2DSStateCtrl.h"

#include "xui_desktop.h"
#include "onity_animator.h"
#include "onity_propstate.h"
#include "onity_state.h"

xui_implement_rtti(onity_state, xui_drawer);

/*
//static
*/
const s32 onity_state::default_width  = 160;
const s32 onity_state::default_height =  32;

/*
//constructor
*/
xui_create_explain(onity_state)( NP2DSState* state )
: xui_drawer(xui_vector<s32>(default_width, default_height))
, m_state(state)
, m_mousedown(xui_vector<s32>(0))
{
	m_propstate = new onity_propstate(this);
}

/*
//destructor
*/
xui_delete_explain(onity_state)( void )
{
	delete m_propstate;
}

/*
//method
*/
xui_method_explain(onity_state, get_propstate,		onity_propstate*)( void )
{
	return m_propstate;
}
xui_method_explain(onity_state, get_state,			NP2DSState*		)( void )
{
	return m_state;
}
xui_method_explain(onity_state, del_statelink,		void			)( NP2DSState* state )
{
	m_propstate->del_statelink(state);
}
xui_method_explain(onity_state, del_paramlink,		void			)( NP2DSParam* param )
{
	m_propstate->del_paramlink(param);
}

/*
//override
*/
xui_method_explain(onity_state, get_rendercolor,	xui_colour		)( void ) const
{
	if (m_state->GetStateCtrl()->GetDefState() == m_state)
		return xui_colour(1.0f, 173.0f/255.0f, 96.0f/255.0f, 19.0f/255.0f);

	return xui_drawer::get_rendercolor();
}

/*
//callback
*/
xui_method_explain(onity_state, on_setrenderpt,		void			)( xui_method_args&  args )
{
	xui_drawer::on_setrenderpt(args);
	xui_vector<s32> pt = get_renderpt();
	m_state->SetPosition(NPVector2((npf32)pt.x, (npf32)pt.y));
}
xui_method_explain(onity_state, on_mousedown,		void			)( xui_method_mouse& args )
{
	xui_drawer::on_mousedown(args);
	if (args.mouse == MB_L)
	{
		m_mousedown = get_renderpt();
	}
}
xui_method_explain(onity_state, on_mousemove,		void			)( xui_method_mouse& args )
{
	xui_drawer::on_mousemove(args);
	if (has_catch())
	{
		xui_vector<s32> curr = xui_desktop::get_ins()->get_mousecurr();
		xui_vector<s32> down = xui_desktop::get_ins()->get_mousedown();
		xui_vector<s32> pt   = curr - down + m_mousedown;
		pt.x = pt.x/10*10;
		pt.y = pt.y/10*10;
		set_renderpt(pt);
	}
}
xui_method_explain(onity_state, on_keybddown,		void			)( xui_method_keybd& args )
{
	xui_drawer::on_keybddown(args);
	if (args.kcode == KEY_DELETE)
	{
		if (m_parent)
		{
			onity_animator* animator = xui_dynamic_cast(onity_animator, m_parent->get_parent());
			animator->del_state(m_state);
		}
	}
}