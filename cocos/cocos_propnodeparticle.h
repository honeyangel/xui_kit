#ifndef __cocos_propnodeparticle_h__
#define __cocos_propnodeparticle_h__

#include "cocos_propnodebase.h"
#include "cocos_propdata_blend.h"

namespace cocos2d
{
	class ParticleSystem;
}

class cocos_propfile;
class cocos_propnodeparticle : public cocos_propnodebase
{
public:
	/*
	//constructor
	*/
	cocos_propnodeparticle( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ParticleSystem*	get_particle	( void );
	cocos_propfile*				get_particlefile( void );
	void						set_particlefile( cocos_propfile* file );

	void						play			( void );
	void						stop			( void );
	void						reload			( void );

protected:
	/*
	//method
	*/
	void						add_particle	( void );

	/*
	//static
	*/
	static void*				get_particle	( void* userptr );
	static void					set_particle	( void* userptr, void* value );
	static cocos_blend_value	get_blendfunc	( void* userptr );
	static void					set_blendfunc	( void* userptr, const cocos_blend_value& value );

	/*
	//member
	*/
	cocos2d::ParticleSystem*	m_particle;
	cocos_propfile*				m_particlefile;
};

#endif//__cocos_propnodeparticle_h__