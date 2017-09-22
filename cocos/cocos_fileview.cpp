#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_desktop.h"
#include "xui_scroll.h"
#include "cocos_mainform.h"
#include "cocos_inspector.h"
#include "cocos_renderview.h"
#include "cocos_filedata.h"
#include "cocos_tileview.h"
#include "cocos_fileview.h"

xui_implement_rtti(cocos_fileview, xui_control);

/*
//constructor
*/
xui_create_explain(cocos_fileview)( void )
: xui_control(xui_vector<s32>(0))
, m_locknode(NULL)
{
	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0, true));
	m_lineview = new xui_treeview(xui_vector<s32>(200), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_lineview,	xm_show				) += new xui_method_member<xui_method_args,		cocos_fileview>(this, &cocos_fileview::on_lineviewshow);
	xui_method_ptrcall(m_lineview,	xm_selectedchange	) += new xui_method_member<xui_method_args,		cocos_fileview>(this, &cocos_fileview::on_fileviewselection);
	xui_method_ptrcall(m_lineview,	xm_mouseclick		) += new xui_method_member<xui_method_mouse,	cocos_fileview>(this, &cocos_fileview::on_fileviewnodeclick);
	xui_method_ptrcall(m_lineview,	xm_mousedoubleclick	) += new xui_method_member<xui_method_mouse,	cocos_fileview>(this, &cocos_fileview::on_fileviewdoubleclk);
	xui_method_ptrcall(m_lineview,	xm_mousedragitem	) += new xui_method_member<xui_method_dragdrop, cocos_fileview>(this, &cocos_fileview::on_fileviewassetdrag);
	xui_method_ptrcall(m_lineview,	xm_renderelse		) += new xui_method_member<xui_method_args,		cocos_fileview>(this, &cocos_fileview::on_lineviewrenderelse);
	xui_method_ptrcall(m_lineview,	set_parent			)(this);
	xui_method_ptrcall(m_lineview,	ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_lineview,	ini_component		)(true, true);
	xui_method_ptrcall(m_lineview,	set_acceptdrag		)(false);
	xui_method_ptrcall(m_lineview,	set_allowmulti		)(false);
	xui_method_ptrcall(m_lineview,	set_hscrollauto		)(false);

	m_tileview = new cocos_tileview;
	xui_method_ptrcall(m_tileview,	xm_mouseclick		) += new xui_method_member<xui_method_mouse,	cocos_fileview>(this, &cocos_fileview::on_fileviewnodeclick);
	xui_method_ptrcall(m_tileview,	xm_mousedoubleclick	) += new xui_method_member<xui_method_mouse,	cocos_fileview>(this, &cocos_fileview::on_fileviewdoubleclk);
	xui_method_ptrcall(m_tileview,	xm_mousedragitem	) += new xui_method_member<xui_method_dragdrop, cocos_fileview>(this, &cocos_fileview::on_fileviewassetdrag);
	xui_method_ptrcall(m_tileview,	xm_renderelse		) += new xui_method_member<xui_method_args,		cocos_fileview>(this, &cocos_fileview::on_tileviewrenderelse);
	xui_method_ptrcall(m_tileview,	set_parent			)(this);
	xui_method_ptrcall(m_tileview,	ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_tileview,	ini_component		)(true, false);

	m_widgetvec.push_back(m_tileview);
	m_widgetvec.push_back(m_lineview);

	xui_action_ctrl_impl<f32>* action = new xui_action_ctrl_impl<f32>(this);
	action->set_soft(true);
	action->add_time(0.0f);
	action->add_time(0.2f);
	action->add_time(0.4f);
	action->add_time(5.0f);
	action->add_data(1.0f);
	action->add_data(1.5f);
	action->add_data(1.0f);
	action->add_data(0.0f);
	m_lockctrl = action;
}

/*
//destructor
*/
xui_delete_explain(cocos_fileview)( void )
{
	delete m_lockctrl;
}

/*
//method
*/
xui_method_explain(cocos_fileview, set_locknode,			void			)( xui_treenode* locknode )
{
	m_locknode = locknode;
	m_lockctrl->reset();
	m_lockctrl->set_play(true);
}
xui_method_explain(cocos_fileview, get_lineview,			xui_treeview*	)( void )
{
	return m_lineview;
}
xui_method_explain(cocos_fileview, get_tileview,			cocos_tileview*	)( void )
{
	return m_tileview;
}
xui_method_explain(cocos_fileview, clear,					void			)( void )
{
	m_locknode = NULL;

	xui_scroll*   lineroll = m_lineview->get_vscroll ();
	xui_scroll*   tileroll = m_tileview->get_viewroll();
	if (lineroll) lineroll->set_value(0);
	if (tileroll) tileroll->set_value(0);

	xui_method_ptrcall(m_lineview,	del_upmostnodeall	)();
	xui_method_ptrcall(m_tileview,	set_viewfile		)(NULL, L"");
}

