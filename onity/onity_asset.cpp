#include "xui_panel.h"
#include "xui_toolbar.h"
#include "xui_button.h"
#include "xui_convas.h"
#include "xui_global.h"
#include "xui_desktop.h"
#include "xui_treenode.h"
#include "onity_treedata.h"
#include "onity_bounding.h"
#include "onity_resource.h"
#include "onity_renderview.h"
#include "onity_gradpane.h"
#include "onity_snaptool.h"
#include "onity_asset.h"

extern bool gInitCompleted;
xui_implement_rtti(onity_asset, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_asset)( void )
: xui_dockpage(xui_vector<s32>(200), AREALIMIT_A, 200, DOCKSTYLE_F)
, m_trans(xui_vector<s32>(0))
, m_ratio(1.0)
, m_multisel(false)
, m_dragview(false)
, m_dragprop(false)
{
	m_linetool	= new xui_toolbar(xui_vector<s32>(0, 20));
	xui_method_ptrcall(m_linetool,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	m_headpane  = new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_headpane,	ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_headpane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_headpane,	set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_headpane,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_headpane,	add_child		)(m_linetool);

	m_drawview	= new onity_renderview(xui_vector<s32>(100), xui_vector<s32>(2048));
	xui_method_ptrcall(m_drawview,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_drawview,	xm_updateself	) += new xui_method_member<xui_method_update,	onity_asset>(this, &onity_asset::on_drawviewupdateself);
	xui_method_ptrcall(m_drawview,	xm_renderself	) += new xui_method_member<xui_method_args,		onity_asset>(this, &onity_asset::on_drawviewrenderself);
	xui_method_ptrcall(m_drawview,	xm_renderelse	) += new xui_method_member<xui_method_args,		onity_asset>(this, &onity_asset::on_drawviewrenderelse);
	xui_method_ptrcall(m_drawview,	xm_noncatch		) += new xui_method_member<xui_method_args,		onity_asset>(this, &onity_asset::on_drawviewnoncatch);
	xui_method_ptrcall(m_drawview,	xm_mouseenter	) += new xui_method_member<xui_method_mouse,	onity_asset>(this, &onity_asset::on_drawviewmouseenter);
	xui_method_ptrcall(m_drawview,	xm_mousedown	) += new xui_method_member<xui_method_mouse,	onity_asset>(this, &onity_asset::on_drawviewmousedown);
	xui_method_ptrcall(m_drawview,	xm_mousemove	) += new xui_method_member<xui_method_mouse,	onity_asset>(this, &onity_asset::on_drawviewmousemove);
	xui_method_ptrcall(m_drawview,	xm_mouserise	) += new xui_method_member<xui_method_mouse,	onity_asset>(this, &onity_asset::on_drawviewmouserise);
	xui_method_ptrcall(m_drawview,	xm_mousewheel	) += new xui_method_member<xui_method_mouse,	onity_asset>(this, &onity_asset::on_drawviewmousewheel);
	m_horzgrad	= new onity_gradpane(FLOWSTYLE_H);
	m_vertgrad	= new onity_gradpane(FLOWSTYLE_V);
	m_drawpane	= new xui_panel(xui_vector<s32>(100));
	xui_method_ptrcall(m_drawpane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_drawpane,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_drawpane,	set_vscrollauto	)(false);
	xui_method_ptrcall(m_drawpane,	add_child		)(m_drawview);

	m_small		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_small,		ini_component	)(ALIGNHORZ_L, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_small,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_small,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_small,		ini_drawer		)(onity_resource::icon_small,  xui_vector<s32>(12));
	xui_method_ptrcall(m_small,		xm_buttonclick	) += new xui_method_member<xui_method_args,		onity_asset>(this, &onity_asset::on_buttonclick);
	m_large		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_large,		ini_component	)(ALIGNHORZ_L, ALIGNVERT_T, 0);
	xui_method_ptrcall(m_large,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_large,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_large,		ini_drawer		)(onity_resource::icon_large,  xui_vector<s32>(12));
	xui_method_ptrcall(m_large,		xm_buttonclick	) += new xui_method_member<xui_method_args,		onity_asset>(this, &onity_asset::on_buttonclick);
	m_reset		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_reset,		ini_component	)(ALIGNHORZ_R, ALIGNVERT_T, 0);
	xui_method_ptrcall(m_reset,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_reset,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_reset,		ini_drawer		)(onity_resource::icon_reset,  xui_vector<s32>(12));
	xui_method_ptrcall(m_reset,		xm_buttonclick	) += new xui_method_member<xui_method_args,		onity_asset>(this, &onity_asset::on_buttonclick);
	m_clear		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		ini_component	)(ALIGNHORZ_R, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_clear,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_clear,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_clear,		ini_drawer		)(onity_resource::icon_delete, xui_vector<s32>(12));
	xui_method_ptrcall(m_clear,		xm_buttonclick	) += new xui_method_member<xui_method_args,		onity_asset>(this, &onity_asset::on_buttonclick);

	m_cubepane  = new xui_panel(xui_vector<s32>(40));
	xui_method_ptrcall(m_cubepane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_cubepane,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_cubepane,	set_vscrollauto	)(false);
	xui_method_ptrcall(m_cubepane,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_cubepane,	add_child		)(m_small);
	xui_method_ptrcall(m_cubepane,	add_child		)(m_large);
	xui_method_ptrcall(m_cubepane,	add_child		)(m_reset);
	xui_method_ptrcall(m_cubepane,	add_child		)(m_clear);

	m_fillpane  = new xui_panel(xui_vector<s32>(100));
	xui_method_ptrcall(m_fillpane,	xm_keybddown	) += new xui_method_member<xui_method_keybd,	onity_asset>(this, &onity_asset::on_fillpanekeybddown);
	xui_method_ptrcall(m_fillpane,	xm_perform		) += new xui_method_member<xui_method_args,		onity_asset>(this, &onity_asset::on_fillpaneperform);
	xui_method_ptrcall(m_fillpane,	xm_renderelse	) += new xui_method_member<xui_method_args,		onity_asset>(this, &onity_asset::on_fillpanerenderelse);
	xui_method_ptrcall(m_fillpane,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_fillpane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_fillpane,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_fillpane,	set_vscrollauto	)(false);
	xui_method_ptrcall(m_fillpane,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_fillpane,	set_sidecolor	)(xui_colour::black);
	xui_method_ptrcall(m_fillpane,	add_child		)(m_cubepane);
	xui_method_ptrcall(m_fillpane,	add_child		)(m_horzgrad);
	xui_method_ptrcall(m_fillpane,	add_child		)(m_vertgrad);
	xui_method_ptrcall(m_fillpane,	add_child		)(m_drawpane);

	m_animctrl	= new xui_action_ctrl_impl<f64>(this);
	xui_method_ptrcall(m_animctrl,	set_soft		)(true);
	xui_method_ptrcall(m_animctrl,	xm_tick			) += new xui_method_member<xui_method_args,		onity_asset>(this, &onity_asset::on_animctrltick);

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
xui_delete_explain(onity_asset)( void )
{
	delete m_animctrl;
	delete m_lockctrl;
}

