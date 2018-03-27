#include "2d/CCSpriteFrame.h"

#include "xui_global.h"
#include "xui_toggle.h"
#include "xui_textbox.h"
#include "xui_linebox.h"
#include "xui_slider.h"
#include "xui_desktop.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "cocos_filedata.h"
#include "cocos_resource.h"
#include "cocos_treedata.h"
#include "cocos_fileview.h"
#include "cocos_tileview.h"
#include "cocos_renderview.h"
#include "cocos_propfile.h"
#include "cocos_propatlas.h"
#include "cocos_propframe.h"
#include "cocos_framedata.h"
#include "cocos_propleaf.h"
#include "cocos_preview.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_selector.h"

xui_implement_rtti(cocos_selector, xui_object_pickwnd);

/*
//constructor
*/
xui_create_explain(cocos_selector)( void )
: xui_object_pickwnd()
{
	m_title		= new xui_drawer(xui_vector<s32>(24));
	xui_method_ptrcall(m_title,		ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_title,		ini_drawer			)(L"Select");
	xui_method_ptrcall(m_title,		set_textalign		)(TEXTALIGN_LT);

	m_clear		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		xm_buttonclick		) += new xui_method_member<xui_method_args,		cocos_selector>(this, &cocos_selector::on_clearclick);
	xui_method_ptrcall(m_clear,		ini_component		)(true, false);
	xui_method_ptrcall(m_clear,		ini_component		)(0, ALIGNVERT_C, DOCKSTYLE_U);
	xui_method_ptrcall(m_clear,		set_corner			)(8);
	xui_method_ptrcall(m_clear,		set_drawcolor		)(true);
	xui_method_ptrcall(m_clear,		ini_drawer			)(cocos_resource::icon_clear);

	m_search	= new xui_textbox(xui_vector<s32>(100, 20));
	xui_method_ptrcall(m_search,	xm_textchanged		) += new xui_method_member<xui_method_args,		cocos_selector>(this, &cocos_selector::on_searchtextchanged);
	xui_method_ptrcall(m_search,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_search,	ini_drawer			)(cocos_resource::icon_search);
	xui_method_ptrcall(m_search,	set_backcolor		)(xui_colour(1.0f, 0.20f));
	xui_method_ptrcall(m_search,	set_drawcolor		)(true);
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_search,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_search,	set_textoffset		)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(8, 2, 24, 2));
	xui_method_ptrcall(m_search,	set_corner			)(10);

	m_header	= new xui_panel(xui_vector<s32>(24));
	xui_method_ptrcall(m_header,	xm_perform			) += new xui_method_member<xui_method_args,		cocos_selector>(this, &cocos_selector::on_headerperform);
	xui_method_ptrcall(m_header,	set_drawcolor		)(false);
	xui_method_ptrcall(m_header,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_header,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_header,	add_child			)(m_search);
	xui_method_ptrcall(m_header,	add_child			)(m_clear);

	m_asset		= new xui_toggle(xui_vector<s32>(60, 24), TOGGLE_BUTTON);
	xui_method_ptrcall(m_asset,		xm_toggleclick		) += new xui_method_member<xui_method_args,		cocos_selector>(this, &cocos_selector::on_assetclick);
	xui_method_ptrcall(m_asset,		set_corner			)(3);
	xui_method_ptrcall(m_asset,		set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_asset,		set_drawcolor		)(true);
	xui_method_ptrcall(m_asset,		set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_asset,		set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_asset,		ini_drawer			)(L"Asset");

	m_scene		= new xui_toggle(xui_vector<s32>(60, 24), TOGGLE_BUTTON);
	xui_method_ptrcall(m_scene,		xm_toggleclick		) += new xui_method_member<xui_method_args,		cocos_selector>(this, &cocos_selector::on_sceneclick);
	xui_method_ptrcall(m_scene,		set_corner			)(3);
	xui_method_ptrcall(m_scene,		set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_scene,		set_drawcolor		)(true);
	xui_method_ptrcall(m_scene,		set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_scene,		set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_scene,		ini_drawer			)(L"Scene");

	m_filter	= new xui_linebox(xui_vector<s32>(24), FLOWSTYLE_H);
	xui_method_ptrcall(m_filter,	ini_component		)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_filter,	set_corner			)(3);
	xui_method_ptrcall(m_filter,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_filter,	add_linectrl		)(m_asset);
	xui_method_ptrcall(m_filter,	add_linectrl		)(m_scene);

	m_backpath	= new xui_button(xui_vector<s32>(24));
	xui_method_ptrcall(m_backpath,	ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_backpath,	ini_drawer			)(cocos_resource::icon_left);
	xui_method_ptrcall(m_backpath,	set_drawcolor		)(true);
	xui_method_ptrcall(m_backpath,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_backpath,	set_enable			)(false);
	xui_method_ptrcall(m_backpath,	set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_backpath,	set_corner			)(3);
	xui_method_ptrcall(m_backpath,	xm_buttonclick		) += new xui_method_member<xui_method_args,		cocos_selector>(this, &cocos_selector::on_backpathclick);

	m_slider	= new xui_slider(xui_vector<s32>(64, 16), FLOWSTYLE_H, ARROWDRAW_NONE);
	xui_method_ptrcall(m_slider,	xm_scroll			) += new xui_method_member<xui_method_args,		cocos_selector>(this, &cocos_selector::on_sliderscroll);
	xui_method_ptrcall(m_slider,	set_backcolor		)(xui_colour(1.0f, 0.2f));
	xui_method_ptrcall(m_slider,	set_borderrt		)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_slider,	ini_component		)(ALIGNHORZ_R, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_slider,	ini_scroll			)(60, 0);

	m_middle	= new xui_panel(xui_vector<s32>(32));
	xui_method_ptrcall(m_middle,	xm_perform			) += new xui_method_member<xui_method_args,		cocos_selector>(this, &cocos_selector::on_middleperform);
	xui_method_ptrcall(m_middle,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_middle,	set_drawcolor		)(false);
	xui_method_ptrcall(m_middle,	add_child			)(m_filter);
	xui_method_ptrcall(m_middle,	add_child			)(m_backpath);
	xui_method_ptrcall(m_middle,	add_child			)(m_slider);

	m_fileview	= new cocos_fileview;
	xui_method_ptrcall(m_fileview,	xm_fileviewselection) += new xui_method_member<xui_method_args,		cocos_selector>(this, &cocos_selector::on_fileviewselection);
	xui_method_ptrcall(m_fileview,	xm_fileviewdoubleclk) += new xui_method_member<xui_method_mouse,	cocos_selector>(this, &cocos_selector::on_fileviewdoubleclk);
	xui_method_ptrcall(m_fileview,	ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_fileview,	set_sidestyle		)(SIDESTYLE_S);

	m_sizectrl	= new xui_control(xui_vector<s32>(4));
	xui_method_ptrcall(m_sizectrl,	xm_mousemove		) += new xui_method_member<xui_method_mouse,	cocos_selector>(this, &cocos_selector::on_sizectrlmousemove);
	xui_method_ptrcall(m_sizectrl,	ini_component		)(0, 0, DOCKSTYLE_U);
	xui_method_ptrcall(m_sizectrl,	set_cursor			)(CURSOR_NS);

	m_drawview	= new cocos_preview;
	xui_method_ptrcall(m_drawview,	set_backcolor		)(xui_colour(1.0f, 0.15f));
	xui_method_ptrcall(m_drawview,	set_drawcolor		)(true);
	xui_method_ptrcall(m_drawview,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_drawview,	ini_component		)(0, 0, DOCKSTYLE_L);

	m_accept	= new xui_button(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_accept,	xm_buttonclick		) += new xui_method_member<xui_method_args,		cocos_selector>(this, &cocos_selector::on_accept);
	xui_method_ptrcall(m_accept,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_accept,	set_corner			)(3);
	xui_method_ptrcall(m_accept,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_accept,	set_drawcolor		)(true);
	xui_method_ptrcall(m_accept,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_accept,	set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_accept,	ini_component		)(0, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_accept,	ini_drawer			)(L"Accept");

	m_cancel	= new xui_button(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_cancel,	xm_buttonclick		) += new xui_method_member<xui_method_args,		cocos_selector>(this, &cocos_selector::on_cancel);
	xui_method_ptrcall(m_cancel,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_cancel,	set_corner			)(3);
	xui_method_ptrcall(m_cancel,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_cancel,	set_drawcolor		)(true);
	xui_method_ptrcall(m_cancel,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_cancel,	set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_cancel,	ini_component		)(ALIGNHORZ_R, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_cancel,	ini_drawer			)(L"Cancel");

	m_bottom	= new xui_panel(xui_vector<s32>(128, 200));
	xui_method_ptrcall(m_bottom,	xm_perform			) += new xui_method_member<xui_method_args,		cocos_selector>(this, &cocos_selector::on_bottomperform);
	xui_method_ptrcall(m_bottom,	ini_component		)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_bottom,	set_borderrt		)(xui_rect2d<s32>(0, 4, 0, 0));
	xui_method_ptrcall(m_bottom,	add_child			)(m_drawview);
	xui_method_ptrcall(m_bottom,	add_child			)(m_accept);
	xui_method_ptrcall(m_bottom,	add_child			)(m_cancel);

	add_child(m_title);
	add_child(m_header);
	add_child(m_middle);
	add_child(m_bottom);
	add_child(m_fileview);
	add_child(m_sizectrl);
}

