#include "NP2DSTransition.h"
#include "NP2DSCondition.h"
#include "NP2DSParam.h"
#include "NP2DSStateCtrl.h"

#include "xui_global.h"
#include "xui_dropbox.h"
#include "xui_textbox.h"
#include "xui_itemtag.h"
#include "onity_condition.h"

class onity_param_itemtag : public xui_itemtag
{
public:
	/*
	//constructor
	*/
	onity_param_itemtag( NP2DSParam* param )
	: xui_itemtag(L"")
	, m_param(param)
	{}

	/*
	//method
	*/
	std::wstring	get_text	( void )
	{
		return xui_global::ascii_to_unicode(m_param->GetName());
	}

	NP2DSParam*		get_param	( void )
	{
		return m_param;
	}

protected:
	/*
	//member
	*/
	NP2DSParam*		m_param;
};

xui_implement_rtti(onity_condition, xui_control);

/*
//static
*/
const s32 onity_condition::default_height = 20;

/*
//constructor
*/
xui_create_explain(onity_condition)( NP2DSCondition* condition )
: xui_control(xui_vector<s32>(128, default_height))
, m_condition(condition)
{
	NP2DSParam* param = m_condition->GetLinkParam();

	m_paramname = new xui_dropbox(xui_vector<s32>(128, 18), NULL);
	NP2DSStateCtrl* stateCtrl = m_condition->GetStateCtrl();
	u32 selectedindex = -1;
	const NP2DSStateCtrl::ParamVec& vec = stateCtrl->GetParamVec();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		if (param == vec[i])
			selectedindex = i;

		onity_param_itemtag* itemtag = new onity_param_itemtag(vec[i]);
		m_paramname->add_item(itemtag);
	}
	xui_method_ptrcall(m_paramname, xm_selection	) += new xui_method_member<xui_method_args, onity_condition>(this, &onity_condition::on_paramnameselection);
	xui_method_ptrcall(m_paramname, set_parent		)(this);
	xui_method_ptrcall(m_paramname, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_paramname, set_drawcolor	)(true);
	xui_method_ptrcall(m_paramname, set_borderrt	)(xui_rect2d<s32>(4, 2, 0, 2));
	xui_method_ptrcall(m_paramname, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_paramname, set_corner		)(3);
	xui_method_ptrcall(m_paramname, set_readonly	)(true);
	xui_method_ptrcall(m_paramname, ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_paramname, ini_dropbox		)(selectedindex);

	m_boolvalue = new xui_dropbox(xui_vector<s32>(64, 18), NULL);
	xui_method_ptrcall(m_boolvalue, xm_selection	) += new xui_method_member<xui_method_args, onity_condition>(this, &onity_condition::on_boolvalueselection);
	xui_method_ptrcall(m_boolvalue,	set_parent		)(this);
	xui_method_ptrcall(m_boolvalue, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_boolvalue, set_drawcolor	)(true);
	xui_method_ptrcall(m_boolvalue, set_borderrt	)(xui_rect2d<s32>(4, 2, 0, 2));
	xui_method_ptrcall(m_boolvalue, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_boolvalue, set_corner		)(3);
	xui_method_ptrcall(m_boolvalue, set_readonly	)(true);
	xui_method_ptrcall(m_boolvalue, add_item		)(L"True");
	xui_method_ptrcall(m_boolvalue, add_item		)(L"False");
	xui_method_ptrcall(m_boolvalue, ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_boolvalue, ini_component	)(true, param->GetType() == DT_BOOL);
	xui_method_ptrcall(m_boolvalue, ini_dropbox		)(m_condition->GetConditionValueBool() ? 0 : 1);

	m_numbopera = new xui_dropbox(xui_vector<s32>(32, 18), NULL);
	xui_method_ptrcall(m_numbopera, xm_selection	) += new xui_method_member<xui_method_args, onity_condition>(this, &onity_condition::on_numboperaselection);
	xui_method_ptrcall(m_numbopera,	set_parent		)(this);
	xui_method_ptrcall(m_numbopera, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_numbopera, set_drawcolor	)(true);
	xui_method_ptrcall(m_numbopera, set_borderrt	)(xui_rect2d<s32>(4, 2, 0, 2));
	xui_method_ptrcall(m_numbopera, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_numbopera, set_corner		)(3);
	xui_method_ptrcall(m_numbopera, set_readonly	)(true);
	xui_method_ptrcall(m_numbopera, add_item		)(L"Equal");
	xui_method_ptrcall(m_numbopera, add_item		)(L"Greater");
	xui_method_ptrcall(m_numbopera, add_item		)(L"Less");
	xui_method_ptrcall(m_numbopera, add_item		)(L"EqualGreater");
	xui_method_ptrcall(m_numbopera, add_item		)(L"EqualLess");
	xui_method_ptrcall(m_numbopera, add_item		)(L"NotEqual");
	xui_method_ptrcall(m_numbopera, ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_numbopera, ini_component	)(true, param->GetType() != DT_BOOL);
	xui_method_ptrcall(m_numbopera, ini_dropbox		)((u32)m_condition->GetConditionOper());

	m_numbvalue	= new xui_textbox(xui_vector<s32>(32, 18));
	xui_method_ptrcall(m_numbvalue, xm_nonfocus		) += new xui_method_member<xui_method_args, onity_condition>(this, &onity_condition::on_ctrlnonfocus);
	xui_method_ptrcall(m_numbvalue, xm_getfocus		) += new xui_method_member<xui_method_args, onity_condition>(this, &onity_condition::on_ctrlgetfocus);
	xui_method_ptrcall(m_numbvalue, xm_textchanged	) += new xui_method_member<xui_method_args, onity_condition>(this, &onity_condition::on_numbvaluetextchanged);
	std::wstringstream	text;
	switch (param->GetType())
	{
	case DT_INT:		text << m_condition->GetConditionValueInt  (); break;
	case DT_FLOAT:		text << m_condition->GetConditionValueFloat(); break;
	}
	xui_method_ptrcall(m_numbvalue,	set_parent		)(this);
	xui_method_ptrcall(m_numbvalue,	set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_numbvalue,	set_drawcolor	)(true);
	xui_method_ptrcall(m_numbvalue, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_numbvalue,	set_borderrt	)(xui_rect2d<s32>(4, 2, 2, 2));
	xui_method_ptrcall(m_numbvalue,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_numbvalue,	set_numbtype	)(param->GetType() == DT_INT ? NT_INT : NT_FLOAT);
	xui_method_ptrcall(m_numbvalue, ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_numbvalue,	ini_component	)(true, param->GetType() != DT_BOOL);
	xui_method_ptrcall(m_numbvalue,	ini_drawer		)(text.str());

	m_widgetvec.push_back(m_paramname);
	m_widgetvec.push_back(m_boolvalue);
	m_widgetvec.push_back(m_numbopera);
	m_widgetvec.push_back(m_numbvalue);
}

