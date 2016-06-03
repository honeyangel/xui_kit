#ifndef __onity_propleaf_h__
#define __onity_propleaf_h__

#include "xui_proproot.h"

class onity_savekind;
class onity_propfile;
class onity_propleaf : public xui_proproot
{
public:
	/*
	//constructor
	*/
	onity_propleaf( onity_propfile* propfile );

	/*
	//destructor
	*/
	virtual ~onity_propleaf( void );

	/*
	//method
	*/
	onity_propfile*			get_propfile	( void );
	xui_treedata*			get_linkdata	( void );
	void					set_linkdata	( xui_treedata* linkdata );

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
	xui_treedata*			m_linkdata;
	onity_savekind*			m_savekind;
};

#endif//__onity_propleaf_h__