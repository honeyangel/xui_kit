#include "NPRender.h"
#include "NPTexture.h"
#include "NP2DSImageFile.h"
#include "NP2DSImage.h"
#include "NP2DSRenderUtil.h"
#include "NP2DSRenderStep.h"

#include "xui_convas.h"
#include "xui_button.h"
#include "xui_treenode.h"
#include "xui_treeview.h"
#include "xui_panel.h"
#include "xui_timer.h"
#include "xui_timermgr.h"
#include "xui_toolbar.h"
#include "xui_global.h"
#include "onity_treedata.h"
#include "onity_resource.h"
#include "onity_project.h"
#include "onity_propmodule.h"
#include "onity_propimage.h"
#include "onity_mainform.h"
#include "onity_fileview.h"
#include "onity_boundbox.h"
#include "onity_project.h"
#include "onity_renderview.h"
#include "onity_snaptool.h"
#include "onity_inspector.h"
#include "onity_module.h"

xui_implement_rtti(onity_module, onity_asset);

/*
//constructor
*/
xui_create_explain(onity_module)( void )
: onity_asset()
{
	ini_namectrl(onity_resource::icon_scene, L"Module");

	m_automode	= new xui_button(xui_vector<s32>(80, 20));
	xui_method_ptrcall(m_automode,	ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_automode,	set_corner		)(3);
	xui_method_ptrcall(m_automode,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_automode,	set_drawcolor	)(true);
	xui_method_ptrcall(m_automode,	set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_automode,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_automode,	ini_drawer		)(L"Auto Mode");
	xui_method_ptrcall(m_automode,	xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_module>(this, &onity_module::on_buttonclick);
	xui_method_ptrcall(m_linetool,	add_item		)(m_showbbox);
	xui_method_ptrcall(m_linetool,	add_separate	)();
	xui_method_ptrcall(m_linetool,	add_item		)(m_automode);

	m_addtimer	= xui_timermgr::get_ins()->add_timer(this, 0.1f, NULL);
	xui_method_ptrcall(m_addtimer,	xm_tick			) += new xui_method_member<xui_method_args,	onity_module>(this, &onity_module::on_timertick);

	add_pagectrl(m_headpane);
	add_pagectrl(m_fillpane);
}

/*
//destructor
*/
xui_delete_explain(onity_module)( void )
{
	xui_timermgr::get_ins()->del_timer(m_addtimer);
}

/*
//method
*/
xui_method_explain(onity_module, get_editprop,			onity_propmodule*	)( void )
{
	return m_editprop;
}
xui_method_explain(onity_module, set_editprop,			void				)( onity_propmodule* prop )
{
	m_editprop = prop;
	m_addtimer->set_enable(false);
}
xui_method_explain(onity_module, hit_propvisible,		onity_boundbox*		)( const xui_vector<s32>& pt )
{
	xui_treenode* filenode = get_filenode();
	if (filenode)
	{
		xui_proproot_vec vec = m_editprop->get_subprop();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			onity_propimage*	prop = dynamic_cast<onity_propimage*>(vec[i]);
			onity_boundbox*		bbox = prop->get_boundbox();
			xui_rect2d<s32>		rect = bbox->get_bounding(m_trans, m_ratio);
			if (rect.was_inside(pt))
				return bbox;
		}
	}

	return NULL;
}
xui_method_explain(onity_module, get_treeview,			xui_treeview*		)( void )
{
	xui_treenode* filenode = get_filenode();
	if (filenode)
		return xui_dynamic_cast(xui_treeview, filenode->get_parent());

	return NULL;
}
xui_method_explain(onity_module, get_filenode,			xui_treenode*		)( void )
{
	if (m_editprop)
	{
		onity_fileview* fileview = onity_mainform::get_ptr()->get_project()->get_fileview();
		xui_treeview*	lineview = fileview->get_lineview();
		xui_treedata*	filedata = m_editprop->get_linkdata(lineview);
		if (filedata)
			return filedata->get_node();
	}

	return NULL;
}
xui_method_explain(onity_module, get_selectedboundbox,	onity_boundbox_vec	)( void )
{
	onity_boundbox_vec vec;

	xui_treenode* filenode = get_filenode();
	if (filenode)
	{
		for (u32 i = 0; i < filenode->get_leafnodecount(); ++i)
		{
			xui_treenode* node = filenode->get_leafnode(i);
			if (node->was_selected())
			{
				onity_treedata*		data = (onity_treedata*)node->get_linkdata();
				onity_propimage*	prop = dynamic_cast<onity_propimage*>(data->get_prop());
				onity_boundbox*		bbox = prop->get_boundbox();
				vec.push_back(bbox);
			}
		}
	}

	return vec;
}

