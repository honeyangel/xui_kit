#include "2d/CCParticleSystemQuad.h"
#include "renderer/CCTextureCache.h"

#include "xui_global.h"
#include "xui_toggle.h"
#include "xui_propview.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_propctrl_vector.h"
#include "xui_propctrl_expand.h"
#include "xui_textbox.h"
#include "cocos_savekind.h"
#include "cocos_mainform.h"
#include "cocos_inspector.h"
#include "cocos_propdata_texture.h"
#include "cocos_resource.h"
#include "cocos_propparticle.h"

/*
//constructor
*/
xui_create_explain(cocos_propparticle)( const std::wstring& full )
: cocos_propfile(cocos_resource::icon_particle, full)
, m_modify(false)
{
	load();

	m_imagekind = new xui_propkind(this, L"Particle Texture",  "ParticleTex",   xui_kindctrl::create, cocos_resource::icon_resource, true);
	xui_method_ptrcall(m_imagekind, xm_propchanged) += new xui_method_member<xui_method_propdata, cocos_propparticle>(this, &cocos_propparticle::on_propchanged);
	m_imagekind->add_propdata(new cocos_propdata_texture(
		m_imagekind,
		L"Texture",
		get_texture,
		set_texture,
		this));

	m_valuekind = new xui_propkind(this, L"Particle Property", "ParticleValue", xui_kindctrl::create, cocos_resource::icon_particle, true);
	xui_method_ptrcall(m_valuekind, xm_propchanged) += new xui_method_member<xui_method_propdata, cocos_propparticle>(this, &cocos_propparticle::on_propchanged);
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind, 
		L"Duration", 
		xui_propctrl_number::create, 
		get_duration, 
		set_duration, 
		this, 
		NT_FLOAT, 
		0.1));
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind,
		L"Max Particles",
		xui_propctrl_number::create,
		get_maxparticle,
		set_maxparticle,
		this,
		NT_UNSIGNEDINT));
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind,
		L"Lifespan",
		xui_propctrl_number::create,
		get_lifespan,
		set_lifespan,
		this,
		NT_FLOAT,
		0.1));
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind,
		L"Lifespan Var",
		xui_propctrl_number::create,
		get_lifespanvar,
		set_lifespanvar,
		this,
		NT_FLOAT,
		0.1));
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind,
		L"Start Size",
		xui_propctrl_number::create,
		get_startsize,
		set_startsize,
		this,
		NT_UNSIGNEDINT));
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind,
		L"Start Size Var",
		xui_propctrl_number::create,
		get_startsizevar,
		set_startsizevar,
		this,
		NT_UNSIGNEDINT));
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind,
		L"Finish Size",
		xui_propctrl_number::create,
		get_finishsize,
		set_finishsize,
		this,
		NT_UNSIGNEDINT));
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind,
		L"Finish Size Var",
		xui_propctrl_number::create,
		get_finishsizevar,
		set_finishsizevar,
		this,
		NT_UNSIGNEDINT));
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind,
		L"Angle",
		xui_propctrl_number::create,
		get_angle,
		set_angle,
		this,
		NT_UNSIGNEDINT));
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind,
		L"Angle Var",
		xui_propctrl_number::create,
		get_anglevar,
		set_anglevar,
		this,
		NT_UNSIGNEDINT));
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind,
		L"Rotation Start",
		xui_propctrl_number::create,
		get_rotationstart,
		set_rotationstart,
		this,
		NT_UNSIGNEDINT));
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind,
		L"Rotation Start Var",
		xui_propctrl_number::create,
		get_rotationstartvar,
		set_rotationstartvar,
		this,
		NT_UNSIGNEDINT));
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind,
		L"Rotation End",
		xui_propctrl_number::create,
		get_rotationend,
		set_rotationend,
		this,
		NT_UNSIGNEDINT));
	m_valuekind->add_propdata(new xui_propdata_number_func(
		m_valuekind,
		L"Rotation End Var",
		xui_propctrl_number::create,
		get_rotationendvar,
		set_rotationendvar,
		this,
		NT_UNSIGNEDINT));

	m_colorkind = new xui_propkind(this, L"Particle Color", "ParticleColor", xui_kindctrl::create, cocos_resource::icon_visual, true);
	xui_method_ptrcall(m_colorkind, xm_propchanged) += new xui_method_member<xui_method_propdata, cocos_propparticle>(this, &cocos_propparticle::on_propchanged);
	m_colorkind->add_propdata(new xui_propdata_colour(
		m_colorkind,
		L"Start Color",
		xui_propctrl_colour::create,
		get_startcolor,
		set_startcolor,
		this));
	m_colorkind->add_propdata(new xui_propdata_colour(
		m_colorkind,
		L"Start Color Var",
		xui_propctrl_colour::create,
		get_startcolorvar,
		set_startcolorvar,
		this));
	m_colorkind->add_propdata(new xui_propdata_colour(
		m_colorkind,
		L"End Color",
		xui_propctrl_colour::create,
		get_endcolor,
		set_endcolor,
		this));
	m_colorkind->add_propdata(new xui_propdata_colour(
		m_colorkind,
		L"End Color Var",
		xui_propctrl_colour::create,
		get_endcolorvar,
		set_endcolorvar,
		this));

	m_emittkind = new xui_propkind(this, L"Particle Emitter", "ParticleEmitter", xui_kindctrl::create, cocos_resource::icon_legend, true);
	xui_method_ptrcall(m_emittkind, xm_propchanged) += new xui_method_member<xui_method_propdata, cocos_propparticle>(this, &cocos_propparticle::on_propchanged);
	xui_propdata_vec				subprop;
	std::map<s32, std::vector<u32>>	showmap;
	std::map<s32, std::wstring>		textmap;
	subprop.push_back(new xui_propdata_number_func(
		m_emittkind,
		L"Speed",
		xui_propctrl_number::create,
		get_speed,
		set_speed,
		this,
		NT_INT));
	subprop.push_back(new xui_propdata_number_func(
		m_emittkind,
		L"SpeedVar",
		xui_propctrl_number::create,
		get_speedvar,
		set_speedvar,
		this,
		NT_INT));
	subprop.push_back(new xui_propdata_number_func(
		m_emittkind,
		L"Radial Acc",
		xui_propctrl_number::create,
		get_radialacc,
		set_radialacc,
		this,
		NT_INT));
	subprop.push_back(new xui_propdata_number_func(
		m_emittkind,
		L"Radial Acc Var",
		xui_propctrl_number::create,
		get_radialaccvar,
		set_radialaccvar,
		this,
		NT_INT));
	subprop.push_back(new xui_propdata_number_func(
		m_emittkind,
		L"Tangen Acc",
		xui_propctrl_number::create,
		get_tangenacc,
		set_tangenacc,
		this,
		NT_INT));
	subprop.push_back(new xui_propdata_number_func(
		m_emittkind,
		L"Tangen Acc Var",
		xui_propctrl_number::create,
		get_tangenaccvar,
		set_tangenaccvar,
		this,
		NT_INT));
	subprop.push_back(new xui_propdata_number_func(
		m_emittkind,
		L"Max Radius",
		xui_propctrl_number::create,
		get_maxradius,
		set_maxradius,
		this,
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_number_func(
		m_emittkind,
		L"Max Radius Var",
		xui_propctrl_number::create,
		get_maxradiusvar,
		set_maxradiusvar,
		this,
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_number_func(
		m_emittkind,
		L"Min Radius",
		xui_propctrl_number::create,
		get_minradius,
		set_minradius,
		this,
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_number_func(
		m_emittkind,
		L"Min Radius Var",
		xui_propctrl_number::create,
		get_minradiusvar,
		set_minradiusvar,
		this,
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_number_func(
		m_emittkind,
		L"Rotate Per Second",
		xui_propctrl_number::create,
		get_rotatespeed,
		set_rotatespeed,
		this,
		NT_INT));
	subprop.push_back(new xui_propdata_number_func(
		m_emittkind,
		L"Rotate Per Second Var",
		xui_propctrl_number::create,
		get_rotatespeedvar,
		set_rotatespeedvar,
		this,
		NT_INT));
	textmap[0] = L"Gravity";
	textmap[1] = L"Radial";
	showmap[0].push_back(0);
	showmap[0].push_back(1);
	showmap[0].push_back(2);
	showmap[0].push_back(3);
	showmap[0].push_back(4);
	showmap[0].push_back(5);
	showmap[1].push_back(6);
	showmap[1].push_back(7);
	showmap[1].push_back(8);
	showmap[1].push_back(9);
	showmap[1].push_back(10);
	showmap[1].push_back(11);
	m_emittkind->add_propdata(new xui_propdata_expand_enum_func(
		m_emittkind,
		L"Emitter Type",
		xui_propctrl_expand_enum::create,
		textmap,
		get_emittertype,
		set_emittertype,
		this,
		subprop,
		true,
		showmap));

	m_transkind = new xui_propkind(this, L"Particle Transform", "ParticlePosition", xui_kindctrl::create, cocos_resource::icon_transform, true);
	xui_method_ptrcall(m_transkind, xm_propchanged) += new xui_method_member<xui_method_propdata, cocos_propparticle>(this, &cocos_propparticle::on_propchanged);
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
		L"Position Var",
		xui_propctrl_vector::create,
		get_positionvar,
		set_positionvar,
		this,
		NT_INT));

	m_blendkind = new xui_propkind(this, L"Blend Function", "ParticleBlend", xui_kindctrl::create, cocos_resource::icon_world, true);
	xui_method_ptrcall(m_blendkind, xm_propchanged) += new xui_method_member<xui_method_propdata, cocos_propparticle>(this, &cocos_propparticle::on_propchanged);
	m_blendkind->add_propdata(new cocos_propdata_blend(
		m_blendkind, 
		get_blendfunc, 
		set_blendfunc, 
		this));
#if 0
	textmap.clear();
	textmap[GL_ZERO]				= L"GL_ZERO";
	textmap[GL_ONE]					= L"GL_ONE";
	textmap[GL_SRC_COLOR]			= L"GL_SRC_COLOR";
	textmap[GL_ONE_MINUS_SRC_COLOR]	= L"GL_ONE_MINUS_SRC_COLOR";
	textmap[GL_SRC_ALPHA]			= L"GL_SRC_ALPHA";
	textmap[GL_ONE_MINUS_SRC_ALPHA] = L"GL_ONE_MINUS_SRC_ALPHA";
	textmap[GL_DST_COLOR]			= L"GL_DST_COLOR";
	textmap[GL_ONE_MINUS_DST_COLOR] = L"GL_ONE_MINUS_DST_COLOR";
	textmap[GL_DST_ALPHA]			= L"GL_DST_ALPHA";
	textmap[GL_ONE_MINUS_DST_ALPHA] = L"GL_ONE_MINUS_DST_ALPHA";
	textmap[GL_SRC_ALPHA_SATURATE]	= L"GL_SRC_ALPHA_SATURATE";
	subprop.clear();
	subprop.push_back(new xui_propdata_enum_func(
		m_blendkind,
		L"Src",
		xui_propctrl_enum::create,
		textmap,
		get_blendsrc,
		set_blendsrc,
		this));
	subprop.push_back(new xui_propdata_enum_func(
		m_blendkind,
		L"Dst",
		xui_propctrl_enum::create,
		textmap,
		get_blenddst,
		set_blenddst,
		this));

	textmap.clear();
	textmap[0] = L"Normal";
	textmap[1] = L"Addition";
	textmap[2] = L"User";
	showmap.clear();
	showmap[2].push_back(0);
	showmap[2].push_back(1);
	m_blendkind->add_propdata(new xui_propdata_expand_enum_func(
		m_blendkind,
		L"Blend",
		xui_propctrl_expand_enum::create,
		textmap,
		get_blendfunction,
		set_blendfunction,
		this,
		subprop,
		true,
		showmap));
#endif

	//m_menukind = new xui_propkind(this, L"", "SpriteSRCAdd", onity_kindctrl_particleadd::create, NULL, true, false, true);
	//add_propkind(m_menukind);
	add_propkind(m_imagekind);
	add_propkind(m_valuekind);
	add_propkind(m_colorkind);
	add_propkind(m_emittkind);
	add_propkind(m_transkind);
	add_propkind(m_blendkind);
}

/*
//destructor
*/
xui_delete_explain(cocos_propparticle)( void )
{
	delete m_particle;
}

/*
//method
*/
xui_method_explain(cocos_propparticle, get_particle,			cocos2d::ParticleSystem*	)( void )
{
	return m_particle;
}
//xui_method_explain(onity_propparticle, add_spritesrc,		void			)( void )
//{
//	m_modify = true;
//
//	NPParticleSpriteSRC* spritesrc = new NPParticleSpriteSRC;
//	m_particle->Attach(spritesrc);
//	onity_propkind_particlesrc* propspritesrc = new onity_propkind_particlesrc(this, spritesrc);
//	add_propkind(propspritesrc);
//
//	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
//	inspector->get_propview()->reset();
//}
//xui_method_explain(onity_propparticle, del_spritesrc,		void			)( xui_propkind* propkind )
//{
//	m_modify = true;
//
//	onity_propkind_particlesrc* propspritesrc = dynamic_cast<onity_propkind_particlesrc*>(propkind);
//	NPParticleSpriteSRC* spritesrc = propspritesrc->get_spritesrc();
//	m_particle->Detach(spritesrc);
//	propspritesrc->non_ctrl();
//	del_propkind(propspritesrc);
//	delete spritesrc;
//
//	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
//	inspector->get_propview()->reset();
//}
//xui_method_explain(onity_propparticle, set_spritesrcindex,	void			)( xui_propkind* propkind, u32 oldindex, u32 newindex )
//{
//	m_modify = true;
//
//	NPParticleCommonSRC* src = m_particle->At(oldindex);
//	m_particle->Detach(src);
//
//	for (u32 i = 0; i < m_propkind.size(); ++i)
//	{
//		if (m_propkind[i] == propkind)
//		{
//			m_propkind.erase(m_propkind.begin()+i);
//			break;
//		}
//	}
//
//	if (oldindex < newindex)
//		--newindex;
//
//	m_particle->Attach(src, newindex);
//	for (u32 i = 0; i < m_propkind.size(); ++i)
//	{
//		onity_propkind_particlesrc* propkindsrc = dynamic_cast<onity_propkind_particlesrc*>(m_propkind[i]);
//		if (propkindsrc)
//		{
//			m_propkind.insert(m_propkind.begin()+i+newindex, propkind);
//			break;
//		}
//	}
//
//	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
//	inspector->get_propview()->reset();
//}

/*
//override
*/
xui_method_explain(cocos_propparticle, get_dragtype,			std::string					)( void )
{
	return "ParticleSystem";
}
xui_method_explain(cocos_propparticle, get_dragdata,			void*						)( void )
{
	return this;
}
xui_method_explain(cocos_propparticle, was_modify,				bool						)( void )
{
	return m_modify;
}
xui_method_explain(cocos_propparticle, load,					void						)( void )
{
	m_modify = false;

	if (get_ctrl())
	{
		cocos_inspector* inspector = cocos_mainform::get_ptr()->get_inspector();
		inspector->set_proproot(NULL);
	}

	if (m_particle == NULL)
		m_particle = cocos2d::ParticleSystemQuad::create();

	if (xui_global::has_file(m_fullname))
	{
		m_particle->initWithFile(xui_global::unicode_to_ascii(m_fullname));
		m_particle->resetSystem();
		m_particle->setPosition(0.0f, 0.0f);
	}
}
xui_method_explain(cocos_propparticle, save_as,					void						)( const std::wstring& fullname, bool modify )
{
	m_modify = modify;
	//TODO
	FILE* file = fopen(xui_global::unicode_to_ascii(fullname).c_str(), "w");
	if (file)
	{
		fclose(file);
	}
}

/*
//event
*/
xui_method_explain(cocos_propparticle, on_propchanged,			void						)( xui_component* sender, xui_method_propdata&	args )
{
	m_modify = true;
	m_particle->resetSystem();
}
xui_method_explain(cocos_propparticle, on_flagchanged,			void						)( xui_component* sender, xui_method_args&		args )
{

}

/*
//static
*/
xui_method_explain(cocos_propparticle, get_texture,				void*						)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getTexture();
}
xui_method_explain(cocos_propparticle, set_texture,				void						)( void* userptr, void* value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	if (value == NULL)
	{
		particle->setTexture(cocos2d::TextureCache::getInstance()->getDummyImage());
	}
	else
	{
		particle->setTexture((cocos2d::Texture2D*)value);
	}
}

