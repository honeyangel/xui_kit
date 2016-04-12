#ifndef __onity_propasset_h__
#define __onity_propasset_h__

#include "xui_proproot.h"

class onity_savekind;
class onity_propfile;
class onity_propasset : public xui_proproot
{
public:
	/*
	//constructor
	*/
	onity_propasset( onity_propfile* propfile );

protected:
	/*
	//member
	*/
	onity_savekind*		m_savekind;
};

#endif//__onity_propasset_h__