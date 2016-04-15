#ifndef __onity_propmodule_h__
#define __onity_propmodule_h__

#include "onity_prop2dsres.h"

class onity_propmodule : public onity_prop2dsres
{
public:
	/*
	//constructor
	*/
	onity_propmodule( const std::wstring& full );

	/*
	//override
	*/
	virtual NP2DSAssetFile*		get_resfile		( void );
	virtual xui_proproot*		new_subprop		( u32 id );

protected:
	/*
	//static
	*/
	static xui_bitmap*			get_texicon		( xui_propdata* propdata );
	static std::wstring			get_texname		( xui_propdata* propdata );
	static f64					get_texwidth	( void* userptr );
	static f64					get_texheight	( void* userptr );
	static void*				get_texfile		( void* userptr );
	static void					set_texfile		( void* userptr, void* value );
	static bool					get_texsplit	( void* userptr );
	static void					set_texsplit	( void* userptr, bool  value );
	static s32					get_texformat	( void* userptr );
	static void					set_texformat	( void* userptr, s32   value );

	/*
	//member
	*/
	xui_propkind*				m_texkind;
};

#endif//__onity_propmodule_h__