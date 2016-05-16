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

	/*
	//method
	*/
	onity_propfile*		get_propfile	( void );

	/*
	//override
	*/
	virtual void		on_detach		( const xui_proproot_vec& proproot );

protected:
	/*
	//member
	*/
	onity_savekind*		m_savekind;
};

#endif//__onity_propasset_h__