/*
//static
*/
xui_method_explain(cocos_selector, get_ptr,					xui_object_pickwnd*	)( void )
{
	static xui_object_pickwnd* ptr = NULL;
	if (ptr == NULL)
	{
		ptr = new cocos_selector;
		xui_method_ptrcall(ptr, set_sidestyle	)(SIDESTYLE_S);
		xui_method_ptrcall(ptr, set_rendersz	)(xui_vector<s32>(800, 600));
		xui_method_ptrcall(ptr, set_renderpt	)(xui_vector<s32>(200, 100));
		xui_method_ptrcall(ptr, set_borderrt	)(xui_rect2d<s32>(8));
		xui_method_ptrcall(ptr, ini_component	)(true, false);
		xui_desktop::get_ins()->add_child(ptr);
	}

	return ptr;
}

/*
//override
*/
xui_method_explain(cocos_selector, get_value,				void*				)( void )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propctrl->get_propdata().front());

	std::vector<xui_treenode*> vec = m_fileview->get_lineview()->get_selectednode();
	if (vec.size() > 0)
	{
		xui_treenode*   node = vec.front();
		cocos_treedata* data = (cocos_treedata*)node->get_linkdata();
		xui_proproot*   prop = data->get_prop();

		cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(prop);
		cocos_propleaf* propleaf = dynamic_cast<cocos_propleaf*>(prop);
		if (propfile && dataobject->has_droptype(propfile->get_dragtype()))
			return propfile->get_dragdata();
		if (propleaf && dataobject->has_droptype(propfile->get_dragtype()))
			return propleaf->get_dragdata();
	}

	return NULL;
}
xui_method_explain(cocos_selector, set_value,				void				)( void* value )
{
	xui_method_ptrcall(m_asset,  ini_toggle )(true);
	xui_method_ptrcall(m_scene,	 ini_toggle	)(false);
	xui_method_ptrcall(m_search, set_text	)(L"");
	refresh_fileview();

	xui_treeview*   lineview = m_fileview->get_lineview();
	cocos_tileview* tileview = m_fileview->get_tileview();
	xui_treenode*   lastnode = NULL;
	std::vector<xui_treenode*> vec = lineview->get_entirenode();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_treenode*   node = vec[i];
		cocos_treedata* data = (cocos_treedata*)node->get_linkdata();
		xui_proproot*   prop = data->get_prop();

		cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(prop);
		cocos_propleaf* propleaf = dynamic_cast<cocos_propleaf*>(prop);
		if (propfile && propfile->get_dragdata() == value)
		{
			lastnode = node;
			break;
		}
		if (propleaf && propleaf->get_dragdata() == value)
		{
			lastnode = node;
			break;
		}
	}

	if (lastnode)
	{
		lineview->set_selectednode(lastnode, true);
		if (lineview->was_visible())
			lineview->set_nodevisible(lastnode);
		if (tileview->was_visible())
			tileview->set_tilevisible(lastnode);
	}
}

