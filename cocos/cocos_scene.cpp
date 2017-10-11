#include "2d/CCScene.h"
#include "2d/CCNode.h"
#include "2d/CCSpriteFrame.h"
#include "2d/WeCChartFontManager.h"
#include "base/CCDirector.h"

#include "xui_panel.h"
#include "xui_toolbar.h"
#include "xui_button.h"
#include "xui_convas.h"
#include "xui_global.h"
#include "xui_toggle.h"
#include "xui_desktop.h"
#include "xui_treenode.h"
#include "xui_linebox.h"
#include "cocos_mainform.h"
#include "cocos_hierarchy.h"
#include "cocos_propcsd.h"
#include "cocos_propnodebase.h"
#include "cocos_treedata.h"
#include "cocos_filedata.h"
#include "cocos_boundbox.h"
#include "cocos_resource.h"
#include "cocos_renderview.h"
#include "cocos_gradpane.h"
#include "cocos_snaptool.h"
#include "cocos_alignbox.h"
#include "cocos_blankbox.h"
#include "cocos_scene.h"

xui_implement_rtti(cocos_scene, xui_dockpage);

/*
//constructor
*/
xui_create_explain(cocos_scene)( cocos_propcsd* prop )
: xui_dockpage(xui_vector<s32>(200), AREALIMIT_N, 200, DOCKSTYLE_F)
, m_trans(xui_vector<s32>(0))
, m_ratio(1.0)
, m_multisel(false)
, m_dragview(false)
, m_operator(0)
, m_editprop(prop)
{
	ini_namectrl(cocos_resource::icon_scene, cocos_filedata::get_safe(m_editprop->get_fullname()));
	set_autofree(true);

	m_showbbox	= new xui_toggle(xui_vector<s32>(20, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_showbbox,	ini_drawer		)(cocos_resource::icon_visible);
	xui_method_ptrcall(m_showbbox,	set_drawcolor	)(true);
	xui_method_ptrcall(m_showbbox,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_showbbox,	set_corner		)(3);

	m_drawview = new cocos_renderview(xui_vector<s32>(100), xui_vector<s32>(2048));
	xui_method_ptrcall(m_drawview,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_drawview,	xm_updateself	) += new xui_method_member<xui_method_update,	cocos_scene>(this, &cocos_scene::on_drawviewupdateself);
	xui_method_ptrcall(m_drawview,	xm_renderself	) += new xui_method_member<xui_method_args,		cocos_scene>(this, &cocos_scene::on_drawviewrenderself);
	xui_method_ptrcall(m_drawview,	xm_renderelse	) += new xui_method_member<xui_method_args,		cocos_scene>(this, &cocos_scene::on_drawviewrenderelse);
	xui_method_ptrcall(m_drawview,	xm_noncatch		) += new xui_method_member<xui_method_args,		cocos_scene>(this, &cocos_scene::on_drawviewnoncatch);
	xui_method_ptrcall(m_drawview,	xm_mouseenter	) += new xui_method_member<xui_method_mouse,	cocos_scene>(this, &cocos_scene::on_drawviewmouseenter);
	xui_method_ptrcall(m_drawview,	xm_mousedown	) += new xui_method_member<xui_method_mouse,	cocos_scene>(this, &cocos_scene::on_drawviewmousedown);
	xui_method_ptrcall(m_drawview,	xm_mousemove	) += new xui_method_member<xui_method_mouse,	cocos_scene>(this, &cocos_scene::on_drawviewmousemove);
	xui_method_ptrcall(m_drawview,	xm_mouserise	) += new xui_method_member<xui_method_mouse,	cocos_scene>(this, &cocos_scene::on_drawviewmouserise);
	xui_method_ptrcall(m_drawview,	xm_mousewheel	) += new xui_method_member<xui_method_mouse,	cocos_scene>(this, &cocos_scene::on_drawviewmousewheel);
	xui_method_ptrcall(m_drawview,	xm_mousedragover) += new xui_method_member<xui_method_dragdrop, cocos_scene>(this, &cocos_scene::on_drawviewmousedragover);
	xui_method_ptrcall(m_drawview,	xm_mousedragdrop) += new xui_method_member<xui_method_dragdrop, cocos_scene>(this, &cocos_scene::on_drawviewmousedragdrop);

	m_linetool	= new xui_toolbar(xui_vector<s32>(0, 20));
	xui_method_ptrcall(m_linetool,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	m_snaptool = new cocos_snaptool(m_drawview);
	m_alignbox = new cocos_alignbox(xui_vector<s32>(20), get_selectedvec);
	m_blankbox = new cocos_blankbox(xui_vector<s32>(20), get_selectedvec);
	xui_method_ptrcall(m_linetool, add_item			)(m_showbbox);
	xui_method_ptrcall(m_linetool, add_item			)(m_snaptool->get_snapctrl());
	xui_method_ptrcall(m_linetool, add_separate		)();
	xui_method_ptrcall(m_linetool, add_item			)(m_alignbox->get_horzline());
	xui_method_ptrcall(m_linetool, add_separate		)();
	xui_method_ptrcall(m_linetool, add_item			)(m_alignbox->get_vertline());
	xui_method_ptrcall(m_linetool, add_separate		)();
	xui_method_ptrcall(m_linetool, add_item			)(m_blankbox->get_horzline());
	xui_method_ptrcall(m_linetool, add_separate		)();
	xui_method_ptrcall(m_linetool, add_item			)(m_blankbox->get_vertline());

	m_headpane  = new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_headpane,	ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_headpane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_headpane,	set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_headpane,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_headpane,	add_child		)(m_linetool);

	m_horzgrad	= new cocos_gradpane(FLOWSTYLE_H);
	m_vertgrad	= new cocos_gradpane(FLOWSTYLE_V);
	m_drawpane	= new xui_panel(xui_vector<s32>(100));
	xui_method_ptrcall(m_drawpane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_drawpane,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_drawpane,	set_vscrollauto	)(false);
	xui_method_ptrcall(m_drawpane,	add_child		)(m_drawview);

	m_small		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_small,		ini_component	)(ALIGNHORZ_L, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_small,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_small,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_small,		ini_drawer		)(cocos_resource::icon_small,  xui_vector<s32>(12));
	xui_method_ptrcall(m_small,		xm_buttonclick	) += new xui_method_member<xui_method_args,		cocos_scene>(this, &cocos_scene::on_buttonclick);
	m_large		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_large,		ini_component	)(ALIGNHORZ_L, ALIGNVERT_T, 0);
	xui_method_ptrcall(m_large,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_large,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_large,		ini_drawer		)(cocos_resource::icon_large,  xui_vector<s32>(12));
	xui_method_ptrcall(m_large,		xm_buttonclick	) += new xui_method_member<xui_method_args,		cocos_scene>(this, &cocos_scene::on_buttonclick);
	m_reset		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_reset,		ini_component	)(ALIGNHORZ_R, ALIGNVERT_T, 0);
	xui_method_ptrcall(m_reset,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_reset,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_reset,		ini_drawer		)(cocos_resource::icon_reset,  xui_vector<s32>(12));
	xui_method_ptrcall(m_reset,		xm_buttonclick	) += new xui_method_member<xui_method_args,		cocos_scene>(this, &cocos_scene::on_buttonclick);
	m_clear		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		ini_component	)(ALIGNHORZ_R, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_clear,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_clear,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_clear,		ini_drawer		)(cocos_resource::icon_delete, xui_vector<s32>(12));
	xui_method_ptrcall(m_clear,		xm_buttonclick	) += new xui_method_member<xui_method_args,		cocos_scene>(this, &cocos_scene::on_buttonclick);

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
	xui_method_ptrcall(m_fillpane,	xm_keybddown	) += new xui_method_member<xui_method_keybd,	cocos_scene>(this, &cocos_scene::on_fillpanekeybddown);
	xui_method_ptrcall(m_fillpane,	xm_perform		) += new xui_method_member<xui_method_args,		cocos_scene>(this, &cocos_scene::on_fillpaneperform);
	xui_method_ptrcall(m_fillpane,	xm_renderelse	) += new xui_method_member<xui_method_args,		cocos_scene>(this, &cocos_scene::on_fillpanerenderelse);
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
	xui_method_ptrcall(m_animctrl,	xm_tick			) += new xui_method_member<xui_method_args,		cocos_scene>(this, &cocos_scene::on_animctrltick);

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

	add_pagectrl(m_headpane);
	add_pagectrl(m_fillpane);

	m_rootnode = cocos2d::Node::create();
	m_drawview->get_2droot()->addChild(m_rootnode);
}

