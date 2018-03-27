#include "NPParticleSpriteSRC.h"
#include "NPParticleModLifetimeClasses.h"
#include "NPParticleModAccelClasses.h"
#include "NPParticleModLocationClasses.h"
#include "NPParticleModRotationClasses.h"
#include "NPParticleModSizeClasses.h"
#include "NPParticleModTexCoordClasses.h"
#include "NPParticleModUberClasses.h"
#include "NPParticleModColorClasses.h"
#include "NPParticleModVelocityClasses.h"

#include "xui_propctrl_stdvec.h"
#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_listview.h"
#include "xui_listitem.h"
#include "xui_numbbox.h"
#include "xui_dropbox.h"
#include "xui_propkind.h"
#include "onity_propctrl_particlemod.h"

//////////////////////////////////////////////////////////////////////////
//propdata
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(onity_propdata_curve)( xui_propkind* kind, const std::wstring& name, NPCurveControlPoint* ptr )
: xui_propdata(kind, name, onity_propctrl_curve::create)
, m_ptr(ptr)
{}

/*
//method
*/
xui_method_explain(onity_propdata_curve,		get_value,					NPCurveControlPoint		)( void ) const
{
	return (*m_ptr);
}
xui_method_explain(onity_propdata_curve,		set_value,					void					)( const NPCurveControlPoint& value )
{
	if (m_ptr->Interp   != value.Interp   ||
		m_ptr->KeyValue != value.KeyValue ||
		m_ptr->RetValue != value.RetValue ||
		m_ptr->TanEnter != value.TanEnter ||
		m_ptr->TanLeave != value.TanLeave)
	{
		*m_ptr = value;
		on_valuechanged();
	}
}

/*
//constructor
*/
xui_create_explain(onity_propdata_particlemod)( xui_propkind* kind, const std::wstring& name, NPParticleSpriteSRC* spritesrc )
: xui_propdata(kind, name, onity_propctrl_particlemod::create)
, m_spritesrc(spritesrc)
{
	for (u32 i = 0; i < m_spritesrc->NumParticleMOD(); ++i)
		add_modifydata(m_spritesrc->At(i));
}

/*
//destructor
*/
xui_delete_explain(onity_propdata_particlemod)( void )
{
	for (u32 i = 0; i < m_spritemodvec.size(); ++i)
		delete m_spritemodvec[i];
}

/*
//method
*/
xui_method_explain(onity_propdata_particlemod,	get_modifydatavec,			const xui_propdata_vec&	)( void ) const
{
	return m_spritemodvec;
}
xui_method_explain(onity_propdata_particlemod,	add_modify,					xui_propdata*			)( NPParticleMOD* mod )
{
	m_spritesrc->Attach(mod);
	xui_propdata* propdata = add_modifydata(mod);

	xui_method_propdata args;
	args.propdata = this;
	m_kind->xm_propchanged(m_ctrl, args);

	return propdata;
}
xui_method_explain(onity_propdata_particlemod,	del_modify,					void					)( xui_propdata* propdata )
{
	for (u32 i = 0; i < m_spritemodvec.size(); ++i)
	{
		if (m_spritemodvec[i] == propdata)
		{
			NPParticleMOD* mod = m_spritesrc->At(i);
			m_spritesrc->Detach(mod);
			delete mod;

			delete m_spritemodvec[i];
			m_spritemodvec.erase(m_spritemodvec.begin()+i);
			break;
		}
	}

	xui_method_propdata args;
	args.propdata = this;
	m_kind->xm_propchanged(m_ctrl, args);
}

/*
//override
*/
xui_method_explain(onity_propdata_particlemod,	non_ctrl,					void					)( void )
{
	xui_propdata::non_ctrl();
	for (u32 i = 0; i < m_spritemodvec.size(); ++i)
		m_spritemodvec[i]->non_ctrl();
}
	
