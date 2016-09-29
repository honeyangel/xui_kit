#ifndef __onity_propctrl_particlemod_h__
#define __onity_propctrl_particlemod_h__

#include "xui_propctrl_expand.h"

class NPParticleSpriteSRC;
class NPParticleMOD;
class NPParticleSeedLifetimeMOD;
class NPParticleAttractAccelMOD;
class NPParticleDampingAccelMOD;
class NPParticleSeedAccelMOD;
class NPParticleSineAccelMOD;
class NPParticleSeedLocationMOD;
class NPParticleOrbitLocationMOD;
class NPParticleSeedRotation2DMOD;
class NPParticleCurvedUniformSizeMOD;
class NPParticleCurvedSizeMOD;
class NPParticleUniformOverLifeSizeMOD;
class NPParticleOverLifeSizeMOD;
class NPParticleSeedSizeMOD;
class NPParticleUniformSeedSizeMOD;
class NPParticleFlipbookUVMOD;
class NPParticleScrollAnimMOD;
class NPParticleTileSubTexMOD;
class NPParticleCircleSpawnMOD;
class NPParticleEllipseSpawnMod;
class NPParticleAttractVelocityMOD;
class NPParticleSeedVelocityMOD;
class NPParticleHorizontalStopMOD;
class NPParticleCurvedColorMOD;
class NPParticleCurvedAlphaMOD;
class NPParticleFixedColorMOD;
class NPParticleOverLifeColorMOD;
class NPParticleSeedColorMOD;

struct NPCurveControlPoint;
class onity_propdata_curve : public xui_propdata
{
public:
	/*
	//constructor
	*/
	onity_propdata_curve( xui_propkind* kind, const std::wstring& name, NPCurveControlPoint* ptr );

	/*
	//method
	*/
	NPCurveControlPoint		get_value				( void ) const;
	void					set_value				( const NPCurveControlPoint& value );

protected:
	/*
	//member
	*/
	NPCurveControlPoint*	m_ptr;
};
class onity_propctrl_curve : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_propctrl*	create					( xui_propdata* propdata );

	/*
	//constructor
	*/
	onity_propctrl_curve( void );

	/*
	//override
	*/
	virtual void			on_linkpropdata			( bool selfupdate = false );
	virtual void			on_editvalue			( xui_propedit* sender );

protected:
	/*
	//callback
	*/
	virtual void			on_perform				( xui_method_args& args );

	/*
	//event
	*/
	void					on_enumctrlselection	( xui_component* sender, xui_method_args& args );
	void					on_textctrltextchanged	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*				m_namectrl;
	xui_dropbox*			m_enumctrl;
	xui_numbbox*			m_timectrl;
	xui_numbbox*			m_numbctrl;
};

class onity_propdata_particlemod : public xui_propdata
{
public:
	/*
	//constructor
	*/
	onity_propdata_particlemod( xui_propkind* kind, const std::wstring& name, NPParticleSpriteSRC* spritesrc );

	/*
	//destructor
	*/
	virtual ~onity_propdata_particlemod( void );

	/*
	//mdthod
	*/
	const xui_propdata_vec&	get_modifydatavec		( void ) const;
	xui_propdata*			add_modify				( NPParticleMOD* mod );
	void					del_modify				( xui_propdata* propdata );