/*
//destructor
*/
xui_delete_explain(cocos_scene)( void )
{
	m_rootnode->removeAllChildren();
	m_drawview->get_2droot()->removeChild(m_rootnode);
	delete m_rootnode;
	delete m_animctrl;
	delete m_lockctrl;
	delete m_alignbox;
	delete m_blankbox;
}

/*
//method
*/
xui_method_explain(cocos_scene, get_selectedvec,			cocos_boundbox_vec		)( void )
{
	cocos_scene* scene = cocos_mainform::get_ptr()->get_scene();
	if (scene)
		return scene->get_selectedboundbox();

	return cocos_boundbox_vec();
}
xui_method_explain(cocos_scene, set_editprop,				void					)( cocos_propcsd* prop )
{
	m_editprop = prop;
}
xui_method_explain(cocos_scene, get_editprop,				cocos_propcsd*			)( void )
{
	return m_editprop;
}
xui_method_explain(cocos_scene, set_rootvisible,			void					)( void )
{
	cocos2d::Node* root = m_editprop->get_node();
	cocos2d::Size  size = root->getContentSize();

	xui_vector<s32> trans;
	trans.x = (m_drawview->get_renderw() - size.width ) / 2;
	trans.y = (m_drawview->get_renderh() - size.height) / 2;
	set_trans(trans);
}
xui_method_explain(cocos_scene, get_trans,					const xui_vector<s32>&	)( void ) const
{
	return m_trans;
}
xui_method_explain(cocos_scene, get_ratio,					f64						)( void ) const
{
	return m_ratio;
}
xui_method_explain(cocos_scene, set_trans,					void					)( const xui_vector<s32>& trans )
{
	m_trans = trans;
	m_horzgrad->set_trans(trans.x);
	m_vertgrad->set_trans(trans.y);
	m_snaptool->set_trans(trans);
}
xui_method_explain(cocos_scene, set_ratio,					void					)( f64 ratio )
{
	m_ratio = ratio;
	m_horzgrad->set_ratio(ratio);
	m_vertgrad->set_ratio(ratio);
	m_snaptool->set_ratio(ratio);
	m_animctrl->clear();
}
xui_method_explain(cocos_scene, set_nodevisible,			void					)( cocos_boundbox* bbox )
{
	xui_rect2d<s32> rt = bbox->ori_bounding();
	rt.ax = (rt.ax*m_ratio);
	rt.bx = (rt.bx*m_ratio);
	rt.ay = (rt.ay*m_ratio);
	rt.by = (rt.by*m_ratio);

	xui_vector<s32> start = -m_trans;
	xui_vector<s32> range =  m_drawview->get_rendersz();
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
xui_method_explain(cocos_scene, set_toolupdate,				void					)( void )
{
	m_alignbox->set_lineupdate();
	m_blankbox->set_lineupdate();
}
xui_method_explain(cocos_scene, hit_propvisible,			cocos_boundbox*			)( const xui_vector<s32>& pt )
{
	cocos_hierarchy* hierarchy = cocos_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();
	xui_treenode* rootnode = treeview->get_upmostnode(0);
	std::vector<xui_treenode*> nodevec = rootnode->get_leafnodearray();
	for (s32 i = (s32)nodevec.size()-1; i >= 0; --i)
	{
		cocos_boundbox* result = hit_propvisible(pt, nodevec[i]);
		if (result)
			return result;
	}

	return NULL;
}
xui_method_explain(cocos_scene, get_operatorboundbox,		cocos_boundbox*			)( const xui_vector<s32>& pt, u08& result )
{
	s32 viewh = m_drawview->get_renderh();
	cocos_boundbox_vec vec = get_selectedboundbox();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		result = vec[i]->hit_operator(m_trans, m_ratio, viewh, pt);
		if (result > 0)
			return vec[i];
	}

	cocos_boundbox* pick = hit_propvisible(pt);
	if (pick)
	{
		result = BO_MOVE;
		return pick;
	}

	return NULL;
}
xui_method_explain(cocos_scene, get_selectedboundbox,		cocos_boundbox_vec		)( void )
{
	cocos_boundbox_vec vec;

	cocos_hierarchy* hierarchy = cocos_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();
	std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		xui_treenode*		node = nodevec[i];
		if (node->get_rootnode() == NULL)
			continue;

		cocos_treedata*		data = (cocos_treedata*)node->get_linkdata();
		cocos_propnodebase* prop = dynamic_cast<cocos_propnodebase*>(data->get_prop());
		cocos_boundbox*		bbox = prop->get_boundbox();
		vec.push_back(bbox);
	}

	return vec;
}

