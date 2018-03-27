#include "2d/CCScene.h"
#include "2d/CCNode.h"
#include "2d/CCSpriteFrame.h"
#include "2d/WeCChartFontManager.h"
#include "base/CCDirector.h"

#include "xui_toolbar.h"
#include "xui_button.h"
#include "xui_dockview.h"
#include "xui_convas.h"
#include "xui_dropbox.h"
#include "xui_propctrl.h"
#include "xui_propctrl_stdvec.h"
#include "xui_propctrl_vector.h"
#include "xui_kindctrl.h"
#include "xui_global.h"
#include "cocos_mainform.h"
#include "cocos_inspector.h"
#include "cocos_propcsd.h"
#include "cocos_filedata.h"
#include "cocos_boundbox.h"
#include "cocos_resource.h"
#include "cocos_renderview.h"
#include "cocos_game.h"

xui_implement_rtti(cocos_game, xui_dockpage);

/*
//constructor
*/
xui_create_explain(cocos_game)( void )
: xui_dockpage(xui_vector<s32>(200), AREALIMIT_A, 200, DOCKSTYLE_F)
, m_viewprop(NULL)
, m_propsize(NULL)
{
	ini_namectrl(cocos_resource::icon_game, L"Game");
	set_autofree(true);

	m_userctrl = new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_userctrl, ini_drawer		)(cocos_resource::icon_setting);
	xui_method_ptrcall(m_userctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_userctrl, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_userctrl, set_corner		)(3);
	xui_method_ptrcall(m_userctrl, xm_buttonclick	) += new xui_method_member<xui_method_args,		cocos_game>(this, &cocos_game::on_userctrlclick);

	m_dropctrl = new xui_dropbox(xui_vector<s32>(128, 20), NULL);
	xui_method_ptrcall(m_dropctrl, ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_dropctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_dropctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_dropctrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 0, 2));
	xui_method_ptrcall(m_dropctrl, set_corner		)(3);
	xui_method_ptrcall(m_dropctrl, set_readonly		)(true);
	xui_method_ptrcall(m_dropctrl, xm_selection		) += new xui_method_member<xui_method_args,		cocos_game>(this, &cocos_game::on_dropctrlselection);

	m_linetool = new xui_toolbar(xui_vector<s32>(20));
	xui_method_ptrcall(m_linetool, ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_linetool, set_drawcolor	)(false);
	xui_method_ptrcall(m_linetool, add_item			)(m_dropctrl);
	xui_method_ptrcall(m_linetool, add_item			)(m_userctrl);

	m_headpane = new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_headpane, ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_headpane, set_drawcolor	)(false);
	xui_method_ptrcall(m_headpane, set_hscrollauto	)(false);
	xui_method_ptrcall(m_headpane, set_borderrt		)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_headpane, add_child		)(m_linetool);	

	m_drawview = new cocos_renderview(xui_vector<s32>(100), xui_vector<s32>(2048));
	xui_method_ptrcall(m_drawview, xm_updateself	) += new xui_method_member<xui_method_update,	cocos_game>(this, &cocos_game::on_drawviewupdateself);
	xui_method_ptrcall(m_drawview, xm_renderself	) += new xui_method_member<xui_method_args,		cocos_game>(this, &cocos_game::on_drawviewrenderself);
	xui_method_ptrcall(m_drawview, xm_renderelse	) += new xui_method_member<xui_method_args,		cocos_game>(this, &cocos_game::on_drawviewrenderelse);
	xui_method_ptrcall(m_drawview, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_drawview, set_sidecolor	)(xui_colour::black);

	add_pagectrl(m_headpane);
	add_pagectrl(m_drawview);

	m_rootnode = cocos2d::Node::create();
	m_drawview->get_2droot()->addChild(m_rootnode);
	load_config();
}

/*
//destructor
*/
xui_delete_explain(cocos_game)( void )
{
	m_rootnode->removeAllChildrenWithCleanup(false);
	m_drawview->get_2droot()->removeChild(m_rootnode);
	delete m_rootnode;
	delete m_propsize;
}

