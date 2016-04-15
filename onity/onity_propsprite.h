#ifndef __onity_propsprite_h__
#define __onity_propsprite_h__

#include "onity_prop2dsres.h"

class onity_propsprite : public onity_prop2dsres
{
public:
	/*
	//constructor
	*/
	onity_propsprite( const std::wstring& full );

	/*
	//override
	*/
	virtual NP2DSAssetFile*		get_resfile		( void );
	virtual xui_proproot*		new_subprop		( u32 id );
};

#endif//__onity_propsprite_h__