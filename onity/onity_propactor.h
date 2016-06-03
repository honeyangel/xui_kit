#ifndef __onity_propactor_h__
#define __onity_propactor_h__

#include "onity_prop2dsasset.h"

class onity_propactor : public onity_prop2dsasset
{
public:
	/*
	//constructor
	*/
	onity_propactor( onity_propfile* propfile, u32 id );

	/*
	//destructor
	*/
	virtual ~onity_propactor( void );

	/*
	//method
	*/
	const xui_proproot_vec&		get_dummys		( void ) const;
	void						add_dummy		( void );
	void						del_dummy		( xui_proproot* proproot );
	const xui_proproot_vec&		get_layers		( void ) const;
	xui_proproot*				add_layer		( void );
	void						del_layer		( xui_proproot* proproot );
	void						set_layerindex	( xui_proproot* proproot, u32 index );

protected:
	/*
	//static
	*/
	static bool					was_loop		( void* userptr );
	static void					set_loop		( void* userptr, bool value );
	static f64					get_loopstart	( void* userptr );
	static void					set_loopstart	( void* userptr, f64  value );
	static xui_proproot_vec		get_dummys		( void* userptr );
	static void					add_dummy		( void* userptr );
	static void					del_dummy		( void* userptr, xui_proproot* proproot );

	/*
	//member
	*/
	xui_propkind*				m_actorkind;
	xui_proproot_vec			m_propdummys;
	xui_proproot_vec			m_proplayers;
};

#endif//__onity_propactor_h__