/*
//method
*/
xui_method_explain(cocos_game, get_viewprop,			cocos_propcsd*	)( void )
{
	return m_viewprop;
}
xui_method_explain(cocos_game, set_viewprop,			void			)( cocos_propcsd* prop )
{
	m_viewprop = prop;
}
xui_method_explain(cocos_game, get_sizedata,			cocos_sizedata&	)( void )
{
	return m_sizedata;
}
xui_method_explain(cocos_game, add_sizedata,			void			)( void )
{
	m_sizedata.push_back(xui_vector<f64>(640, 1136));
}
xui_method_explain(cocos_game, del_sizedata,			void			)( void )
{
	if (m_sizedata.size() > 1)
		m_sizedata.erase(m_sizedata.begin()+m_sizedata.size()-1);
}
xui_method_explain(cocos_game, new_sizeprop,			xui_propdata*	)( u32 index, xui_propkind* kind )
{
	return new xui_propdata_vector(
		kind,
		L"",
		xui_propctrl_vector::create,
		get_size,
		set_size,
		(void*)index,
		NT_INT);
}
xui_method_explain(cocos_game, set_dropctrlupdate,		void			)( void )
{
	u32 index = m_dropctrl->get_selectedindex();
	m_dropctrl->del_itemall();
	for (u32 i = 0; i < m_sizedata.size(); ++i)
	{
		std::wstringstream text;
		text << m_sizedata[i].w;
		text << L"*";
		text << m_sizedata[i].h;
		m_dropctrl->add_item(text.str());
	}
	m_dropctrl->ini_dropbox(index == -1 ? 0 : index);
}
xui_method_explain(cocos_game, load_config,				void			)( void )
{
	FILE* file = fopen("cocos_editor/cocos_editor.resolution", "r");
	if (file)
	{
		while (feof(file) == false)
		{
			std::string line = xui_global::get_fileline(file);
			if (line.length() > 0)
			{
				s32 w = 0;
				s32 h = 0;
				sscanf(line.c_str(), "%d,%d", &w, &h);
				m_sizedata.push_back(xui_vector<f64>(w, h));
			}
		}

		fclose(file);
	}
	else
	{
		m_sizedata.push_back(xui_vector<f64>(640, 1136));
	}

	set_dropctrlupdate();
}
xui_method_explain(cocos_game, save_config,				void			)( void )
{
	FILE* file = fopen("cocos_editor/cocos_editor.resolution", "w");
	if (file)
	{
		char buffer[32];
		for (u32 i = 0; i < m_sizedata.size(); ++i)
		{
			sprintf(buffer, "%d,%d\n", (s32)m_sizedata[i].w, (s32)m_sizedata[i].h);
			fwrite(buffer, 1, strlen(buffer), file);
		}

		fclose(file);
	}

	set_dropctrlupdate();
}

/*
//callback
*/
xui_method_explain(cocos_game, on_perform,				void			)( xui_method_args& args )
{
	xui_dockpage::on_perform(args);

	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> sz = rt.get_sz();
	sz.h -= m_headpane->get_renderh();

	u32 index = m_dropctrl->get_selectedindex();
	if (index < m_sizedata.size())
	{
		s32 w  = m_sizedata[index].w;
		s32 h  = m_sizedata[index].h;
		f32 sw = (f32)sz.w / (f32)w;
		f32 sh = (f32)sz.h / (f32)h;
		f32 s  = (sw < sh ? sw : sh);
		if (s  > 1.0f)
			s  = 1.0f;
		s32 fw = (s32)(w*s);
		s32 fh = (s32)(h*s);
		m_drawview->on_perform_w(fw);
		m_drawview->on_perform_h(fh);
		m_drawview->on_perform_x((sz.w - fw) / 2);
		m_drawview->on_perform_y((sz.h - fh) / 2 + rt.ay + m_headpane->get_renderh());
	}
}
xui_method_explain(cocos_game, on_renderself,			void			)( xui_method_args& args )
{
	xui_dockpage::on_renderself(args);
	xui_dockview* dockview = xui_dynamic_cast(xui_dockview, m_parent);
	if (dockview->get_showpage() == this)
	{
		xui_rect2d<s32> rt = m_headpane->get_renderrtabs();
		xui_vector<s32> p1 = xui_vector<s32>(rt.ax, rt.by);
		xui_vector<s32> p2 = xui_vector<s32>(rt.bx, rt.by);
		xui_convas::get_ins()->draw_line(p1, p2, xui_colour::black);
	}
}