/*
//method
*/
xui_method_explain(onity_asset, get_trans,					const xui_vector<s32>&	)( void ) const
{
	return m_trans;
}
xui_method_explain(onity_asset, get_ratio,					f64						)( void ) const
{
	return m_ratio;
}
xui_method_explain(onity_asset, set_trans,					void					)( const xui_vector<s32>& trans )
{
	m_trans = trans;
	m_horzgrad->set_trans(trans.x);
	m_vertgrad->set_trans(trans.y);
}
xui_method_explain(onity_asset, set_ratio,					void					)( f64 ratio )
{
	m_ratio = ratio;
	m_horzgrad->set_ratio(ratio);
	m_vertgrad->set_ratio(ratio);
	m_animctrl->clear();
}
xui_method_explain(onity_asset, set_nodevisible,			void					)( onity_bounding* prop )
{
	//onity_treedata* data  = (onity_treedata*)node->get_linkdata();
	//onity_editrect* edit  = (onity_editrect*)data->get_prop();
	xui_rect2d<s32> rt    = prop->ori_bounding();
	xui_vector<s32> start = -m_trans;
	xui_vector<s32> range = (m_drawview->get_rendersz().to<f64>() / m_ratio).to<s32>();
	if (start.x > rt.ax)
		start.x = rt.ax;
	if (start.y > rt.ay)
		start.y = rt.ay;
	if (start.x+range.w < rt.bx)
		start.x = rt.bx-range.w;
	if (start.y+range.h < rt.by)
		start.y = rt.by-range.h;

	set_trans(-start);

	m_lockctrl->reset();
	m_lockctrl->set_play(true);
}
xui_method_explain(onity_asset, set_toolupdate,				void					)( void )
{}
xui_method_explain(onity_asset, hit_propvisible,			onity_proproot*			)( const xui_vector<s32>& pt )
{
	return NULL;
}

