#ifndef __onity_propentity_h__
#define __onity_propentity_h__

#include "Entity.h"
#include "onity_proproot.h"

class onity_propentity : public onity_proproot
{
public:
	/*
	//constructor
	*/
	onity_propentity( Omiga::Entity* entity );

	/*
	//method
	*/
	Omiga::Entity*			get_entity		( void );

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
	xui_propkind*			m_transkind;
	xui_propkind*			m_graphkind;
	xui_propkind*			m_boundkind;
	xui_propkind*			m_box2dkind;
	xui_propkind*			m_logickind;
	xui_propkind*			m_inputkind;
	Omiga::Entity*			m_entity;
};

#endif//__onity_propentity_h__