/*
//callback
*/
xui_method_explain(cocos_fileview, on_updateself,			void			)( xui_method_update& args )
{
	xui_control::on_updateself(args);
	if (m_locknode)
	{
		m_lockctrl->update(args.delta);
		if (m_lockctrl->was_play() == false)
			m_locknode = NULL;
	}
}
xui_method_explain(cocos_fileview, on_perform,				void			)( xui_method_args&   args )
{
	xui_control::on_perform(args);
	if (m_tileview->was_visible() == false)
		m_tileview->on_perform_sz(m_lineview->get_rendersz());
	if (m_lineview->was_visible() == false)
		m_lineview->on_perform_sz(m_tileview->get_rendersz());
}

/*
//event
*/
xui_method_explain(cocos_fileview, on_lineviewshow,			void			)( xui_component* sender, xui_method_args&     args )
{
	m_tileview->set_viewfile(NULL, L"");
}
xui_method_explain(cocos_fileview, on_lineviewrenderelse,	void			)( xui_component* sender, xui_method_args&     args )
{
	if (m_locknode == NULL)
		return;

	xui_treenode* rootnode = m_locknode->get_rootnode();
	if (rootnode && rootnode->was_expanded() == false)
		return;

	xui_rect2d<s32> rt = m_locknode->get_renderrtabs();
	rt.set_w(m_lineview->get_renderrtins().get_w());
	draw_lockround(rt);
}
xui_method_explain(cocos_fileview, on_tileviewrenderelse,	void			)( xui_component* sender, xui_method_args&     args )
{
	if (m_locknode == NULL)
		return;

	xui_treenode* rootnode = m_locknode->get_rootnode();
	if (rootnode && rootnode->was_expanded() == false)
		return;

	std::vector<xui_treenode*> nodes = m_lineview->get_entirenode(false);
	u32 index = 0;
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		if (nodes[i] == m_locknode)
		{
			index = i;
			break;
		}
	}

	cocos_renderview* drawview = m_tileview->get_drawview();
	xui_rect2d<s32> rt = drawview->get_renderrtins() + drawview->get_screenpt();
	s32 s, c, g, w, h;
	m_tileview->get_tileinfo(s, c, g, w, h);
	s32 ic = index % c;
	s32 ir = index / c;

	rt.oft_x(ic*w);
	rt.oft_y(ir*h - m_tileview->get_viewroll()->get_value());
	rt.set_w(s);
	rt.set_h(m_tileview->get_tilesize());
	draw_lockround(rt);
}
xui_method_explain(cocos_fileview, on_fileviewselection,	void			)( xui_component* sender, xui_method_args&     args )
{
	xm_fileviewselection(this, args);
}
xui_method_explain(cocos_fileview, on_fileviewnodeclick,	void			)( xui_component* sender, xui_method_mouse&    args )
{
	if (args.mouse == MB_L)
	{
		xui_vector<s32> delta = args.point - xui_desktop::get_ins()->get_mousedown();
		if ((xui_abs(delta.x) < 4) && (xui_abs(delta.y) < 4))
		{
			xm_fileviewnodeclick(this, args);
		}
	}
}
xui_method_explain(cocos_fileview, on_fileviewdoubleclk,	void			)( xui_component* sender, xui_method_mouse&	   args )
{
	xm_fileviewdoubleclk(this, args);
}
xui_method_explain(cocos_fileview, on_fileviewassetdrag,	void			)( xui_component* sender, xui_method_dragdrop& args )
{
	xm_fileviewassetdrag(this, args);
}

/*
//method
*/
xui_method_explain(cocos_fileview, draw_lockround,			void			)( const xui_rect2d<s32>& rt )
{
	xui_action_ctrl_impl<f32>* action = (xui_action_ctrl_impl<f32>*)m_lockctrl;
	f32 sa = action->sample();
	f32 s  = xui_max(sa, 1.0f);
	f32 ox = -0.5f * rt.get_w() * s + 0.5f * rt.get_w();
	f32 oy = -0.5f * rt.get_h() * s + 0.5f * rt.get_h();
	f32 fw =  s    * rt.get_w();
	f32 fh =  s    * rt.get_h();
	xui_rect2d<s32> drawrt = rt;
	drawrt.oft_x((s32)ox);
	drawrt.oft_y((s32)oy);
	drawrt.set_w((s32)fw);
	drawrt.set_h((s32)fh);
	xui_convas::get_ins()->draw_round(drawrt, xui_colour(sa, 1.0f, 0.0f, 0.0f), xui_rect2d<s32>(3), 3);
}