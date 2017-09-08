#ifndef __cocos_propnodeloadingbar_h__
#define __cocos_propnodeloadingbar_h__

#include "cocos_propnodewidget.h"

namespace cocos2d
{
	namespace ui
	{
		class LoadingBar;
	}
}

class cocos_propnodeloadingbar : public cocos_propnodewidget
{
public:
	/*
	//constructor
	*/
	cocos_propnodeloadingbar( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::LoadingBar*	get_loadingbar	( void );

protected:
	/*
	//method
	*/
	void						add_loadingbar	( void );

	/*
	//static
	*/
	static cocos_value_unitvec	def_size		( void* userptr );
	static void*				get_background	( void* userptr );
	static void					set_background	( void* userptr, void* value );
	static f64					get_percent		( void* userptr );
	static void					set_percent		( void* userptr, f64 value );
	static s32					get_direction	( void* userptr );
	static void					set_direction	( void* userptr, s32 value );

	/*
	//member
	*/
	cocos2d::ui::LoadingBar*	m_loadingbar;
};

#endif//__cocos_propnodeloadingbar_h__