#include "NP2DSTransRef.h"
#include "xui_kindctrl.h"
#include "xui_propdata.h"
#include "xui_propctrl_vector.h"
#include "xui_propctrl_stdvec.h"
#include "xui_textbox.h"
#include "onity_resource.h"
#include "onity_boundbox.h"
#include "onity_propctrl_sceneparam.h"
#include "onity_propeditnode.h"

/*
//constructor
*/
xui_create_explain(onity_propeditnode)( void )
: m_boundctrl(NULL)
{
	m_transkind = new xui_propkind(this, L"Transform", "SceneTransform", xui_kindctrl::create, onity_resource::icon_transform, true);
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

	m_paramkind = new xui_propkind(this, L"Design", "SceneDesign", xui_kindctrl::create, onity_resource::icon_design, true);
	m_paramkind->add_propdata(new xui_propdata_stdvec_func<NP2DSTransRef::SParam>(
		m_paramkind,
		L"Parameters",
		xui_propctrl_stdvec::create,
		add_param,
		del_param,
		new_paramprop,
		get_params,
		this));
}

/*
//destructor
*/
xui_delete_explain(onity_propeditnode)( void )
{
	delete m_boundctrl;
}

/*
//method
*/
xui_method_explain(onity_propeditnode, get_boundbox,	onity_boundbox*	)( void )
{
	return m_boundctrl;
}

/*
//virtual
*/
xui_method_explain(onity_propeditnode, get_2dsref,		NP2DSTransRef*	)( void )
{
	return NULL;
}
xui_method_explain(onity_propeditnode, get_scale,		xui_vector<f64>	)( void )
{
	return xui_vector<f64>(1.0);
}
xui_method_explain(onity_propeditnode, set_scale,		void			)( const xui_vector<f64>& value )
{}

/*
//static
*/
xui_method_explain(onity_propeditnode, get_position,	xui_vector<f64>	)( void* userptr )
{
	onity_propeditnode* prop = (onity_propeditnode*)userptr;
	return prop->get_boundbox()->ori_position().to<f64>();
}
xui_method_explain(onity_propeditnode, set_position,	void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_propeditnode* prop = (onity_propeditnode*)userptr;
	prop->get_boundbox()->set_position(value.to<s32>());
}
xui_method_explain(onity_propeditnode, get_scale,		xui_vector<f64>	)( void* userptr )
{
	onity_propeditnode* prop = (onity_propeditnode*)userptr;
	return prop->get_scale();
}
xui_method_explain(onity_propeditnode, set_scale,		void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_propeditnode* prop = (onity_propeditnode*)userptr;
	prop->set_scale(value);
}
xui_method_explain(onity_propeditnode, get_params,		ParamVec&		)( void* userptr )
{
	onity_propeditnode* prop = (onity_propeditnode*)userptr;
	NP2DSTransRef* ref = prop->get_2dsref();
	if (ref)
		return ref->GetSceneParamVec();

	static ParamVec empty;
	return empty;
}
xui_method_explain(onity_propeditnode, add_param,		void			)( void* userptr )
{
	onity_propeditnode* prop = (onity_propeditnode*)userptr;
	NP2DSTransRef* ref = prop->get_2dsref();
	if (ref)
		ref->AddSceneParam(NP2DSTransRef::SParam());
}
xui_method_explain(onity_propeditnode, del_param,		void			)( void* userptr )
{
	onity_propeditnode* prop = (onity_propeditnode*)userptr;
	NP2DSTransRef* ref = prop->get_2dsref();
	if (ref)
		ref->DelSceneParam(ref->GetSceneParamCount()-1);
}
xui_method_explain(onity_propeditnode, new_paramprop,	xui_propdata*	)( void* userptr, u32 i, xui_propkind* propkind )
{
	onity_propeditnode* prop = (onity_propeditnode*)userptr;
	NP2DSTransRef* ref = prop->get_2dsref();
	if (ref)
		return new onity_propdata_sceneparam(
			propkind, 
			L"Param", 
			onity_propctrl_sceneparam::create, 
			&(ref->GetSceneParamVec()[i]));

	return NULL;
}