/*
//method
*/
xui_method_explain(onity_condition, get_condition,				NP2DSCondition*	)( void )
{
	return m_condition;
}

/*
//callback
*/
xui_method_explain(onity_condition, on_perform,					void			)( xui_method_args& args )
{
	xui_control::on_perform(args);

	xui_rect2d<s32> rt = get_renderrtins();
	s32 x = rt.ax;
	s32 w = (rt.get_w()-4) / 2;

	//paramname
	m_paramname->on_perform_x(x);
	m_paramname->on_perform_w(w);

	//boolvalue
	x +=  w+4;
	m_boolvalue->on_perform_x(x);
	m_boolvalue->on_perform_w(w);

	//numbopera
	w  = (w-4) / 2;
	m_numbopera->on_perform_x(x);
	m_numbopera->on_perform_w(w);

	//numbvalue
	x +=  w+4;
	m_numbvalue->on_perform_x(x);
	m_numbvalue->on_perform_w(w);
}

/*
//event
*/
xui_method_explain(onity_condition, on_ctrlnonfocus,			void			)( xui_component* sender, xui_method_args& args )
{
	xui_control* ctrl = xui_dynamic_cast(xui_control, sender);
	ctrl->set_sidecolor(xui_control::default_sidecolor);
}
xui_method_explain(onity_condition, on_ctrlgetfocus,			void			)( xui_component* sender, xui_method_args& args )
{
	xui_control* ctrl = xui_dynamic_cast(xui_control, sender);
	ctrl->set_sidecolor(xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
}
xui_method_explain(onity_condition, on_paramnameselection,		void			)( xui_component* sender, xui_method_args& args )
{
	onity_param_itemtag* itemtag = (onity_param_itemtag*)m_paramname->get_selecteditem();
	NP2DSParam* param = itemtag->get_param();
	m_condition->SetLinkParam(param);

	std::wstringstream	text;
	switch (param->GetType())
	{
	case DT_INT:		text << m_condition->GetConditionValueInt  (); break;
	case DT_FLOAT:		text << m_condition->GetConditionValueFloat(); break;
	}

	xui_method_ptrcall(m_boolvalue, ini_component	)(true, param->GetType() == DT_BOOL);
	xui_method_ptrcall(m_boolvalue, ini_dropbox		)(m_condition->GetConditionValueBool() ? 0 : 1);
	xui_method_ptrcall(m_numbopera, ini_component	)(true, param->GetType() != DT_BOOL);
	xui_method_ptrcall(m_numbopera, ini_dropbox		)((u32)m_condition->GetConditionOper());
	xui_method_ptrcall(m_numbvalue,	set_numbtype	)(param->GetType() == DT_INT ? NT_INT : NT_FLOAT);
	xui_method_ptrcall(m_numbvalue,	ini_component	)(true, param->GetType() != DT_BOOL);
	xui_method_ptrcall(m_numbvalue,	ini_drawer		)(text.str());
}
xui_method_explain(onity_condition, on_boolvalueselection,		void			)( xui_component* sender, xui_method_args& args )
{
	u32 selectedindex = m_boolvalue->get_selectedindex();
	m_condition->SetConditionValueBool(selectedindex == 0);
}
xui_method_explain(onity_condition, on_numboperaselection,		void			)( xui_component* sender, xui_method_args& args )
{
	u32 selectedindex = m_numbopera->get_selectedindex();
	m_condition->SetConditionOper((npu08)selectedindex);
}
xui_method_explain(onity_condition, on_numbvaluetextchanged,	void			)( xui_component* sender, xui_method_args& args )
{
	std::wstringstream text(m_numbvalue->get_text().c_str());

	NP2DSParam* param = m_condition->GetLinkParam();
	switch (param->GetType())
	{
	case DT_INT:
		{
			nps32   value;
			text >> value;
			m_condition->SetConditionValueInt  (value);
		}
		break;
	case DT_FLOAT:
		{
			npf32   value;
			text >> value;
			m_condition->SetConditionValueFloat(value);
		}
		break;
	}
}