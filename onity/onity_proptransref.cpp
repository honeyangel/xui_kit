#include "NPPropertyA.h"
#include "NP2DSTransRef.h"

#include "xui_textbox.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "xui_propdata.h"
#include "xui_propctrl_expand.h"
#include "xui_propctrl_vector.h"
#include "xui_propctrl_slider.h"
#include "onity_resource.h"
#include "onity_propctrl_pivot.h"
#include "onity_propctrl_flip.h"
#include "onity_propfile.h"
#include "onity_proptransref.h"

/*
//constructor
*/
xui_create_explain(onity_proptransref)( onity_propfile* propfile, NP2DSTransRef* transref )
: onity_propleaf(propfile)
, m_transref(transref)
{
	add_localkind();
	add_worldkind();
	add_graphkind();
}

/*
//method
*/
xui_method_explain(onity_proptransref, get_transref,		NP2DSTransRef*	)( void )
{
	return m_transref;
}

xui_method_explain(onity_proptransref, add_localkind,		void			)( void )
{
	m_localkind = new xui_propkind(this, L"LocalTransform", "LocalTransform", xui_kindctrl::create, onity_resource::icon_local, true);
	m_localkind->add_propdata(new xui_propdata_number_func(
		m_localkind,
		L"Flip",
		onity_propctrl_flip::create,
		get_localflip,
		set_localflip,
		this,
		NT_UNSIGNEDINT));
	m_localkind->add_propdata(new onity_propdata_pivot(
		m_localkind,
		L"Pivot",
		onity_propctrl_pivot::create,
		get_localpivot,
		set_localpivot,
		get_pivotboundbox,
		this,
		NT_INT));

	m_localkind->add_propdata(new xui_propdata_number_func(
		m_localkind,
		L"Rotation",
		xui_propctrl_number::create,
		get_localangle,
		set_localangle,
		this,
		NT_INT));

	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_vector(
		m_localkind,
		L"",
		xui_propctrl_vector::create,
		get_localscale,
		set_localscale,
		this,
		NT_FLOAT,
		0.01));
	m_localkind->add_propdata(new xui_propdata_expand_number(
		m_localkind,
		L"Scaling",
		xui_propctrl_expand_number::create,
		subprop,
		get_localscaleunit,
		set_localscaleunit,
		this,
		NT_FLOAT,
		0.01));

	add_propkind(m_localkind);
}
xui_method_explain(onity_proptransref, add_worldkind,		void			)( void )
{
	m_worldkind = new xui_propkind(this, L"WorldTransform", "WorldTransform", xui_kindctrl::create, onity_resource::icon_world, true);
	m_worldkind->add_propdata(new xui_propdata_number_func(
		m_worldkind,
		L"Flip",
		onity_propctrl_flip::create,
		get_worldflip,
		set_worldflip,
		this,
		NT_UNSIGNEDINT));
	m_worldkind->add_propdata(new xui_propdata_vector(
		m_worldkind,
		L"Translate",
		xui_propctrl_vector::create,
		get_worldtrans,
		set_worldtrans,
		this,
		NT_INT));
	m_worldkind->add_propdata(new xui_propdata_number_func(
		m_worldkind,
		L"Rotation",
		xui_propctrl_number::create,
		get_worldangle,
		set_worldangle,
		this,
		NT_INT));

	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_vector(
		m_worldkind,
		L"",
		xui_propctrl_vector::create,
		get_worldscale,
		set_worldscale,
		this,
		NT_FLOAT,
		0.01));
	m_worldkind->add_propdata(new xui_propdata_expand_number(
		m_worldkind,
		L"Scaling",
		xui_propctrl_expand_number::create,
		subprop,
		get_worldscaleunit,
		set_worldscaleunit,
		this,
		NT_FLOAT,
		0.01));

	add_propkind(m_worldkind);
}
xui_method_explain(onity_proptransref, add_graphkind,		void			)( void )
{
	xui_propdata_vec				subprop;
	xui_propenum_map				textmap;
	std::map<s32, std::vector<u32>> showmap;

	m_graphkind = new xui_propkind(this, L"Graph", "Graph", xui_kindctrl::create, onity_resource::icon_texture, true);

	//filter
	textmap[0] = L"Point";
	textmap[1] = L"Linear";
	m_graphkind->add_propdata(new xui_propdata_enum_func(
		m_graphkind,
		L"Filter",
		xui_propctrl_enum::create,
		textmap,
		get_filter,
		set_filter,
		this));

	//texenv
	subprop.push_back(new xui_propdata_colour(
		m_graphkind,
		L"Vertex Colour",
		xui_propctrl_colour::create,
		get_vertexcolor,
		set_vertexcolor,
		this));
	textmap.clear();
	textmap[0] = L"Only Texture";
	textmap[1] = L"Modulate";
	textmap[2] = L"Add";
	textmap[3] = L"Texture Alpha With Color";
	textmap[4] = L"Only Color";
	showmap[1].push_back(0);
	showmap[2].push_back(0);
	showmap[3].push_back(0);
	showmap[4].push_back(0);
	m_graphkind->add_propdata(new xui_propdata_expand_enum_func(
		m_graphkind,
		L"Shader",
		xui_propctrl_expand_enum::create,
		textmap,
		get_texenv,
		set_texenv,
		this,
		subprop,
		true,
		showmap));

	//alpha blend
	subprop.clear();
	textmap.clear();
	textmap[0 ] = L"Default";
	textmap[1 ] = L"One";
	textmap[2 ] = L"Zero";
	textmap[3 ] = L"SrcAlpha";
	textmap[4 ] = L"SrcColor";
	textmap[5 ] = L"DstColor";
	textmap[6 ] = L"DstAlpha";
	textmap[7 ] = L"SrcAlphaSat";
	textmap[8 ] = L"InvSrcColor";
	textmap[9 ] = L"InvDstColor";
	textmap[10] = L"InvSrcAlpha";
	textmap[11] = L"InvDstAlpha";
	subprop.push_back(new xui_propdata_enum_func(
		m_graphkind,
		L"Src Mode",
		xui_propctrl_enum::create,
		textmap,
		get_blendsrc,
		set_blendsrc,
		this));
	subprop.push_back(new xui_propdata_enum_func(
		m_graphkind,
		L"Dst Mode",
		xui_propctrl_enum::create,
		textmap,
		get_blenddst,
		set_blenddst,
		this));
	textmap.clear();
	textmap[0] = L"User";
	textmap[1] = L"Dark";
	textmap[2] = L"Add";
	textmap[3] = L"Default";
	m_graphkind->add_propdata(new xui_propdata_expand_enum_func(
		m_graphkind,
		L"Alpha Blend",
		xui_propctrl_expand_enum::create,
		textmap,
		get_blendmode,
		set_blendmode,
		this,
		subprop,
		true));

	//outline
	subprop.clear();
	showmap.clear();
	showmap[1].push_back(0);
	showmap[1].push_back(1);
	subprop.push_back(new xui_propdata_number_func(
		m_graphkind,
		L"Outline Width",
		xui_propctrl_number::create,
		get_outlinewidth,
		set_outlinewidth,
		this,
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_colour(
		m_graphkind,
		L"Outline Colour",
		xui_propctrl_colour::create,
		get_outlinecolor,
		set_outlinecolor,
		this));
	m_graphkind->add_propdata(new xui_propdata_expand_bool(
		m_graphkind,
		L"Outline",
		xui_propctrl_expand_bool::create,
		subprop,
		was_outline,
		set_outline,
		this,
		true,
		showmap));

	add_propkind(m_graphkind);
}

/*
//static
*/
xui_method_explain(onity_proptransref, get_localflip,		f64				)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	return (f64)prop->get_transref()->GetLocalFlips();
}
xui_method_explain(onity_proptransref, set_localflip,		void			)( void* userptr, f64 value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetLocalFlips((npu08)value);
}
xui_method_explain(onity_proptransref, get_localangle,		f64				)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	return (f64)prop->get_transref()->GetLocalAngle();
}
xui_method_explain(onity_proptransref, set_localangle,		void			)( void* userptr, f64 value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetLocalAngle((npf32)value);
}
xui_method_explain(onity_proptransref, get_pivotboundbox,	xui_rect2d<f64>	)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	NPRect rect = prop->get_transref()->GetOrignBounding();
	return xui_rect2d<f64>((f64)rect.LT, (f64)rect.TP, (f64)rect.RT, (f64)rect.BM);
}
xui_method_explain(onity_proptransref, get_localscaleunit,	f64				)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	return (f64)prop->get_transref()->GetLocalScale().x;
}
xui_method_explain(onity_proptransref, set_localscaleunit,	void			)( void* userptr, f64 value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetLocalScale(NPVector3((npf32)value, (npf32)value, 1.0f));
}
xui_method_explain(onity_proptransref, get_localscale,		xui_vector<f64>	)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	NPVector3 scale = prop->get_transref()->GetLocalScale();
	return xui_vector<f64>((f64)scale.x, (f64)scale.y);
}
xui_method_explain(onity_proptransref, set_localscale,		void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetLocalScale(NPVector3((f32)value.x, (f32)value.y, 1.0f));
}
xui_method_explain(onity_proptransref, get_localpivot,		xui_vector<f64>	)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	NPVector3 pivot = prop->get_transref()->GetLocalAnglePivot();
	return xui_vector<f64>((f64)pivot.x, (f64)pivot.y);
}
xui_method_explain(onity_proptransref, set_localpivot,		void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetLocalAnglePivot(NPVector3((f32)value.x, (f32)value.y, 0.0f));
}

