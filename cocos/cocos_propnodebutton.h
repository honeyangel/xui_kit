#ifndef __cocos_propnodebutton_h__
#define __cocos_propnodebutton_h__

#include "cocos_propnodewidget.h"

namespace cocos2d
{
	namespace ui
	{
		class Button;
	}
}

class cocos_propnodebutton : public cocos_propnodewidget
{
public:
	/*
	//constructor
	*/
	cocos_propnodebutton( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::Button*		get_button		( void );
	cocos_propfile*				get_fontfile	( void );
	void						set_fontfile	( cocos_propfile* file );

protected:
	/*
	//method
	*/
	void						add_button		( void );

	/*
	//static
	*/
	static cocos_value_unitvec	def_size		( void* userptr );
	static cocos_value_scale9	get_scale9		( void* userptr );
	static void					set_scale9		( void* userptr, const cocos_value_scale9& value );
	static void*				get_normal		( void* userptr );
	static void					set_normal		( void* userptr, void* value );
	static void*				get_press		( void* userptr );
	static void					set_press		( void* userptr, void* value );
	static void*				get_disable		( void* userptr );
	static void					set_disable		( void* userptr, void* value );
	static bool					was_enable		( void* userptr );
	static void					set_enable		( void* userptr, bool  value );
	static std::wstring			get_text		( void* userptr );
	static void					set_text		( void* userptr, const std::wstring& value );
	static void*				get_font		( void* userptr );
	static void					set_font		( void* userptr, void* value );
	static f64					get_fontsize	( void* userptr );
	static void					set_fontsize	( void* userptr, f64   value );
	static xui_colour			get_fontdraw	( void* userptr );
	static void					set_fontdraw	( void* userptr, const xui_colour& value );

	/*
	//member
	*/
	cocos2d::ui::Button*		m_button;
	cocos_propfile*				m_fontfile;
};

#endif//__cocos_propnodebutton_h__