/*
//method
*/
xui_method_explain(onity_propdata_particlemod,	add_modifydata,				xui_propdata*			)( NPParticleMOD* mod )
{
	xui_propdata* propdata = NULL;

#define INIT_MOD_CASE(class_name, method) if	  (NPIsExaKindOf(class_name, mod)) propdata = method(NPDynamicCast(class_name, mod))
#define NEXT_MOD_CASE(class_name, method) else if (NPIsExaKindOf(class_name, mod)) propdata = method(NPDynamicCast(class_name, mod))
	INIT_MOD_CASE(NPParticleSeedLifetimeMOD,		new_seedlifetime);
	NEXT_MOD_CASE(NPParticleAttractAccelMOD,		new_attractaccel);
	NEXT_MOD_CASE(NPParticleDampingAccelMOD,		new_dampingaccel);
	NEXT_MOD_CASE(NPParticleSeedAccelMOD,			new_seedaccel);
	NEXT_MOD_CASE(NPParticleSineAccelMOD,			new_sineaccel);
	NEXT_MOD_CASE(NPParticleSeedLocationMOD,		new_seedlocation);
	NEXT_MOD_CASE(NPParticleOrbitLocationMOD,		new_orbitlocation);
	NEXT_MOD_CASE(NPParticleSeedRotation2DMOD,		new_seedrotation);
	NEXT_MOD_CASE(NPParticleCurvedUniformSizeMOD,	new_curveduniformsize);
	NEXT_MOD_CASE(NPParticleCurvedSizeMOD,			new_curvedsize);
	NEXT_MOD_CASE(NPParticleUniformOverLifeSizeMOD, new_uniformoverlifesize);
	NEXT_MOD_CASE(NPParticleOverLifeSizeMOD,		new_overlifesize);
	NEXT_MOD_CASE(NPParticleSeedSizeMOD,			new_seedsize);
	NEXT_MOD_CASE(NPParticleUniformSeedSizeMOD,		new_uniformseedsize);
	NEXT_MOD_CASE(NPParticleFlipbookUVMOD,			new_flipbookuv);
	NEXT_MOD_CASE(NPParticleScrollAnimMOD,			new_scrollanim);
	NEXT_MOD_CASE(NPParticleTileSubTexMOD,			new_tilesubtex);
	NEXT_MOD_CASE(NPParticleCircleSpawnMOD,			new_circlespawn);
	NEXT_MOD_CASE(NPParticleEllipseSpawnMod,		new_ellipsespawn);
	NEXT_MOD_CASE(NPParticleAttractVelocityMOD,		new_attractvelocity);
	NEXT_MOD_CASE(NPParticleSeedVelocityMOD,		new_seedvelocity);
	NEXT_MOD_CASE(NPParticleHorizontalStopMOD,		new_horizontalstop);
	NEXT_MOD_CASE(NPParticleCurvedColorMOD,			new_curvedcolor);
	NEXT_MOD_CASE(NPParticleCurvedAlphaMOD,			new_curvedalpha);
	NEXT_MOD_CASE(NPParticleFixedColorMOD,			new_fixedcolor);
	NEXT_MOD_CASE(NPParticleOverLifeColorMOD,		new_overlifecolor);
	NEXT_MOD_CASE(NPParticleSeedColorMOD,			new_seedcolor);
#undef  INIT_MOD_CASE
#undef  NEXT_MOD_CASE

	m_spritemodvec.push_back(propdata);

	return propdata;
}
xui_method_explain(onity_propdata_particlemod,	new_seedlifetime,			xui_propdata*			)( NPParticleSeedLifetimeMOD*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseLifetime",
		xui_propctrl_number::create,
		&mod->mMinBaseLifetime,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseLifetime",
		xui_propctrl_number::create,
		&mod->mMaxBaseLifetime,
		NT_FLOAT));

	return new xui_propdata_expand(m_kind, L"SeedLifetime", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_attractaccel,			xui_propdata*			)( NPParticleAttractAccelMOD*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"AttractLocationX",
		xui_propctrl_number::create,
		&mod->mAttractLocationX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"AttractLocationY",
		xui_propctrl_number::create,
		&mod->mAttractLocationY,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"AttractPower",
		xui_propctrl_number::create,
		&mod->mAttractPower,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"AttractKillZone",
		xui_propctrl_number::create,
		&mod->mAttractKillZone,
		NT_INT));

	return new xui_propdata_expand(m_kind, L"AttractAccel", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_dampingaccel,			xui_propdata*			)( NPParticleDampingAccelMOD*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"DampingSize",
		xui_propctrl_number::create,
		&mod->mDampingSize,
		NT_FLOAT));

	return new xui_propdata_expand(m_kind, L"DampingAccel", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_seedaccel,				xui_propdata*			)( NPParticleSeedAccelMOD*				mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseAccelX",
		xui_propctrl_number::create,
		&mod->mMinBaseAccelX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseAccelY",
		xui_propctrl_number::create,
		&mod->mMinBaseAccelY,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseAccelX",
		xui_propctrl_number::create,
		&mod->mMaxBaseAccelX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseAccelY",
		xui_propctrl_number::create,
		&mod->mMaxBaseAccelY,
		NT_INT));

	return new xui_propdata_expand(m_kind, L"SeedAccel", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_sineaccel,				xui_propdata*			)( NPParticleSineAccelMOD*				mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"BaseAccelX",
		xui_propctrl_number::create,
		&mod->mBaseAccelX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"BaseAccelY",
		xui_propctrl_number::create,
		&mod->mBaseAccelY,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"Cycle",
		xui_propctrl_number::create,
		&mod->mCycle,
		NT_FLOAT));

	return new xui_propdata_expand(m_kind, L"SineAccel", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_seedlocation,			xui_propdata*			)( NPParticleSeedLocationMOD*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseLocationX",
		xui_propctrl_number::create,
		&mod->mMinBaseLocationX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseLocationY",
		xui_propctrl_number::create,
		&mod->mMinBaseLocationY,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseLocationX",
		xui_propctrl_number::create,
		&mod->mMaxBaseLocationX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseLocationY",
		xui_propctrl_number::create,
		&mod->mMaxBaseLocationY,
		NT_INT));

	return new xui_propdata_expand(m_kind, L"SeedLocation", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_orbitlocation,			xui_propdata*			)( NPParticleOrbitLocationMOD*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"OrbitX",
		xui_propctrl_number::create,
		&mod->mOrbitX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"OrbitY",
		xui_propctrl_number::create,
		&mod->mOrbitY,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"OrbitRate",
		xui_propctrl_number::create,
		&mod->mOrbitRate,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"OrbitOffset",
		xui_propctrl_number::create,
		&mod->mOrbitOffset,
		NT_FLOAT));

	return new xui_propdata_expand(m_kind, L"OrbitLocation", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_seedrotation,			xui_propdata*			)( NPParticleSeedRotation2DMOD*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinInitSpin",
		xui_propctrl_number::create,
		&mod->mMinInitSpin,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxInitSpin",
		xui_propctrl_number::create,
		&mod->mMaxInitSpin,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinSpinRate",
		xui_propctrl_number::create,
		&mod->mMinSpinRate,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxSpinRate",
		xui_propctrl_number::create,
		&mod->mMaxSpinRate,
		NT_FLOAT));

	return new xui_propdata_expand(m_kind, L"SeedRotation", xui_propctrl_expand_plus::create, subprop);
}
std::vector<NPCurveControlPoint>& get_sizedata( void* userptr )
{
	NPParticleCurvedUniformSizeMOD* mod = (NPParticleCurvedUniformSizeMOD*)userptr;
	return mod->mCurveChannelSize.mControlPoints;
}
xui_propdata* new_sizeprop( void* userptr, u32 index, xui_propkind* kind )
{
	NPParticleCurvedUniformSizeMOD* mod = (NPParticleCurvedUniformSizeMOD*)userptr;
	return new onity_propdata_curve(kind, L"", &(mod->mCurveChannelSize.mControlPoints[index]));
}
xui_method_explain(onity_propdata_particlemod,	new_curveduniformsize,		xui_propdata*			)( NPParticleCurvedUniformSizeMOD*		mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_stdvec_func<NPCurveControlPoint>(
		m_kind,
		L"Size",
		xui_propctrl_stdvec::create,
		NULL,
		NULL,
		new_sizeprop,
		get_sizedata,
		mod));

	return new xui_propdata_expand(m_kind, L"CurvedUniformSize", xui_propctrl_expand_plus::create, subprop);
}
std::vector<NPCurveControlPoint>& get_sizexdata( void* userptr )
{
	NPParticleCurvedSizeMOD* mod = (NPParticleCurvedSizeMOD*)userptr;
	return mod->mCurveChannelX.mControlPoints;
}
std::vector<NPCurveControlPoint>& get_sizeydata( void* userptr )
{
	NPParticleCurvedSizeMOD* mod = (NPParticleCurvedSizeMOD*)userptr;
	return mod->mCurveChannelY.mControlPoints;
}
xui_propdata* new_sizexprop( void* userptr, u32 index, xui_propkind* kind )
{
	NPParticleCurvedSizeMOD* mod = (NPParticleCurvedSizeMOD*)userptr;
	return new onity_propdata_curve(kind, L"", &(mod->mCurveChannelX.mControlPoints[index]));
}
xui_propdata* new_sizeyprop( void* userptr, u32 index, xui_propkind* kind )
{
	NPParticleCurvedSizeMOD* mod = (NPParticleCurvedSizeMOD*)userptr;
	return new onity_propdata_curve(kind, L"", &(mod->mCurveChannelY.mControlPoints[index]));
}
xui_method_explain(onity_propdata_particlemod,	new_curvedsize,				xui_propdata*			)( NPParticleCurvedSizeMOD*				mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_stdvec_func<NPCurveControlPoint>(
		m_kind,
		L"SizeX",
		xui_propctrl_stdvec::create,
		NULL,
		NULL,
		new_sizexprop,
		get_sizexdata,
		mod));
	subprop.push_back(new xui_propdata_stdvec_func<NPCurveControlPoint>(
		m_kind,
		L"SizeY",
		xui_propctrl_stdvec::create,
		NULL,
		NULL,
		new_sizeyprop,
		get_sizeydata,
		mod));

	return new xui_propdata_expand(m_kind, L"CurvedSize", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_uniformoverlifesize,	xui_propdata*			)( NPParticleUniformOverLifeSizeMOD*	mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseSize",
		xui_propctrl_number::create,
		&mod->mMinBaseSize,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseSize",
		xui_propctrl_number::create,
		&mod->mMaxBaseSize,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"FadeBeginTime",
		xui_propctrl_number::create,
		&mod->mFadeBeginTime,
		NT_FLOAT));

	return new xui_propdata_expand(m_kind, L"UniformOverLifeSize", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_overlifesize,			xui_propdata*			)( NPParticleOverLifeSizeMOD*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseSizeX",
		xui_propctrl_number::create,
		&mod->mMinBaseSizeX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseSizeY",
		xui_propctrl_number::create,
		&mod->mMinBaseSizeY,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseSizeX",
		xui_propctrl_number::create,
		&mod->mMaxBaseSizeX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseSizeY",
		xui_propctrl_number::create,
		&mod->mMaxBaseSizeY,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"FadeBeginTime",
		xui_propctrl_number::create,
		&mod->mFadeBeginTime,
		NT_FLOAT));

	return new xui_propdata_expand(m_kind, L"OverLifeSize", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_seedsize,				xui_propdata*			)( NPParticleSeedSizeMOD*				mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseSizeX",
		xui_propctrl_number::create,
		&mod->mMinBaseSizeX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseSizeY",
		xui_propctrl_number::create,
		&mod->mMinBaseSizeY,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseSizeX",
		xui_propctrl_number::create,
		&mod->mMaxBaseSizeX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseSizeY",
		xui_propctrl_number::create,
		&mod->mMaxBaseSizeY,
		NT_INT));

	return new xui_propdata_expand(m_kind, L"SeedSize", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_uniformseedsize,		xui_propdata*			)( NPParticleUniformSeedSizeMOD*		mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseSize",
		xui_propctrl_number::create,
		&mod->mMinBaseSize,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseSize",
		xui_propctrl_number::create,
		&mod->mMaxBaseSize,
		NT_INT));

	return new xui_propdata_expand(m_kind, L"UniformSeedSize", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_flipbookuv,				xui_propdata*			)( NPParticleFlipbookUVMOD*				mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_bool(
		m_kind,
		L"Loop",
		xui_propctrl_bool::create,
		NULL, 
		NULL,
		&mod->mLoop));
	subprop.push_back(new xui_propdata_number_impl<u32>(
		m_kind,
		L"StartTile",
		xui_propctrl_number::create,
		&mod->mStartTile,
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"FrameRate",
		xui_propctrl_number::create,
		&mod->mFramerate,
		NT_UNSIGNEDINT));

	return new xui_propdata_expand(m_kind, L"FlipbookUV", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_scrollanim,				xui_propdata*			)( NPParticleScrollAnimMOD*				mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_bool(
		m_kind,
		L"Loop",
		xui_propctrl_bool::create,
		NULL, 
		NULL,
		&mod->mLoop));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"SpeedU",
		xui_propctrl_number::create,
		&mod->mSpeedU,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"SpeedV",
		xui_propctrl_number::create,
		&mod->mSpeedV,
		NT_FLOAT));

	return new xui_propdata_expand(m_kind, L"ScrollAnim", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_tilesubtex,				xui_propdata*			)( NPParticleTileSubTexMOD*				mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<s32>(
		m_kind,
		L"TileIndex",
		xui_propctrl_number::create,
		&mod->mTileIndex,
		NT_INT));

	return new xui_propdata_expand(m_kind, L"TileSubTex", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_circlespawn,			xui_propdata*			)( NPParticleCircleSpawnMOD*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"AngleDelta",
		xui_propctrl_number::create,
		&mod->mAngleDelta,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"CircleRadius",
		xui_propctrl_number::create,
		&mod->mCircleRadius,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinRadialAccel",
		xui_propctrl_number::create,
		&mod->mMinRadialAccel,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxRadialAccel",
		xui_propctrl_number::create,
		&mod->mMaxRadialAccel,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinTangentAccel",
		xui_propctrl_number::create,
		&mod->mMinTangentAccel,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxTangentAccel",
		xui_propctrl_number::create,
		&mod->mMaxTangentAccel,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinRadialVelocity",
		xui_propctrl_number::create,
		&mod->mMinRadialVelocity,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxRadialVelocity",
		xui_propctrl_number::create,
		&mod->mMaxRadialVelocity,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinArcAngle",
		xui_propctrl_number::create,
		&mod->mMinArcAngle,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxArcAngle",
		xui_propctrl_number::create,
		&mod->mMaxArcAngle,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"DistributrDelta",
		xui_propctrl_number::create,
		&mod->mDistributeDelta,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"InitialAngle",
		xui_propctrl_number::create,
		&mod->mInitialAngle,
		NT_INT));

	return new xui_propdata_expand(m_kind, L"CircleSpawn", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_ellipsespawn,			xui_propdata*			)( NPParticleEllipseSpawnMod*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"AngleDelta",
		xui_propctrl_number::create,
		&mod->mAngleDelta,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"EllipseAxisX",
		xui_propctrl_number::create,
		&mod->mEllipseAxisX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"EllipseAxisY",
		xui_propctrl_number::create,
		&mod->mEllipseAxisY,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinRadialAccel",
		xui_propctrl_number::create,
		&mod->mMinRadialAccel,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxRadialAccel",
		xui_propctrl_number::create,
		&mod->mMaxRadialAccel,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinTangentAccel",
		xui_propctrl_number::create,
		&mod->mMinTangentAccel,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxTangentAccel",
		xui_propctrl_number::create,
		&mod->mMaxTangentAccel,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinRadialVelocity",
		xui_propctrl_number::create,
		&mod->mMinRadialVelocity,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxRadialVelocity",
		xui_propctrl_number::create,
		&mod->mMaxRadialVelocity,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinArcAngle",
		xui_propctrl_number::create,
		&mod->mMinArcAngle,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxArcAngle",
		xui_propctrl_number::create,
		&mod->mMaxArcAngle,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"InitialAngle",
		xui_propctrl_number::create,
		&mod->mInitialAngle,
		NT_INT));

	return new xui_propdata_expand(m_kind, L"EllipseSpawn", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_attractvelocity,		xui_propdata*			)( NPParticleAttractVelocityMOD*		mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"AttractLocationX",
		xui_propctrl_number::create,
		&mod->mAttractLocationX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"AttractLocationY",
		xui_propctrl_number::create,
		&mod->mAttractLocationY,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"AttractPower",
		xui_propctrl_number::create,
		&mod->mAttractPower,
		NT_FLOAT));

	return new xui_propdata_expand(m_kind, L"AttractVelocity", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_seedvelocity,			xui_propdata*			)( NPParticleSeedVelocityMOD*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseVelocityX",
		xui_propctrl_number::create,
		&mod->mMinBaseVelocityX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseVelocityY",
		xui_propctrl_number::create,
		&mod->mMinBaseVelocityY,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseVelocityX",
		xui_propctrl_number::create,
		&mod->mMaxBaseVelocityX,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseVelocityY",
		xui_propctrl_number::create,
		&mod->mMaxBaseVelocityY,
		NT_INT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinMultiplier",
		xui_propctrl_number::create,
		&mod->mMinMultiplier,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxMultiplier",
		xui_propctrl_number::create,
		&mod->mMaxMultiplier,
		NT_FLOAT));

	return new xui_propdata_expand(m_kind, L"SeedVelocity", xui_propctrl_expand_plus::create, subprop);
}
xui_method_explain(onity_propdata_particlemod,	new_horizontalstop,			xui_propdata*			)( NPParticleHorizontalStopMOD*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinStopThreshold",
		xui_propctrl_number::create,
		&mod->mMinStopThreshold,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxStopThreshold",
		xui_propctrl_number::create,
		&mod->mMaxStopThreshold,
		NT_FLOAT));

	return new xui_propdata_expand(m_kind, L"HorizontalStop", xui_propctrl_expand_plus::create, subprop);
}
std::vector<NPCurveControlPoint>& get_coloradata( void* userptr )
{
	NPParticleCurvedColorMOD* mod = (NPParticleCurvedColorMOD*)userptr;
	return mod->mCurveChannelA.mControlPoints;
}
std::vector<NPCurveControlPoint>& get_colorrdata( void* userptr )
{
	NPParticleCurvedColorMOD* mod = (NPParticleCurvedColorMOD*)userptr;
	return mod->mCurveChannelR.mControlPoints;
}
std::vector<NPCurveControlPoint>& get_colorgdata( void* userptr )
{
	NPParticleCurvedColorMOD* mod = (NPParticleCurvedColorMOD*)userptr;
	return mod->mCurveChannelG.mControlPoints;
}
std::vector<NPCurveControlPoint>& get_colorbdata( void* userptr )
{
	NPParticleCurvedColorMOD* mod = (NPParticleCurvedColorMOD*)userptr;
	return mod->mCurveChannelB.mControlPoints;
}
xui_propdata* new_coloraprop( void* userptr, u32 index, xui_propkind* kind )
{
	NPParticleCurvedColorMOD* mod = (NPParticleCurvedColorMOD*)userptr;
	return new onity_propdata_curve(kind, L"", &(mod->mCurveChannelA.mControlPoints[index]));
}
xui_propdata* new_colorrprop( void* userptr, u32 index, xui_propkind* kind )
{
	NPParticleCurvedColorMOD* mod = (NPParticleCurvedColorMOD*)userptr;
	return new onity_propdata_curve(kind, L"", &(mod->mCurveChannelR.mControlPoints[index]));
}
xui_propdata* new_colorgprop( void* userptr, u32 index, xui_propkind* kind )
{
	NPParticleCurvedColorMOD* mod = (NPParticleCurvedColorMOD*)userptr;
	return new onity_propdata_curve(kind, L"", &(mod->mCurveChannelG.mControlPoints[index]));
}
xui_propdata* new_colorbprop( void* userptr, u32 index, xui_propkind* kind )
{
	NPParticleCurvedColorMOD* mod = (NPParticleCurvedColorMOD*)userptr;
	return new onity_propdata_curve(kind, L"", &(mod->mCurveChannelB.mControlPoints[index]));
}
xui_method_explain(onity_propdata_particlemod,	new_curvedcolor,			xui_propdata*			)( NPParticleCurvedColorMOD*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_stdvec_func<NPCurveControlPoint>(
		m_kind,
		L"Alpha",
		xui_propctrl_stdvec::create,
		NULL,
		NULL,
		new_coloraprop,
		get_coloradata,
		mod));
	subprop.push_back(new xui_propdata_stdvec_func<NPCurveControlPoint>(
		m_kind,
		L"Red",
		xui_propctrl_stdvec::create,
		NULL,
		NULL,
		new_colorrprop,
		get_colorrdata,
		mod));
	subprop.push_back(new xui_propdata_stdvec_func<NPCurveControlPoint>(
		m_kind,
		L"Green",
		xui_propctrl_stdvec::create,
		NULL,
		NULL,
		new_colorgprop,
		get_colorgdata,
		mod));
	subprop.push_back(new xui_propdata_stdvec_func<NPCurveControlPoint>(
		m_kind,
		L"Blue",
		xui_propctrl_stdvec::create,
		NULL,
		NULL,
		new_colorbprop,
		get_colorbdata,
		mod));

	return new xui_propdata_expand(m_kind, L"CurvedColor", xui_propctrl_expand_plus::create, subprop);
}
std::vector<NPCurveControlPoint>& get_alphadata( void* userptr )
{
	NPParticleCurvedAlphaMOD* mod = (NPParticleCurvedAlphaMOD*)userptr;
	return mod->mCurveChannelA.mControlPoints;
}
xui_propdata* new_alphaprop( void* userptr, u32 index, xui_propkind* kind )
{
	NPParticleCurvedAlphaMOD* mod = (NPParticleCurvedAlphaMOD*)userptr;
	return new onity_propdata_curve(kind, L"", &(mod->mCurveChannelA.mControlPoints[index]));
}
xui_method_explain(onity_propdata_particlemod,	new_curvedalpha,			xui_propdata*			)( NPParticleCurvedAlphaMOD*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_stdvec_func<NPCurveControlPoint>(
		m_kind,
		L"Alpha",
		xui_propctrl_stdvec::create,
		NULL,
		NULL,
		new_alphaprop,
		get_alphadata,
		mod));

	return new xui_propdata_expand(m_kind, L"CurvedAlpha", xui_propctrl_expand_plus::create, subprop);
}