/*
//event
*/
xui_method_explain(cocos_scene, on_buttonclick,				void					)( xui_component* sender, xui_method_args&		args )
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
xui_method_explain(cocos_scene, on_fillpaneperform,			void					)( xui_component* sender, xui_method_args&		args )
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
xui_method_explain(cocos_scene, on_fillpanerenderelse,		void					)( xui_component* sender, xui_method_args&		args )
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
xui_method_explain(cocos_scene, on_fillpanekeybddown,		void					)( xui_component* sender, xui_method_keybd&		args )
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
		case KEY_UARROW: delta.y =  1; break;
		case KEY_DARROW: delta.y = -1; break;
		}

		on_keybdmoveimpl(delta);
	}
	else
	if (args.kcode == KEY_ESC)
	{
		cocos_hierarchy* hierarchy = cocos_mainform::get_ptr()->get_hierarchy();
		xui_treeview* treeview = hierarchy->get_treeview();
		treeview->non_selectednode();
	}
	else
	if (args.kcode == KEY_DELETE)
	{
		cocos_hierarchy* hierarchy = cocos_mainform::get_ptr()->get_hierarchy();
		hierarchy->del_propnode();
	}
}
xui_method_explain(cocos_scene, on_drawviewnoncatch,		void					)( xui_component* sender, xui_method_args&		args )
{
	m_dragview = false;
	m_operator = 0;
	m_multisel = false;
	m_drawview->set_cursor(CURSOR_DEFAULT);
	xui_global::set_cursor(CURSOR_DEFAULT);
}
xui_method_explain(cocos_scene, on_drawviewupdateself,		void					)( xui_component* sender, xui_method_update&	args )
{
	m_animctrl->update(args.delta);
	m_lockctrl->update(args.delta);
	m_rootnode->setPosition(m_trans.x, m_trans.y);
	m_rootnode->setScale(m_ratio);
	if (m_editprop)
	{
		cocos2d::Node* node = m_editprop->get_node();
		node->update(args.delta);
	}
}
xui_method_explain(cocos_scene, on_drawviewrenderself,		void					)( xui_component* sender, xui_method_args&		args )
{
	xui_convas::get_ins()->clear(xui_colour(1.0f, 0.3f));
	if (m_editprop)
	{
		cocos2d::Node* node = m_editprop->get_node();
		cocos2d::Size  size = node->getContentSize();
		if (size.width  > 0 &&
			size.height > 0)
		{
			xui_rect2d<s32> rt;
			rt.ax = m_trans.x;
			rt.ay = m_drawview->get_renderh() - m_trans.y - size.height*m_ratio;
			rt.set_w(size.width *m_ratio);
			rt.set_h(size.height*m_ratio);
			xui_convas::get_ins()->fill_rectangle(rt, xui_colour::black);
		}

		m_rootnode->addChild(node);
		cocos2d::WeCCharFontManager::getInstance()->forceClearMemory();
		cocos2d::Director::getInstance()->drawScene();
		m_rootnode->removeAllChildren();
	}
}
xui_method_explain(cocos_scene, on_drawviewrenderelse,		void					)( xui_component* sender, xui_method_args&		args )
{
	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();
	xui_convas::get_ins()->set_cliprect(cliprect.get_inter(m_drawview->get_renderrtabs()));

	s32 viewh = m_drawview->get_renderh();
	cocos_hierarchy* hierarchy = cocos_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();

	xui_vector<s32> pt = m_drawview->get_screenpt();
	xui_vector<s32> p1;
	xui_vector<s32> p2;
	p1 = xui_vector<s32>(m_trans.x,					0);
	p2 = xui_vector<s32>(m_trans.x,					m_drawview->get_renderh());
	xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour::gray);
	p1 = xui_vector<s32>(0,							m_drawview->get_renderh() - m_trans.y);
	p2 = xui_vector<s32>(m_drawview->get_renderw(), m_drawview->get_renderh() - m_trans.y);
	xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour::gray);

	//line
	std::vector<s32> linevec;
	linevec = m_horzgrad->get_lines();
	for (u32 i = 0; i < linevec.size(); ++i)
	{
		s32 line = linevec[i];
		p1 = xui_vector<s32>(xui_round(line*m_ratio) + m_trans.x, 0);
		p2 = xui_vector<s32>(xui_round(line*m_ratio) + m_trans.x, m_drawview->get_renderh());
		xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour(1.0f, 0.0f, 1.0f, 1.0f));
	}
	linevec = m_vertgrad->get_lines();
	for (u32 i = 0; i < linevec.size(); ++i)
	{
		s32 line = linevec[i];
		p1 = xui_vector<s32>(0,							m_drawview->get_renderh() - (xui_round(line*m_ratio) + m_trans.y));
		p2 = xui_vector<s32>(m_drawview->get_renderw(),	m_drawview->get_renderh() - (xui_round(line*m_ratio) + m_trans.y));
		xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour(1.0f, 0.0f, 1.0f, 1.0f));
	}

	//ratio
	std::wstringstream text;
	text << (s32)(m_ratio * 100);
	text << L"%";
	xui_rect2d<s32> textrt = xui_rect2d<s32>(-40, -20, 0, 0) + pt + m_drawview->get_rendersz();
	xui_convas::get_ins()->draw_round(textrt, xui_control::default_sidecolor, xui_rect2d<s32>(3, 0, 0, 0));
	textrt = xui_convas::get_ins()->calc_draw(text.str(), xui_family::default, textrt, TEXTALIGN_RC, true);
	xui_convas::get_ins()->draw_text(text.str(), xui_family::default, textrt, xui_family_render::default, true);

	//multi
	xui_rect2d<s32> multirt;
	if (m_multisel)
	{
		p1 = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousedown());
		p2 = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousecurr());
		multirt.ax = xui_min(p1.x, p2.x);
		multirt.bx = xui_max(p1.x, p2.x);
		multirt.ay = xui_min(p1.y, p2.y);
		multirt.by = xui_max(p1.y, p2.y);
		xui_convas::get_ins()->fill_rectangle(multirt+pt, xui_colour(0.5f, 42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
		xui_convas::get_ins()->draw_rectangle(multirt+pt, xui_colour(1.0f, 0.0f, 0.9f, 0.9f));
	}

	//snap
	if (m_operator == BO_MOVE)
	{
		cocos_boundbox_vec selectedvec = get_selectedboundbox();
		cocos_boundbox*	selfbbox = m_snaptool->cal_selfbbox(selectedvec);
		if (selfbbox)
		{
			cocos_propnodebase*		selfprop = dynamic_cast<cocos_propnodebase*>(selfbbox->get_linkprop());
			xui_treedata*			treedata = selfprop->get_linkdata(treeview);
			xui_treenode*			treenode = treedata->get_node();
			xui_treenode*			rootnode = treenode->get_rootnode();

			cocos_boundbox_vec vec;
			for (u32 i = 0; i < rootnode->get_leafnodecount(); ++i)
			{
				xui_treenode*		leafnode = rootnode->get_leafnode(i);
				if (leafnode->was_selected())
					continue;

				cocos_treedata*		leafdata = (cocos_treedata*)leafnode->get_linkdata();
				cocos_propnodebase* leafprop = dynamic_cast<cocos_propnodebase*>(leafdata->get_prop());
				cocos_boundbox*		leafbbox = leafprop->get_boundbox();
				vec.push_back(leafbbox);
			}

			m_snaptool->set_snapdraw(vec, selectedvec);
		}
	}

	//boundbox
	std::vector<xui_treenode*> nodevec = treeview->get_entirenode();
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		xui_treenode*		node = nodevec[i];
		if (node->get_rootnode() == NULL)
			continue;

		cocos_treedata*		data = (cocos_treedata*)node->get_linkdata();
		cocos_propnodebase* prop = dynamic_cast<cocos_propnodebase*>(data->get_prop());
		cocos_boundbox*		bbox = prop->get_boundbox();
		bbox->draw(m_trans, m_ratio, viewh, pt, m_showbbox->was_push());

		if (m_multisel && node->was_selected() == false)
		{
			xui_rect2d<s32> rt = bbox->get_bounding(m_trans, m_ratio, viewh);
			if (multirt.get_inter(rt).was_valid())
				xui_convas::get_ins()->draw_rectangle(rt+pt, xui_colour(1.0f, 1.0f, 0.65f, 0.0f));
		}
	}

	draw_locknode();

	xui_convas::get_ins()->set_cliprect(cliprect);
}
xui_method_explain(cocos_scene, on_drawviewmouseenter,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	//m_drawview->req_focus();
}
xui_method_explain(cocos_scene, on_drawviewmousedown,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	if (args.mouse == MB_L)
	{
		u08				op	 = 0;
		xui_vector<s32>	pt   = m_drawview->get_renderpt(args.point);
		cocos_boundbox*	pick = get_operatorboundbox(pt, op);
		u32 cursor = cocos_boundbox::get_opcursor(op);
		m_drawview->set_cursor(cursor);
		xui_global::set_cursor(cursor);

		if (pick)
		{
			on_mousepickimpl(
				pick, 
				args.alt,
				args.ctrl,
				args.shift,
				op);

			if (pick->was_selected())
			{
				m_operator = op;
				s32 viewh  = m_drawview->get_renderh();
				cocos_boundbox_vec  vec = get_selectedboundbox();
				for (u32 i = 0; i < vec.size(); ++i)
					vec[i]->syn_bounding(m_trans, m_ratio, viewh);
			}
		}
		else
		{
			m_multisel = true;
		}
	}
	else
	if (args.mouse == MB_M)
	{
		m_dragview  = true;
		m_drawview->set_cursor(CURSOR_HAND);
		xui_global::set_cursor(CURSOR_HAND);
		xui_desktop::get_ins()->set_catchctrl(m_drawview);
	}
}
xui_method_explain(cocos_scene, on_drawviewmousemove,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	xui_vector<s32> drag = xui_desktop::get_ins()->get_mousemove();

	if (m_dragview)
	{
		set_trans(m_trans+xui_vector<s32>(drag.x, -drag.y));
	}
	else
	if (m_operator > 0)
	{
		if (m_operator == BO_MOVE)
		{
			u08 mode = DRAGMOVE_UNLIMIT;
			if		(args.shift && args.ctrl)	mode = DRAGMOVE_Y;
			else if (args.shift)				mode = DRAGMOVE_X;
			else
			{}

			if (mode == DRAGMOVE_X)
				drag.y = 0;
			if (mode == DRAGMOVE_Y)
				drag.x = 0;
		}

		on_mousedragimpl(drag);
	}
	else
	{
		u08				op	 = 0;
		xui_vector<s32>	pt   = m_drawview->get_renderpt(args.point);
		cocos_boundbox*	pick = get_operatorboundbox(pt, op);
		u32 cursor = cocos_boundbox::get_opcursor(op);
		m_drawview->set_cursor(cursor);
		xui_global::set_cursor(cursor);
	}
}
xui_method_explain(cocos_scene, on_drawviewmouserise,		void					)( xui_component* sender, xui_method_mouse&		args )
{
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

		if (m_operator == BO_MOVE)
		{
			cocos_hierarchy* hierarchy = cocos_mainform::get_ptr()->get_hierarchy();
			xui_treeview* treeview = hierarchy->get_treeview();

			cocos_boundbox_vec selectedvec = get_selectedboundbox();
			cocos_boundbox*	selfbbox = m_snaptool->cal_selfbbox(selectedvec);
			if (selfbbox)
			{
				cocos_propnodebase*		selfprop = dynamic_cast<cocos_propnodebase*>(selfbbox->get_linkprop());
				xui_treedata*			treedata = selfprop->get_linkdata(treeview);
				xui_treenode*			treenode = treedata->get_node();
				xui_treenode*			rootnode = treenode->get_rootnode();

				cocos_boundbox_vec vec;
				for (u32 i = 0; i < rootnode->get_leafnodecount(); ++i)
				{
					xui_treenode*		leafnode = rootnode->get_leafnode(i);
					if (leafnode->was_selected())
						continue;

					cocos_treedata*		leafdata = (cocos_treedata*)leafnode->get_linkdata();
					cocos_propnodebase* leafprop = dynamic_cast<cocos_propnodebase*>(leafdata->get_prop());
					cocos_boundbox*		leafbbox = leafprop->get_boundbox();
					vec.push_back(leafbbox);
				}

				m_snaptool->use_snapinfo(vec, selectedvec);
			}
		}
	}
	else
	if (args.mouse == MB_M)
	{
		xui_desktop::get_ins()->set_catchctrl(NULL);
	}
}
xui_method_explain(cocos_scene, on_drawviewmousewheel,		void					)( xui_component* sender, xui_method_mouse&		args )
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
xui_method_explain(cocos_scene, on_drawviewmousedragover,	void					)( xui_component* sender, xui_method_dragdrop&	args )
{
	if (args.type == "Toolbox" ||
		args.type == "ParticleSystem" ||
		args.type == "SpriteFrame")
	{
		args.allow = true;
	}
}
xui_method_explain(cocos_scene, on_drawviewmousedragdrop,	void					)( xui_component* sender, xui_method_dragdrop&	args )
{
	cocos_hierarchy* hierarchy = cocos_mainform::get_ptr()->get_hierarchy();

	cocos_propnodebase* newprop = NULL;
	if (args.type == "Toolbox")
	{
		xui_button* toolctrl = (xui_button*)args.data;
		newprop = hierarchy->new_propnode(NULL, toolctrl->get_text());
	}
	else
	if (args.type == "ParticleSystem")
	{
		newprop = hierarchy->new_propnode(NULL, (cocos_propfile*)args.data);
	}
	else
	if (args.type == "SpriteFrame")
	{
		newprop = hierarchy->new_propnode(NULL, (cocos2d::SpriteFrame*)args.data);
	}

	xui_vector<s32> pt = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousecurr());
	float x = xui_round((pt.x - m_trans.x) / m_ratio);
	float y = xui_round((m_drawview->get_renderh() - pt.y - m_trans.y) / m_ratio);
	newprop->get_node()->setPosition(x, y);
}
xui_method_explain(cocos_scene, on_animctrltick,			void					)( xui_component* sender, xui_method_args&		args )
{
	xui_action_ctrl_impl<f64>* action = (xui_action_ctrl_impl<f64>*)m_animctrl;
	m_ratio = action->sample();
	m_horzgrad->set_ratio(m_ratio);
	m_vertgrad->set_ratio(m_ratio);
	m_snaptool->set_ratio(m_ratio);
	if (m_ratio == 0.1 || m_ratio == 5.0)
		m_animctrl->clear();
}

