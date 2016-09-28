#include "NPParticleSFX.h"
#include "xui_textbox.h"
#include "xui_kindctrl.h"
#include "xui_propctrl_vector.h"
#include "onity_resource.h"
#include "onity_propctrl_transref.h"
#include "onity_propnodeparticle.h"

/*
//constructor
*/
xui_create_explain(onity_propnodeparticle)( NPParticleSFX* particle )
: onity_propnoderender(particle)
, m_particle(particle)
{
	m_assetkind = new xui_propkind(this, L"Resource", "NodeParticleRes", xui_kindctrl::create, onity_resource::icon_resource, true);
	m_assetkind->add_propdata(new onity_propdata_particle(
		m_assetkind, 
		L"Particle", 
		get_asset, 
		set_asset, 
		this));

	m_transkind = new xui_propkind(this, L"Resource Transform", "NPTransform", xui_kindctrl::create, NULL, true);
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

	add_propkind(m_assetkind);
	add_propkind(m_transkind);
}

/*
//method
*/
xui_method_explain(onity_propnodeparticle, get_particle,	NPParticleSFX*	)( void )
{
	return m_particle;
}

/*
//static
*/
xui_method_explain(onity_propnodeparticle, get_position,	xui_vector<f64>	)( void* userptr )
{
	onity_propnodeparticle* prop = (onity_propnodeparticle*)userptr;
	NPParticleSFX* particle = prop->get_particle();
	NPVector3 position = particle->GetWorldT();
	return xui_vector<f64>((f64)position.x, (f64)position.y);
}
xui_method_explain(onity_propnodeparticle, set_position,	void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_propnodeparticle* prop = (onity_propnodeparticle*)userptr;
	NPParticleSFX* particle = prop->get_particle();
	particle->SetWorldT(NPVector3((f32)value.x, (f32)value.y, 0.0f));
}
xui_method_explain(onity_propnodeparticle, get_scale,		xui_vector<f64>	)( void* userptr )
{
	onity_propnodeparticle* prop = (onity_propnodeparticle*)userptr;
	NPParticleSFX* particle = prop->get_particle();
	NPVector3 scale = particle->GetWorldS();
	return xui_vector<f64>((f64)scale.x, (f64)scale.y);
}
xui_method_explain(onity_propnodeparticle, set_scale,		void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_propnodeparticle* prop = (onity_propnodeparticle*)userptr;
	NPParticleSFX* particle = prop->get_particle();
	particle->SetWorldS(NPVector3((f32)value.x, (f32)value.y, 1.0f));
}
xui_method_explain(onity_propnodeparticle, get_rotation,	f64				)( void* userptr )
{
	onity_propnodeparticle* prop = (onity_propnodeparticle*)userptr;
	NPParticleSFX* particle = prop->get_particle();
	NPQuaternion rotation = particle->GetWorldR();
	npf32 x, y, z, angle;
	rotation.ToAxisAngle(x, y, z, angle);
	return (f64)NPMathWrap::ToDegree(angle);
}
xui_method_explain(onity_propnodeparticle, set_rotation,	void			)( void* userptr, f64   value )
{
	onity_propnodeparticle* prop = (onity_propnodeparticle*)userptr;
	NPParticleSFX* particle = prop->get_particle();
	NPQuaternion rotation;
	rotation.SetRotationZ(NPMathWrap::ToRadian((f32)value));
	particle->SetWorldR(rotation);
}
xui_method_explain(onity_propnodeparticle, get_asset,		void*			)( void* userptr )
{
	onity_propnodeparticle* prop = (onity_propnodeparticle*)userptr;
	NPParticleSFX* particle = prop->get_particle();
	return particle;
}
xui_method_explain(onity_propnodeparticle, set_asset,		void			)( void* userptr, void* value )
{
	onity_propnodeparticle* prop = (onity_propnodeparticle*)userptr;
	NPParticleSFX* particle = (NPParticleSFX*)value;
	std::string filename = (particle == NULL) ? "" : particle->GetSourceFileName();
	prop->get_particle()->LoadFromXML(filename);
}