/*
//callback
*/
xui_method_explain(cocos_selector, on_perform,				void				)( xui_method_args& args )
{
	xui_window::on_perform(args);
	xui_rect2d<s32> rt = m_fileview->get_renderrt()+m_fileview->get_renderpt();
	m_sizectrl->on_perform_w(rt.get_w());
	m_sizectrl->on_perform_x(rt.ax);
	m_sizectrl->on_perform_y(rt.by-m_sizectrl->get_renderh());
}

/*
//event
*/
xui_method_explain(cocos_selector, on_searchtextchanged,	void				)( xui_component* sender, xui_method_args& args )
{
	m_clear->ini_component(true, m_search->get_text().length() > 0);
	if (m_fileview->get_tileview()->get_viewfile())
	{
		m_fileview->get_tileview()->set_viewtext(xui_global::get_upper(m_search->get_text()));
	}
	else
	{
		refresh_fileview();
		refresh_backpath();
	}
}
xui_method_explain(cocos_selector, on_backpathclick,		void				)( xui_component* sender, xui_method_args& args )
{
	if (m_fileview->get_tileview()->get_viewfile())
	{
		m_fileview->get_tileview()->get_viewfile()->set_expanded(false);
		m_fileview->get_tileview()->set_viewfile(NULL, L"");
	}

	m_slider->set_value((s32)m_slider->get_data());
	refresh_backpath();
}
xui_method_explain(cocos_selector, on_assetclick,			void				)( xui_component* sender, xui_method_args& args )
{
	m_asset->ini_toggle(true);
	m_scene->ini_toggle(false);
	refresh_fileview();
}
xui_method_explain(cocos_selector, on_sceneclick,			void				)( xui_component* sender, xui_method_args& args )
{
	m_asset->ini_toggle(false);
	m_scene->ini_toggle(true);
	refresh_fileview();
}
xui_method_explain(cocos_selector, on_sliderscroll,			void				)( xui_component* sender, xui_method_args& args )
{
	xui_vector<s32> move = xui_desktop::get_ins()->get_mousemove();

	s32 value = m_slider->get_value();
	if (value < 10 && (value > 0 && m_fileview->get_tileview()->get_viewfile() != NULL))
	{
		value = 10;
		m_slider->ini_scroll(m_slider->get_range(), value);
	}

	xui_treeview*      lineview = m_fileview->get_lineview();
	cocos_tileview*	   tileview = m_fileview->get_tileview();
	bool               lineshow = m_fileview->get_lineview()->was_visible();
	bool               tileshow = m_fileview->get_tileview()->was_visible();
	xui_method_ptrcall(lineview, set_visible	)(value == 0);
	xui_method_ptrcall(tileview, set_visible	)(value >  0);
	xui_method_ptrcall(tileview, set_tilesize	)(xui_max(10, value) + 40);
	m_fileview->refresh();

	std::vector<xui_treenode*> nodevec = lineview->get_selectednode();
	if (nodevec.size() > 0)
	{
		if (tileview->was_visible() && tileshow == false)
			tileview->set_tilevisible(nodevec.front());
		if (lineview->was_visible() && lineshow == false)
			lineview->set_nodevisible(nodevec.front());
	}
}
xui_method_explain(cocos_selector, on_fileviewselection,	void				)( xui_component* sender, xui_method_args& args )
{
	std::vector<xui_treenode*> vec = m_fileview->get_lineview()->get_selectednode();
	if (vec.size() > 0)
	{
		xui_treenode*   node = vec.front();
		cocos_treedata* data = (cocos_treedata*)node->get_linkdata();
		xui_proproot*   prop = data->get_prop();
		m_drawview->set_viewprop(prop);
	}
}
xui_method_explain(cocos_selector, on_fileviewdoubleclk,	void				)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == MB_L)
	{
		std::vector<xui_treenode*> vec = m_fileview->get_lineview()->get_selectednode();
		if (vec.size() > 0)
		{
			xui_treenode*   node = vec.front();
			cocos_treedata* data = (cocos_treedata*)node->get_linkdata();
			cocos_propfile* prop = dynamic_cast<cocos_propfile*>(data->get_prop());
			if (prop && prop->get_dragtype().length() == 0)
			{
				cocos_tileview* tileview = m_fileview->get_tileview();
				tileview->set_viewfile(node, xui_global::get_upper(m_search->get_text()));
				refresh_backpath();

				m_slider->set_data((void*)m_slider->get_value());
				if (m_slider->get_value() == 0)
					m_slider->set_value(10);
			}
			else
			{
				xui_method_args other_args;
				on_accept(this, other_args);
			}
		}
	}
}
xui_method_explain(cocos_selector, on_sizectrlmousemove,	void				)( xui_component* sender, xui_method_mouse& args )
{
	if (m_sizectrl->has_catch())
	{
		s32 delta  = xui_desktop::get_ins()->get_mousemove().y;
		s32 height = m_bottom->get_renderh() - delta;
		height = xui_max(height, 200);
		height = xui_min(height, 300);
		m_bottom->set_renderh(height);		
	}
}
xui_method_explain(cocos_selector, on_clearclick,			void				)( xui_component* sender, xui_method_args& args )
{
	m_search->set_text(L"");
}
xui_method_explain(cocos_selector, on_headerperform,		void				)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt = m_header->get_renderrtins();
	xui_method_ptrcall(m_clear,	   on_perform_x)(rt.ax+m_search->get_renderw()-m_clear->get_renderw()-2);
}
xui_method_explain(cocos_selector, on_middleperform,		void				)( xui_component* sender, xui_method_args& args )
{
	xui_method_ptrcall(m_backpath, on_perform_x)(m_filter->get_renderx()+m_filter->get_renderw()+6);
}
xui_method_explain(cocos_selector, on_bottomperform,		void				)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt = m_bottom->get_renderrtins();
	xui_method_ptrcall(m_drawview, on_perform_w)(rt.get_h()-24);
	xui_method_ptrcall(m_accept,   on_perform_x)(m_cancel->get_renderx()-m_accept->get_renderw()-10);
}

