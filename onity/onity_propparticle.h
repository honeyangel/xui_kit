#ifndef __onity_propparticle_h__
#define __onity_propparticle_h__

#include "onity_propfile.h"

class NPParticleSFX;
class onity_propparticle : public onity_propfile
{
public:
	/*
	//constructor
	*/
	onity_propparticle( const std::wstring& full );

	/*
	//destructor
	*/
	virtual ~onity_propparticle( void );

	/*
	//method
	*/
	NPParticleSFX*			get_particle		( void );
	void					add_spritesrc		( void );
	void					del_spritesrc		( xui_propkind* propkind );
	void					set_spritesrcindex	( xui_propkind* propkind, u32 oldindex, u32 newindex );

	/*
	//override
	*/
	virtual std::string		get_dragtype		( void );
	virtual void*			get_dragdata		( void );
	virtual bool			was_modify			( void );
	virtual void			load				( void );
	virtual void			save				( void );

protected:
	/*
	//event
	*/
	void					on_propchanged		( xui_component* sender, xui_method_propdata&	args );
	void					on_flagchanged		( xui_component* sender, xui_method_args&		args );

	/*
	//member
	*/
	xui_propkind*			m_menukind;
	bool					m_modify;
	NPParticleSFX*			m_particle;
};

#endif//__onity_propparticle_h__