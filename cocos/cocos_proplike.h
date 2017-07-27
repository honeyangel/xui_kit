#ifndef __cocos_proplike_h__
#define __cocos_proplike_h__

#include "cocos_proppath.h"

class cocos_proplike : public cocos_proppath
{
public:
	/*
	//constructor
	*/
	cocos_proplike( void );

	/*
	//destructor
	*/
	virtual ~cocos_proplike( void );

	/*
	//method
	*/
	void	load	( void );
	void	save	( void );
};

#endif//__cocos_proplike_h__