/*
//method
*/
xui_method_explain(cocos_selector, refresh_fileview,		void				)( void )
{
	m_fileview->clear();
	if (m_scene->was_push())
		return;

	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propctrl->get_propdata().front());
	xui_droptype_vec typevec = dataobject->get_droptype();
	xui_proproot_vec filevec;
	for (u32 i = 0; i < typevec.size(); ++i)
	{
		s32 type = convert_droptype(typevec[i]);
		cocos_mainform::get_ptr()->get_project()->get_pathfile(type, filevec);
	}

	std::vector<std::wstring> itemvec = xui_global::get_split(m_search->get_text(), L';');
	std::wstring			  filekey = xui_global::get_upper((itemvec.size() > 0) ? itemvec[0] : L"");
	std::wstring			  leafkey = xui_global::get_upper((itemvec.size() > 1) ? itemvec[1] : L"");

	xui_treeview*   lineview = m_fileview->get_lineview();
	cocos_tileview* tileview = m_fileview->get_tileview();
	lineview->add_upmostnode(0, new cocos_filedata(NULL, L"None", NULL));

	std::wstring keytext = xui_global::get_upper(m_search->get_text());
	for (u32 i = 0, index = 1; i < filevec.size(); ++i)
	{
		cocos_propfile*		prop		= dynamic_cast<cocos_propfile*>(filevec[i]);
		std::wstring		safe		= cocos_filedata::get_safe(prop->get_fullname());
		cocos_propatlas*	propatlas	= dynamic_cast<cocos_propatlas*>(prop);
		bool showroot = (keytext.length() == 0 || xui_global::get_upper(safe).find(keytext) != -1);
		bool showleaf = (keytext.length() != 0 && propatlas && propatlas->has_frame(keytext));
		if (showroot || showleaf)
		{
			xui_treenode* node = lineview->add_upmostnode(index, new cocos_filedata(prop->get_fileicon(), prop->get_fullname(), prop));
			if (propatlas)
			{
				if (showleaf)
					node->set_expanded(true);

				std::vector<xui_proproot*> subprop = propatlas->get_subprop();
				for (u32 isub = 0, isubindex = 0; isub < subprop.size(); ++isub)
				{
					cocos_propframe* propframe = dynamic_cast<cocos_propframe*>(subprop[isub]);
					std::wstring framename = xui_global::ascii_to_unicode(propframe->get_framename());
					if (showroot || xui_global::get_upper(framename).find(keytext) != -1)
					{
						node->add_leafnode(isubindex, new cocos_framedata(propframe->get_frameicon(), propframe));
						++isubindex;
					}
				}
			}

			++index;
		}
	}

	lineview->refresh();
	tileview->get_drawview()->refresh();
}
xui_method_explain(cocos_selector, refresh_backpath,		void				)( void )
{
	m_backpath->set_enable(m_search->get_text().length() == 0 && m_fileview->get_tileview()->get_viewfile() != NULL);
}
xui_method_explain(cocos_selector, convert_droptype,		s32					)( const std::string& droptype )
{
	if		(droptype == "Texture2D")		return FILTER_TEXTURE;
	else if (droptype == "SpriteFrame")		return FILTER_ATLAS;
	else if (droptype == "ParticleSystem")	return FILTER_PARTICLE;
	else if (droptype == "Material")		return FILTER_MATERIAL;
	else if (droptype == "VertShader")		return FILTER_VERTSHADER;
	else if (droptype == "FragShader")		return FILTER_FRAGSHADER;
	else if (droptype == "TTF")				return FILTER_TTF;
	else if (droptype == "FNT")				return FILTER_FNT;
	else if (droptype == "SpineAtlas")		return FILTER_SPINEATLAS;
	else if (droptype == "Spine")			return FILTER_SPINE;
	else if (droptype == "C3B")				return FILTER_C3B;
	else if (droptype == "CSD")				return FILTER_CSD;
	else									return FILTER_NONE;
}