xui_method_explain(onity_proptransref, get_worldflip,		f64				)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	return (f64)prop->get_transref()->GetWorldFlips();
}
xui_method_explain(onity_proptransref, set_worldflip,		void			)( void* userptr, f64 value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetWorldFlips((npu08)value);
}
xui_method_explain(onity_proptransref, get_worldangle,		f64				)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	return (f64)prop->get_transref()->GetWorldAngle();
}
xui_method_explain(onity_proptransref, set_worldangle,		void			)( void* userptr, f64 value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetWorldAngle((f32)value);
}
xui_method_explain(onity_proptransref, get_worldscaleunit,	f64				)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	return prop->get_transref()->GetWorldScale().x;
}
xui_method_explain(onity_proptransref, set_worldscaleunit,	void			)( void* userptr, f64 value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetWorldScale(NPVector3((npf32)value, (npf32)value, 1.0f));
}
xui_method_explain(onity_proptransref, get_worldscale,		xui_vector<f64>	)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	NPVector3 scale = prop->get_transref()->GetWorldScale();
	return xui_vector<f64>((f64)scale.x, (f64)scale.y);
}
xui_method_explain(onity_proptransref, set_worldscale,		void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetWorldScale(NPVector3((f32)value.x, (f32)value.y, 1.0f));
}
xui_method_explain(onity_proptransref, get_worldtrans,		xui_vector<f64>	)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	NPVector3 trans = prop->get_transref()->GetWorldTrans();
	return xui_vector<f64>((f64)trans.x, (f64)trans.y);
}
xui_method_explain(onity_proptransref, set_worldtrans,		void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetWorldTrans(NPVector3((f32)value.x, (f32)value.y, 0.0f));
}