xui_method_explain(cocos_propparticle, get_duration,			f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getDuration();
}
xui_method_explain(cocos_propparticle, set_duration,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setDuration(value);
}
xui_method_explain(cocos_propparticle, get_maxparticle,			f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getTotalParticles();
}
xui_method_explain(cocos_propparticle, set_maxparticle,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setTotalParticles(value);
}
xui_method_explain(cocos_propparticle, get_lifespan,			f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getLife();
}
xui_method_explain(cocos_propparticle, set_lifespan,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setLife(value);
}
xui_method_explain(cocos_propparticle, get_lifespanvar,			f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getLifeVar();
}
xui_method_explain(cocos_propparticle, set_lifespanvar,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setLifeVar(value);
}
xui_method_explain(cocos_propparticle, get_startsize,			f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getStartSize();
}
xui_method_explain(cocos_propparticle, set_startsize,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setStartSize(value);
}
xui_method_explain(cocos_propparticle, get_startsizevar,		f64							)( void* userptr)
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getStartSizeVar();
}
xui_method_explain(cocos_propparticle, set_startsizevar,		void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setStartSizeVar(value);
}
xui_method_explain(cocos_propparticle, get_finishsize,			f64							)( void* userptr)
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getEndSize();
}
xui_method_explain(cocos_propparticle, set_finishsize,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setEndSize(value);
}
xui_method_explain(cocos_propparticle, get_finishsizevar,		f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getEndSizeVar();
}
xui_method_explain(cocos_propparticle, set_finishsizevar,		void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setEndSizeVar(value);
}
xui_method_explain(cocos_propparticle, get_angle,				f64							)( void* userptr)
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getAngle();
}
xui_method_explain(cocos_propparticle, set_angle,				void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setAngle(value);
}
xui_method_explain(cocos_propparticle, get_anglevar,			f64							)( void* userptr)
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getAngleVar();
}
xui_method_explain(cocos_propparticle, set_anglevar,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setAngleVar(value);
}
xui_method_explain(cocos_propparticle, get_rotationstart,		f64							)( void* userptr)
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getStartSpin();
}
xui_method_explain(cocos_propparticle, set_rotationstart,		void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setStartSpin(value);
}
xui_method_explain(cocos_propparticle, get_rotationstartvar,	f64							)( void* userptr)
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getStartSpinVar();
}
xui_method_explain(cocos_propparticle, set_rotationstartvar,	void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setStartSpinVar(value);
}
xui_method_explain(cocos_propparticle, get_rotationend,			f64							)( void* userptr)
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getEndSpin();
}
xui_method_explain(cocos_propparticle, set_rotationend,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setEndSpin(value);
}
xui_method_explain(cocos_propparticle, get_rotationendvar,		f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return particle->getEndSpinVar();
}
xui_method_explain(cocos_propparticle, set_rotationendvar,		void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setEndSpinVar(value);
}

