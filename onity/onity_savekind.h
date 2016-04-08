#ifndef __onity_savekind_h__
#define __onity_savekind_h__

#include "xui_propkind.h"

class onity_propfile;
class onity_savekind : public xui_propkind
{
public:
	/*
	//constructor
	*/
	onity_savekind( xui_proproot* root, onity_propfile* propfile );

	/*
	//method
	*/
	onity_propfile*		get_propfile( void );

protected:
	/*
	//member
	*/
	onity_propfile*		m_propfile;
};

#endif//__onity_savekind_h__