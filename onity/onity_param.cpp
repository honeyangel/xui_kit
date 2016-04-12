#include "NP2DSParam.h"
#include "NP2DSStateCtrl.h"

#include "xui_convas.h"
#include "xui_global.h"
#include "xui_textbox.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "onity_animator.h"
#include "onity_stateview.h"
#include "onity_propcontroller.h"
#include "onity_param.h"

xui_implement_rtti(onity_param, xui_control);

/*
//static
*/
const s32 onity_param::default_width  = 200;
const s32 onity_param::default_height =  20;

/*
//constructor
*/
xui_create_explain(onity_param)( NP2DSParam* param, bool reqfocus )
: xui_control(xui_vector<s32>(default_width, default_height))
, m_param(param)
{
	m_namectrl	= new xui_textbox(xui_vector<s32>(120, 18));
	xui_method_ptrcall(m_namectrl,	xm_nonfocus		) += new xui_method_member<xui_method_args, onity_param>(this, &onity_param::on_editctrlnonfocus);
	xui_method_ptrcall(m_namectrl,	xm_getfocus		) += new xui_method_member<xui_method_args, onity_param>(this, &onity_param::on_editctrlgetfocus);
	xui_method_ptrcall(m_namectrl,	xm_nonfocus		) += new xui_method_member<xui_method_args, onity_param>(this, &onity_param::on_namectrltextchanged);
	xui_method_ptrcall(m_namectrl,	xm_textenter	) += new xui_method_member<xui_method_args, onity_param>(this, &onity_param::on_namectrltextchanged);
	xui_method_ptrcall(m_namectrl,	set_parent		)(this);
	xui_method_ptrcall(m_namectrl,	set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_namectrl,	set_drawcolor	)(true);
	xui_method_ptrcall(m_namectrl,	set_borderrt	)(xui_rect2d<s32>(4, 2, 2, 2));
	xui_method_ptrcall(m_namectrl,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_namectrl,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_namectrl,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_namectrl,	ini_drawer		)(xui_global::ascii_to_unicode(param->GetName()));

	m_boolctrl	= xui_toggle::create();
	xui_method_ptrcall(m_boolctrl,	xm_toggleclick	) += new xui_method_member<xui_method_args, onity_param>(this, &onity_param::on_boolctrlclick);
	xui_method_ptrcall(m_boolctrl,	set_parent		)(this);
	xui_method_ptrcall(m_boolctrl,	ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_boolctrl,	ini_component	)(true, param->GetType() == DT_BOOL);
	xui_method_ptrcall(m_boolctrl,	ini_toggle		)(param->GetValueBool());

	std::wstringstream	text;
	switch (param->GetType())
	{
	case DT_INT:		text << param->GetValueInt  (); break;
	case DT_FLOAT:		text << param->GetValueFloat();	break;
	}

	m_numbctrl	= new xui_textbox(xui_vector<s32>(52, 18));
	xui_method_ptrcall(m_numbctrl,	xm_nonfocus		) += new xui_method_member<xui_method_args, onity_param>(this, &onity_param::on_editctrlnonfocus);
	xui_method_ptrcall(m_numbctrl,	xm_getfocus		) += new xui_method_member<xui_method_args, onity_param>(this, &onity_param::on_editctrlgetfocus);
	xui_method_ptrcall(m_numbctrl,	xm_textchanged	) += new xui_method_member<xui_method_args, onity_param>(this, &onity_param::on_numbctrltextchanged);
	xui_method_ptrcall(m_numbctrl,	set_parent		)(this);
	xui_method_ptrcall(m_numbctrl,	set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_numbctrl,	set_drawcolor	)(true);
	xui_method_ptrcall(m_numbctrl,	set_borderrt	)(xui_rect2d<s32>(4, 2, 2, 2));
	xui_method_ptrcall(m_numbctrl,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_numbctrl,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_numbctrl,	ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_numbctrl,	ini_component	)(true, param->GetType() != DT_BOOL);
	xui_method_ptrcall(m_numbctrl,	ini_drawer		)(text.str());
	xui_method_ptrcall(m_numbctrl,	set_numbtype	)(param->GetType() == DT_INT ? NT_INT : NT_FLOAT);

	m_delete	= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_delete,	xm_renderself	) += new xui_method_member<xui_method_args, onity_param>(this, &onity_param::on_deleterenderself);
	xui_method_ptrcall(m_delete,	xm_buttonclick	) += new xui_method_member<xui_method_args, onity_param>(this, &onity_param::on_deleteclick);
	xui_method_ptrcall(m_delete,	set_parent		)(this);
	xui_method_ptrcall(m_delete,	set_backcolor	)(xui_colour::white);
	xui_method_ptrcall(m_delete,	set_movecolor	)(xui_button::default_downcolor);
	xui_method_ptrcall(m_delete,	ini_component	)(ALIGNHORZ_R, ALIGNVERT_C, 0);

	m_widgetvec.push_back(m_namectrl);
	m_widgetvec.push_back(m_boolctrl);
	m_widgetvec.push_back(m_numbctrl);
	m_widgetvec.push_back(m_delete);
	invalid();

	if (reqfocus)
	{
		xui_method_ptrcall(m_namectrl,	set_selecttext	)(0, m_namectrl->get_text().length());
		xui_method_ptrcall(m_namectrl,	req_focus		)();
	}
}