/*
//event
*/
xui_method_explain(onity_module, on_buttonclick,		void				)( xui_component* sender, xui_method_args&	args )
{
	onity_asset::on_buttonclick(sender, args);
	if (sender == m_automode)
	{

	}
}
xui_method_explain(onity_module, on_fillpanekeybddown,	void				)( xui_component* sender, xui_method_keybd& args )
{
	onity_asset::on_fillpanekeybddown(sender, args);
	xui_treeview* treeview = get_treeview();
	xui_treenode* filenode = get_filenode();
	if (treeview)
	{
		if (args.kcode == KEY_ESC)
		{
			treeview->non_selectednode(false);
			onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
			inspector->set_proproot(m_editprop);
		}
		else
		if (args.kcode == KEY_DELETE)
		{
			onity_boundbox_vec  vec = get_selectedboundbox();
			for (u32 i = 0; i < vec.size(); ++i)
			{
				onity_boundbox* bbox = vec[i];
				onity_proproot*	prop = dynamic_cast<onity_proproot*>(bbox->get_linkprop());
				xui_treedata*	data = prop->get_linkdata(treeview);
				xui_treenode*	node = data->get_node();
				filenode->del_leafnode(node);

				m_editprop->del_subprop(prop);
			}
		}
	}
}
//xui_method_explain(onity_module, on_drawviewnoncatch,	void				)( xui_component* sender, xui_method_args&	args )
//{
//	onity_asset::on_drawviewnoncatch(sender, args);
//
//	xui_proproot_vec vec = get_selectedprop();
//	for (u32 i= 0; i < vec.size(); ++i)
//	{
//		onity_propimage* prop = dynamic_cast<onity_propimage*>(vec[i]);
//		if (prop)
//		{
//			prop->set_lockdata(false);
//		}
//	}
//}
xui_method_explain(onity_module, on_drawviewrenderself, void				)( xui_component* sender, xui_method_args&	args )
{
	xui_convas::get_ins()->clear(xui_colour(1.0f, 0.3f));

	xui_vector<s32> size = sender->get_rendersz();
	NPRender::GetIns()->SetResolutionW(size.w);
	NPRender::GetIns()->SetResolutionH(size.h);
	NPRender::GetIns()->SetViewport(0, 0, size.w, size.h);
	if (m_editprop)
	{
		NP2DSImageFile* file = (NP2DSImageFile*)m_editprop->get_resfile();
		if (file)
		{
			NPTexture* texture = file->GetSCTex();
			if (texture)
			{
				npu32 id = texture->GetKey();
				NPColor color = (get_filenode() == NULL) ? NPColor(1.0f, 1.0f, 1.0f, 0.5f) : NPColor::White;
				NP2DSImage temp(-1, "", -1, 0, 0, texture->GetWidth(), texture->GetHeight(), true);
				NP2DSRenderUtil::GetIns()->DrawImage(&temp, NPMatrix4x4::Identity, color, NP2DSRenderUtil::TEXENV_MODULATE, 0, 0, 0, id);
			}
		}
	}

	NPVector3 scale((f32)m_ratio,   (f32)m_ratio,   1.0f);
	NPVector3 trans((f32)m_trans.x, (f32)m_trans.y, 0.0f);
	NP2DSRenderStep::GetIns()->SetEntryLocalT(trans);
	NP2DSRenderStep::GetIns()->SetEntryWorldS(scale);
	NP2DSRenderStep::GetIns()->RenderImmediate();
}
xui_method_explain(onity_module, on_drawviewrenderelse, void				)( xui_component* sender, xui_method_args&	args )
{
	onity_asset::on_drawviewrenderelse(sender, args);

	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();
	xui_convas::get_ins()->set_cliprect(cliprect.get_inter(m_drawview->get_renderrtabs()));

	xui_vector<s32> pt = m_drawview->get_screenpt();
	if (m_editprop)
	{
		NP2DSImageFile* file = (NP2DSImageFile*)m_editprop->get_resfile();
		if (file)
		{
			NPTexture* texture = file->GetSCTex();
			if (texture)
			{
				s32 x1 = pt.x + xui_round(m_trans.x*m_ratio);
				s32 y1 = pt.y + xui_round(m_trans.y*m_ratio);
				s32 x2 = pt.x + xui_round(m_trans.x*m_ratio + texture->GetWidth ()*m_ratio);
				s32 y2 = pt.y + xui_round(m_trans.y*m_ratio + texture->GetHeight()*m_ratio);
				xui_convas::get_ins()->draw_rectangle(xui_rect2d<s32>(x1, y1, x2, y2), xui_colour::gray);
				//xui_vector<s32> p1;
				//xui_vector<s32> p2;
				//p1 = xui_vector<s32>(x2, y1);
				//p2 = xui_vector<s32>(x2, y2);
				//xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour::gray);
				//p1 = xui_vector<s32>(x2, y2);
				//p2 = xui_vector<s32>(x1, y2);
				//xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour::gray);
			}
		}

		if (get_treeview())
		{
			xui_proproot_vec vec = m_editprop->get_subprop();
			for (u32 i = 0; i < vec.size(); ++i)
			{
				onity_propimage* prop = dynamic_cast<onity_propimage*>(vec[i]);
				onity_boundbox*  bbox = prop->get_boundbox();
				bbox->draw(m_trans, m_ratio, pt, m_showbbox->was_push());
			}
		}

#if 0
		xui_treeview* treeview = get_treeview();
		if (treeview)
		{
			xui_proproot_vec vec = m_editprop->get_subprop();
			for (u32 i = 0; i < vec.size(); ++i)
			{
				onity_propimage* prop = dynamic_cast<onity_propimage*>(vec[i]);
				xui_treedata*    data = prop->get_linkdata(treeview);
				xui_treenode*	 node = data->get_node();
				if (node->was_selected() == false && m_showaabb->was_push() == false)
					continue;

				xui_rect2d<s32>  rect = prop->get_bounding(m_trans, m_ratio);
				xui_convas::get_ins()->draw_rectangle(rect+pt, xui_colour(1.0f, 0.7f));
			}

			xui_proproot_vec selectedvec = get_selectedprop();
			for (u32 i = 0; i < selectedvec.size(); ++i)
			{
				onity_propimage* prop = dynamic_cast<onity_propimage*>(selectedvec[i]);
				xui_rect2d<s32>  rect = prop->get_bounding(m_trans, m_ratio);
				xui_convas::get_ins()->fill_rectangle(rect+pt, xui_colour(0.3f, 1.0f, 0.0f, 0.0f));
				xui_convas::get_ins()->draw_rectangle(rect+pt, xui_colour(1.0f, 0.7f));
			}
		}
#endif
	}

	xui_convas::get_ins()->set_cliprect(cliprect);
}
xui_method_explain(onity_module, on_drawviewmouserise,	void				)( xui_component* sender, xui_method_mouse& args )
{
	onity_asset::on_drawviewmouserise(sender, args);
	if (args.mouse == MB_L && get_filenode())
	{
		xui_proproot_vec vec;
		onity_boundbox_vec  selectedvec = get_selectedboundbox();
		for (u32 i = 0; i < selectedvec.size(); ++i)
			vec.push_back(selectedvec[i]->get_linkprop());

		onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
		if (vec.size())
		{
			inspector->set_proproot(vec);
		}
		else
		{
			inspector->set_proproot(m_editprop);
		}
	}
}
xui_method_explain(onity_module, on_timertick,			void				)( xui_component* sender, xui_method_args&	args )
{

}

