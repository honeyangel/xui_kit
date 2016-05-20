#ifndef __onity_pathctrl_h__
#define __onity_pathctrl_h__

#include "onity_filectrl.h"

class NP2DSAssetFile;
class onity_pathctrl : public xui_kindctrl
{
	xui_declare_rtti

public:
	/*
	//create
	*/
	static xui_kindctrl*	create				( xui_propkind* propkind );

	/*
	//constructor
	*/
	onity_pathctrl( xui_propkind* propkind );
};

#endif//__onity_pathctrl_h__