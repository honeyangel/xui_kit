#include "NP2DSSceneLayer.h"

#include "xui_global.h"
#include "xui_textbox.h"
#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_kindctrl.h"
#include "xui_propctrl_vector.h"
#include "onity_propscenelayer.h"

/*
//constructor
*/
xui_create_explain(onity_propscenelayer)( NP2DSSceneLayer* scenelayer )
: onity_propentitytemp(scenelayer->GetName())
, m_scenelayer(scenelayer)
, m_snaplength(0)
{
	m_legendkind = new xui_propkind(this, L"Legendary Layer", "SceneLayer", xui_kindctrl::create, NULL, true, true, true);
	m_legendkind->add_propdata(new xui_propdata_vector(
		m_legendkind,
		L"CellSize",
		xui_propctrl_vector::create,
		get_cellsize,
		set_cellsize,
		this,
		NT_UNSIGNEDINT));
	m_legendkind->add_propdata(new xui_propdata_number_func(
		m_legendkind,
		L"Rows",
		xui_propctrl_number::create,
		get_rows,
		set_rows,
		this,
		NT_UNSIGNEDINT));
	m_legendkind->add_propdata(new xui_propdata_number_func(
		m_legendkind,
		L"Cols",
		xui_propctrl_number::create,
		get_cols,
		set_cols,
		this,
		NT_UNSIGNEDINT));

	m_designkind = new xui_propkind(this, L"Design", "SceneLayerDesign", xui_kindctrl::create, NULL, true, true, true);
	m_designkind->add_propdata(new xui_propdata_number_func(
		m_legendkind,
		L"Snap",
		xui_propctrl_number::create,
		get_snaplength,
		set_snaplength,
		this,
		NT_UNSIGNEDINT));

	add_propkind(m_legendkind);
	add_propkind(m_designkind);
}

/*
//method
*/
xui_method_explain(onity_propscenelayer, get_scenelayer,	NP2DSSceneLayer*)( void )
{
	return m_scenelayer;
}
xui_method_explain(onity_propscenelayer, rna_template,		void			)( const std::wstring& text )
{
	onity_propentitytemp::rna_template(text);
	m_scenelayer->SetName(xui_global::unicode_to_ascii(text));
}
xui_method_explain(onity_propscenelayer, get_snaplength,	s32				)( void ) const
{
	return m_snaplength;
}
xui_method_explain(onity_propscenelayer, set_snaplength,	void			)( s32 length )
{
	m_snaplength = length;
}

/*
//static
*/
xui_method_explain(onity_propscenelayer, get_cellsize,		xui_vector<f64>	)( void* userptr )
{
	onity_propscenelayer* prop = (onity_propscenelayer*)userptr;
	f64 w = (f64)prop->get_scenelayer()->GetCellW();
	f64 h = (f64)prop->get_scenelayer()->GetCellH();
	return xui_vector<f64>(w, h);
}
xui_method_explain(onity_propscenelayer, set_cellsize,		void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_propscenelayer* prop = (onity_propscenelayer*)userptr;
	prop->get_scenelayer()->SetCellW((npu16)value.w);
	prop->get_scenelayer()->SetCellH((npu16)value.h);
}
xui_method_explain(onity_propscenelayer, get_rows,			f64				)( void* userptr )
{
	onity_propscenelayer* prop = (onity_propscenelayer*)userptr;
	return (f64)prop->get_scenelayer()->GetCellR();
}
xui_method_explain(onity_propscenelayer, set_rows,			void			)( void* userptr, f64 value )
{
	onity_propscenelayer* prop = (onity_propscenelayer*)userptr;
	prop->get_scenelayer()->SetCellR((npu16)value);
}
xui_method_explain(onity_propscenelayer, get_cols,			f64				)( void* userptr )
{
	onity_propscenelayer* prop = (onity_propscenelayer*)userptr;
	return (f64)prop->get_scenelayer()->GetCellC();
}
xui_method_explain(onity_propscenelayer, set_cols,			void			)( void* userptr, f64 value )
{
	onity_propscenelayer* prop = (onity_propscenelayer*)userptr;
	prop->get_scenelayer()->SetCellC((npu16)value);
}
xui_method_explain(onity_propscenelayer, get_snaplength,	f64				)( void* userptr )
{
	onity_propscenelayer* prop = (onity_propscenelayer*)userptr;
	return (f64)prop->get_snaplength();
}
xui_method_explain(onity_propscenelayer, set_snaplength,	void			)( void* userptr, f64 value )
{
	onity_propscenelayer* prop = (onity_propscenelayer*)userptr;
	prop->set_snaplength((s32)value);
}