/*
//method
*/
xui_method_explain(cocos_scene, get_incratio,				f64						)( void )
{
	xui_action_ctrl_impl<f64>* action = (xui_action_ctrl_impl<f64>*)m_animctrl;
	f64 ratio = action->has_data() ? action->get_data(1) : m_ratio;
	if (ratio < 1.0)	ratio += 0.1;
	else				ratio += 0.2;

	return xui_min(ratio, 5.0);
}
xui_method_explain(cocos_scene, get_decratio,				f64						)( void )
{
	xui_action_ctrl_impl<f64>* action = (xui_action_ctrl_impl<f64>*)m_animctrl;
	f64 ratio = action->has_data() ? action->get_data(1) : m_ratio;
	if (ratio <= 1.0)	ratio -= 0.1;
	else				ratio -= 0.2;

	return xui_max(ratio, 0.1);
}
xui_method_explain(cocos_scene, hit_propvisible,			cocos_boundbox*			)( const xui_vector<s32>& pt, xui_treenode* root )
{
	std::vector<xui_treenode*> nodevec = root->get_leafnodearray();
	for (s32 i = (s32)nodevec.size() - 1; i >= 0; --i)
	{
		cocos_boundbox* result = hit_propvisible(pt, nodevec[i]);
		if (result)
			return result;
	}

	cocos_treedata*		data = (cocos_treedata*)root->get_linkdata();
	cocos_propnodebase* prop = dynamic_cast<cocos_propnodebase*>(data->get_prop());
	cocos_boundbox*		bbox = prop->get_boundbox();
	xui_rect2d<s32>		rect = bbox->get_bounding(m_trans, m_ratio, m_drawview->get_renderh());
	if (rect.was_inside(pt))
		return bbox;

	return NULL;
}