/*
//event
*/
xui_method_explain(cocos_game, on_userctrlclick,		void			)( xui_component* sender, xui_method_args&		args )
{
	if (m_propsize == NULL)
	{
		m_propsize = new xui_proproot;
		xui_propkind* kind = new xui_propkind(m_propsize, L"Resolution", "Resolution", xui_kindctrl::create, cocos_resource::icon_setting, false);
		m_propsize->add_propkind(kind);
		kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_game>(this, &cocos_game::on_propsizepropchanged);
		kind->add_propdata(new xui_propdata_stdvec_func< xui_vector<f64> >(
			kind,
			L"Config",
			xui_propctrl_stdvec::create,
			add_sizedata,
			del_sizedata,
			new_sizeprop,
			get_sizedata,
			this));
	}

	cocos_mainform::get_ptr()->get_inspector()->set_proproot(m_propsize);
}
xui_method_explain(cocos_game, on_dropctrlselection,	void			)( xui_component* sender, xui_method_args&		args )
{
	refresh();
}
xui_method_explain(cocos_game, on_drawviewupdateself,	void			)( xui_component* sender, xui_method_update&	args )
{
	if (m_viewprop)
	{
		cocos2d::Node* node = m_viewprop->get_node();
		node->update(args.delta);
	}
}
xui_method_explain(cocos_game, on_drawviewrenderself,	void			)( xui_component* sender, xui_method_args&		args )
{
	xui_convas::get_ins()->clear(xui_colour(1.0f, 0.3f));
	if (m_viewprop)
	{
		cocos2d::Node* node = m_viewprop->get_node();
		m_rootnode->addChild(node);
		cocos2d::GLView* glview = cocos2d::Director::getInstance()->getOpenGLView();
		glview->setDesignResolutionSize(640.0f, 852.0f, ResolutionPolicy::FIXED_WIDTH);
		//cocos2d::WeCCharFontManager::getInstance()->forceClearMemory();
		cocos2d::Director::getInstance()->drawScene();
		m_rootnode->removeAllChildrenWithCleanup(false);
	}
}
xui_method_explain(cocos_game, on_drawviewrenderelse,	void			)( xui_component* sender, xui_method_args&		args )
{
#if 0
	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();
	xui_convas::get_ins()->set_cliprect(cliprect.get_inter(m_drawview->get_renderrtabs()));

	xui_vector<s32> pt = m_drawview->get_screenpt();
	xui_vector<s32> p1;
	xui_vector<s32> p2;
	p1 = xui_vector<s32>(m_trans.x, 0);
	p2 = xui_vector<s32>(m_trans.x, m_drawview->get_renderh());
	xui_convas::get_ins()->draw_line(pt + p1, pt + p2, xui_colour::gray);
	p1 = xui_vector<s32>(0, m_drawview->get_renderh() - m_trans.y);
	p2 = xui_vector<s32>(m_drawview->get_renderw(), m_drawview->get_renderh() - m_trans.y);
	xui_convas::get_ins()->draw_line(pt + p1, pt + p2, xui_colour::gray);

	std::vector<s32> linevec;
	linevec = m_horzgrad->get_lines();
	for (u32 i = 0; i < linevec.size(); ++i)
	{
		s32 line = linevec[i];
		p1 = xui_vector<s32>(xui_round(line*m_ratio) + m_trans.x, 0);
		p2 = xui_vector<s32>(xui_round(line*m_ratio) + m_trans.x, m_drawview->get_renderh());
		xui_convas::get_ins()->draw_line(pt + p1, pt + p2, xui_colour(1.0f, 0.0f, 1.0f, 1.0f));
	}
	linevec = m_vertgrad->get_lines();
	for (u32 i = 0; i < linevec.size(); ++i)
	{
		s32 line = linevec[i];
		p1 = xui_vector<s32>(0, m_drawview->get_renderh() - (xui_round(line*m_ratio) + m_trans.y));
		p2 = xui_vector<s32>(m_drawview->get_renderw(), m_drawview->get_renderh() - (xui_round(line*m_ratio) + m_trans.y));
		xui_convas::get_ins()->draw_line(pt + p1, pt + p2, xui_colour(1.0f, 0.0f, 1.0f, 1.0f));
	}


	cocos_boundbox_vec  selectedvec = get_selectedboundbox();
	for (u32 i = 0; i < selectedvec.size(); ++i)
		selectedvec[i]->draw(m_trans, m_ratio, pt);

	if (m_operator == BO_MOVE)
	{
		cocos_boundbox*	selfbbox = m_snaptool->cal_selfbbox(selectedvec);
		if (selfbbox)
		{
			//onity_propeditnode*		selfprop = dynamic_cast<onity_propeditnode*>(selfbbox->get_linkprop());
			//xui_treedata*			treedata = selfprop->get_linkdata(treeview);
			//xui_treenode*			treenode = treedata->get_node();
			//xui_treenode*			rootnode = treenode->get_rootnode();

			//cocos_boundbox_vec vec;
			//for (u32 i = 0; i < rootnode->get_leafnodecount(); ++i)
			//{
			//	xui_treenode*		leafnode = rootnode->get_leafnode(i);
			//	if (leafnode->was_selected())
			//		continue;

			//	onity_treedata*		leafdata = (onity_treedata*)leafnode->get_linkdata();
			//	onity_propeditnode* leafprop = dynamic_cast<onity_propeditnode*>(leafdata->get_prop());
			//	onity_boundbox*		leafbbox = leafprop->get_boundbox();
			//	vec.push_back(leafbbox);
			//}

			//m_snaptool->set_snapdraw(vec, selectedvec);
		}
	}

	xui_convas::get_ins()->set_cliprect(cliprect);
#endif
}
xui_method_explain(cocos_game, on_propsizepropchanged,	void			)( xui_component* sender, xui_method_propdata&	args )
{
	save_config();
}

