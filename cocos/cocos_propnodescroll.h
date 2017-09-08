#ifndef __cocos_propnodescroll_h__
#define __cocos_propnodescroll_h__

#include "cocos_propnodepanel.h"

namespace cocos2d
{
	namespace ui
	{
		class ScrollView;
	}
}

class cocos_propnodescroll : public cocos_propnodepanel
{
public:
	/*
	//constructor
	*/
	cocos_propnodescroll( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::ScrollView*	get_scroll		( void );

protected:
	/*
	//method
	*/
	void						add_scroll		( void );
	void						add_bounce		( void );

	/*
	//static
	*/
	static bool					was_bounce		( void* userptr );
	static void					set_bounce		( void* userptr, bool value );
	static s32					get_direction	( void* userptr );
	static void					set_direction	( void* userptr, s32  value );
	static xui_vector<f64>		get_innersize	( void* userptr );
	static void					set_innersize	( void* userptr, const xui_vector<f64>& value );

	/*
	//member
	*/
	cocos2d::ui::ScrollView*	m_scroll;
};

class cocos_propnodescrollview : public cocos_propnodescroll
{
public:
	/*
	//constructor
	*/
	cocos_propnodescrollview( cocos_propcsd* file, cocos2d::Node* node );
};

#endif//__cocos_propnodescroll_h__