/*
//event
*/
xui_method_explain(onity_asset, on_buttonclick,				void					)( xui_component* sender, xui_method_args&		args )
{
	if (sender == m_large)
	{
		set_ratio(get_incratio());
	}
	else
	if (sender == m_small)
	{
		set_ratio(get_decratio());
	}
	else
	if (sender == m_reset)
	{
		set_trans(xui_vector<s32>(0));
		set_ratio(1.0);
	}
	else
	if (sender == m_clear)
	{
		m_horzgrad->del_lines();
		m_vertgrad->del_lines();
	}

	m_drawview->req_focus();
}
xui_method_explain(onity_asset, on_fillpaneperform,			void					)( xui_component* sender, xui_method_args&		args )
{
	xui_method_ptrcall(m_horzgrad, on_perform_x)(m_vertgrad->get_renderw());
	xui_method_ptrcall(m_vertgrad, on_perform_y)(m_horzgrad->get_renderh());
	xui_method_ptrcall(m_drawpane, on_perform_x)(m_vertgrad->get_renderw());
	xui_method_ptrcall(m_drawpane, on_perform_y)(m_horzgrad->get_renderh());
	xui_method_ptrcall(m_horzgrad, on_perform_w)(m_fillpane->get_renderw()-m_horzgrad->get_renderx());
	xui_method_ptrcall(m_vertgrad, on_perform_h)(m_fillpane->get_renderh()-m_vertgrad->get_rendery());
	xui_method_ptrcall(m_drawpane, on_perform_w)(m_horzgrad->get_renderw());
	xui_method_ptrcall(m_drawpane, on_perform_h)(m_vertgrad->get_renderh());
}
xui_method_explain(onity_asset, on_fillpanerenderelse,		void					)( xui_component* sender, xui_method_args&		args )
{
	xui_vector<s32> p1;
	xui_vector<s32> p2;
	xui_vector<s32> pt = m_fillpane->get_screenpt();
	p1 = xui_vector<s32>(0,							m_horzgrad->get_renderh());
	p2 = xui_vector<s32>(m_fillpane->get_renderw(), m_horzgrad->get_renderh());
	xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour::black);
	p1 = xui_vector<s32>(m_vertgrad->get_renderw(), 0);
	p2 = xui_vector<s32>(m_vertgrad->get_renderw(), m_fillpane->get_renderh());
	xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour::black);
}
xui_method_explain(onity_asset, on_fillpanekeybddown,		void					)( xui_component* sender, xui_method_keybd&		args )
{
	if (args.kcode == KEY_LARROW ||
		args.kcode == KEY_RARROW ||
		args.kcode == KEY_UARROW ||
		args.kcode == KEY_DARROW)
	{
		xui_vector<s32> delta(0);
		switch (args.kcode)
		{
		case KEY_LARROW: delta.x = -1; break;
		case KEY_RARROW: delta.x =  1; break;
		case KEY_UARROW: delta.y = -1; break;
		case KEY_DARROW: delta.y =  1; break;
		}

		on_keybdmoveimpl(delta);
	}
}
xui_method_explain(onity_asset, on_drawviewnoncatch,		void					)( xui_component* sender, xui_method_args&		args )
{
	m_dragview = false;
	m_dragprop = false;
	m_multisel = false;
	m_drawview->set_cursor(CURSOR_DEFAULT);
	xui_global::set_cursor(CURSOR_DEFAULT);
}
xui_method_explain(onity_asset, on_drawviewupdateself,		void					)( xui_component* sender, xui_method_update&	args )
{
	m_animctrl->update(args.delta);
	m_lockctrl->update(args.delta);
}
xui_method_explain(onity_asset, on_drawviewrenderself,		void					)( xui_component* sender, xui_method_args&		args )
{}
xui_method_explain(onity_asset, on_drawviewrenderelse,		void					)( xui_component* sender, xui_method_args&		args )
{}
xui_method_explain(onity_asset, on_drawviewmouseenter,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	m_drawview->req_focus();
}
xui_method_explain(onity_asset, on_drawviewmousedown,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	if (gInitCompleted == false)
		return;

	if (args.mouse == MB_L)
	{
		onity_proproot* pick = hit_propvisible(m_drawview->get_renderpt(args.point));
		if (pick)
		{
			on_mousepickimpl(
				pick, 
				args.alt,
				args.ctrl,
				args.shift);
		}
		else
		{
			m_multisel = true;
		}
	}
	else
	if (args.mouse == MB_R)
	{
		m_dragview  = true;
		m_drawview->set_cursor(CURSOR_HAND);
		xui_global::set_cursor(CURSOR_HAND);
		xui_desktop::get_ins()->set_catchctrl(m_drawview);
	}
}
xui_method_explain(onity_asset, on_drawviewmousemove,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	xui_vector<s32> delta(0);
	xui_vector<s32> drag = xui_desktop::get_ins()->get_mousemove();

	if (m_dragview)
	{
		delta.x = xui_round(drag.x/m_ratio);
		delta.y = xui_round(drag.y/m_ratio);
		set_trans(m_trans+delta);
	}
	else
	if (m_dragprop)
	{
		u08 mode = DRAGMOVE_UNLIMIT;
		if		(args.shift && args.ctrl)	mode = DRAGMOVE_Y;
		else if (args.shift)				mode = DRAGMOVE_X;
		else
		{}

		if (mode == DRAGMOVE_UNLIMIT || 
			mode == DRAGMOVE_X)
			delta.x = drag.x;
		if (mode == DRAGMOVE_UNLIMIT || 
			mode == DRAGMOVE_Y)
			delta.y = drag.y;

		on_mousedragimpl(delta);
	}
}
xui_method_explain(onity_asset, on_drawviewmouserise,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	if (gInitCompleted == false)
		return;

	if (args.mouse == MB_L)
	{
		if (m_multisel)
		{
			xui_vector<s32>  p1 = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousedown());
			xui_vector<s32>  p2 = m_drawview->get_renderpt(args.point);
			xui_rect2d<s32>  rt;
			rt.ax  = xui_min(p1.x, p2.x);
			rt.bx  = xui_max(p1.x, p2.x);
			rt.ay  = xui_min(p1.y, p2.y);
			rt.by  = xui_max(p1.y, p2.y);
			on_mulselectimpl(rt, args.ctrl);
		}
	}
	else
	if (args.mouse == MB_R)
	{
		xui_desktop::get_ins()->set_catchctrl(NULL);
	}
}
xui_method_explain(onity_asset, on_drawviewmousewheel,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	f64 start = m_ratio;
	f64 final = m_ratio;
	if (args.wheel > 0) final = get_incratio();
	else 				final = get_decratio();

	xui_action_ctrl_impl<f64>* action = (xui_action_ctrl_impl<f64>*)m_animctrl;
	action->clear();
	action->add_time(0.0f);
	action->add_time(0.5f);
	action->add_data(start);
	action->add_data(final);
	action->set_play(true);
}
xui_method_explain(onity_asset, on_animctrltick,			void					)( xui_component* sender, xui_method_args&		args )
{
	xui_action_ctrl_impl<f64>* action = (xui_action_ctrl_impl<f64>*)m_animctrl;
	m_ratio = action->sample();
	m_horzgrad->set_ratio(m_ratio);
	m_vertgrad->set_ratio(m_ratio);
	if (m_ratio == 0.1 || m_ratio == 5.0)
		m_animctrl->clear();
}

