#ifndef __xui_proproot_h__
#define __xui_proproot_h__

#include "xui_propkind.h"

class xui_proproot
{
public:
	/*
	//constructor
	*/
	xui_proproot( void );

	/*
	//destructor
	*/
	virtual ~xui_proproot( void );

	/*
	//method
	*/
	const xui_propkind_vec&	get_propkind	( void ) const;
	bool					has_propkind	( xui_propkind* propkind ) const;
	void					add_propkind	( xui_propkind* propkind );

protected:
	/*
	//member
	*/
	xui_propkind_vec		m_propkind;
};

#endif//__xui_proproot_h__
