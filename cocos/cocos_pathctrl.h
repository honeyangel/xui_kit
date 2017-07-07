#ifndef __cocos_pathctrl_h__
#define __cocos_pathctrl_h__

#include "cocos_filectrl.h"

class cocos_pathctrl : public xui_kindctrl
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
	cocos_pathctrl( xui_propkind* propkind );
};

#endif//__cocos_pathctrl_h__