/*
//method
*/
xui_method_explain(onity_asset, get_incratio,				f64						)( void )
{
	xui_action_ctrl_impl<f64>* action = (xui_action_ctrl_impl<f64>*)m_animctrl;
	f64 ratio = action->has_data() ? action->get_data(1) : m_ratio;
	if (ratio < 1.0)	ratio += 0.1;
	else				ratio += 1.0;

	return xui_min(ratio, 5.0);
}
xui_method_explain(onity_asset, get_decratio,				f64						)( void )
{
	xui_action_ctrl_impl<f64>* action = (xui_action_ctrl_impl<f64>*)m_animctrl;
	f64 ratio = action->has_data() ? action->get_data(1) : m_ratio;
	if (ratio <= 1.0)	ratio -= 0.1;
	else				ratio -= 1.0;

	return xui_max(ratio, 0.1);
}

/*
//virtual
*/
xui_method_explain(onity_asset, on_keybdmoveimpl,			void					)( const xui_vector<s32>& delta )
{}
xui_method_explain(onity_asset, on_mousepickimpl,			void					)( onity_proproot* pick, bool alt, bool ctrl, bool shift )
{}
xui_method_explain(onity_asset, on_mousedragimpl,			void					)( const xui_vector<s32>& delta )
{}
xui_method_explain(onity_asset, on_mulselectimpl,			void					)( const xui_rect2d<s32>& rt, bool ctrl )
{}

