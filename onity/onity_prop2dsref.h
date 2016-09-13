#ifndef __onity_prop2dsref_h__
#define __onity_prop2dsref_h__

#include "onity_proproot.h"

class NP2DSTransRef;
class onity_prop2dsref : public onity_proproot
{
public:
	/*
	//constructor
	*/
	onity_prop2dsref( NP2DSTransRef* ref );

	/*
	//method
	*/
	NP2DSTransRef*			get_2dsref		( void );
	void					set_newref		( NP2DSAsset* asset );

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
	NP2DSTransRef*			m_2dsref;
	xui_propkind*			m_transkind;
	xui_propkind*			m_paramkind;
};

#endif//__onity_prop2dsref_h__