xui_colour	fixedcolor_getcolor( void* userptr )
{
	NPParticleFixedColorMOD* mod = (NPParticleFixedColorMOD*)userptr;
	return xui_colour(mod->mBaseColorA, mod->mBaseColorR, mod->mBaseColorG, mod->mBaseColorB);
}
void		fixedcolor_setcolor( void* userptr, const xui_colour& value )
{
	NPParticleFixedColorMOD* mod = (NPParticleFixedColorMOD*)userptr;
	mod->mBaseColorA = value.a;
	mod->mBaseColorR = value.r;
	mod->mBaseColorG = value.g;
	mod->mBaseColorB = value.b;
}
xui_method_explain(onity_propdata_particlemod,	new_fixedcolor,				xui_propdata*			)( NPParticleFixedColorMOD*				mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_colour(
		m_kind,
		L"BaseColor",
		xui_propctrl_colour::create,
		fixedcolor_getcolor,
		fixedcolor_setcolor,
		mod));

	return new xui_propdata_expand(m_kind, L"CurvedAlpha", xui_propctrl_expand_plus::create, subprop);
}

xui_colour	overlifecolor_getmincolor( void* userptr )
{
	NPParticleOverLifeColorMOD* mod = (NPParticleOverLifeColorMOD*)userptr;
	return xui_colour(mod->mMinBaseColorA, mod->mMinBaseColorR, mod->mMinBaseColorG, mod->mMinBaseColorB);
}
void		overlifecolor_setmincolor( void* userptr, const xui_colour& value )
{
	NPParticleOverLifeColorMOD* mod = (NPParticleOverLifeColorMOD*)userptr;
	mod->mMinBaseColorA = value.a;
	mod->mMinBaseColorR = value.r;
	mod->mMinBaseColorG = value.g;
	mod->mMinBaseColorB = value.b;
}
xui_colour	overlifecolor_getmaxcolor( void* userptr )
{
	NPParticleOverLifeColorMOD* mod = (NPParticleOverLifeColorMOD*)userptr;
	return xui_colour(mod->mMaxBaseColorA, mod->mMaxBaseColorR, mod->mMaxBaseColorG, mod->mMaxBaseColorB);
}
void		overlifecolor_setmaxcolor( void* userptr, const xui_colour& value )
{
	NPParticleOverLifeColorMOD* mod = (NPParticleOverLifeColorMOD*)userptr;
	mod->mMaxBaseColorA = value.a;
	mod->mMaxBaseColorR = value.r;
	mod->mMaxBaseColorG = value.g;
	mod->mMaxBaseColorB = value.b;
}
xui_method_explain(onity_propdata_particlemod,	new_overlifecolor,			xui_propdata*			)( NPParticleOverLifeColorMOD*			mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_colour(
		m_kind,
		L"MinBaseColor",
		xui_propctrl_colour::create,
		overlifecolor_getmincolor,
		overlifecolor_setmincolor,
		mod));
	subprop.push_back(new xui_propdata_colour(
		m_kind,
		L"MaxBaseColor",
		xui_propctrl_colour::create,
		overlifecolor_getmaxcolor,
		overlifecolor_setmaxcolor,
		mod));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"FadeBeginTime",
		xui_propctrl_number::create,
		&mod->mFadeBeginTime,
		NT_FLOAT));

	return new xui_propdata_expand(m_kind, L"OverLifeColor", xui_propctrl_expand_plus::create, subprop);
}

