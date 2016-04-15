#ifndef __onity_prop2dsasset_h__
#define __onity_prop2dsasset_h__

#include "onity_propasset.h"

class onity_prop2dsasset : public onity_propasset
{
public:
	/*
	//constructor
	*/
	onity_prop2dsasset( onity_propfile* propfile, u32 id );

	/*
	//method
	*/
	u32					get_assetid		( void ) const;
	NP2DSAsset*			get_asset		( void );

protected:
	/*
	//static
	*/
	static s32			get_freetype	( void* userptr );
	static void			set_freetype	( void* userptr, s32 value );

	/*
	//event
	*/
	void				on_namechanged	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_propkind*		m_reskind;
	u32					m_assetid;
};

#endif//__onity_prop2dsasset_h__