	/*
	//override
	*/
	virtual void			non_ctrl				( void );

protected:
	/*
	//method
	*/
	xui_propdata*			add_modifydata			( NPParticleMOD* mod );
	xui_propdata*			new_seedlifetime		( NPParticleSeedLifetimeMOD*		mod );
	xui_propdata*			new_attractaccel		( NPParticleAttractAccelMOD*		mod );
	xui_propdata*			new_dampingaccel		( NPParticleDampingAccelMOD*		mod );
	xui_propdata*			new_seedaccel			( NPParticleSeedAccelMOD*			mod );
	xui_propdata*			new_sineaccel			( NPParticleSineAccelMOD*			mod );
	xui_propdata*			new_seedlocation		( NPParticleSeedLocationMOD*		mod );
	xui_propdata*			new_orbitlocation		( NPParticleOrbitLocationMOD*		mod );
	xui_propdata*			new_seedrotation		( NPParticleSeedRotation2DMOD*		mod );
	xui_propdata*			new_curveduniformsize	( NPParticleCurvedUniformSizeMOD*	mod );
	xui_propdata*			new_curvedsize			( NPParticleCurvedSizeMOD*			mod );
	xui_propdata*			new_uniformoverlifesize	( NPParticleUniformOverLifeSizeMOD*	mod );
	xui_propdata*			new_overlifesize		( NPParticleOverLifeSizeMOD*		mod );
	xui_propdata*			new_seedsize			( NPParticleSeedSizeMOD*			mod );
	xui_propdata*			new_uniformseedsize		( NPParticleUniformSeedSizeMOD*		mod );
	xui_propdata*			new_flipbookuv			( NPParticleFlipbookUVMOD*			mod );
	xui_propdata*			new_scrollanim			( NPParticleScrollAnimMOD*			mod );
	xui_propdata*			new_tilesubtex			( NPParticleTileSubTexMOD*			mod );
	xui_propdata*			new_circlespawn			( NPParticleCircleSpawnMOD*			mod );
	xui_propdata*			new_ellipsespawn		( NPParticleEllipseSpawnMod*		mod );
	xui_propdata*			new_attractvelocity		( NPParticleAttractVelocityMOD*		mod );
	xui_propdata*			new_seedvelocity		( NPParticleSeedVelocityMOD*		mod );
	xui_propdata*			new_horizontalstop		( NPParticleHorizontalStopMOD*		mod );
	xui_propdata*			new_curvedcolor			( NPParticleCurvedColorMOD*			mod );
	xui_propdata*			new_curvedalpha			( NPParticleCurvedAlphaMOD*			mod );
	xui_propdata*			new_fixedcolor			( NPParticleFixedColorMOD*			mod );
	xui_propdata*			new_overlifecolor		( NPParticleOverLifeColorMOD*		mod );
	xui_propdata*			new_seedcolor			( NPParticleSeedColorMOD*			mod );

	/*
	//member
	*/
	NPParticleSpriteSRC*	m_spritesrc;
	xui_propdata_vec		m_spritemodvec;
};

class onity_propctrl_particlemod : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_propctrl*	create					( xui_propdata* propdata );

	/*
	//constructor
	*/
	onity_propctrl_particlemod( xui_propdata* propdata );

	/*
	//override
	*/
	virtual void			on_linkpropdata			( bool selfupdate = false );
	virtual void			on_editvalue			( xui_propedit* sender );

protected:
	/*
	//callback
	*/
	virtual void			on_renderback			( xui_method_args& args );
	virtual void			on_invalid				( xui_method_args& args );
	virtual void			on_perform				( xui_method_args& args );

	/*
	//event
	*/
	void					on_middleinvalid		( xui_component* sender, xui_method_args& args );
	void					on_middleselection		( xui_component* sender, xui_method_args& args );
	void					on_deleteclick			( xui_component* sender, xui_method_args& args );
	void					on_deleterenderself		( xui_component* sender, xui_method_args& args );
	void					on_insertrenderself		( xui_component* sender, xui_method_args& args );
	void					on_menuclick			( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*				m_header;
	xui_listview*			m_middle;
	xui_drawer*				m_nontip;
	xui_toggle*				m_insert;
	xui_button*				m_delete;
	xui_propctrl_expand*	m_expand;

	xui_menuitem*			m_seedlifetime;
	xui_menuitem*			m_attractaccel;
	xui_menuitem*			m_dampingaccel;
	xui_menuitem*			m_seedaccel;
	xui_menuitem*			m_sineaccel;
	xui_menuitem*			m_seedlocation;
	xui_menuitem*			m_orbitlocation;
	xui_menuitem*			m_seedrotation;
	xui_menuitem*			m_curveduniformsize;
	xui_menuitem*			m_curvedsize;
	xui_menuitem*			m_uniformoverlifesize;
	xui_menuitem*			m_overlifesize;
	xui_menuitem*			m_seedsize;
	xui_menuitem*			m_uniformseedsize;
	xui_menuitem*			m_flipbookuv;
	xui_menuitem*			m_scrollanim;
	xui_menuitem*			m_tilesubtex;
	xui_menuitem*			m_circlespawn;
	xui_menuitem*			m_ellipsespawn;
	xui_menuitem*			m_attractvelocity;
	xui_menuitem*			m_seedvelocity;
	xui_menuitem*			m_horizontalstop;
	xui_menuitem*			m_curvedcolor;
	xui_menuitem*			m_curvedalpha;
	xui_menuitem*			m_fixedcolor;
	xui_menuitem*			m_overlifecolor;
	xui_menuitem*			m_seedcolor;
};

#endif//__onity_propctrl_particlemod_h__