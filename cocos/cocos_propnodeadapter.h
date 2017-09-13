#ifndef __cocos_propnodeadapter_h__
#define __cocos_propnodeadapter_h__

#include "cocos_propnodecustom.h"

namespace cocos2d
{
	namespace ui
	{
		class AdapterNode;
	}
}

class cocos_propnodeadapter : public cocos_propnodecustom
{
public:
	/*
	//constructor
	*/
	cocos_propnodeadapter( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::AdapterNode*	get_adapter			( void );
	void						add_adapter			( void );

protected:
	/*
	//static
	*/
	static f64					get_designheight	( void* userptr );
	static void					set_designheight	( void* userptr, f64 value );

	/*
	//member
	*/
	cocos2d::ui::AdapterNode*	m_adapter;
};

#endif//__cocos_propnodeadapter_h__