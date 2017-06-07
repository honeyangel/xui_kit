#ifndef __onity_propentitynode_h__
#define __onity_propentitynode_h__

#include "Entity.h"
#include "onity_propeditnode.h"

class NPNode;
class NPRect;
class onity_propentitynode : public onity_propeditnode
{
public:
	/*
	//constructor
	*/
	onity_propentitynode( Omiga::Entity* entity );

	/*
	//method
	*/
	Omiga::Entity*			get_entity		( void );
	virtual NP2DSTransRef*	get_2dsref		( void );
	virtual xui_vector<f64>	get_scale		( void );
	virtual void			set_scale		( const xui_vector<f64>& value );

protected:
	/*
	//static
	*/
	static bool				was_collidable	( void* userptr );
	static xui_rect2d<f64>	get_collision	( void* userptr );
	static void*			get_phyactor	( void* userptr );
	static void				set_phyactor	( void* userptr, void* value );
	static f64				get_damping		( void* userptr );
	static void				set_damping		( void* userptr, f64   value );
	static f64				get_gravity		( void* userptr );
	static void				set_gravity		( void* userptr, f64   value );
	static xui_vector<f64>	get_velocity	( void* userptr );
	static void				set_velocity	( void* userptr, const xui_vector<f64>& value );

	/*
	//member
	*/
	xui_propkind*			m_graphkind;
	xui_propkind*			m_boundkind;
	xui_propkind*			m_box2dkind;
	xui_propkind*			m_logickind;
	xui_propkind*			m_inputkind;
	Omiga::Entity*			m_entity;
};

#endif//__onity_propentitynode_h__