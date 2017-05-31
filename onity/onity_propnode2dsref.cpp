#include "NP2DSImage.h"
#include "NP2DSFrame.h"
#include "NP2DSActor.h"
#include "NP2DSImageRef.h"
#include "NP2DSFrameRef.h"
#include "NP2DSActorRef.h"
#include "NP2DSSceneLayer.h"

#include "xui_textbox.h"
#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_kindctrl.h"
#include "xui_propctrl_vector.h"
#include "xui_propctrl_stdvec.h"
#include "onity_propctrl_transref.h"
#include "onity_propctrl_sceneparam.h"
#include "onity_mainform.h"
#include "onity_inspector.h"
#include "onity_resource.h"
#include "onity_propnode2dsref.h"

/*
//constructor
*/
xui_create_explain(onity_propnode2dsref)( NP2DSTransRef* ref )
: onity_propnoderender(ref)
, m_2dsref(ref)
{
	m_transkind = new xui_propkind(this, L"Resource Transform", "Transform", xui_kindctrl::create, onity_resource::icon_transform, true);
	m_transkind->add_propdata(new xui_propdata_vector(
		m_transkind, 
		L"Position", 
		xui_propctrl_vector::create, 
		get_position, 
		set_position, 
		this, 
		NT_INT));
	m_transkind->add_propdata(new xui_propdata_vector(
		m_transkind,
		L"Scale",
		xui_propctrl_vector::create,
		get_scale,
		set_scale,
		this,
		NT_FLOAT));
	m_transkind->add_propdata(new xui_propdata_number_func(
		m_transkind,
		L"Rotation",
		xui_propctrl_number::create,
		get_rotation,
		set_rotation,
		this,
		NT_INT));

	add_propkind(m_transkind);
}

/*
//method
*/
xui_method_explain(onity_propnode2dsref, get_2dsref,	NP2DSTransRef*	)( void )
{
	return m_2dsref;
}

/*
//static
*/
xui_method_explain(onity_propnode2dsref, get_position,	xui_vector<f64>	)( void* userptr )
{
	onity_propnode2dsref* prop = (onity_propnode2dsref*)userptr;
	NPVector3 trans = prop->get_2dsref()->GetWorldTrans();
	return xui_vector<f64>((f64)trans.x, (f64)trans.y);
}
xui_method_explain(onity_propnode2dsref, set_position,	void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_propnode2dsref* prop = (onity_propnode2dsref*)userptr;
	prop->get_2dsref()->SetWorldTrans(NPVector3((npf32)value.x, (npf32)value.y, 1.0f));
}
xui_method_explain(onity_propnode2dsref, get_scale,		xui_vector<f64>	)( void* userptr )
{
	onity_propnode2dsref* prop = (onity_propnode2dsref*)userptr;
	NPVector3 scale = prop->get_2dsref()->GetWorldScale();
	return xui_vector<f64>((f64)scale.x, (f64)scale.y);
}
xui_method_explain(onity_propnode2dsref, set_scale,		void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_propnode2dsref* prop = (onity_propnode2dsref*)userptr;
	prop->get_2dsref()->SetWorldScale(NPVector3((npf32)value.x, (npf32)value.y, 1.0f));
}
xui_method_explain(onity_propnode2dsref, get_rotation,	f64				)( void* userptr )
{
	onity_propnode2dsref* prop = (onity_propnode2dsref*)userptr;
	return (f64)prop->get_2dsref()->GetWorldAngle();
}
xui_method_explain(onity_propnode2dsref, set_rotation,	void			)( void* userptr, f64   value )
{
	onity_propnode2dsref* prop = (onity_propnode2dsref*)userptr;
	prop->get_2dsref()->SetWorldAngle((npf32)value);
}