/*
//drawsnap
*/
xui_method_explain(onity_asset, draw_locknode,				void					)( const xui_rect2d<s32>& rt )
{
	if (m_lockctrl->was_play())
	{
		xui_action_ctrl_impl<f32>* action = (xui_action_ctrl_impl<f32>*)m_lockctrl;
		f32 sa = action->sample();
		f32 s  = xui_max(sa, 1.0f);
		f32 ox = -0.5f * rt.get_w() * s + 0.5f * rt.get_w();
		f32 oy = -0.5f * rt.get_h() * s + 0.5f * rt.get_h();
		f32 fw =  s    * rt.get_w();
		f32 fh =  s    * rt.get_h();
		xui_vector<s32> drawpt = m_drawview->get_screenpt();
		xui_rect2d<s32> drawrt = rt + drawpt;
		drawrt.oft_x((s32)ox);
		drawrt.oft_y((s32)oy);
		drawrt.set_w((s32)fw);
		drawrt.set_h((s32)fh);
		xui_convas::get_ins()->draw_round(drawrt, xui_colour(sa, 1.0f, 0.0f, 0.0f), xui_rect2d<s32>(3), 3);
	}
}
xui_method_explain(onity_asset, draw_multisel,				void					)( void )
{
	if (m_multisel)
	{
		xui_vector<s32> pt = m_drawview->get_screenpt();
		xui_vector<s32> p1 = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousedown());
		xui_vector<s32> p2 = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousecurr());
		xui_rect2d<s32> rt;
		rt.ax = xui_min(p1.x, p2.x);
		rt.bx = xui_max(p1.x, p2.x);
		rt.ay = xui_min(p1.y, p2.y);
		rt.by = xui_max(p1.y, p2.y);
		xui_convas::get_ins()->fill_rectangle(rt+pt, xui_colour(0.5f, 42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
		xui_convas::get_ins()->draw_rectangle(rt+pt, xui_colour(1.0f, 0.0f, 0.9f, 0.9f));
	}
}