/*
//method
*/
xui_method_explain(onity_param, get_param, NP2DSParam*)( void )
{
	return m_param;
}

/*
//callback
*/
xui_method_explain(onity_param, on_perform,				void)( xui_method_args& args )
{
	xui_control::on_perform(args);
	s32 x = m_namectrl->get_renderx()+m_namectrl->get_renderw()+6;
	m_boolctrl->on_perform_x(x);
	m_numbctrl->on_perform_x(x);
}

/*
//event
*/
xui_method_explain(onity_param, on_editctrlnonfocus,	void)( xui_component* sender, xui_method_args& args )
{
	xui_control* control = xui_dynamic_cast(xui_control, sender);
	control->set_sidecolor(xui_control::default_sidecolor);
}
xui_method_explain(onity_param, on_editctrlgetfocus,	void)( xui_component* sender, xui_method_args& args )
{
	xui_control* control = xui_dynamic_cast(xui_control, sender);
	control->set_sidecolor(xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
}
xui_method_explain(onity_param, on_namectrltextchanged, void)( xui_component* sender, xui_method_args& args )
{
	std::wstring text = m_namectrl->get_text();
	std::string  name = xui_global::unicode_to_ascii(text);

	NP2DSStateCtrl* stateCtrl = m_param->GetStateCtrl();
	bool has_same = false;
	const NP2DSStateCtrl::ParamVec& vec = stateCtrl->GetParamVec();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		if (vec[i] != m_param && vec[i]->GetName() == name)
		{
			has_same = true;
			break;
		}
	}

	if (has_same || name.length() == 0)
	{
		s32 number = 0;
		while (true)
		{
			std::stringstream temp;
			temp << ((name.length() == 0) ? "New Param" : name.c_str());;
			temp << number;

			has_same = false;
			for (u32 i = 0; i < vec.size(); ++i)
			{
				if (vec[i] != m_param && vec[i]->GetName() == temp.str())
				{
					has_same = true;
					break;
				}
			}

			if (has_same == false)
			{
				name = temp.str();
				text = xui_global::ascii_to_unicode(name);
				m_namectrl->ini_textbox(text);
				break;
			}

			++number;
		}
	}

	if (m_param->GetName() != name)
	{
		m_param->SetName(name);
		stateCtrl->SetNeedSave(true);
	}
}
xui_method_explain(onity_param, on_numbctrltextchanged, void)( xui_component* sender, xui_method_args& args )
{
	std::wstringstream text(m_numbctrl->get_text());
	switch (m_param->GetType())
	{
	case DT_INT:
		{
			s32 value;
			text >> value;
			m_param->SetValueInt  (value);
		}
		break;
	case DT_FLOAT:
		{
			f32 value;
			text >> value;
			m_param->SetValueFloat(value);
		}
		break;
	}
}
xui_method_explain(onity_param, on_boolctrlclick,		void)( xui_component* sender, xui_method_args& args )
{
	m_param->SetValueBool(m_boolctrl->was_push());
}
xui_method_explain(onity_param, on_deleterenderself,	void)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax + rt.get_w()/2, rt.ay + rt.get_h()/2);

	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
}
xui_method_explain(onity_param, on_deleteclick,			void)( xui_component* sender, xui_method_args& args )
{
	if (m_parent)
	{
		onity_animator* animator = xui_dynamic_cast(onity_animator, m_parent->get_parent());
		if (animator)
		{
			onity_stateview* stateview = animator->get_stateview();
			stateview->on_delparam(m_param);
			animator->get_editprop()->get_statectrl()->DelParam(m_param);
		}
	}
}