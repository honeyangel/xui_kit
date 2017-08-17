#include "xui_panel.h"
#include "xui_drawer.h"
#include "xui_button.h"
#include "xui_plusctrl.h"
#include "cocos_toolpane.h"

xui_implement_rtti(cocos_toolpane, xui_control);

const xui_vector<s32> tool_size = xui_vector<s32>(80);
const xui_vector<s32> tool_icon = xui_vector<s32>(48);

/*
//constructor
*/
xui_create_explain(cocos_toolpane)( const std::wstring& name )
: xui_control(xui_vector<s32>(20))
{
	m_plusctrl = new xui_plusctrl(PLUSRENDER_NORMAL, true);
	xui_method_ptrcall(m_plusctrl, ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, DOCKSTYLE_U);
	xui_method_ptrcall(m_plusctrl, ini_component	)(true, true);
	xui_method_ptrcall(m_plusctrl, xm_expand		) += new xui_method_member<xui_method_args, cocos_toolpane>(this, &cocos_toolpane::on_plusctrlexpand);
	m_namectrl = new xui_drawer(xui_vector<s32>(20));
	xui_method_ptrcall(m_namectrl, ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_namectrl, ini_drawer		)(name);
	xui_method_ptrcall(m_namectrl, set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_namectrl, set_textoffset	)(xui_vector<s32>(20, 0));
	m_nameline = new xui_panel(xui_vector<s32>(24));
	xui_method_ptrcall(m_nameline, ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_nameline, set_parent		)(this);
	xui_method_ptrcall(m_nameline, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_nameline, set_drawcolor	)(false);
	xui_method_ptrcall(m_nameline, set_hscrollauto	)(false);
	xui_method_ptrcall(m_nameline, set_vscrollauto	)(false);
	xui_method_ptrcall(m_nameline, add_child		)(m_namectrl);
	xui_method_ptrcall(m_nameline, add_child		)(m_plusctrl);
	m_toolline = new xui_panel(xui_vector<s32>(20));
	xui_method_ptrcall(m_toolline, ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_toolline, set_parent		)(this);
	xui_method_ptrcall(m_toolline, set_borderrt		)(xui_rect2d<s32>(10));
	xui_method_ptrcall(m_toolline, set_drawcolor	)(false);
	xui_method_ptrcall(m_toolline, set_hscrollauto	)(false);
	xui_method_ptrcall(m_toolline, set_vscrollauto	)(false);
	xui_method_ptrcall(m_toolline, xm_invalid		) += new xui_method_member<xui_method_args, cocos_toolpane>(this, &cocos_toolpane::on_toollineinvalid);
	xui_method_ptrcall(m_toolline, xm_perform		) += new xui_method_member<xui_method_args, cocos_toolpane>(this, &cocos_toolpane::on_toollineperform);	

	m_widgetvec.push_back(m_nameline);
	m_widgetvec.push_back(m_toolline);
}

/*
//method
*/
xui_method_explain(cocos_toolpane, add_toolctrl,			void)( xui_bitmap* icon, const std::wstring& name )
{
	xui_button* button = new xui_button(tool_size);
	xui_method_ptrcall(button, ini_drawer		)(icon);
	xui_method_ptrcall(button, ini_drawer		)(name);
	xui_method_ptrcall(button, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(button, set_drawcolor	)(true);
	xui_method_ptrcall(button, set_backcolor	)(xui_colour::transparent);
	xui_method_ptrcall(button, set_iconsize		)(tool_icon);
	xui_method_ptrcall(button, set_iconalign	)(IMAGE_ABOVE_TEXT);
	xui_method_ptrcall(button, set_textalign	)(TEXTALIGN_CB);
	xui_method_ptrcall(button, xm_mousedragitem	) += new xui_method_member<xui_method_dragdrop, cocos_toolpane>(this, &cocos_toolpane::on_toolctrlmousedrag);

	m_toolline->add_child(button);
}

/*
//override
*/
xui_method_explain(cocos_toolpane, on_invalid,				void)( xui_method_args& args )
{
	xui_vector<s32> sz = get_rendersz();
	sz.h = m_nameline->get_renderh() + (m_toolline->was_visible() ? m_toolline->get_renderh() : 0);

	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}

/*
//event
*/
xui_method_explain(cocos_toolpane, on_plusctrlexpand,		void)( xui_component* sender, xui_method_args&		args )
{
	m_toolline->set_visible(m_plusctrl->was_expanded());
	refresh();
}
xui_method_explain(cocos_toolpane, on_toollineinvalid,		void)( xui_component* sender, xui_method_args&		args )
{
	xui_rect2d<s32> bd = m_toolline->get_borderrt();
	xui_rect2d<s32> rt = m_toolline->get_renderrtins();
	s32 col = rt.get_w() / tool_size.w;
	s32 row = (s32)ceil((f64)m_toolline->get_childcount() / col);
	xui_vector<s32> sz = get_rendersz();
	sz.h = bd.ay + bd.by + tool_size.h * row;

	if (m_toolline->get_rendersz() != sz)
	{
		m_toolline->set_rendersz(sz);
	}
	else
	{
		m_toolline->perform();
	}
}
xui_method_explain(cocos_toolpane, on_toollineperform,		void)( xui_component* sender, xui_method_args&		args )
{
	std::vector<xui_control*> children = m_toolline->get_children();
	xui_rect2d<s32> rt = m_toolline->get_renderrtins();
	xui_vector<s32> pt(rt.ax, rt.ay);
	for (u32 i = 0; i < children.size(); ++i)
	{
		children[i]->on_perform_pt(pt);
		pt.x += tool_size.w;

		if (pt.x  + tool_size.w > rt.bx)
		{
			pt.x  = rt.ax;
			pt.y += tool_size.h;
		}
	}
}
xui_method_explain(cocos_toolpane, on_toolctrlmousedrag,	void)( xui_component* sender, xui_method_dragdrop&	args )
{
	args.allow = true;
	args.type  = "Toolbox";
	args.data  = sender;
}