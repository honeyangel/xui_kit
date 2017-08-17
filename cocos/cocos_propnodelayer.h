#ifndef __cocos_propnodelayer_h__
#define __cocos_propnodelayer_h__

#include "cocos_propnodebase.h"

class cocos_propnodelayer : public cocos_propnodebase
{
public:
	/*
	//constructor
	*/
	cocos_propnodelayer( cocos_propcsd* file, cocos2d::Node* node );

protected:
	/*
	//method
	*/
	void					add_content		( void );

	/*
	//static
	*/
	static xui_vector<f64>	get_contentsize	( void* userptr );
	static void				set_contentsize	( void* userptr, const xui_vector<f64>& value );
};

#endif//__cocos_propnodelayer_h__