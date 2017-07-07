#include "NP2DSTransRef.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_treedata.h"

#include "onity_mainform.h"
#include "onity_hierarchy.h"
#include "onity_propcoursenode.h"
#include "onity_boundbox_coursenode.h"

/*
//constructor
*/
xui_create_explain(onity_boundbox_coursenode)( xui_proproot* prop )
: onity_boundbox(prop, ST_NONE)
{}

/*
//override
*/
xui_method_explain(onity_boundbox_coursenode, ori_position, xui_vector<s32>	)( void )
{
	onity_propcoursenode* prop = dynamic_cast<onity_propcoursenode*>(m_linkprop);
	NPVector3 pos = prop->get_2dsref()->GetWorldTrans();
	return xui_vector<s32>((s32)pos.x, (s32)pos.y);
}
xui_method_explain(onity_boundbox_coursenode, set_position, void			)( const xui_vector<s32>& pos )
{
	onity_propcoursenode* prop = dynamic_cast<onity_propcoursenode*>(m_linkprop);
	prop->get_2dsref()->SetWorldTrans(NPVector3((f32)pos.x, (f32)pos.y, 0.0f));
}
xui_method_explain(onity_boundbox_coursenode, ori_bounding, xui_rect2d<s32>	)( void )
{
	onity_propcoursenode* prop = dynamic_cast<onity_propcoursenode*>(m_linkprop);
	NPRect rt = prop->get_2dsref()->GetWorldBounding();
	return xui_rect2d<s32>(rt.LT, rt.TP, rt.RT, rt.BM);
}
xui_method_explain(onity_boundbox_coursenode, was_selected,	bool			)( void )
{
	onity_propcoursenode* prop = dynamic_cast<onity_propcoursenode*>(m_linkprop);
	xui_treeview* treeview = onity_mainform::get_ptr()->get_hierarchy()->get_treeview();
	xui_treedata* treedata = prop->get_linkdata(treeview);
	return treedata->get_node()->was_selected();
}