/*
//static
*/
xui_method_explain(cocos_game, get_sizedata,			cocos_sizedata&	)( void* userptr )
{
	return cocos_mainform::get_ptr()->get_game()->get_sizedata();
}
xui_method_explain(cocos_game, add_sizedata,			void			)( void* userptr )
{
	cocos_mainform::get_ptr()->get_game()->add_sizedata();
}
xui_method_explain(cocos_game, del_sizedata,			void			)( void* userptr )
{
	cocos_mainform::get_ptr()->get_game()->del_sizedata();
}
xui_method_explain(cocos_game, new_sizeprop,			xui_propdata*	)( void* userptr, u32 index, xui_propkind* kind )
{
	return cocos_mainform::get_ptr()->get_game()->new_sizeprop(index, kind);
}
xui_method_explain(cocos_game, get_size,				xui_vector<f64>	)( void* userptr )
{
	u32 index = (u32)userptr;
	cocos_sizedata& sizedata = cocos_mainform::get_ptr()->get_game()->get_sizedata();
	return sizedata[index];
}
xui_method_explain(cocos_game, set_size,				void			)( void* userptr, const xui_vector<f64>& value )
{
	u32 index = (u32)userptr;
	cocos_sizedata& sizedata = cocos_mainform::get_ptr()->get_game()->get_sizedata();
	sizedata[index] = value;
}
