#include "NPFileName.h"
#include "NP2DSSceneFile.h"
#include "NPFile.h"

#include "xui_global.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_button.h"
#include "xui_toolbar.h"
#include "xui_desktop.h"
#include "onity_resource.h"
#include "onity_propcourse.h"
#include "onity_courseconfigdata.h"
#include "onity_propcourseconfig.h"
#include "onity_mainform.h"
#include "onity_inspector.h"
#include "onity_project.h"
#include "onity_course.h"

xui_implement_rtti(onity_course, xui_window);

/*
//constructor
*/
xui_create_explain(onity_course)( void )
: xui_window(xui_vector<s32>(480, 640), false)
{
	m_coursetext	= new xui_drawer(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_coursetext,	xm_mousemove		) += new xui_method_member<xui_method_mouse,	onity_course>(this, &onity_course::on_coursetextmousemove);
	xui_method_ptrcall(m_coursetext,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_coursetext,	set_text			)(L"Course Build");

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_BOOL,  20, L"flag", NULL, 0, false, TOGGLE_NORMAL));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0, false));
	m_courseview	= new xui_treeview(xui_vector<s32>(200), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_courseview,	xm_keybddown		) += new xui_method_member<xui_method_keybd,	onity_course>(this, &onity_course::on_courseviewkeybddown);
	xui_method_ptrcall(m_courseview,	xm_mousedown		) += new xui_method_member<xui_method_mouse,	onity_course>(this, &onity_course::on_courseviewmousedown);
	xui_method_ptrcall(m_courseview,	xm_mousedoubleclick	) += new xui_method_member<xui_method_mouse,	onity_course>(this, &onity_course::on_courseviewmousedclick);
	xui_method_ptrcall(m_courseview,	xm_mousedragover	) += new xui_method_member<xui_method_dragdrop, onity_course>(this, &onity_course::on_courseviewdragover);
	xui_method_ptrcall(m_courseview,	xm_mousedragdrop	) += new xui_method_member<xui_method_dragdrop, onity_course>(this, &onity_course::on_courseviewdragdrop);
	xui_method_ptrcall(m_courseview,	ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_courseview,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_courseview,	set_drawcolor		)(true);
	xui_method_ptrcall(m_courseview,	set_borderrt		)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_courseview,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_courseview,	set_allowmulti		)(true);

	m_run		= new xui_button(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_run,			xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_course>(this, &onity_course::on_accept);
	xui_method_ptrcall(m_run,			ini_component		)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_run,			set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_run,			set_corner			)(3);
	xui_method_ptrcall(m_run,			set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_run,			set_drawcolor		)(true);
	xui_method_ptrcall(m_run,			set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_run,			set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_run,			ini_drawer			)(L"Run");

	m_close		= new xui_button(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_close,			xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_course>(this, &onity_course::on_cancel);
	xui_method_ptrcall(m_close,			ini_component		)(ALIGNHORZ_R, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_close,			set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_close,			set_corner			)(3);
	xui_method_ptrcall(m_close,			set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_close,			set_drawcolor		)(true);
	xui_method_ptrcall(m_close,			set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_close,			set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_close,			ini_drawer			)(L"Close");

	m_bottom = new xui_panel(xui_vector<s32>(128, 32));
	xui_method_ptrcall(m_bottom,		ini_component		)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_bottom,		set_borderrt		)(xui_rect2d<s32>(0, 8, 0, 0));
	xui_method_ptrcall(m_bottom,		add_child			)(m_run);
	xui_method_ptrcall(m_bottom,		add_child			)(m_close);

	add_child(m_bottom);
	add_child(m_coursetext);
	add_child(m_courseview);
}

