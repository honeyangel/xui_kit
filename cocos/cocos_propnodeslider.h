#ifndef __cocos_propnodeslider_h__
#define __cocos_propnodeslider_h__

#include "cocos_propnodewidget.h"

namespace cocos2d
{
	namespace ui
	{
		class Slider;
	}
}

class cocos_propnodeslider : public cocos_propnodewidget
{
public:
	/*
	//constructor
	*/
	cocos_propnodeslider( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::Slider*		get_slider		( void );

protected:
	/*
	//method
	*/
	void						add_slider		( void );

	/*
	//static
	*/
	static cocos_value_unitvec	def_size		( void* userptr );
	static void*				get_backimage	( void* userptr );
	static void					set_backimage	( void* userptr, void* value );
	static void*				get_foreimage	( void* userptr );
	static void					set_foreimage	( void* userptr, void* value );
	static void*				get_normal		( void* userptr );
	static void					set_normal		( void* userptr, void* value );
	static void*				get_press		( void* userptr );
	static void					set_press		( void* userptr, void* value );
	static void*				get_disable		( void* userptr );
	static void					set_disable		( void* userptr, void* value );
	static bool					was_enable		( void* userptr );
	static void					set_enable		( void* userptr, bool  value );
	static f64					get_percent		( void* userptr );
	static void					set_percent		( void* userptr, f64   value );

	/*
	//member
	*/
	cocos2d::ui::Slider*		m_slider;
};

#endif//__cocos_propnodeslider_h__