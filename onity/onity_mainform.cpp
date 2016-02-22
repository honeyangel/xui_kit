#include "xui_bitmap.h"
#include "xui_toggle.h"
#include "xui_linebox.h"
#include "onity_mainform.h"

/*
//constructor
*/
xui_create_explain(onity_mainform)( void )
: xui_window(xui_vector<s32>(0))
{
	m_dockstyle		= DOCKSTYLE_F;
	m_border		= xui_rect2d<s32>(6);

	m_select		= xui_toggle::create(xui_bitmap::create("icon/select.png"),		32);
	m_translate		= xui_toggle::create(xui_bitmap::create("icon/translate.png"),	32);
	m_scale			= xui_toggle::create(xui_bitmap::create("icon/scale.png"),		32);
	m_rotation		= xui_toggle::create(xui_bitmap::create("icon/rotation.png"),	32);
	xui_method_ptrcall(m_select,		xm_click) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clicktransform);
	xui_method_ptrcall(m_translate,		xm_click) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clicktransform);
	xui_method_ptrcall(m_scale,			xm_click) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clicktransform);
	xui_method_ptrcall(m_rotation,		xm_click) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clicktransform);
	xui_method_ptrcall(m_translate,		ini_toggle		)(true);

	//transform
	xui_linebox* line_transform = xui_linebox::create();
	xui_method_ptrcall(line_transform,	ini_component)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_select);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_translate);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_scale);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_rotation);

	m_anchor		= xui_toggle::create(xui_bitmap::create("icon/pivot.png"), L"Pivot", 80);
	m_coordinate	= xui_toggle::create(NULL, L"Local", 80);
	xui_method_ptrcall(m_anchor,		set_downcolor	)(xui_button::default_backcolor);
	xui_method_ptrcall(m_coordinate,	set_downcolor	)(xui_button::default_backcolor);
	xui_method_ptrcall(m_anchor,		xm_click) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickanchor);
	xui_method_ptrcall(m_coordinate,	xm_click) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickcoordinate);

	//anchor
	//coordinate
	xui_linebox* line_operator = xui_linebox::create();
	xui_method_ptrcall(line_operator,	ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(line_operator,	set_renderpt	)(xui_vector<s32>(160, 0));
	xui_method_ptrcall(line_operator,	add_linectrl	)(m_anchor);
	xui_method_ptrcall(line_operator,	add_linectrl	)(m_coordinate);

	//run
	//pause
	//layout

	//panel
	xui_panel* top_panel = new xui_panel(xui_vector<s32>(24));
	xui_method_ptrcall(top_panel,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(top_panel,		set_borderrt	)(xui_rect2d<s32>(8, 0, 8, 0));
	xui_method_ptrcall(top_panel,		add_child		)(line_transform);
	xui_method_ptrcall(top_panel,		add_child		)(line_operator);

	add_child(top_panel);
}

/*
//event
*/
xui_method_explain(onity_mainform, on_clicktransform,	void)( xui_component* sender, xui_method_args& args )
{
	xui_toggle* operator_toggle[4];
	operator_toggle[0] = m_select;
	operator_toggle[1] = m_translate;
	operator_toggle[2] = m_scale;
	operator_toggle[3] = m_rotation;
	for (u32 i = 0; i < 4; ++i)
	{
		operator_toggle[i]->ini_toggle(sender == operator_toggle[i]);
	}
}
xui_method_explain(onity_mainform, on_clickanchor,		void)( xui_component* sender, xui_method_args& args )
{
	xui_toggle* toggle = xui_dynamic_cast(xui_toggle, sender);
	if (toggle->was_push())
	{
		toggle->ini_drawer(xui_bitmap::create("icon/center.png"));
		toggle->ini_drawer(L"Center");
	}
	else
	{
		toggle->ini_drawer(xui_bitmap::create("icon/pivot.png"));
		toggle->ini_drawer(L"Pivot");
	}
}
xui_method_explain(onity_mainform, on_clickcoordinate,	void)( xui_component* sender, xui_method_args& args )
{
	//xui_toggle* toggle = xui_dynamic_cast(xui_toggle, sender);
	//if (toggle->was_push()) toggle->set_icon(NULL);
	//else					toggle->set_icon(NULL);
}