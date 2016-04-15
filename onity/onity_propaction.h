#ifndef __onity_propaction_h__
#define __onity_propaction_h__

#include "onity_prop2dsres.h"

class onity_propaction : public onity_prop2dsres
{
public:
	/*
	//constructor
	*/
	onity_propaction( const std::wstring& full );

	/*
	//override
	*/
	virtual xui_proproot*		new_subprop		( u32 id );
	virtual NP2DSAssetFile*		get_resfile		( void );
};

#endif//__onity_propaction_h__