#ifndef __onity_proplike_h__
#define __onity_proplike_h__

#include "onity_proppath.h"

class onity_proplike : public onity_proppath
{
public:
	/*
	//constructor
	*/
	onity_proplike( void );

	/*
	//destructor
	*/
	virtual ~onity_proplike( void );

	/*
	//method
	*/
	void	save	( void );
};

#endif//__onity_proplike_h__