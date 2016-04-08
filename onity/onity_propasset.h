#ifndef __onity_propasset_h__
#define __onity_propasset_h__

#include "xui_proproot.h"

class onity_savekind;
class onity_propasset : public xui_proproot
{
public:
	/*
	//constructor
	*/
	onity_propasset( void );

	/*
	//virtual
	*/
	virtual void		on_propchanged	( xui_component* sender, xui_method_propdata& args );

protected:
	/*
	//member
	*/
	onity_savekind*		m_savekind;
};

#endif//__onity_propasset_h__