#include "2d/CCParticleSystem.h"

#include "xui_global.h"
#include "xui_kindctrl.h"
#include "cocos_resource.h"
#include "cocos_filedata.h"
#include "cocos_mainform.h"
#include "cocos_propparticle.h"
#include "cocos_project.h"
#include "cocos_boundbox.h"
#include "cocos_propdata_particle.h"
#include "cocos_propctrl_particlectrl.h"
#include "cocos_propnodeparticle.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodeparticle)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodebase(cocos_resource::icon_particle, file, node)
, m_particle(dynamic_cast<cocos2d::ParticleSystem*>(node))
{
	add_location	();
	add_position	();
	add_aligndock	(false);
	add_base		();
	add_common		();
	add_special		();
	add_particle	();
	add_advance		();
	add_userdata	();

	m_boundbox->add_operator(OP_SCALE);
	m_boundbox->set_ignoresz(true);
}

/*
//method
*/
xui_method_explain(cocos_propnodeparticle, get_particle,	cocos2d::ParticleSystem*)( void )
{
	return m_particle;
}
xui_method_explain(cocos_propnodeparticle, get_particlefile,cocos_propfile*			)( void )
{
	if (m_particlefile == NULL && m_particle->getResourceFile().length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_particle->getResourceFile());
		std::wstring work = xui_global::get_workpath();
		if (full.length() > work.length())
		{
			full.erase(0, work.length() + 1);
			std::wstring  path = cocos_filedata::get_path(full);
			std::wstring  file = cocos_filedata::get_file(full);
			path = path.substr(0, path.length() - 1);

			cocos_project* project = cocos_mainform::get_ptr()->get_project();
			m_particlefile = project->get_pathfile(path, file);
		}
	}

	return m_particlefile;
}
xui_method_explain(cocos_propnodeparticle, set_particlefile,void					)( cocos_propfile* file )
{
	m_particlefile = file;
	if (m_particlefile)
	{
		reload();
	}
	else
	{
		m_particle->init();
	}
}

xui_method_explain(cocos_propnodeparticle, play,			void					)( void )
{
	m_particle->resetSystem();
}
xui_method_explain(cocos_propnodeparticle, stop,			void					)( void )
{
	m_particle->stopSystem();
}
xui_method_explain(cocos_propnodeparticle, reload,			void					)( void )
{
	if (m_particlefile)
	{
		cocos2d::Vec2 pos = m_particle->getPosition();
		m_particle->initWithFile(xui_global::unicode_to_ascii(m_particlefile->get_fullname()));
		m_particle->setPosition(pos);
		m_particle->resetSystem();
	}
}

/*
//method
*/
xui_method_explain(cocos_propnodeparticle, add_particle,	void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "NodeParticle", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodeparticle>(this, &cocos_propnodeparticle::on_propchanged);
	kind->add_propdata(new cocos_propdata_particlectrl(
		kind,
		this));
	kind->add_propdata(new cocos_propdata_particle(
		kind,
		L"Particle",
		get_particle,
		set_particle,
		this));
	kind->add_propdata(new cocos_propdata_blend(
		kind,
		get_blendfunc,
		set_blendfunc,
		this));

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodeparticle, get_particle,	void*					)( void* userptr )
{
	cocos_propnodeparticle* prop = (cocos_propnodeparticle*)userptr;
	return prop->get_particlefile();
}
xui_method_explain(cocos_propnodeparticle, set_particle,	void					)( void* userptr, void* value )
{
	cocos_propnodeparticle* prop = (cocos_propnodeparticle*)userptr;
	prop->set_particlefile((cocos_propfile*)value);
}
xui_method_explain(cocos_propnodeparticle, get_blendfunc,	cocos_blend_value		)( void* userptr )
{
	cocos_propnodeparticle* prop = (cocos_propnodeparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::BlendFunc func = particle->getBlendFunc();
	return cocos_blend_value(func.src, func.dst);
}
xui_method_explain(cocos_propnodeparticle, set_blendfunc,	void					)( void* userptr, const cocos_blend_value& value )
{
	cocos_propnodeparticle* prop = (cocos_propnodeparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::BlendFunc func;
	func.src = value.src;
	func.dst = value.dst;
	particle->setBlendFunc(func);
}