xui_colour	seedcolor_getmincolor( void* userptr )
{
	NPParticleSeedColorMOD* mod = (NPParticleSeedColorMOD*)userptr;
	return xui_colour(mod->mMinBaseColorA, mod->mMinBaseColorR, mod->mMinBaseColorG, mod->mMinBaseColorB);
}
void		seedcolor_setmincolor( void* userptr, const xui_colour& value )
{
	NPParticleSeedColorMOD* mod = (NPParticleSeedColorMOD*)userptr;
	mod->mMinBaseColorA = value.a;
	mod->mMinBaseColorR = value.r;
	mod->mMinBaseColorG = value.g;
	mod->mMinBaseColorB = value.b;
}
xui_colour	seedcolor_getmaxcolor( void* userptr )
{
	NPParticleSeedColorMOD* mod = (NPParticleSeedColorMOD*)userptr;
	return xui_colour(mod->mMaxBaseColorA, mod->mMaxBaseColorR, mod->mMaxBaseColorG, mod->mMaxBaseColorB);
}
void		seedcolor_setmaxcolor( void* userptr, const xui_colour& value )
{
	NPParticleSeedColorMOD* mod = (NPParticleSeedColorMOD*)userptr;
	mod->mMaxBaseColorA = value.a;
	mod->mMaxBaseColorR = value.r;
	mod->mMaxBaseColorG = value.g;
	mod->mMaxBaseColorB = value.b;
}
xui_method_explain(onity_propdata_particlemod,	new_seedcolor,				xui_propdata*			)( NPParticleSeedColorMOD*				mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_colour(
		m_kind,
		L"MinBaseColor",
		xui_propctrl_colour::create,
		seedcolor_getmincolor,
		seedcolor_setmincolor,
		mod));
	subprop.push_back(new xui_propdata_colour(
		m_kind,
		L"MaxBaseColor",
		xui_propctrl_colour::create,
		seedcolor_getmaxcolor,
		seedcolor_setmaxcolor,
		mod));

	return new xui_propdata_expand(m_kind, L"SeedColor", xui_propctrl_expand_plus::create, subprop);
}

