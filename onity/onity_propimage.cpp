#include "NP2DSImage.h"

#include "xui_textbox.h"
#include "xui_kindctrl.h"
#include "xui_propkind.h"
#include "xui_propctrl_rect2d.h"
#include "onity_propimage.h"

/*
//constructor
*/
xui_create_explain(onity_propimage)( onity_propfile* propfile, u32 id )
: onity_prop2dsasset(propfile, id)
{
	m_imagekind = new xui_propkind(this, L"", "2DSImage", xui_kindctrl::create, NULL, true, false);
	std::map<s32, std::wstring> textmap;
	textmap[0] = L"None";
	textmap[1] = L"Rows";
	textmap[2] = L"Whole";
	m_imagekind->add_propdata(new xui_propdata_enum_func(
		m_imagekind,
		L"Split Mode",
		xui_propctrl_enum::create,
		textmap,
		get_splitmode,
		set_splitmode,
		this));
	m_imagekind->add_propdata(new xui_propdata_bool(
		m_imagekind,
		L"Alpha Blend",
		xui_propctrl_bool::create,
		was_alphablend,
		set_alphablend,
		this));
	m_imagekind->add_propdata(new xui_propdata_bool(
		m_imagekind,
		L"Scale Style",
		xui_propctrl_bool::create,
		was_scalestyle,
		set_scalestyle,
		this));
	m_imagekind->add_propdata(new xui_propdata_rect2d(
		m_imagekind,
		L"Rectangle",
		xui_propctrl_rect2d::create,
		get_rectangle,
		set_rectangle,
		this,
		NT_UNSIGNEDINT));

	add_propkind(m_imagekind);
}

/*
//method
*/
xui_method_explain(onity_propimage, get_image,		NP2DSImage*		)( void )
{
	return NPDynamicCast(NP2DSImage, get_asset());
}

/*
//static
*/
xui_method_explain(onity_propimage, get_splitmode,	s32				)( void* userptr )
{
	onity_propimage* propimage = (onity_propimage*)userptr;
	NP2DSImage* image = propimage->get_image();
	return (s32)image->GetSplitMode();
}
xui_method_explain(onity_propimage, set_splitmode,	void			)( void* userptr, s32  value )
{
	onity_propimage* propimage = (onity_propimage*)userptr;
	NP2DSImage* image = propimage->get_image();
	image->SetSplitMode((npu32)value);
}
xui_method_explain(onity_propimage, was_alphablend, bool			)( void* userptr )
{
	onity_propimage* propimage = (onity_propimage*)userptr;
	NP2DSImage* image = propimage->get_image();
	return image->WasAlphablend();
}
xui_method_explain(onity_propimage, set_alphablend, void			)( void* userptr, bool value )
{
	onity_propimage* propimage = (onity_propimage*)userptr;
	NP2DSImage* image = propimage->get_image();
	image->SetAlphablend(value);
}
xui_method_explain(onity_propimage, was_scalestyle, bool			)( void* userptr )
{
	onity_propimage* propimage = (onity_propimage*)userptr;
	NP2DSImage* image = propimage->get_image();
	return image->WasScalestyle();
}
xui_method_explain(onity_propimage, set_scalestyle, void			)( void* userptr, bool value )
{
	onity_propimage* propimage = (onity_propimage*)userptr;
	NP2DSImage* image = propimage->get_image();
	image->SetScalestyle(value);
}
xui_method_explain(onity_propimage, get_rectangle,	xui_rect2d<f64>	)( void* userptr )
{
	onity_propimage* propimage = (onity_propimage*)userptr;
	NP2DSImage* image = propimage->get_image();
	f64 x = (f64)image->GetSrcX();
	f64 y = (f64)image->GetSrcY();
	f64 w = (f64)image->GetSrcW();
	f64 h = (f64)image->GetSrcH();
	return xui_rect2d<f64>(x, y, x+w, y+h);
}
xui_method_explain(onity_propimage, set_rectangle,	void			)( void* userptr, const xui_rect2d<f64>& value )
{
	onity_propimage* propimage = (onity_propimage*)userptr;
	NP2DSImage* image = propimage->get_image();
	image->SetSrcX((nps32)value.ax);
	image->SetSrcY((nps32)value.ay);
	image->SetSrcW((nps32)value.get_w());
	image->SetSrcH((nps32)value.get_h());
}