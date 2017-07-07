#ifndef __cocos_savekind_h__
#define __cocos_savekind_h__

#include "xui_propkind.h"

class cocos_propfile;
class cocos_savekind : public xui_propkind
{
public:
	/*
	//constructor
	*/
	cocos_savekind( xui_proproot* root, cocos_propfile* propfile );

	/*
	//method
	*/
	cocos_propfile*		get_propfile( void );

protected:
	/*
	//member
	*/
	cocos_propfile*		m_propfile;
};

#endif//__cocos_savekind_h__