#ifndef __onity_propctrl_particlemod_h__
#define __onity_propctrl_particlemod_h__

#include "xui_propctrl_expand.h"

class NPParticleSpriteSRC;
class NPParticleMOD;
class NPParticleSeedLifetimeMOD;
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
	xui_propdata_vec		get_modifydatavec		( void );
	void					add_modify				( void );
	void					del_modify				( void );

	/*
	//override
	*/
	virtual void			non_ctrl				( void );

protected:
	/*
	//typedef
	*/
	typedef std::map<xui_propdata*, NPParticleMOD*> 
		modify_propdata_map;

	/*
	//method
	*/
	void					add_modifydata			( NPParticleMOD* mod );
	xui_propdata*			new_seedlifetime		( NPParticleSeedLifetimeMOD* mod );

	/*
	//member
	*/
	NPParticleSpriteSRC*	m_spritesrc;
	modify_propdata_map		m_spritemodmap;
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
	virtual void			on_linkpropdata			( void );
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

	/*
	//member
	*/
	xui_drawer*				m_header;
	xui_listview*			m_middle;
	xui_drawer*				m_nontip;
	xui_toggle*				m_insert;
	xui_button*				m_delete;
	xui_propctrl_expand*	m_expand;
};

#endif//__onity_propctrl_particlemod_h__