//////////////////////////////////////////////////////////////////////////
//propctrl
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(onity_propctrl_curve, xui_propctrl);

/*
//constructor
*/
xui_create_explain(onity_propctrl_curve)( void )
{
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_namectrl, set_parent		)(this);
	xui_method_ptrcall(m_namectrl, set_textalign	)(TEXTALIGN_LC);

	m_timectrl = new xui_numbbox(xui_vector<s32>(32, 18), NT_FLOAT, 1, true);
	xui_method_ptrcall(m_timectrl, set_parent		)(this);
	xui_method_ptrcall(m_timectrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_timectrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_timectrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 4, 2));
	xui_method_ptrcall(m_timectrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_timectrl, set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_timectrl, xm_textchanged	) += new xui_method_member<xui_method_args, onity_propctrl_curve>(this, &onity_propctrl_curve::on_textctrltextchanged);

	m_enumctrl = new xui_dropbox(xui_vector<s32>(128, 18), NULL);
	xui_method_ptrcall(m_enumctrl, set_parent		)(this);
	xui_method_ptrcall(m_enumctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_enumctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_enumctrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 0, 2));
	xui_method_ptrcall(m_enumctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_enumctrl, set_corner		)(3);
	xui_method_ptrcall(m_enumctrl, set_readonly		)(true);
	xui_method_ptrcall(m_enumctrl, add_item			)(L"Linear");
	xui_method_ptrcall(m_enumctrl, add_item			)(L"Bezier");
	xui_method_ptrcall(m_enumctrl, add_item			)(L"Break" );
	xui_method_ptrcall(m_enumctrl, xm_selection		) += new xui_method_member<xui_method_args, onity_propctrl_curve>(this, &onity_propctrl_curve::on_enumctrlselection);

	m_numbctrl = new xui_numbbox(xui_vector<s32>(32, 18), NT_FLOAT, 1, true);
	xui_method_ptrcall(m_numbctrl, set_parent		)(this);
	xui_method_ptrcall(m_numbctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_numbctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_numbctrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 4, 2));
	xui_method_ptrcall(m_numbctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_numbctrl, set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_numbctrl, xm_textchanged	) += new xui_method_member<xui_method_args, onity_propctrl_curve>(this, &onity_propctrl_curve::on_textctrltextchanged);

	m_widgetvec.push_back(m_namectrl);
	m_widgetvec.push_back(m_timectrl);
	m_widgetvec.push_back(m_enumctrl);
	m_widgetvec.push_back(m_numbctrl);
}

