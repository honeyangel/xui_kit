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
	NPParticleSFX*			get_particle	( void );

	/*
	//override
	*/
	virtual std::string		get_dragtype	( void );
	virtual void*			get_dragdata	( void );

protected:
	/*
	//member
	*/
	NPParticleSFX*			m_particle;
};

#endif//__onity_propparticle_h__