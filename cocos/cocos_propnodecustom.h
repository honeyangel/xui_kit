#ifndef __cocos_propnodecustom_h__
#define __cocos_propnodecustom_h__

#include "cocos_propnodewidget.h"

class cocos_propnodecustom : public cocos_propnodewidget
{
public:
	/*
	//constructor
	*/
	cocos_propnodecustom( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	void	add_custom	( void );
};

#endif//__cocos_propnodecustom_h__