xui_method_explain(cocos_propparticle, get_startcolor,			xui_colour					)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::Color4F color = particle->getStartColor();
	return xui_colour(color.a, color.r, color.g, color.b);
}
xui_method_explain(cocos_propparticle, set_startcolor,			void						)( void* userptr, const xui_colour& value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setStartColor(cocos2d::Color4F(value.r, value.g, value.b, value.a));
}
xui_method_explain(cocos_propparticle, get_startcolorvar,		xui_colour					)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::Color4F color = particle->getStartColorVar();
	return xui_colour(color.a, color.r, color.g, color.b);
}
xui_method_explain(cocos_propparticle, set_startcolorvar,		void						)( void* userptr, const xui_colour& value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setStartColorVar(cocos2d::Color4F(value.r, value.g, value.b, value.a));
}
xui_method_explain(cocos_propparticle, get_endcolor,			xui_colour					)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::Color4F color = particle->getEndColor();
	return xui_colour(color.a, color.r, color.g, color.b);
}
xui_method_explain(cocos_propparticle, set_endcolor,			void						)( void* userptr, const xui_colour& value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setEndColor(cocos2d::Color4F(value.r, value.g, value.b, value.a));
}
xui_method_explain(cocos_propparticle, get_endcolorvar,			xui_colour					)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::Color4F color = particle->getEndColorVar();
	return xui_colour(color.a, color.r, color.g, color.b);
}
xui_method_explain(cocos_propparticle, set_endcolorvar,			void						)( void* userptr, const xui_colour& value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setEndColorVar(cocos2d::Color4F(value.r, value.g, value.b, value.a));
}