/*
//static
*/
xui_method_explain(onity_propctrl_curve,		create,						xui_propctrl*			)( xui_propdata* propdata )
{
	return new onity_propctrl_curve;
}

/*
//override
*/
xui_method_explain(onity_propctrl_curve,		on_linkpropdata,			void					)( bool selfupdate )
{
	if (selfupdate == false)
	{
		m_namectrl->set_text(m_propdata->get_name());
		m_timectrl->ini_textbox(L"");
		m_enumctrl->ini_dropbox(-1);
		m_numbctrl->ini_textbox(L"");
	}

	onity_propdata_curve* datacurve = dynamic_cast<onity_propdata_curve*>(m_propdata);
	NPCurveControlPoint value = datacurve->get_value();
	m_enumctrl->ini_dropbox((u32)value.Interp);
	std::wstringstream time;
	time << value.KeyValue;
	m_timectrl->ini_textbox(time.str());
	std::wstringstream numb;
	numb << value.RetValue;
	m_numbctrl->ini_textbox(numb.str());
}
xui_method_explain(onity_propctrl_curve,		on_editvalue,				void					)( xui_propedit* sender )
{
	onity_propdata_curve* datacurve = dynamic_cast<onity_propdata_curve*>(m_propdata);
	NPCurveControlPoint value = datacurve->get_value();
	value.Interp = (ECurveInterpMode)m_enumctrl->get_selectedindex();
	std::wstringstream time(m_timectrl->get_text().c_str());
	time >> value.KeyValue;
	std::wstringstream numb(m_numbctrl->get_text().c_str());
	numb >> value.RetValue;
	datacurve->set_value(value);
}

/*
//callback
*/
xui_method_explain(onity_propctrl_curve,		on_perform,					void					)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);

	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;

	//enumctrl
	pt.x  = rt.get_w()/2;
	pt.y  = rt.get_h()/2 - m_enumctrl->get_renderh()/2;
	m_enumctrl->on_perform_pt(pt);

	//timectrl
	s32 textwidth = (rt.get_w()/2 - m_enumctrl->get_renderw() - 8) / 2;
	pt.x += m_enumctrl->get_renderw() + 4;
	pt.y  = rt.get_h()/2 - m_timectrl->get_renderh()/2;
	m_timectrl->on_perform_pt(pt);
	m_timectrl->on_perform_w (textwidth);
	//numbctrl
	pt.x += m_timectrl->get_renderw() + 4;
	pt.y  = rt.get_h()/2 - m_numbctrl->get_renderh()/2;
	m_numbctrl->on_perform_pt(pt);
	m_numbctrl->on_perform_w (rt.bx - pt.x);
	//namectrl
	s32 indent = get_indent();
	m_namectrl->on_perform_w (rt.get_w()/2);
	m_namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}

/*
//event
*/
xui_method_explain(onity_propctrl_curve,		on_enumctrlselection,		void					)( xui_component* sender, xui_method_args& args )
{
	on_editvalue(NULL);
}
xui_method_explain(onity_propctrl_curve,		on_textctrltextchanged,		void					)( xui_component* sender, xui_method_args& args )
{
	on_editvalue(NULL);
}

xui_implement_rtti(onity_propctrl_particlemod, xui_propctrl);

