#ifndef __onity_propleaf_h__
#define __onity_propleaf_h__

#include "onity_proproot.h"

class onity_savekind;
class onity_propfile;
class onity_propleaf : public onity_proproot
{
public:
	/*
	//constructor
	*/
	onity_propleaf( onity_propfile* propfile );

	/*
	//method
	*/
	onity_propfile*			get_propfile	( void );

	/*
	//virtual
	*/
	virtual std::string		get_dragtype	( void );
	virtual void*			get_dragdata	( void );

	/*
	//override
	*/
	virtual void			on_detach		( const xui_proproot_vec& proproot );

protected:
	/*
	//member
	*/
	onity_savekind*			m_savekind;
};

#endif//__onity_propleaf_h__