/*
//method
*/
xui_method_explain(onity_course, set_firstcourse,			void)( onity_propcourse* editprop )
{
	if (m_courseview->get_upmostnodecount() == 0)
	{
		onity_proproot* newprop = new onity_propcourseconfig(editprop->get_scenefile());
		onity_treedata* newdata = new onity_courseconfigdata(onity_resource::icon_scene, newprop);
		m_courseview->add_upmostnode(0, newdata);
	}

	xui_treenode*			node = m_courseview->get_upmostnode(0);
	onity_treedata*			data = (onity_treedata*)node->get_linkdata();
	onity_propcourseconfig* prop = dynamic_cast<onity_propcourseconfig*>(data->get_prop());
	prop->set_course(editprop->get_scenefile());
	node->use_linkdata();

	save_courseconfig();
}
xui_method_explain(onity_course, save_courseconfig,			void)( void )
{
	NPFile file;
	if (file.Open("onity.levelcfg", NPFile::OM_WRITE))
	{
		std::vector<xui_treenode*> nodevec;
		for (u32 i = 0; i < m_courseview->get_upmostnodecount(); ++i)
		{
			xui_treenode*			node = m_courseview->get_upmostnode(i);
			onity_courseconfigdata* data = (onity_courseconfigdata*)node->get_linkdata();
			if (data->get_flag(0))
			{
				nodevec.push_back(node);
			}
		}

		for (u32 i = 0; i < nodevec.size(); ++i)
		{
			xui_treenode*			node = nodevec[i];
			onity_treedata*			data = (onity_treedata*)node->get_linkdata();
			onity_propcourseconfig* prop = dynamic_cast<onity_propcourseconfig*>(data->get_prop());
			std::stringstream		line;
			line << i;
			line << ";";
			line << prop->get_configstring().c_str();
			file.WriteLine(line.str());
		}

		file.Close();
	}
}

/*
//callback
*/
xui_method_explain(onity_course, on_accept,					void)( xui_component* sender, xui_method_args&		args )
{
	set_visible(false);
	save_courseconfig();
	xui_window::on_accept(sender, args);
}
xui_method_explain(onity_course, on_cancel,					void)( xui_component* sender, xui_method_args&		args )
{
	set_visible(false);
	xui_window::on_cancel(sender, args);
}

/*
//event
*/
xui_method_explain(onity_course, on_coursetextmousemove,	void)( xui_component* sender, xui_method_mouse&		args )
{
	if (m_coursetext->has_catch())
	{
		xui_vector<s32> pt = get_renderpt() + xui_desktop::get_ins()->get_mousemove();
		pt.x = xui_max(pt.x, 0);
		pt.y = xui_max(pt.y, 0);
		pt.x = xui_min(pt.x, xui_desktop::get_ins()->get_renderw()-get_renderw());
		pt.y = xui_min(pt.y, xui_desktop::get_ins()->get_renderh()-get_renderh());
		set_renderpt(pt);
	}
}
xui_method_explain(onity_course, on_courseviewkeybddown,	void)( xui_component* sender, xui_method_keybd&		args )
{
	if (args.kcode == KEY_DELETE)
	{
		for (s32 i = m_courseview->get_upmostnodecount()-1; i >=1; --i)
		{
			xui_treenode* node = m_courseview->get_upmostnode((u32)i);
			if (node->was_selected())
				m_courseview->del_upmostnode(node);
		}
	}
}
xui_method_explain(onity_course, on_courseviewmousedown,	void)( xui_component* sender, xui_method_mouse&		args )
{
	xui_proproot_vec propvec;
	std::vector<xui_treenode*> nodevec = m_courseview->get_selectednode();
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		xui_treenode*	node = nodevec[i];
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		propvec.push_back(data->get_prop());
	}

	if (propvec.size() > 0)
	{
		onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
		inspector->set_proproot(propvec);
	}
}
xui_method_explain(onity_course, on_courseviewmousedclick,	void)( xui_component* sender, xui_method_mouse&		args )
{
	xui_treenode* node = m_courseview->choose_node(m_courseview->get_renderpt(args.point));
	if (node)
	{
		onity_treedata*			data = (onity_treedata*)node->get_linkdata();
		onity_propcourseconfig* prop = dynamic_cast<onity_propcourseconfig*>(data->get_prop());

		std::string full = prop->get_course()->GetName();
		std::string path = NPFileNameHelper::PathName(full);
		std::string file = NPFileNameHelper::FileName(full);
		path = path.substr(0, path.length()-1);
		file = NPFileNameHelper::SafeName(file);

		onity_project* project = onity_mainform::get_ptr()->get_project();
		project->loc_filenode(xui_global::ascii_to_unicode(path), xui_global::ascii_to_unicode(file), 0);
		onity_mainform::get_ptr()->set_pageshow(project);
	}
}
xui_method_explain(onity_course, on_courseviewdragover,		void)( xui_component* sender, xui_method_dragdrop&	args )
{
	if (args.type == "NP2DSSceneFile")
		args.allow = true;
}
xui_method_explain(onity_course, on_courseviewdragdrop,		void)( xui_component* sender, xui_method_dragdrop&	args )
{
	NP2DSSceneFile* course  = (NP2DSSceneFile*)args.data;
	onity_proproot* newprop = new onity_propcourseconfig(course);
	onity_treedata* newdata = new onity_courseconfigdata(onity_resource::icon_scene, newprop);
	m_courseview->add_upmostnode(m_courseview->get_upmostnodecount(), newdata);
}