/*
//virtual
*/
xui_method_explain(cocos_scene, on_keybdmoveimpl,			void					)( const xui_vector<s32>& delta )
{
	cocos_boundbox_vec  vec = get_selectedboundbox();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		vec[i]->set_position(vec[i]->ori_position()+delta);
	}
}
xui_method_explain(cocos_scene, on_mousepickimpl,			void					)( cocos_boundbox* pick, bool alt, bool ctrl, bool shift, u08 op )
{
	cocos_hierarchy* hierarchy = cocos_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();

	cocos_propnodebase* prop = dynamic_cast<cocos_propnodebase*>(pick->get_linkprop());
	xui_treedata*		data = prop->get_linkdata(treeview);
	xui_treenode*		node = data->get_node();
	if (ctrl && !shift)
	{
		treeview->set_selectednode(node, !node->was_selected());
	}
	else
	if (node->was_selected() == false)
	{
		treeview->non_selectednode(false);
		treeview->set_selectednode(node, true);
	}

	if (node->was_selected() && op == BO_MOVE)
	{
		u08 mode = DRAGMOVE_UNLIMIT;
		if		(shift && ctrl) mode = DRAGMOVE_Y;
		else if (shift)			mode = DRAGMOVE_X;
		else
		{}

		std::vector<cocos_boundbox*> vec;
		xui_treenode* rootnode = node->get_rootnode();
		for (u32 i = 0; i < rootnode->get_leafnodecount(); ++i)
		{
			xui_treenode* leafnode = rootnode->get_leafnode(i);
			if (leafnode->was_selected())
				continue;

			cocos_treedata*		leafdata = (cocos_treedata*)leafnode->get_linkdata();
			cocos_propnodebase* leafprop = dynamic_cast<cocos_propnodebase*>(leafdata->get_prop());
			cocos_boundbox*		leafbbox = leafprop->get_boundbox();
			vec.push_back(leafbbox);
		}

		m_snaptool->cal_snapinfo(
			vec,
			prop->get_boundbox()->ori_bounding(),
			mode,
			0);
	}
}
xui_method_explain(cocos_scene, on_mousedragimpl,			void					)( const xui_vector<s32>& delta )
{
	s32 viewh = m_drawview->get_renderh();
	cocos_boundbox_vec  vec = get_selectedboundbox();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		vec[i]->opt_bounding(
			m_trans, 
			m_ratio, 
			viewh, 
			delta, 
			m_operator);
	}
}
xui_method_explain(cocos_scene, on_mulselectimpl,			void					)( const xui_rect2d<s32>& rt, bool ctrl )
{
	cocos_hierarchy* hierarchy = cocos_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();
	std::vector<xui_treenode*> nextvec;
	std::vector<xui_treenode*> nodevec = treeview->get_entirenode();
	s32 viewh = m_drawview->get_renderh();
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		xui_treenode*		node = nodevec[i];
		if (node->get_rootnode() == NULL)
			continue;

		cocos_treedata*		data = (cocos_treedata*)node->get_linkdata();
		cocos_propnodebase* prop = dynamic_cast<cocos_propnodebase*>(data->get_prop());
		cocos_boundbox*		bbox = prop->get_boundbox();
		xui_rect2d<s32>		temp = bbox->get_bounding(m_trans, m_ratio, viewh);
		if (temp.get_inter(rt).was_valid())
		{
			nextvec.push_back(node);
		}
	}

	if (nextvec.size() > 0)
	{
		if (ctrl == false)
			treeview->non_selectednode(false);

		treeview->set_selectednode(nextvec);
	}
}

