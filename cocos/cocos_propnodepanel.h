#ifndef __cocos_propnodepanel_h__
#define __cocos_propnodepanel_h__

#include "cocos_propnodewidget.h"

class cocos_propnodepanel : public cocos_propnodewidget
{
public:
	/*
	//constructor
	*/
	cocos_propnodepanel( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::Layout*		get_panel		( void );

protected:
	/*
	//method
	*/
	void						add_clip		( void );
	void						add_background	( void );

	/*
	//static
	*/
	static cocos_value_unitvec	def_size		( void* userptr );
	static cocos_value_scale9	get_scale9		( void* userptr );
	static void					set_scale9		( void* userptr, const cocos_value_scale9& value );
	static bool					was_clip		( void* userptr );
	static void					set_clip		( void* userptr, bool  value );
	static s32					get_colortype	( void* userptr );
	static void					set_colortype	( void* userptr, s32   value );
	static xui_colour			get_backcolor	( void* userptr );
	static void					set_backcolor	( void* userptr, const xui_colour& value );
	static f64					get_backalpha	( void* userptr );
	static void					set_backalpha	( void* userptr, f64   value );
	static xui_colour			get_startcolor	( void* userptr );
	static void					set_startcolor	( void* userptr, const xui_colour& value );
	static xui_colour			get_endcolor	( void* userptr );
	static void					set_endcolor	( void* userptr, const xui_colour& value );
	static f64					get_colordir	( void* userptr );
	static void					set_colordir	( void* userptr, f64   value );
	static void*				get_background	( void* userptr );
	static void					set_background	( void* userptr, void* value );

	/*
	//member
	*/
	cocos2d::ui::Layout*		m_panel;
};

class cocos_propnodelayout : public cocos_propnodepanel
{
public:
	/*
	//constructor
	*/
	cocos_propnodelayout( cocos_propcsd* file, cocos2d::Node* node );
};

#endif//__cocos_propnodepanel_h__