/*
//override
*/
//xui_method_explain(onity_module, on_keybdmoveimpl,		void				)( const xui_vector<s32>& delta )
//{
//	xui_proproot_vec vec = get_selectedprop();
//	for (u32 i = 0; i < vec.size(); ++i)
//	{
//		onity_propimage* prop = dynamic_cast<onity_propimage*>(vec[i]);
//		prop->set_position(prop->ori_position()+delta);
//	}
//}
xui_method_explain(onity_module, on_mousepickimpl,		void				)( onity_boundbox* pick, bool alt, bool ctrl, bool shift, u08 op )
{
	xui_treeview* treeview = get_treeview();
	if (treeview)
	{
		onity_propimage* prop = dynamic_cast<onity_propimage*>(pick->get_linkprop());
		xui_treedata*	 data = prop->get_linkdata(treeview);
		xui_treenode*	 node = data->get_node();

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

		//if (m_drawview->has_catch() && node->was_selected())
		//{
			//m_dragprop = true;
			//xui_method_ptrcall(m_drawview,	set_cursor		)(CURSOR_HAND);
			//xui_static_inscall(xui_global,	set_cursor		)(CURSOR_HAND);

			//lock update bounding
			//xui_proproot_vec vec = get_selectedprop();
			//for (u32 i = 0; i < vec.size(); ++i)
			//{
			//	onity_propimage* prop = dynamic_cast<onity_propimage*>(vec[i]);
			//	if (prop)
			//	{
			//		prop->get_position(m_trans, m_ratio);
			//		prop->get_bounding(m_trans, m_ratio);
			//		prop->set_lockdata(true);
			//	}
			//}
		//}
	}
}
//xui_method_explain(onity_module, on_mousedragimpl,		void				)( const xui_vector<s32>& delta )
//{
//	xui_proproot_vec vec = get_selectedprop();
//	for (u32 i = 0; i < vec.size(); ++i)
//	{
//		onity_propimage* prop = dynamic_cast<onity_propimage*>(vec[i]);
//		if (prop)
//		{
//			prop->mov_position(delta);
//
//			xui_vector<s32> pos = prop->get_position(m_trans, m_ratio);
//			pos.x = xui_round(pos.x/m_ratio - m_trans.x);
//			pos.y = xui_round(pos.y/m_ratio - m_trans.y);
//			prop->set_position(pos);
//		}
//	}
//}
xui_method_explain(onity_module, on_mulselectimpl,		void				)( const xui_rect2d<s32>& rt, bool ctrl )
{
	xui_treeview* treeview = get_treeview();
	if (treeview)
	{
		std::vector<xui_treenode*> nextvec;
		xui_proproot_vec propvec = m_editprop->get_subprop();
		for (u32 i = 0; i < propvec.size(); ++i)
		{
			onity_propimage* prop	= dynamic_cast<onity_propimage*>(propvec[i]);
			onity_boundbox*	 bbox	= prop->get_boundbox();
			xui_rect2d<s32>	 temp	= bbox->get_bounding(m_trans, m_ratio);
			if (temp.get_inter(rt).was_valid())
			{
				nextvec.push_back(prop->get_linkdata(treeview)->get_node());
			}
		}

		if (nextvec.size() > 0)
		{
			if (ctrl == false)
				treeview->non_selectednode(false);

			treeview->set_selectednode(nextvec);
		}
	}
}