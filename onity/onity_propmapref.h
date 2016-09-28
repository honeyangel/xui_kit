#ifndef __onity_propmapref_h__
#define __onity_propmapref_h__

#include "NP2DSTransRef.h"
#include "onity_propnode2dsref.h"

typedef std::vector<NP2DSTransRef::SParam>& ParamVec;
class onity_propmapref : public onity_propnode2dsref
{
public:
	/*
	//constructor
	*/
	onity_propmapref( NP2DSTransRef* ref );

	/*
	//method
	*/
	void					set_newref		( NP2DSAsset* asset );

protected:
	/*
	//static
	*/
	static void*			get_asset		( void* userptr );
	static void				set_asset		( void* userptr, void* value );
	static ParamVec&		get_params		( void* userptr );
	static void				add_param		( void* userptr );
	static void				del_param		( void* userptr );
	static xui_propdata*	new_paramprop	( void* userptr, u32 i, xui_propkind* propkind );

	/*
	//member
	*/
	xui_propkind*			m_paramkind;
};

#endif//__onity_propmapref_h__