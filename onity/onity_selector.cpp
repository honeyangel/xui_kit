#include "NPStringUtil.h"
#include "NP2DSAsset.h"

#include "xui_global.h"
#include "xui_toggle.h"
#include "xui_textbox.h"
#include "xui_linebox.h"
#include "xui_slider.h"
#include "xui_desktop.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "onity_filedata.h"
#include "onity_2dsassetdata.h"
#include "onity_prop2dsres.h"
#include "onity_prop2dsasset.h"
#include "onity_tempdata.h"
#include "onity_resource.h"
#include "onity_treedata.h"
#include "onity_fileview.h"
#include "onity_tileview.h"
#include "onity_renderview.h"
#include "onity_propfile.h"
#include "onity_propleaf.h"
#include "onity_propjson.h"
#include "onity_proptempold.h"
#include "onity_preview.h"
#include "onity_mainform.h"
#include "onity_project.h"
#include "onity_selector.h"

xui_implement_rtti(onity_selector, xui_object_pickwnd);

/*
//constructor
*/
xui_create_explain(onity_selector)( void )
: xui_object_pickwnd()
{
	m_title		= new xui_drawer(xui_vector<s32>(24));
	xui_method_ptrcall(m_title,		ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_title,		ini_drawer			)(L"Select");
	xui_method_ptrcall(m_title,		set_textalign		)(TEXTALIGN_LT);

	m_clear		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_selector>(this, &onity_selector::on_clearclick);
	xui_method_ptrcall(m_clear,		ini_component		)(true, false);
	xui_method_ptrcall(m_clear,		ini_component		)(0, ALIGNVERT_C, DOCKSTYLE_U);
	xui_method_ptrcall(m_clear,		set_corner			)(8);
	xui_method_ptrcall(m_clear,		set_drawcolor		)(true);
	xui_method_ptrcall(m_clear,		ini_drawer			)(onity_resource::icon_clear);

	m_search	= new xui_textbox(xui_vector<s32>(100, 20));
	xui_method_ptrcall(m_search,	xm_textchanged		) += new xui_method_member<xui_method_args,		onity_selector>(this, &onity_selector::on_searchtextchanged);
	xui_method_ptrcall(m_search,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_search,	ini_drawer			)(onity_resource::icon_search);
	xui_method_ptrcall(m_search,	set_backcolor		)(xui_colour(1.0f, 0.20f));
	xui_method_ptrcall(m_search,	set_drawcolor		)(true);
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_search,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_search,	set_textoffset		)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(8, 2, 24, 2));
	xui_method_ptrcall(m_search,	set_corner			)(10);

	m_header	= new xui_panel(xui_vector<s32>(24));
	xui_method_ptrcall(m_header,	xm_perform			) += new xui_method_member<xui_method_args,		onity_selector>(this, &onity_selector::on_headerperform);
	xui_method_ptrcall(m_header,	set_drawcolor		)(false);
	xui_method_ptrcall(m_header,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_header,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_header,	add_child			)(m_search);
	xui_method_ptrcall(m_header,	add_child			)(m_clear);

	m_asset		= new xui_toggle(xui_vector<s32>(60, 24), TOGGLE_BUTTON);
	xui_method_ptrcall(m_asset,		xm_toggleclick		) += new xui_method_member<xui_method_args,		onity_selector>(this, &onity_selector::on_assetclick);
	xui_method_ptrcall(m_asset,		set_corner			)(3);
	xui_method_ptrcall(m_asset,		set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_asset,		set_drawcolor		)(true);
	xui_method_ptrcall(m_asset,		set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_asset,		set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_asset,		ini_drawer			)(L"Asset");

	m_scene		= new xui_toggle(xui_vector<s32>(60, 24), TOGGLE_BUTTON);
	xui_method_ptrcall(m_scene,		xm_toggleclick		) += new xui_method_member<xui_method_args,		onity_selector>(this, &onity_selector::on_sceneclick);
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
	xui_method_ptrcall(m_backpath,	ini_drawer			)(onity_resource::icon_left);
	xui_method_ptrcall(m_backpath,	set_drawcolor		)(true);
	xui_method_ptrcall(m_backpath,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_backpath,	set_enable			)(false);
	xui_method_ptrcall(m_backpath,	set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_backpath,	set_corner			)(3);
	xui_method_ptrcall(m_backpath,	xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_selector>(this, &onity_selector::on_backpathclick);

	m_slider	= new xui_slider(xui_vector<s32>(64, 16), FLOWSTYLE_H, ARROWDRAW_NONE);
	xui_method_ptrcall(m_slider,	xm_scroll			) += new xui_method_member<xui_method_args,		onity_selector>(this, &onity_selector::on_sliderscroll);
	xui_method_ptrcall(m_slider,	set_backcolor		)(xui_colour(1.0f, 0.2f));
	xui_method_ptrcall(m_slider,	set_borderrt		)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_slider,	ini_component		)(ALIGNHORZ_R, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_slider,	ini_scroll			)(60, 0);

	m_middle	= new xui_panel(xui_vector<s32>(32));
	xui_method_ptrcall(m_middle,	xm_perform			) += new xui_method_member<xui_method_args,		onity_selector>(this, &onity_selector::on_middleperform);
	xui_method_ptrcall(m_middle,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_middle,	set_drawcolor		)(false);
	xui_method_ptrcall(m_middle,	add_child			)(m_filter);
	xui_method_ptrcall(m_middle,	add_child			)(m_backpath);
	xui_method_ptrcall(m_middle,	add_child			)(m_slider);

	m_fileview	= new onity_fileview;
	xui_method_ptrcall(m_fileview,	xm_fileviewselection) += new xui_method_member<xui_method_args,		onity_selector>(this, &onity_selector::on_fileviewselection);
	xui_method_ptrcall(m_fileview,	xm_fileviewdoubleclk) += new xui_method_member<xui_method_mouse,	onity_selector>(this, &onity_selector::on_fileviewdoubleclk);
	xui_method_ptrcall(m_fileview,	ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_fileview,	set_sidestyle		)(SIDESTYLE_S);

	m_sizectrl	= new xui_control(xui_vector<s32>(4));
	xui_method_ptrcall(m_sizectrl,	xm_mousemove		) += new xui_method_member<xui_method_mouse,	onity_selector>(this, &onity_selector::on_sizectrlmousemove);
	xui_method_ptrcall(m_sizectrl,	ini_component		)(0, 0, DOCKSTYLE_U);
	xui_method_ptrcall(m_sizectrl,	set_cursor			)(CURSOR_NS);

	m_drawview	= new onity_preview;
	xui_method_ptrcall(m_drawview,	set_backcolor		)(xui_colour(1.0f, 0.15f));
	xui_method_ptrcall(m_drawview,	set_drawcolor		)(true);
	xui_method_ptrcall(m_drawview,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_drawview,	ini_component		)(0, 0, DOCKSTYLE_L);

	m_accept	= new xui_button(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_accept,	xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_selector>(this, &onity_selector::on_accept);
	xui_method_ptrcall(m_accept,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_accept,	set_corner			)(3);
	xui_method_ptrcall(m_accept,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_accept,	set_drawcolor		)(true);
	xui_method_ptrcall(m_accept,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_accept,	set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_accept,	ini_component		)(0, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_accept,	ini_drawer			)(L"Accept");

	m_cancel	= new xui_button(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_cancel,	xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_selector>(this, &onity_selector::on_cancel);
	xui_method_ptrcall(m_cancel,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_cancel,	set_corner			)(3);
	xui_method_ptrcall(m_cancel,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_cancel,	set_drawcolor		)(true);
	xui_method_ptrcall(m_cancel,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_cancel,	set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_cancel,	ini_component		)(ALIGNHORZ_R, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_cancel,	ini_drawer			)(L"Cancel");

	m_bottom	= new xui_panel(xui_vector<s32>(128, 200));
	xui_method_ptrcall(m_bottom,	xm_perform			) += new xui_method_member<xui_method_args,		onity_selector>(this, &onity_selector::on_bottomperform);
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
xui_method_explain(onity_selector, get_ptr,					xui_object_pickwnd*	)( void )
{
	static xui_object_pickwnd* ptr = NULL;
	if (ptr == NULL)
	{
		ptr = new onity_selector;
		xui_method_ptrcall(ptr, set_sidestyle	)(SIDESTYLE_S);
		xui_method_ptrcall(ptr, set_rendersz	)(xui_vector<s32>(800, 600));
		xui_method_ptrcall(ptr, set_renderpt	)(xui_vector<s32>(200,  100));
		xui_method_ptrcall(ptr, set_borderrt	)(xui_rect2d<s32>(8));
		xui_method_ptrcall(ptr, ini_component	)(true, false);
		xui_desktop::get_ins()->add_child(ptr);
	}

	return ptr;
}

/*
//override
*/
xui_method_explain(onity_selector, get_value,				void*				)( void )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propctrl->get_propdata().front());

	std::vector<xui_treenode*> vec = m_fileview->get_lineview()->get_selectednode();
	if (vec.size() > 0)
	{
		xui_treenode*   node = vec.front();
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		xui_proproot*   prop = data->get_prop();

		onity_propfile* propfile = dynamic_cast<onity_propfile*>(prop);
		onity_propleaf* propleaf = dynamic_cast<onity_propleaf*>(prop);
		if (propfile && dataobject->has_droptype(propfile->get_dragtype()))
			return propfile->get_dragdata();
		if (propleaf && dataobject->has_droptype(propfile->get_dragtype()))
			return propleaf->get_dragdata();
	}

	return NULL;
}
xui_method_explain(onity_selector, set_value,				void				)( void* value )
{
	xui_method_ptrcall(m_asset,  ini_toggle )(true);
	xui_method_ptrcall(m_scene,	 ini_toggle	)(false);
	xui_method_ptrcall(m_search, set_text	)(L"");
	refresh_fileview();

	xui_treeview*   lineview = m_fileview->get_lineview();
	onity_tileview* tileview = m_fileview->get_tileview();
	xui_treenode*   lastnode = NULL;
	std::vector<xui_treenode*> vec = lineview->get_entirenode();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_treenode*   node = vec[i];
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		xui_proproot*   prop = data->get_prop();

		onity_propfile* propfile = dynamic_cast<onity_propfile*>(prop);
		onity_propleaf* propleaf = dynamic_cast<onity_propleaf*>(prop);
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
xui_method_explain(onity_selector, on_perform,				void				)( xui_method_args& args )
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
xui_method_explain(onity_selector, on_searchtextchanged,	void				)( xui_component* sender, xui_method_args& args )
{
	m_clear->ini_component(true, m_search->get_text().length() > 0);
	refresh_fileview();
	refresh_backpath();
}
xui_method_explain(onity_selector, on_backpathclick,		void				)( xui_component* sender, xui_method_args& args )
{
	if (m_fileview->get_tileview()->get_viewfile())
	{
		m_fileview->get_tileview()->get_viewfile()->set_expanded(false);
		m_fileview->get_tileview()->set_viewfile(NULL);
	}

	m_slider->set_value((s32)m_slider->get_data());
	refresh_backpath();
}
xui_method_explain(onity_selector, on_assetclick,			void				)( xui_component* sender, xui_method_args& args )
{
	m_asset->ini_toggle(true);
	m_scene->ini_toggle(false);
	refresh_fileview();
}
xui_method_explain(onity_selector, on_sceneclick,			void				)( xui_component* sender, xui_method_args& args )
{
	m_asset->ini_toggle(false);
	m_scene->ini_toggle(true);
	refresh_fileview();
}
xui_method_explain(onity_selector, on_sliderscroll,			void				)( xui_component* sender, xui_method_args& args )
{
	xui_vector<s32> move = xui_desktop::get_ins()->get_mousemove();

	s32 value = m_slider->get_value();
	if (value < 10 && value > 0)
	{
		value = 10;
		m_slider->ini_scroll(m_slider->get_range(), value);
	}

	xui_treeview*      lineview = m_fileview->get_lineview();
	onity_tileview*	   tileview = m_fileview->get_tileview();
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
xui_method_explain(onity_selector, on_fileviewselection,	void				)( xui_component* sender, xui_method_args& args )
{
	std::vector<xui_treenode*> vec = m_fileview->get_lineview()->get_selectednode();
	if (vec.size() > 0)
	{
		xui_treenode*   node = vec.front();
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		xui_proproot*   prop = data->get_prop();
		m_drawview->set_viewprop(prop);
	}
}
xui_method_explain(onity_selector, on_fileviewdoubleclk,	void				)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == MB_L)
	{
		std::vector<xui_treenode*> vec = m_fileview->get_lineview()->get_selectednode();
		if (vec.size() > 0)
		{
			xui_treenode*   node = vec.front();
			onity_treedata* data = (onity_treedata*)node->get_linkdata();
			onity_propfile* prop = dynamic_cast<onity_propfile*>(data->get_prop());
			if (prop && prop->get_dragtype().length() == 0)
			{
				onity_tileview* tileview = m_fileview->get_tileview();
				tileview->set_viewfile(node);
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
xui_method_explain(onity_selector, on_sizectrlmousemove,	void				)( xui_component* sender, xui_method_mouse& args )
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
xui_method_explain(onity_selector, on_clearclick,			void				)( xui_component* sender, xui_method_args& args )
{
	m_search->set_text(L"");
}
xui_method_explain(onity_selector, on_headerperform,		void				)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt = m_header->get_renderrtins();
	xui_method_ptrcall(m_clear,	   on_perform_x)(rt.ax+m_search->get_renderw()-m_clear->get_renderw()-2);
}
xui_method_explain(onity_selector, on_middleperform,		void				)( xui_component* sender, xui_method_args& args )
{
	xui_method_ptrcall(m_backpath, on_perform_x)(m_filter->get_renderx()+m_filter->get_renderw()+6);
}
xui_method_explain(onity_selector, on_bottomperform,		void				)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt = m_bottom->get_renderrtins();
	xui_method_ptrcall(m_drawview, on_perform_w)(rt.get_h()-24);
	xui_method_ptrcall(m_accept,   on_perform_x)(m_cancel->get_renderx()-m_accept->get_renderw()-10);
}

/*
//method
*/
xui_method_explain(onity_selector, refresh_fileview,		void				)( void )
{
	m_fileview->clear();
	if (m_scene->was_push())
		return;

	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propctrl->get_propdata().front());
	xui_droptype_vec typevec = dataobject->get_droptype();
	xui_proproot_vec filevec;
	for (u32 i = 0; i < typevec.size(); ++i)
	{
		std::wstring suff = convert_dropsuff(typevec[i]);
		onity_mainform::get_ptr()->get_project()->get_pathfile(suff, filevec);
	}

	std::vector<std::string> itemvec = NPStringUtil::Split(xui_global::unicode_to_ascii(m_search->get_text()), ';');
	std::wstring			 filekey = (itemvec.size() > 0) ? xui_global::ascii_to_unicode(itemvec[0]) : L"";
	std::string				 leafkey = (itemvec.size() > 1) ? itemvec[1] : "";

	xui_treeview*   lineview = m_fileview->get_lineview();
	onity_tileview* tileview = m_fileview->get_tileview();
	lineview->add_upmostnode(0, new onity_filedata(NULL, L"None", NULL));
	for (u32 i = 0, index = 1; i < filevec.size(); ++i)
	{
		onity_propfile* prop = dynamic_cast<onity_propfile*>(filevec[i]);
		if (filekey.length() == 0 || onity_filedata::get_file(prop->get_fullname()).find(filekey) != -1)
		{
			xui_treenode* node = lineview->add_upmostnode(index, new onity_filedata(prop->get_fileicon(), prop->get_fullname(), prop));
			onity_prop2dsres* prop2dsres = dynamic_cast<onity_prop2dsres*>(prop);
			if (prop2dsres)
			{
				if (leafkey.length() > 0)
					node->set_expanded(true);

				std::vector<xui_proproot*> subprop = prop2dsres->get_subprop();
				for (u32 isub = 0, isubindex = 0; isub < subprop.size(); ++isub)
				{
					onity_prop2dsasset* propasset = dynamic_cast<onity_prop2dsasset*>(subprop[isub]);
					NP2DSAsset* asset = propasset->get_asset();
					std::stringstream keytext;
					keytext << asset->GetKey();
					if (leafkey.length() == 0 || leafkey == keytext.str() || asset->GetName().find(leafkey) != -1)
					{
						node->add_leafnode(isubindex, new onity_2dsassetdata(propasset->get_resicon(), propasset));
						++isubindex;
					}
				}
			}

			onity_propjson* propjson = dynamic_cast<onity_propjson*>(prop);
			if (propjson)
			{
				if (leafkey.length() > 0)
					node->set_expanded(true);

				std::vector<xui_proproot*> subprop = propjson->get_templates();
				for (u32 isub = 0, isubindex = 0; isub < subprop.size(); ++isub)
				{
					onity_proptempold* proptemp = dynamic_cast<onity_proptempold*>(subprop[isub]);
					Omiga::EntityTemplate* temp = proptemp->get_template();
					if (leafkey.length() == 0 || temp->GetName().find(leafkey) != -1)
					{
						node->add_leafnode(isubindex, new onity_tempdata(onity_resource::icon_local, proptemp));
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
xui_method_explain(onity_selector, refresh_backpath,		void				)( void )
{
	m_backpath->set_enable(m_search->get_text().length() == 0 && m_fileview->get_tileview()->get_viewfile() != NULL);
}
xui_method_explain(onity_selector, convert_dropsuff,		std::wstring		)( const std::string& droptype )
{
	if		(droptype == "NPSourceTexture")	return L".png";
	else if (droptype == "NP2DSImage")		return L".npModule";
	else if (droptype == "NP2DSFrame")		return L".npSprite";
	else if (droptype == "NP2DSActor")		return L".npAction";
	else if (droptype == "NPParticleSFX")	return L".particle";
	else if (droptype == "NP2DSStateCtrl")	return L".controller";
	else if (droptype == "EntityTemplate")	return L".json";
	else									return L"";
}