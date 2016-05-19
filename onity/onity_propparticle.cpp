#include "NPParticleSFX.h"
#include "xui_global.h"
#include "onity_propparticle.h"

/*
//constructor
*/
xui_create_explain(onity_propparticle)( const std::wstring& full )
: onity_propfile(full)
, m_particle(NULL)
{}

/*
//destructor
*/
xui_delete_explain(onity_propparticle)( void )
{
	delete m_particle;
}

/*
//method
*/
xui_method_explain(onity_propparticle, get_particle, NPParticleSFX*	)( void )
{
	if (m_particle == NULL)
	{
		m_particle = new NPParticleSFX;
		m_particle->LoadFromXML(xui_global::unicode_to_ascii(get_full()));
	}

	return m_particle;
}

/*
//override
*/
xui_method_explain(onity_propparticle, get_dragtype, std::string	)( void )
{
	return "NPParticleSFX";
}
xui_method_explain(onity_propparticle, get_dragdata, void*			)( void )
{
	return get_particle();
}