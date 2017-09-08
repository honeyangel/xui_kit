#ifndef __cocos_propnodecheckbox_h__
#define __cocos_propnodecheckbox_h__

#include "cocos_propnodewidget.h"

namespace cocos2d
{
	namespace ui
	{
		class CheckBox;
	}
}

class cocos_propnodecheckbox : public cocos_propnodewidget
{
public:
	/*
	//constructor
	*/
	cocos_propnodecheckbox( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::CheckBox*		get_checkbox		( void );

protected:
	/*
	//method
	*/
	void						add_checkbox		( void );

	/*
	//static
	*/
	static cocos_value_unitvec	def_size			( void* userptr );
	static void*				get_normal			( void* userptr );
	static void					set_normal			( void* userptr, void* value );
	static void*				get_press			( void* userptr );
	static void					set_press			( void* userptr, void* value );
	static void*				get_disable			( void* userptr );
	static void					set_disable			( void* userptr, void* value );
	static void*				get_checknormal		( void* userptr );
	static void					set_checknormal		( void* userptr, void* value );
	static void*				get_checkdisable	( void* userptr );
	static void					set_checkdisable	( void* userptr, void* value );
	static bool					was_enable			( void* userptr );
	static void					set_enable			( void* userptr, bool  value );
	static bool					was_checked			( void* userptr );
	static void					set_checked			( void* userptr, bool  value );

	/*
	//member
	*/
	cocos2d::ui::CheckBox*		m_checkbox;
};

#endif//__cocos_propnodecheckbox_h__