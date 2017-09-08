#ifndef __cocos_propnodewidget_h__
#define __cocos_propnodewidget_h__

#include "cocos_propnodebase.h"
#include "cocos_propdata_scale9.h"

namespace cocos2d
{
	namespace ui
	{
		class Widget;
		class Scale9Sprite;
	}
}

class cocos_propnodewidget : public cocos_propnodebase
{
public:
	/*
	//constructor
	*/
	cocos_propnodewidget( xui_bitmap* icon, cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::Widget*		get_widget		( void );

protected:
	/*
	//method
	*/
	void						add_touch		( void );
	void						add_scale9		( cocos_propdata_scale9::get_func userget, cocos_propdata_scale9::set_func userset );
	void						add_callback	( void );

	/*
	//static
	*/
	static bool					was_touchenable	( void* userptr );
	static void					set_touchenable	( void* userptr, bool value );

	/*
	//member
	*/
	cocos2d::ui::Widget*		m_widget;
};

#endif//__cocos_propnodewidget_h__