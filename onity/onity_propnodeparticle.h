#ifndef __onity_propnodeparticle_h__
#define __onity_propnodeparticle_h__

#include "onity_propnoderender.h"

class NPParticleSFX;
class onity_propnodeparticle : public onity_propnoderender
{
public:
	/*
	//constructor
	*/
	onity_propnodeparticle( NPParticleSFX* particle );

	/*
	//method
	*/
	NPParticleSFX*			get_particle	( void );

protected:
	/*
	//static
	*/
	static xui_vector<f64>	get_position	( void* userptr );
	static void				set_position	( void* userptr, const xui_vector<f64>& value );
	static xui_vector<f64>	get_scale		( void* userptr );
	static void				set_scale		( void* userptr, const xui_vector<f64>& value );
	static f64				get_rotation	( void* userptr );
	static void				set_rotation	( void* userptr, f64   value );
	static void*			get_asset		( void* userptr );
	static void				set_asset		( void* userptr, void* value );

	/*
	//member
	*/
	NPParticleSFX*			m_particle;
	xui_propkind*			m_transkind;
	xui_propkind*			m_assetkind;
};
#endif//__onity_propnodeparticle_h__