xui_method_explain(cocos_propparticle, get_position,			xui_vector<f64>				)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::Vec2 position = particle->getPosition();
	return xui_vector<f64>(position.x, position.y);
}
xui_method_explain(cocos_propparticle, set_position,			void						)( void* userptr, const xui_vector<f64>& value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setPosition(cocos2d::Vec2(value.x, value.y));
}
xui_method_explain(cocos_propparticle, get_positionvar,			xui_vector<f64>				)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::Vec2 position = particle->getPosVar();
	return xui_vector<f64>(position.x, position.y);
}
xui_method_explain(cocos_propparticle, set_positionvar,			void						)( void* userptr, const xui_vector<f64>& value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setPosVar(cocos2d::Vec2(value.x, value.y));
}

xui_method_explain(cocos_propparticle, get_emittertype,			s32							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	return (s32)particle->getEmitterMode();
}
xui_method_explain(cocos_propparticle, set_emittertype,			void						)( void* userptr, s32 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setEmitterMode((cocos2d::ParticleSystem::Mode)value);
}
xui_method_explain(cocos_propparticle, get_speed,				f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	if (particle->getEmitterMode() == cocos2d::ParticleSystem::Mode::GRAVITY)
		return particle->getSpeed();

	return 0.0;
}
xui_method_explain(cocos_propparticle, set_speed,				void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setSpeed(value);
}
xui_method_explain(cocos_propparticle, get_speedvar,			f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	if (particle->getEmitterMode() == cocos2d::ParticleSystem::Mode::GRAVITY)
		return particle->getSpeedVar();

	return 0.0;
}
xui_method_explain(cocos_propparticle, set_speedvar,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setSpeedVar(value);
}
xui_method_explain(cocos_propparticle, get_radialacc,			f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	if (particle->getEmitterMode() == cocos2d::ParticleSystem::Mode::GRAVITY)
		return particle->getRadialAccel();

	return 0.0;
}
xui_method_explain(cocos_propparticle, set_radialacc,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setRadialAccel(value);
}
xui_method_explain(cocos_propparticle, get_radialaccvar,		f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	if (particle->getEmitterMode() == cocos2d::ParticleSystem::Mode::GRAVITY)
		return particle->getRadialAccelVar();

	return 0.0;
}
xui_method_explain(cocos_propparticle, set_radialaccvar,		void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setRadialAccelVar(value);
}
xui_method_explain(cocos_propparticle, get_tangenacc,			f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	if (particle->getEmitterMode() == cocos2d::ParticleSystem::Mode::GRAVITY)
		return particle->getTangentialAccel();

	return 0.0;
}
xui_method_explain(cocos_propparticle, set_tangenacc,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setTangentialAccel(value);
}
xui_method_explain(cocos_propparticle, get_tangenaccvar,		f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	if (particle->getEmitterMode() == cocos2d::ParticleSystem::Mode::GRAVITY)
		return particle->getTangentialAccelVar();

	return 0.0;
}
xui_method_explain(cocos_propparticle, set_tangenaccvar,		void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setTangentialAccelVar(value);
}
xui_method_explain(cocos_propparticle, get_maxradius,			f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	if (particle->getEmitterMode() == cocos2d::ParticleSystem::Mode::RADIUS)
		return particle->getStartRadius();

	return 0.0;
}
xui_method_explain(cocos_propparticle, set_maxradius,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setStartRadius(value);
}
xui_method_explain(cocos_propparticle, get_maxradiusvar,		f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	if (particle->getEmitterMode() == cocos2d::ParticleSystem::Mode::RADIUS)
		return particle->getStartRadiusVar();

	return 0.0;
}
xui_method_explain(cocos_propparticle, set_maxradiusvar,		void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setStartRadiusVar(value);
}
xui_method_explain(cocos_propparticle, get_minradius,			f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	if (particle->getEmitterMode() == cocos2d::ParticleSystem::Mode::RADIUS)
		return particle->getEndRadius();

	return 0.0;
}
xui_method_explain(cocos_propparticle, set_minradius,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setEndRadius(value);
}
xui_method_explain(cocos_propparticle, get_minradiusvar,		f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	if (particle->getEmitterMode() == cocos2d::ParticleSystem::Mode::RADIUS)
		return particle->getEndRadiusVar();

	return 0.0;
}
xui_method_explain(cocos_propparticle, set_minradiusvar,		void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setEndRadiusVar(value);
}
xui_method_explain(cocos_propparticle, get_rotatespeed,			f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	if (particle->getEmitterMode() == cocos2d::ParticleSystem::Mode::RADIUS)
		return particle->getRotatePerSecond();

	return 0.0;
}
xui_method_explain(cocos_propparticle, set_rotatespeed,			void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setRotatePerSecond(value);
}
xui_method_explain(cocos_propparticle, get_rotatespeedvar,		f64							)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	if (particle->getEmitterMode() == cocos2d::ParticleSystem::Mode::RADIUS)
		return particle->getRotatePerSecondVar();

	return 0.0;
}
xui_method_explain(cocos_propparticle, set_rotatespeedvar,		void						)( void* userptr, f64 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	particle->setRotatePerSecondVar(value);
}