/*
//drawsnap
*/
xui_method_explain(cocos_scene, draw_locknode,				void					)( void )
{
	if (m_lockctrl->was_play())
	{
		cocos_boundbox_vec vec = get_selectedboundbox();
		if (vec.size() > 0)
		{
			cocos_boundbox*	head = vec.front();
			xui_rect2d<s32>	rect = head->get_bounding(m_trans, m_ratio, m_drawview->get_renderh());

			xui_action_ctrl_impl<f32>* action = (xui_action_ctrl_impl<f32>*)m_lockctrl;
			f32 sa = action->sample();
			f32 s  = xui_max(sa, 1.0f);
			f32 ox = -0.5f * rect.get_w() * s + 0.5f * rect.get_w();
			f32 oy = -0.5f * rect.get_h() * s + 0.5f * rect.get_h();
			f32 fw =  s    * rect.get_w();
			f32 fh =  s    * rect.get_h();
			xui_vector<s32> drawpt = m_drawview->get_screenpt();
			xui_rect2d<s32> drawrt = rect + drawpt;
			drawrt.oft_x((s32)ox);
			drawrt.oft_y((s32)oy);
			drawrt.set_w((s32)fw);
			drawrt.set_h((s32)fh);
			xui_convas::get_ins()->draw_round(drawrt, xui_colour(sa, 1.0f, 0.0f, 0.0f), xui_rect2d<s32>(3), 3);
		}
	}
}
//xui_method_explain(cocos_scene, draw_multisel,				void					)( void )
//{
//	if (m_multisel)
//	{
//		xui_vector<s32> pt = m_drawview->get_screenpt();
//		xui_vector<s32> p1 = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousedown());
//		xui_vector<s32> p2 = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousecurr());
//		xui_rect2d<s32> rt;
//		rt.ax = xui_min(p1.x, p2.x);
//		rt.bx = xui_max(p1.x, p2.x);
//		rt.ay = xui_min(p1.y, p2.y);
//		rt.by = xui_max(p1.y, p2.y);
//		xui_convas::get_ins()->fill_rectangle(rt+pt, xui_colour(0.5f, 42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
//		xui_convas::get_ins()->draw_rectangle(rt+pt, xui_colour(1.0f, 0.0f, 0.9f, 0.9f));
//	}
//}