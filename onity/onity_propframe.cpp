#include "NP2DSFrame.h"

#include "xui_textbox.h"
#include "xui_kindctrl.h"
#include "xui_propkind.h"
#include "xui_propctrl_rect2d.h"
#include "xui_propctrl_expand.h"
#include "onity_propframe.h"

/*
//constructor
*/
xui_create_explain(onity_propframe)( onity_propfile* propfile, u32 id )
: onity_prop2dsasset(propfile, id)
{
	m_framekind = new xui_propkind(this, L"", "2DSFrame", xui_kindctrl::create, NULL, true, false);
	std::vector<xui_propdata*>       subprop;
	std::map<s32, std::vector<u32> > editmap;
	editmap[1].push_back(0);
	subprop.push_back(new xui_propdata_rect2d(
		m_framekind,
		L"Bounding Box",
		xui_propctrl_rect2d::create,
		get_boundingbox,
		set_boundingbox,
		this,
		NT_UNSIGNEDINT));

	m_framekind->add_propdata(new xui_propdata_expand_bool(
		m_framekind,
		L"Bounding Custom",
		xui_propctrl_expand_bool::create,
		subprop,
		was_boundcustom,
		set_boundcustom,
		this, 
		true,
		xui_expandvary::empty_map,
		editmap));

	add_propkind(m_framekind);
}

/*
//static
*/
xui_method_explain(onity_propframe, was_boundcustom, bool			)( void* userptr )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	NP2DSFrame* frame = (NP2DSFrame*)propframe->get_asset();
	return frame->WasBoundCustom();
}
xui_method_explain(onity_propframe, set_boundcustom, void			)( void* userptr, bool value )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	NP2DSFrame* frame = (NP2DSFrame*)propframe->get_asset();
	frame->SetBoundCustom(value);
}
xui_method_explain(onity_propframe, get_boundingbox, xui_rect2d<f64>)( void* userptr )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	NP2DSFrame* frame = (NP2DSFrame*)propframe->get_asset();
	NPRect rect = frame->GetBounding();
	return xui_rect2d<f64>((f64)rect.LT, (f64)rect.TP, (f64)rect.RT, (f64)rect.BM);
}
xui_method_explain(onity_propframe, set_boundingbox, void			)( void* userptr, const xui_rect2d<f64>& value )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	NP2DSFrame* frame = (NP2DSFrame*)propframe->get_asset();
	NPRect rect;
	rect.LT = (nps32)value.ax;
	rect.TP = (nps32)value.ay;
	rect.RT = (nps32)value.bx;
	rect.BM = (nps32)value.by;
	frame->SetBounding(rect);
}