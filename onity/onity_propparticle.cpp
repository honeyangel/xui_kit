#include "NPParticleSFX.h"
#include "NPParticleSpriteSRC.h"

#include "xui_global.h"
#include "xui_toggle.h"
#include "xui_propview.h"
#include "onity_savekind.h"
#include "onity_mainform.h"
#include "onity_inspector.h"
#include "onity_resource.h"
#include "onity_propkind_particlesrc.h"
#include "onity_propparticle.h"

/*
//constructor
*/
xui_create_explain(onity_propparticle)( const std::wstring& full )
: onity_propfile(onity_resource::icon_particle, full)
, m_particle(NULL)
, m_modify(false)
{
	m_menukind = new xui_propkind(this, L"", "SpriteSRCAdd", onity_kindctrl_particleadd::create, NULL, true, false, true);
	add_propkind(m_menukind);

	load();
}

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
xui_method_explain(onity_propparticle, get_particle,		NPParticleSFX*	)( void )
{
	return m_particle;
}
xui_method_explain(onity_propparticle, add_spritesrc,		void			)( void )
{
	m_modify = true;

	NPParticleSpriteSRC* spritesrc = new NPParticleSpriteSRC;
	m_particle->Attach(spritesrc);
	onity_propkind_particlesrc* propspritesrc = new onity_propkind_particlesrc(this, spritesrc);
	add_propkind(propspritesrc);

	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
	inspector->get_propview()->reset();
}
xui_method_explain(onity_propparticle, del_spritesrc,		void			)( xui_propkind* propkind )
{
	m_modify = true;

	onity_propkind_particlesrc* propspritesrc = dynamic_cast<onity_propkind_particlesrc*>(propkind);
	NPParticleSpriteSRC* spritesrc = propspritesrc->get_spritesrc();
	m_particle->Detach(spritesrc);
	propspritesrc->non_ctrl();
	del_propkind(propspritesrc);
	delete spritesrc;

	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
	inspector->get_propview()->reset();
}
xui_method_explain(onity_propparticle, set_spritesrcindex,	void			)( xui_propkind* propkind, u32 oldindex, u32 newindex )
{
	m_modify = true;

	NPParticleCommonSRC* src = m_particle->At(oldindex);
	m_particle->Detach(src);

	for (u32 i = 0; i < m_propkind.size(); ++i)
	{
		if (m_propkind[i] == propkind)
		{
			m_propkind.erase(m_propkind.begin()+i);
			break;
		}
	}

	if (oldindex < newindex)
		--newindex;

	m_particle->Attach(src, newindex);
	for (u32 i = 0, index = 0; i < m_propkind.size(); ++i)
	{
		onity_propkind_particlesrc* propkindsrc = dynamic_cast<onity_propkind_particlesrc*>(m_propkind[i]);
		if (propkindsrc)
		{
			if (index == newindex)
			{
				m_propkind.insert(m_propkind.begin()+i, propkind);
				break;
			}

			++index;
		}
	}

	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
	inspector->get_propview()->reset();
}

/*
//override
*/
xui_method_explain(onity_propparticle, get_dragtype,		std::string		)( void )
{
	return "NPParticleSFX";
}
xui_method_explain(onity_propparticle, get_dragdata,		void*			)( void )
{
	return get_particle();
}
xui_method_explain(onity_propparticle, was_modify,			bool			)( void )
{
	return m_modify;
}
xui_method_explain(onity_propparticle, load,				void			)( void )
{
	m_modify = false;

	if (get_ctrl())
	{
		onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
		inspector->set_proproot(NULL);
	}

	for (u32 i = 0; i < m_propkind.size(); ++i)
	{
		if (m_propkind[i] == m_basekind ||
			m_propkind[i] == m_savekind ||
			m_propkind[i] == m_menukind)
			continue;

		delete m_propkind[i];
	}

	m_propkind.clear();
	m_propkind.push_back(m_basekind);
	m_propkind.push_back(m_menukind);
	m_propkind.push_back(m_savekind);

	delete m_particle;
	m_particle = new NPParticleSFX;
	m_particle->LoadFromXML(xui_global::unicode_to_ascii(m_fullname));

	for (u32 i = 0; i < m_particle->NumParticleSRC(); ++i)
	{
		NPParticleSpriteSRC* spritesrc = NPDynamicCast(NPParticleSpriteSRC, m_particle->At(i));
		if (spritesrc)
		{
			xui_propkind* propkind = new onity_propkind_particlesrc(this, spritesrc);
			xui_method_ptrcall(propkind, xm_flagchanged) += new xui_method_member<xui_method_args,		onity_propparticle>(this, &onity_propparticle::on_flagchanged);
			xui_method_ptrcall(propkind, xm_propchanged) += new xui_method_member<xui_method_propdata,	onity_propparticle>(this, &onity_propparticle::on_propchanged);
			add_propkind(propkind);
		}
	}
}
xui_method_explain(onity_propparticle, save,				void			)( void )
{
	m_modify = false;
	m_particle->SaveIntoXML(xui_global::unicode_to_ascii(m_fullname));
}

/*
//event
*/
xui_method_explain(onity_propparticle, on_propchanged,		void			)( xui_component* sender, xui_method_propdata&	args )
{
	std::wstring name = args.propdata->get_name();
	if (name == L"TexAtlasBiasX" ||
		name == L"TexAtlasBiasY" ||
		name == L"TexAtlasSizeX" ||
		name == L"TexAtlasSizeY")
	{
		onity_propkind_particlesrc* propspritesrc = dynamic_cast<onity_propkind_particlesrc*>(args.propdata->get_kind());
		NPParticleSpriteSRC* spritesrc = propspritesrc->get_spritesrc();
		spritesrc->ComputeTexAtlasParam();
	}

	m_modify = true;
}
xui_method_explain(onity_propparticle, on_flagchanged,		void			)( xui_component* sender, xui_method_args&		args )
{
	xui_toggle*   toggle   = xui_dynamic_cast(xui_toggle,   sender);
	xui_kindctrl* kindctrl = xui_dynamic_cast(xui_kindctrl, toggle->get_parent());
	onity_propkind_particlesrc* propspritesrc = dynamic_cast<onity_propkind_particlesrc*>(kindctrl->get_propkind());
	NPParticleSpriteSRC* spritesrc = propspritesrc->get_spritesrc();
	spritesrc->SetFlag(PARTICLESRC_HIDDEN, !toggle->was_push());
}