xui_method_explain(onity_proptransref, get_filter,			s32				)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	return (s32)prop->get_transref()->GetFilter();
}
xui_method_explain(onity_proptransref, set_filter,			void			)( void* userptr, s32 value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetFilter((npu32)value);
}
xui_method_explain(onity_proptransref, get_texenv,			s32				)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	return (s32)prop->get_transref()->GetTexEnv();
}
xui_method_explain(onity_proptransref, set_texenv,			void			)( void* userptr, s32 value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetTexEnv((npu32)value);
}
xui_method_explain(onity_proptransref, get_vertexcolor,		xui_colour		)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	NPColor color = prop->get_transref()->GetBlendColor();
	return xui_colour(color.a, color.r, color.g, color.b);
}
xui_method_explain(onity_proptransref, set_vertexcolor,		void			)( void* userptr, const xui_colour&		 value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetBlendColor(NPColor(value.a, value.r, value.g, value.b));
}
xui_method_explain(onity_proptransref, get_blendmode,		s32				)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;

	npu32 src = prop->get_transref()->GetSrcABM();
	npu32 dst = prop->get_transref()->GetDstABM();

	if      (src == NPPropertyA::BM_DEFAULT  && dst == NPPropertyA::BM_DEFAULT)     return BM_DEFAULT;
	else if (src == NPPropertyA::BM_SRCALPHA && dst == NPPropertyA::BM_INVSRCALPHA) return BM_DEFAULT;
	else if (src == NPPropertyA::BM_SRCALPHA && dst == NPPropertyA::BM_ONE)         return BM_ADD;
	else if (src == NPPropertyA::BM_DSTCOLOR && dst == NPPropertyA::BM_ZERO)        return BM_DARK;
	else                                                                            return BM_USER;
}
xui_method_explain(onity_proptransref, set_blendmode,		void			)( void* userptr, s32 value )
{
	npu32 src;
	npu32 dst;
	switch(value)
	{
	case BM_DEFAULT:
		src = NPPropertyA::BM_DEFAULT;
		dst = NPPropertyA::BM_DEFAULT;
		break;
	case BM_ADD:
		src = NPPropertyA::BM_SRCALPHA;
		dst = NPPropertyA::BM_ONE;
		break;
	case BM_DARK:
		src = NPPropertyA::BM_DSTCOLOR;
		dst = NPPropertyA::BM_ZERO;
		break;
	case BM_USER:
		src = NPPropertyA::BM_ONE;
		dst = NPPropertyA::BM_ZERO;
		break;
	}

	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetSrcABM(src);
	prop->get_transref()->SetDstABM(dst);
}
xui_method_explain(onity_proptransref, get_blendsrc,		s32				)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	return (s32)prop->get_transref()->GetSrcABM();
}
xui_method_explain(onity_proptransref, set_blendsrc,		void			)( void* userptr, s32 value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetSrcABM((npu32)value);
}
xui_method_explain(onity_proptransref, get_blenddst,		s32				)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	return (s32)prop->get_transref()->GetDstABM();
}
xui_method_explain(onity_proptransref, set_blenddst,		void			)( void* userptr, s32 value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	prop->get_transref()->SetDstABM((npu32)value);
}
xui_method_explain(onity_proptransref, was_outline,			bool			)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	NP2DSOLInfo info = prop->get_transref()->GetOLInfo();
	return info.outline;
}
xui_method_explain(onity_proptransref, set_outline,			void			)( void* userptr, bool value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	NP2DSOLInfo info = prop->get_transref()->GetOLInfo();
	info.outline = value;
	prop->get_transref()->SetOLInfo(info);
}
xui_method_explain(onity_proptransref, get_outlinewidth,	f64				)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	NP2DSOLInfo info = prop->get_transref()->GetOLInfo();
	return (f64)info.outlineWidth;
}
xui_method_explain(onity_proptransref, set_outlinewidth,	void			)( void* userptr, f64 value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	NP2DSOLInfo info = prop->get_transref()->GetOLInfo();
	info.outlineWidth = (npu08)value;
	prop->get_transref()->SetOLInfo(info);
}
xui_method_explain(onity_proptransref, get_outlinecolor,	xui_colour		)( void* userptr )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	NP2DSOLInfo info = prop->get_transref()->GetOLInfo();
	NPColor color = info.outlineColor;
	return xui_colour(color.a, color.r, color.g, color.b);
}
xui_method_explain(onity_proptransref, set_outlinecolor,	void			)( void* userptr, const xui_colour&		 value )
{
	onity_proptransref* prop = (onity_proptransref*)userptr;
	NP2DSOLInfo info = prop->get_transref()->GetOLInfo();
	info.outlineColor = NPColor(value.r, value.g, value.b, value.a);
	prop->get_transref()->SetOLInfo(info);
}