/*
//constructor
*/
xui_create_explain(onity_propctrl_particlemod)( xui_propdata* propdata )
: xui_propctrl()
{
	m_border = xui_rect2d<s32>(0, 8, 0, 8);
	m_header = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_header,	set_parent			)(this);
	xui_method_ptrcall(m_header,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_header,	set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_header,	ini_drawer			)(propdata->get_name());
	m_nontip = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_nontip,	set_parent			)(this);
	xui_method_ptrcall(m_nontip,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_nontip,	set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_nontip,	ini_component		)(true, false);
	xui_method_ptrcall(m_nontip,	ini_drawer			)(L"Empty");

	m_delete = new xui_button(xui_vector<s32>(24, 16));
	xui_method_ptrcall(m_delete,	set_parent			)(this);
	xui_method_ptrcall(m_delete,	set_backcolor		)(xui_colour::white);
	xui_method_ptrcall(m_delete,	set_movecolor		)(xui_button::default_downcolor);
	xui_method_ptrcall(m_delete,	xm_buttonclick		) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_deleteclick);
	xui_method_ptrcall(m_delete,	xm_renderself		) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_deleterenderself);

	m_middle = xui_listview::create(xui_vector<s32>(0), false);
	xui_method_ptrcall(m_middle,	set_parent			)(this);
	xui_method_ptrcall(m_middle,	set_drawcolor		)(false);
	xui_method_ptrcall(m_middle,	set_sidestyle		)(SIDESTYLE_N);
	xui_method_ptrcall(m_middle,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_middle,	xm_invalid			) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_middleinvalid);
	xui_method_ptrcall(m_middle,	xm_selectedchange	) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_middleselection);

	m_expand = new xui_propctrl_expand_plus;
	xui_method_ptrcall(m_expand,	set_parent			)(this);
	xui_method_ptrcall(m_expand,	ini_component		)(true, false);

	xui_menu*     menu		= xui_menu::create(80);
	xui_menuitem* item		= NULL;
	m_insert = new xui_toggle(xui_vector<s32>(24, 16), TOGGLE_BUTTON);
	xui_method_ptrcall(m_insert,	set_parent			)(this);
	xui_method_ptrcall(m_insert,	set_backcolor		)(xui_colour::white);
	xui_method_ptrcall(m_insert,	set_movecolor		)(xui_button::default_downcolor);
	xui_method_ptrcall(m_insert,	set_menu			)(menu);
	xui_method_ptrcall(m_insert,	xm_renderself		) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_insertrenderself);

	xui_menu* lifetime		= xui_menu::create(80);
	m_seedlifetime			= xui_method_ptrcall(lifetime,	add_item)(NULL, L"SeedLifeTime");
	xui_method_ptrcall(m_seedlifetime,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	item = menu->add_item(NULL, L"LifeTime");
	item->set_submenu(lifetime);

	xui_menu* accel			= xui_menu::create(80);
	m_attractaccel			= xui_method_ptrcall(accel,		add_item)(NULL, L"AttractAccel");
	m_dampingaccel			= xui_method_ptrcall(accel,		add_item)(NULL, L"DampingAccel");
	m_seedaccel				= xui_method_ptrcall(accel,		add_item)(NULL, L"SeedAccel");
	m_sineaccel				= xui_method_ptrcall(accel,		add_item)(NULL, L"SineAccel");
	xui_method_ptrcall(m_attractaccel,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_dampingaccel,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_seedaccel,				xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_sineaccel,				xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	item = menu->add_item(NULL, L"Accel");
	item->set_submenu(accel);

	xui_menu* location		= xui_menu::create(80);
	m_seedlocation			= xui_method_ptrcall(location,	add_item)(NULL, L"SeedLocation");
	m_orbitlocation			= xui_method_ptrcall(location,	add_item)(NULL, L"OrbitLocation");
	xui_method_ptrcall(m_seedlocation,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_orbitlocation,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	item = menu->add_item(NULL, L"Location");
	item->set_submenu(location);

	xui_menu* rotation		= xui_menu::create(80);
	m_seedrotation			= xui_method_ptrcall(rotation,	add_item)(NULL, L"SeedRotation");
	xui_method_ptrcall(m_seedrotation,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	item = menu->add_item(NULL, L"Rotation");
	item->set_submenu(rotation);

	xui_menu* size			= xui_menu::create(80);
	m_curveduniformsize		= xui_method_ptrcall(size,		add_item)(NULL, L"CurvedUniformSize");
	m_curvedsize			= xui_method_ptrcall(size,		add_item)(NULL, L"CurvedSize");
	m_uniformoverlifesize	= xui_method_ptrcall(size,		add_item)(NULL, L"UniformOverLifeSize");
	m_overlifesize			= xui_method_ptrcall(size,		add_item)(NULL, L"OverLifeSize");
	m_seedsize				= xui_method_ptrcall(size,		add_item)(NULL, L"SeedSize");
	m_uniformseedsize		= xui_method_ptrcall(size,		add_item)(NULL, L"UniformSeedSize");
	xui_method_ptrcall(m_curveduniformsize,		xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_curvedsize,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_uniformoverlifesize,	xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_overlifesize,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_seedsize,				xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_uniformseedsize,		xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	item = menu->add_item(NULL, L"Size");
	item->set_submenu(size);

	xui_menu* texcoord		= xui_menu::create(80);
	m_flipbookuv			= xui_method_ptrcall(texcoord,	add_item)(NULL, L"FlipBookUV");
	m_scrollanim			= xui_method_ptrcall(texcoord,	add_item)(NULL, L"ScrollAnim");
	m_tilesubtex			= xui_method_ptrcall(texcoord,	add_item)(NULL, L"TileSubTex");
	xui_method_ptrcall(m_flipbookuv,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_scrollanim,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_tilesubtex,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	item = menu->add_item(NULL, L"TexCoord");
	item->set_submenu(texcoord);

	xui_menu* uber			= xui_menu::create(80);
	m_circlespawn			= xui_method_ptrcall(uber,		add_item)(NULL, L"CircleSpawn");
	m_ellipsespawn			= xui_method_ptrcall(uber,		add_item)(NULL, L"EllipseSpawn");
	xui_method_ptrcall(m_circlespawn,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_ellipsespawn,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	item = menu->add_item(NULL, L"Uber");
	item->set_submenu(uber);

	xui_menu* velocity		= xui_menu::create(80);
	m_attractvelocity		= xui_method_ptrcall(velocity,	add_item)(NULL, L"AttractVelocity");
	m_seedvelocity			= xui_method_ptrcall(velocity,	add_item)(NULL, L"SeedVelocity");
	m_horizontalstop		= xui_method_ptrcall(velocity,	add_item)(NULL, L"HorizontalStop");
	xui_method_ptrcall(m_attractvelocity,		xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_seedvelocity,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_horizontalstop,		xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	item = menu->add_item(NULL, L"Velocity");
	item->set_submenu(velocity);

	xui_menu* color			= xui_menu::create(80);
	m_curvedcolor			= xui_method_ptrcall(color,		add_item)(NULL, L"CurvedColor");
	m_curvedalpha			= xui_method_ptrcall(color,		add_item)(NULL, L"CurvedAlpha");
	m_fixedcolor			= xui_method_ptrcall(color,		add_item)(NULL, L"FixedColor");
	m_overlifecolor			= xui_method_ptrcall(color,		add_item)(NULL, L"OverLifeColor");
	m_seedcolor				= xui_method_ptrcall(color,		add_item)(NULL, L"SeedColor");
	xui_method_ptrcall(m_curvedcolor,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_curvedalpha,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_fixedcolor,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_overlifecolor,			xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);
	xui_method_ptrcall(m_seedcolor,				xm_click) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_menuclick);

	m_widgetvec.push_back(m_header);
	m_widgetvec.push_back(m_insert);
	m_widgetvec.push_back(m_delete);
	m_widgetvec.push_back(m_middle);
	m_widgetvec.push_back(m_nontip);
	m_widgetvec.push_back(m_expand);
}

/*
//static
*/
xui_method_explain(onity_propctrl_particlemod,	create,						xui_propctrl*			)( xui_propdata* propdata )
{
	return new onity_propctrl_particlemod(propdata);
}

/*
//override
*/
xui_method_explain(onity_propctrl_particlemod,	on_linkpropdata,			void					)( bool selfupdate )
{
	onity_propdata_particlemod*  dataspritemod = dynamic_cast<onity_propdata_particlemod*>(m_propdata);
	const xui_propdata_vec& datavec = dataspritemod->get_modifydatavec();

	if (selfupdate == false || m_middle->get_itemcount() != datavec.size())
	{
		xui_method_ptrcall(m_middle, del_itemall)();
		for (u32 i = 0; i < datavec.size(); ++i)
		{
			xui_propdata*  data = datavec[i];
			if (data)
			{
				xui_listitem * item = m_middle->add_item(data->get_name());
				item->set_data(data);
			}
		}
		xui_method_ptrcall(m_expand, ini_component	)(true, false);
		xui_method_ptrcall(m_nontip, ini_component	)(true, datavec.size() == 0);
		xui_method_ptrcall(m_middle, refresh		)();
	}
}
xui_method_explain(onity_propctrl_particlemod,	on_editvalue,				void					)( xui_propedit* sender )
{}

/*
//callback
*/
xui_method_explain(onity_propctrl_particlemod,	on_renderback,				void					)( xui_method_args& args )
{
	xui_propctrl::on_renderback(args);

	xui_rect2d<s32> rt;
	xui_colour		color = get_vertexcolor();
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.4f) * color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 0, 3));
	rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 3, 3));

	color *= m_sidecolor;
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->draw_round(rt, color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax,		rt.by-3	), xui_vector<s32>(rt.ax,		rt.ay	), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx-48,	rt.by	), xui_vector<s32>(rt.ax+3,		rt.by   ), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx,		rt.ay	), xui_vector<s32>(rt.bx,		rt.by	), color);
	xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.ax,			 
		rt.by-6,
		rt.ax+6, 
		rt.by), color, 90, 90, 1);

	rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx,		rt.ay	), xui_vector<s32>(rt.bx,		rt.by-3	), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx-3,	rt.by	), xui_vector<s32>(rt.ax+3,		rt.by	), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax,		rt.by-3	), xui_vector<s32>(rt.ax,		rt.ay	), color);
	xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.ax,			 
		rt.by-6,
		rt.ax+6, 
		rt.by), color, 90, 90, 1);
	xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.bx-6, 
		rt.by-6,			
		rt.bx,		   
		rt.by), color,  0, 90, 1);
}
xui_method_explain(onity_propctrl_particlemod,	on_invalid,					void					)( xui_method_args& args )
{
	xui_vector<s32> sz;
	sz.w  = get_renderw();
	sz.h  = m_border.ay + m_border.by;
	sz.h += m_header->get_renderh();
	sz.h += m_middle->get_renderh();
	sz.h += m_insert->get_renderh();
	sz.h += m_expand->was_visible() ? m_expand->get_renderh() : 0;
	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(onity_propctrl_particlemod,	on_perform,					void					)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);

	s32 indent = get_indent();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt = rt.get_pt();
	//header
	m_header->on_perform_x (pt.x+indent);
	m_header->on_perform_y (pt.y);
	m_header->on_perform_w (rt.get_w()-indent);
	//middle
	pt.y += m_header->get_renderh();
	m_middle->on_perform_x (pt.x+indent);
	m_middle->on_perform_y (pt.y);
	m_middle->on_perform_w (rt.get_w()-indent);
	m_nontip->on_perform_x (pt.x+indent);
	m_nontip->on_perform_y (pt.y+m_middle->get_borderrt().ay);
	m_nontip->on_perform_w (rt.get_w()-indent);
	//delete
	pt.x  = rt.bx - m_delete->get_renderw();
	pt.y += m_middle->get_renderh();
	m_delete->on_perform_x (pt.x);
	m_delete->on_perform_y (pt.y);
	//insert
	pt.x -= m_insert->get_renderw();
	m_insert->on_perform_x (pt.x);
	m_insert->on_perform_y (pt.y);
	//expand
	pt.x  = rt.ax;
	pt.y += m_insert->get_renderh();
	m_expand->on_perform_x (pt.x);
	m_expand->on_perform_y (pt.y);
	m_expand->on_perform_w (rt.get_w());
}