xui_method_explain(cocos_propparticle, get_blendfunc,			cocos_blend_value			)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::BlendFunc func = particle->getBlendFunc();
	return cocos_blend_value(func.src, func.dst);
}
xui_method_explain(cocos_propparticle, set_blendfunc,			void						)( void* userptr, const cocos_blend_value& value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::BlendFunc func;
	func.src = value.src;
	func.dst = value.dst;
	particle->setBlendFunc(func);
}

#if 0
xui_method_explain(cocos_propparticle, get_blendfunction,		s32				)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::BlendFunc func = particle->getBlendFunc();
	if		(func.src == GL_SRC_ALPHA && func.dst == GL_ONE_MINUS_SRC_ALPHA)	return 0;
	else if (func.src == GL_SRC_ALPHA && func.dst == GL_ONE)					return 1;
	else																		return 2;
}
xui_method_explain(cocos_propparticle, set_blendfunction,		void			)( void* userptr, s32 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::BlendFunc func;
	switch (value)
	{
	case 0:
		func.src = GL_SRC_ALPHA;
		func.dst = GL_ONE_MINUS_SRC_ALPHA;
		break;
	case 1:
		func.src = GL_SRC_ALPHA;
		func.dst = GL_ONE;
		break;
	case 2:
		func.src = GL_ONE;
		func.dst = GL_ONE;
		break;
	}

	particle->setBlendFunc(func);
}
xui_method_explain(cocos_propparticle, get_blendsrc,			s32				)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::BlendFunc func = particle->getBlendFunc();
	return (s32)func.src;
}
xui_method_explain(cocos_propparticle, set_blendsrc,			void			)( void* userptr, s32 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::BlendFunc func = particle->getBlendFunc();
	func.src = value;
	particle->setBlendFunc(func);
}
xui_method_explain(cocos_propparticle, get_blenddst,			s32				)( void* userptr )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::BlendFunc func = particle->getBlendFunc();
	return (s32)func.dst;
}
xui_method_explain(cocos_propparticle, set_blenddst,			void			)( void* userptr, s32 value )
{
	cocos_propparticle* prop = (cocos_propparticle*)userptr;
	cocos2d::ParticleSystem* particle = prop->get_particle();
	cocos2d::BlendFunc func = particle->getBlendFunc();
	func.dst = value;
	particle->setBlendFunc(func);
}
#endif