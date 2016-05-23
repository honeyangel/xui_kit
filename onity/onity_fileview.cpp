#include "xui_treeview.h"
#include "xui_desktop.h"
#include "xui_scroll.h"
#include "onity_mainform.h"
#include "onity_inspector.h"
#include "onity_renderview.h"
#include "onity_filedata.h"
#include "onity_tileview.h"
#include "onity_fileview.h"

xui_implement_rtti(onity_fileview, xui_control);

/*
//constructor
*/
xui_create_explain(onity_fileview)( void )
: xui_control(xui_vector<s32>(0))
{
	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0, true));
	m_lineview = new xui_treeview(xui_vector<s32>(200), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_lineview,	xm_show				) += new xui_method_member<xui_method_args,		onity_fileview>(this, &onity_fileview::on_lineviewshow);
	xui_method_ptrcall(m_lineview,	xm_selectedchange	) += new xui_method_member<xui_method_args,		onity_fileview>(this, &onity_fileview::on_lineviewselection);
	xui_method_ptrcall(m_lineview,	xm_mouseclick		) += new xui_method_member<xui_method_mouse,	onity_fileview>(this, &onity_fileview::on_fileviewnodeclick);
	xui_method_ptrcall(m_lineview,	xm_mousedoubleclick	) += new xui_method_member<xui_method_mouse,	onity_fileview>(this, &onity_fileview::on_fileviewdoubleclk);
	xui_method_ptrcall(m_lineview,	xm_mousedragitem	) += new xui_method_member<xui_method_dragdrop, onity_fileview>(this, &onity_fileview::on_fileviewassetdrag);
	xui_method_ptrcall(m_lineview,	set_parent			)(this);
	xui_method_ptrcall(m_lineview,	ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_lineview,	ini_component		)(true, true);
	xui_method_ptrcall(m_lineview,	set_acceptdrag		)(false);
	xui_method_ptrcall(m_lineview,	set_allowmulti		)(false);
	xui_method_ptrcall(m_lineview,	set_hscrollauto		)(false);

	m_tileview = new onity_tileview;
	xui_method_ptrcall(m_tileview,	xm_show				) += new xui_method_member<xui_method_args,		onity_fileview>(this, &onity_fileview::on_tileviewshow);
	xui_method_ptrcall(m_tileview,	xm_mouseclick		) += new xui_method_member<xui_method_mouse,	onity_fileview>(this, &onity_fileview::on_fileviewnodeclick);
	xui_method_ptrcall(m_tileview,	xm_mousedoubleclick	) += new xui_method_member<xui_method_mouse,	onity_fileview>(this, &onity_fileview::on_fileviewdoubleclk);
	xui_method_ptrcall(m_tileview,	xm_mousedragitem	) += new xui_method_member<xui_method_dragdrop, onity_fileview>(this, &onity_fileview::on_fileviewassetdrag);
	xui_method_ptrcall(m_tileview,	set_parent			)(this);
	xui_method_ptrcall(m_tileview,	ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_tileview,	ini_component		)(true, false);

	m_widgetvec.push_back(m_tileview);
	m_widgetvec.push_back(m_lineview);
}

/*
//method
*/
xui_method_explain(onity_fileview, get_lineview,			xui_treeview*	)( void )
{
	return m_lineview;
}
xui_method_explain(onity_fileview, get_tileview,			onity_tileview*	)( void )
{
	return m_tileview;
}
xui_method_explain(onity_fileview, clear,					void			)( void )
{
	xui_scroll*   lineroll = m_lineview->get_vscroll ();
	xui_scroll*   tileroll = m_tileview->get_viewroll();
	if (lineroll) lineroll->set_value(0);
	if (tileroll) tileroll->set_value(0);

	xui_method_ptrcall(m_lineview,	del_upmostnodeall	)();
	xui_method_ptrcall(m_tileview,	set_viewfile		)(NULL);
}

/*
//event
*/
xui_method_explain(onity_fileview, on_lineviewshow,			void			)( xui_component* sender, xui_method_args&     args )
{
	m_tileview->set_viewfile(NULL);

	std::vector<xui_treenode*> nodes = m_lineview->get_selectednode();
	if (nodes.size() > 0)
	{
		m_lineview->set_nodevisible(nodes.front());
	}
}
xui_method_explain(onity_fileview, on_tileviewshow,			void			)( xui_component* sender, xui_method_args&     args )
{
	std::vector<xui_treenode*> nodes = m_lineview->get_selectednode();
	if (nodes.size() > 0)
	{
		m_tileview->set_tilevisible(nodes.front());
	}
}
xui_method_explain(onity_fileview, on_lineviewselection,	void			)( xui_component* sender, xui_method_args&     args )
{
	xm_lineviewselection(this, args);
}
xui_method_explain(onity_fileview, on_fileviewnodeclick,	void			)( xui_component* sender, xui_method_mouse&    args )
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
xui_method_explain(onity_fileview, on_fileviewdoubleclk,	void			)( xui_component* sender, xui_method_mouse&	   args )
{
	xm_fileviewdoubleclk(this, args);
}
xui_method_explain(onity_fileview, on_fileviewassetdrag,	void			)( xui_component* sender, xui_method_dragdrop& args )
{
	xm_fileviewassetdrag(this, args);
}