/*
//event
*/
xui_method_explain(onity_propctrl_particlemod,	on_middleinvalid,			void					)( xui_component* sender, xui_method_args& args )
{
	s32 h = (m_middle->get_itemcount() == 0) ? m_nontip->get_renderh() : (m_middle->get_itemcount() * m_middle->get_lineheight());
	h += m_middle->get_borderrt().ay;
	h += m_middle->get_borderrt().by;
	m_middle->set_renderh(h);
}
xui_method_explain(onity_propctrl_particlemod,	on_middleselection,			void					)( xui_component* sender, xui_method_args& args )
{
	xui_propdata_vec datavec = m_expand->get_propdata();
	for (u32 i = 0; i < datavec.size(); ++i)
		datavec[i]->non_ctrl();

	std::vector<xui_listitem*> itemvec = m_middle->get_selecteditem();
	if (itemvec.size() > 0)
	{
		xui_listitem* item = itemvec.front();
		xui_propdata* data = (xui_propdata*)item->get_data();
		m_expand->set_propdata(data);
		m_expand->refresh();

		data->set_ctrl(m_expand);
	}

	m_expand->set_visible(itemvec.size() > 0);
	refresh();
}
xui_method_explain(onity_propctrl_particlemod,	on_deleteclick,				void					)( xui_component* sender, xui_method_args& args )
{
	std::vector<xui_listitem*> itemvec = m_middle->get_selecteditem();
	if (itemvec.size() > 0)
	{
		xui_propdata_vec datavec = m_expand->get_propdata();
		for (u32 i = 0; i < datavec.size(); ++i)
			datavec[i]->non_ctrl();

		m_expand->set_visible(false);

		xui_listitem* item = itemvec.front();
		onity_propdata_particlemod* dataspritemod = dynamic_cast<onity_propdata_particlemod*>(m_propdata);
		dataspritemod->del_modify((xui_propdata*)item->get_data());

		m_middle->del_item(item);
	}
}
xui_method_explain(onity_propctrl_particlemod,	on_menuclick,				void					)( xui_component* sender, xui_method_args& args )
{
	NPParticleMOD* mod = NULL;

	if		(sender == m_seedlifetime)			mod = new NPParticleSeedLifetimeMOD;
	else if (sender == m_attractaccel)			mod = new NPParticleAttractAccelMOD;
	else if (sender == m_dampingaccel)			mod = new NPParticleDampingAccelMOD;
	else if (sender == m_seedaccel)				mod = new NPParticleSeedAccelMOD;
	else if (sender == m_sineaccel)				mod = new NPParticleSineAccelMOD;
	else if (sender == m_seedlocation)			mod = new NPParticleSeedLocationMOD;
	else if (sender == m_orbitlocation)			mod = new NPParticleOrbitLocationMOD;
	else if (sender == m_seedrotation)			mod = new NPParticleSeedRotation2DMOD;
	else if (sender == m_curveduniformsize)		mod = new NPParticleCurvedUniformSizeMOD;
	else if (sender == m_curvedsize)			mod = new NPParticleCurvedSizeMOD;
	else if (sender == m_uniformoverlifesize)	mod = new NPParticleUniformOverLifeSizeMOD;
	else if (sender == m_overlifesize)			mod = new NPParticleOverLifeSizeMOD;
	else if (sender == m_seedsize)				mod = new NPParticleSeedSizeMOD;
	else if (sender == m_uniformseedsize)		mod = new NPParticleUniformSeedSizeMOD;
	else if (sender == m_flipbookuv)			mod = new NPParticleFlipbookUVMOD;
	else if (sender == m_scrollanim)			mod = new NPParticleScrollAnimMOD;
	else if (sender == m_tilesubtex)			mod = new NPParticleTileSubTexMOD;
	else if (sender == m_circlespawn)			mod = new NPParticleCircleSpawnMOD;
	else if (sender == m_ellipsespawn)			mod = new NPParticleEllipseSpawnMod;
	else if (sender == m_attractvelocity)		mod = new NPParticleAttractVelocityMOD;
	else if (sender == m_seedvelocity)			mod = new NPParticleSeedVelocityMOD;
	else if (sender == m_horizontalstop)		mod = new NPParticleHorizontalStopMOD;
	else if (sender == m_curvedcolor)			mod = new NPParticleCurvedColorMOD;
	else if (sender == m_curvedalpha)			mod = new NPParticleCurvedAlphaMOD;
	else if (sender == m_fixedcolor)			mod = new NPParticleFixedColorMOD;
	else if (sender == m_overlifecolor)			mod = new NPParticleOverLifeColorMOD;
	else if (sender == m_seedcolor)				mod = new NPParticleSeedColorMOD;

	if (mod)
	{
		onity_propdata_particlemod* dataspritemod = dynamic_cast<onity_propdata_particlemod*>(m_propdata);
		xui_propdata* data = dataspritemod->add_modify(mod);
		xui_listitem* item = m_middle->add_item(data->get_name());
		item->set_data(data);
	}
}
xui_method_explain(onity_propctrl_particlemod,	on_deleterenderself,		void					)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);

	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
}
xui_method_explain(onity_propctrl_particlemod,	on_insertrenderself,		void					)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);

	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-1,
		center.y-4,
		center.x+1,
		center.y+4), color);
}