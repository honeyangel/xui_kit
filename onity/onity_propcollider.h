#ifndef __onity_propcollider_h__
#define __onity_propcollider_h__

#include "onity_propasset.h"

class NP2DSCollider;
class onity_propcollider : public onity_propasset
{
public:
	/*
	//constructor
	*/
	onity_propcollider( onity_propfile* propfile, NP2DSCollider* collider );

	/*
	//method
	*/
	NP2DSCollider*			get_collider	( void );

protected:
	/*
	//event
	*/
	void					on_namechanged	( xui_component* sender, xui_method_args& args );

	/*
	//static
	*/
	static f64				get_type		( void* userptr );
	static void				set_type		( void* userptr, f64 value );
	static std::wstring		get_user		( void* userptr );
	static void				set_user		( void* userptr, const std::wstring&    value );
	static xui_rect2d<f64>	get_rect		( void* userptr );
	static void				set_rect		( void* userptr, const xui_rect2d<f64>& value );
	
	/*
	//member
	*/
	xui_propkind*			m_basekind;
	NP2DSCollider*			m_collider;
};

#endif//__onity_propcollider_h__