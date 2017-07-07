#include "NP2DSImage.h"
#include "xui_treeview.h"
#include "xui_treedata.h"
#include "xui_treenode.h"
#include "onity_mainform.h"
#include "onity_fileview.h"
#include "onity_project.h"
#include "onity_propimage.h"
#include "onity_boundbox_image.h"

/*
//constructor
*/
xui_create_explain(onity_boundbox_image)( xui_proproot* prop )
: onity_boundbox(prop, ST_ENTIRE)
{}

/*
//override
*/
xui_method_explain(onity_boundbox_image, ori_position,	xui_vector<s32>	)( void )
{
	onity_propimage* prop = dynamic_cast<onity_propimage*>(m_linkprop);
	NP2DSImage* image = prop->get_image();
	if (image)
	{
		return xui_vector<s32>(image->GetSrcX(), image->GetSrcY());
	}

	return xui_vector<s32>();

}
xui_method_explain(onity_boundbox_image, set_position,	void			)( const xui_vector<s32>& pos )
{
	onity_propimage* prop = dynamic_cast<onity_propimage*>(m_linkprop);
	NP2DSImage* image = prop->get_image();
	if (image)
	{
		image->SetSrcX(pos.x);
		image->SetSrcY(pos.y);
	}
}
xui_method_explain(onity_boundbox_image, ori_bounding,	xui_rect2d<s32>	)( void )
{
	onity_propimage* prop = dynamic_cast<onity_propimage*>(m_linkprop);
	NP2DSImage* image = prop->get_image();
	if (image)
	{
		s32 x = image->GetSrcX();
		s32 y = image->GetSrcY();
		s32 w = image->GetSrcW();
		s32 h = image->GetSrcH();
		return xui_rect2d<s32>(x, y, x+w, y+h);
	}

	return xui_rect2d<s32>();
}
xui_method_explain(onity_boundbox_image, set_bounding,	void			)( const xui_rect2d<s32>& rt, u08 op )
{
	onity_boundbox::set_bounding(rt, op);
	onity_propimage* prop = dynamic_cast<onity_propimage*>(m_linkprop);
	NP2DSImage* image = prop->get_image();
	if (image && op > BO_MOVE)
	{
		image->SetSrcX(rt.ax);
		image->SetSrcY(rt.ay);
		image->SetSrcW(rt.get_w());
		image->SetSrcH(rt.get_h());
	}
}
xui_method_explain(onity_boundbox_image, was_selected,	bool			)( void )
{
	onity_propimage* prop	 = dynamic_cast<onity_propimage*>(m_linkprop);
	onity_fileview*	 fileview = onity_mainform::get_ptr()->get_project()->get_fileview();
	xui_treeview*	 lineview = fileview->get_lineview();
	xui_treedata*	 treedata = prop->get_linkdata(lineview);
	if (treedata)
	{
		return treedata->get_node()->was_selected();
	}

	return false;
}

/*
//draw
*/
xui_method_explain(onity_boundbox_image, draw_operator,	void			)( const xui_rect2d<s32>& rt, const xui_vector<s32>& pivot )
{
	xui_convas::get_ins()->fill_rectangle(rt, xui_colour(0.3f, 1.0f, 0.0f, 0.0f));
	onity_boundbox::draw_operator(rt, pivot);
}